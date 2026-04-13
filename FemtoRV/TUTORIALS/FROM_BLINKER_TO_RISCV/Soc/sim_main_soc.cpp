#include "VSoc.h"
#include "verilated.h"
#include <chrono>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <cstdint>
#include <string>

namespace {

bool copy_text_file(const std::string& src, const char* dst) {
    std::ifstream input(src, std::ios::binary);
    if (!input) {
        std::fprintf(stderr, "SIM_INPUT_ERROR cannot_open=%s\n", src.c_str());
        return false;
    }

    std::ofstream output(dst, std::ios::binary | std::ios::trunc);
    if (!output) {
        std::fprintf(stderr, "SIM_INPUT_ERROR cannot_create=%s\n", dst);
        return false;
    }

    output << input.rdbuf();
    if (!output.good()) {
        std::fprintf(stderr, "SIM_INPUT_ERROR cannot_copy=%s dst=%s\n", src.c_str(), dst);
        return false;
    }

    return true;
}

int parse_positive_arg(const char* text) {
    char* end = nullptr;
    long value = std::strtol(text, &end, 10);
    if (end == text || *end != '\0' || value < 0) {
        return -1;
    }
    return static_cast<int>(value);
}

} // namespace

// Verilator 通过这个回调获取当前仿真时间，单位由 testbench 自行约定。
static vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::string progrom_path;
    std::string dataram_path;
    int timeout_sec = 0;
    int heartbeat_sec = 15;
    int heartbeat_mcycles = 5;

    for (int argi = 1; argi < argc; ++argi) {
        const char* arg = argv[argi];
        if (std::strncmp(arg, "--timeout-sec=", 14) == 0) {
            timeout_sec = parse_positive_arg(arg + 14);
        } else if (std::strncmp(arg, "--heartbeat-sec=", 16) == 0) {
            heartbeat_sec = parse_positive_arg(arg + 16);
        } else if (std::strncmp(arg, "--heartbeat-mcycles=", 20) == 0) {
            heartbeat_mcycles = parse_positive_arg(arg + 20);
        } else if (progrom_path.empty()) {
            progrom_path = arg;
        } else if (dataram_path.empty()) {
            dataram_path = arg;
        } else {
            std::fprintf(stderr, "SIM_ARG_WARN ignored=%s\n", arg);
        }
    }

    if (!progrom_path.empty() && !copy_text_file(progrom_path, "PROGROM.hex")) {
        return 2;
    }
    if (!dataram_path.empty() && !copy_text_file(dataram_path, "DATARAM.hex")) {
        return 2;
    }

    if (heartbeat_sec < 0) {
        heartbeat_sec = 15;
    }
    if (heartbeat_mcycles < 0) {
        heartbeat_mcycles = 5;
    }

    const int64_t heartbeat_cycles = static_cast<int64_t>(heartbeat_mcycles) * 1000000LL;
    const auto wall_start = std::chrono::steady_clock::now();
    auto last_heartbeat = wall_start;

    std::printf(
        "SIM_START progrom=%s dataram=%s timeout_sec=%d heartbeat_sec=%d heartbeat_mcycles=%d\n",
        progrom_path.empty() ? "PROGROM.hex" : progrom_path.c_str(),
        dataram_path.empty() ? "DATARAM.hex" : dataram_path.c_str(),
        timeout_sec,
        heartbeat_sec,
        heartbeat_mcycles
    );
    std::fflush(stdout);

    VSoc* top = new VSoc;

    top->CLK = 0;
    top->RESET = 0;
    top->RXD = 1;

    // 一个 tick 对应完整时钟周期，分成高低电平两个半拍调用 eval。
    auto tick = [&](){
        top->CLK = 0;
        top->eval();
        main_time += 5;
        top->CLK = 1;
        top->eval();
        main_time += 5;
    };

    // 先维持若干拍复位，让 SoC 内部状态和存储模型进入确定值。
    for (int i = 0; i < 10; ++i) tick();
    top->RESET = 1;

    // HALT 拉高后再多跑几拍，给流水线尾部和 UART 输出一点收尾空间。
    const int max_cycles = 2000000000;
    int ran_cycles = 0;
    int halt_drain = -1;
    bool timed_out = false;
    for (int i = 0; i < max_cycles; ++i) {
        tick();
        ran_cycles = i + 1;

        const auto now = std::chrono::steady_clock::now();
        const auto wall_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - wall_start).count();
        const bool heartbeat_by_time = heartbeat_sec > 0 &&
            std::chrono::duration_cast<std::chrono::seconds>(now - last_heartbeat).count() >= heartbeat_sec;
        const bool heartbeat_by_cycle = heartbeat_cycles > 0 && (ran_cycles % heartbeat_cycles) == 0;
        if (heartbeat_by_time || heartbeat_by_cycle) {
            std::printf(
                "SIM_HEARTBEAT cycles=%d wall_ms=%lld halt=%d pc=%08x fpc=%08x fdpc=%08x ppc=%08x pred=%d corr=%d\n",
                        ran_cycles,
                        static_cast<long long>(wall_ms),
                        top->HALT ? 1 : 0,
                        top->Soc__DOT__CPU__DOT__PC,
                        top->Soc__DOT__CPU__DOT__F_PC,
                        top->Soc__DOT__CPU__DOT__FD_PC,
                        top->Soc__DOT__CPU__DOT__F_pendingPC,
                        top->Soc__DOT__CPU__DOT__D_predictPC ? 1 : 0,
                        top->Soc__DOT__CPU__DOT__E_correctPC ? 1 : 0
            );
            std::fflush(stdout);
            last_heartbeat = now;
        }

        if (Verilated::gotFinish()) {
            break;
        }
        if (top->HALT && halt_drain < 0) {
            halt_drain = 4;
        } else if (halt_drain == 0) {
            break;
        } else if (halt_drain > 0) {
            --halt_drain;
        }
        if (timeout_sec > 0 && wall_ms >= static_cast<long long>(timeout_sec) * 1000LL) {
            std::printf("SIM_TIMEOUT cycles=%d wall_ms=%lld\n",
                        ran_cycles,
                        static_cast<long long>(wall_ms));
            std::fflush(stdout);
            timed_out = true;
            break;
        }
    }
    const auto wall_end = std::chrono::steady_clock::now();
    const auto total_wall_ms = std::chrono::duration_cast<std::chrono::milliseconds>(wall_end - wall_start).count();
    if (!timed_out && (Verilated::gotFinish() || top->HALT)) {
        std::printf("SIM_DONE finish=1 cycles=%d wall_ms=%lld\n",
                    ran_cycles,
                    static_cast<long long>(total_wall_ms));
    } else {
        std::printf("SIM_DONE finish=0 cycles=%d wall_ms=%lld\n",
                    ran_cycles,
                    static_cast<long long>(total_wall_ms));
    }
    std::fflush(stdout);

    // 主动调用 final，确保 Verilator 模型执行收尾逻辑并释放资源。
    top->final();
    delete top;
    return timed_out ? 124 : 0;
}
