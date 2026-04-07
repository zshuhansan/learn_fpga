#include "VSoc.h"
#include "verilated.h"
#include <cstdio>
#include <cstdint>

// Verilator 通过这个回调获取当前仿真时间，单位由 testbench 自行约定。
static vluint64_t main_time = 0;
double sc_time_stamp() { return main_time; }

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
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
    for (int i = 0; i < max_cycles; ++i) {
        tick();
        ran_cycles = i + 1;
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
    }
    if (Verilated::gotFinish() || top->HALT) {
        std::printf("SIM_DONE finish=1 cycles=%d\n", ran_cycles);
    } else {
        std::printf("SIM_DONE finish=0 cycles=%d\n", ran_cycles);
    }

    // 主动调用 final，确保 Verilator 模型执行收尾逻辑并释放资源。
    top->final();
    delete top;
    return 0;
}
