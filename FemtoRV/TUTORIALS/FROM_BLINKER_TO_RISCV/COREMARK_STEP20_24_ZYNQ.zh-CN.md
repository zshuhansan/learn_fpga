# 在 Step20–24 跑 CoreMark，以及在 Zynq FPGA 上跑“正经” CoreMark（工具链全说明）

本文面向目录 [FROM_BLINKER_TO_RISCV](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV) 的 Step20–24 SoC（`SOC` 顶层），目标是：

- 在本教程 Step20–24 的语境下把 CoreMark 跑起来（仿真/上板都给路径）
- 在你的 Zynq 板子（Zynq-7000 PL）上跑一个“更像正经跑分”的 CoreMark：可复现、可解释、能算出 CoreMark/MHz
- 把涉及到的工具链（RISC-V 裸机编译、Verilog 仿真、FPGA 综合/下载、串口）讲清楚

---

## 0. 先说结论：Step20–22 默认 6kB BRAM 装不下 CoreMark

Step20 的 RAM 在 [step20.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step20.v#L19-L23) 里写死为：

- `1536` 个 32-bit word = `6kB`
- 固件链接脚本 [bram.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/bram.ld#L1-L13) 也同样是 `LENGTH = 0x1800`

CoreMark 的代码体积（`.text`）本身在 2 万字节量级，即使数据段很小也装不进 6kB 的 BRAM，所以：

- **要在“原封不动”的 Step20–22 上跑 CoreMark：做不到**
- **要在 Step20–22 上跑：必须扩 RAM（仿真可随意扩，上板则看 FPGA BRAM 资源）**
- **要在 Step23/24 上跑：走 SPI Flash（代码在 Flash，RAM 只放 `.data/.bss/stack`），可行**

后文按这三条路分别给到操作步骤。

---

## 1. CoreMark 在仓库里的位置（以及这份移植用的计时方式）

- CoreMark 源码目录：[FIRMWARE/COREMARK](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK)
- 端口层（计时/打印/seed/迭代次数）：[core_portme.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.c#L48-L196)、[core_portme.h](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.h#L22-L25)

这份 port 的关键点：

- `barebones_clock()` 用 `rdcycle()` 作为计时源（见 [core_portme.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.c#L48-L66)）
- `CLOCKS_PER_SEC` 在 port 里写死为 `10000000`（10MHz），用于把 cycle 换算成 “秒” 以及 CoreMark/MHz（见 [core_portme.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.c#L61-L66)）
- `ITERATIONS` 默认是 `300`（见 [core_portme.h](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.h#L22-L23)）

因此：

- **在仿真里跑时**：cycle 在仿真循环里每步 +1，`CLOCKS_PER_SEC` 与真实时间无关，CoreMark/MHz 只当“程序跑通+输出逻辑没坏”的验证即可。
- **在硬件上跑时**：把 `CLOCKS_PER_SEC` 设成“CPU 真实频率（Hz）”，CoreMark/MHz 才有意义。

---

## 2. 工具链说明（你到底依赖哪些东西）

### 2.1 RISC-V 裸机编译工具链（本仓库自带自动下载）

本教程固件的 Makefile 是 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile)，它 `include` 了上层的 [makefile.inc](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/makefile.inc) 并自动下载 RISC-V GCC 工具链到：

- `FemtoRV/FIRMWARE/TOOLCHAIN/.../bin/riscv64-unknown-elf-*`

你通常只需要在教程目录下跑：

```bash
cd FIRMWARE
make get_riscv_toolchain
```

它会下载并解压工具链，后续编译链接都用这套工具链路径，不要求你额外装系统级 riscv-gnu-toolchain。

### 2.2 ELF → Verilog HEX 的转换器（firmware_words）

当你用 Step20 的 `$readmemh("firmware.hex", MEM)` 初始化 RAM 时，`firmware.hex` 不是随便一个 hex dump，而是通过工具 [firmware_words](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/makefile.inc#L83-L85) 从 ELF 提取并重排得到。

在本教程 Makefile 里，生成 `.hex` 的规则见 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L13-L18)。

### 2.3 Verilog 仿真（Icarus/Verilator）

- Icarus：入口脚本 [run.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/run.sh)
- Verilator：入口脚本 [run_verilator.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/run_verilator.sh)，主循环 [sim_main.cpp](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/sim_main.cpp#L45-L59)

注意：Step23/24 引入真实 SPI Flash 引脚后，需要更完整的 testbench/flash model，否则仿真里读不到指令（后文会解释怎么绕开）。

### 2.4 FPGA 综合/下载（不同板卡不同工具链）

本教程提供了多块板的脚本入口：`BOARDS/run_xxx.sh stepXX.v`，例如：

- iCE40：`yosys + nextpnr-ice40 + iceprog`（见 [run_icestick.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/BOARDS/run_icestick.sh)）
- Xilinx 7 系 Artix：`yosys + nextpnr-xilinx + openFPGALoader`（见 [run_arty.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/BOARDS/run_arty.sh#L10-L16)）

**Zynq（xc7z\*) 本仓库没有现成脚本和 XDC**，后文给两条路：

- 推荐：Vivado 正规 flow（更现实）
- 可选：开源 flow（取决于你的器件/封装数据库支持情况）

### 2.5 串口终端

教程默认用 [terminal.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/terminal.sh)（`picocom 115200`）读取 `TXD` 输出。上板时你需要确认：

- 你的板子是否把 **PL 的 UART TX/RX** 接到某个 USB-UART（很多 Zynq 板子的板载 USB-UART其实是接 PS，不是 PL）
- 如果没有，就用一个外置 USB-UART 小板接到 PMOD/扩展口

---

## 3. 在 Step23/24（SPI Flash）上跑 CoreMark（推荐：最快跑通，也最接近“真机跑”）

### 3.1 先编译出可烧录的 SPI Flash 镜像

在 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile) 已经补了 CoreMark 的专用目标 `coremark.spiflash1.elf`（多源文件一起链接），你现在可以直接：

```bash
cd FIRMWARE
make get_riscv_toolchain
make coremark.spiflash1.bin
```

如果你用的是 iCE40 板卡并且想直接烧到板载 SPI Flash（对应 `iceprog`），则：

```bash
make coremark.spiflash1.prog
```

说明：

- `spiflash1` 的链接/启动过程，README 已有非常详细的解释（LMA/拷贝 `.data`、清零 `.bss`），见 [README.zh-CN.md:Step24](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/README.zh-CN.md#L2690-L2817)。
- 为了避免某些程序出现 `.eh_frame` 段与 `.data` 的 LMA 重叠，本仓库也已经在 [spiflash1.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/spiflash1.ld#L38-L49) 里把这些段显式放进 FLASH（做法与 [spiflash2.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/spiflash2.ld#L48-L68) 一致）。

### 3.2 用 Step24 的 SoC 上板

Step24 顶层：[step24.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step24.v)

对支持脚本的板子（例如 icestick/icebreaker/ulx3s/arty/cmod_a7/tangnano9k）：

```bash
cd ..
BOARDS/run_<你的板子>.sh step24.v
./terminal.sh
```

你会在串口看到 CoreMark 输出（包括 CRC 校验与 CoreMark/MHz）。

### 3.3 让输出变成“正经跑分”的两个必要调整

1) **把计时频率改成你的 CPU 实际频率**

CoreMark port 的 `CLOCKS_PER_SEC` 当前是 10MHz（见 [core_portme.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.c#L61-L66)）。

你的 SoC 时钟频率通常由综合脚本的 `CPU_FREQ`（MHz）或板级 PLL/分频决定，例如 Arty 脚本里 `CPU_FREQ=100`（见 [run_arty.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/BOARDS/run_arty.sh#L7-L12)）。

你要让 CoreMark/MHz 有意义，应让：

- `CLOCKS_PER_SEC = CPU_FREQ * 1000000`

2) **让运行时间足够长（至少几秒甚至 >10s）**

CoreMark 的“官方可比性”通常要求运行时长足够大（经典说法是目标 10 秒量级）。你可以：

- 提高 [core_portme.h](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.h#L22-L23) 的 `ITERATIONS`
- 或把 `ITERATIONS` 设成 `0`，让 CoreMark 自己先探测每轮耗时，再自动选一个迭代次数（逻辑在 [core_main.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_main.c#L241-L263)）

---

## 4. 在 Step20–22（BRAM + readmemh）里跑 CoreMark（仿真/大 BRAM FPGA 都适用）

这条路的关键是：**把 RAM 扩到能容纳 CoreMark 的代码+数据**。CoreMark 的 `.text` 大概 21kB（以 `spiflash1` 版本为例，见 `riscv64-unknown-elf-size coremark.spiflash1.elf`），因此建议至少做 32kB 甚至 64kB 的 RAM。

### 4.1 需要改的三个地方（必须一致）

1) Verilog RAM 深度（以 Step20 为例）

把 [step20.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step20.v#L19-L23) 的：

- `MEM[0:1535]`（6kB）改成更大，例如 64kB：
  - 64kB / 4 = 16384 words → `MEM[0:16383]`

2) 链接脚本 RAM 区长度

把 [bram.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/bram.ld#L1-L4) 的：

- `LENGTH = 0x1800` 改成 `0x10000`（64kB）

3) 固件构建里的 RAM_SIZE（给 firmware_words 用）

在 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L5-L6) 默认 `RAM_SIZE=6144`。你可以不改文件，直接命令行覆盖：

```bash
cd FIRMWARE
make RAM_SIZE=65536 <你的程序>.hex
```

### 4.2 CoreMark 的 BRAM 版本怎么做

CoreMark 是多源文件工程，不能直接靠 `%.bram.elf: %.o ...` 这种单文件规则生成。

建议做法：

- 继续用 `spiflash1` 在小 RAM 板子上跑（最推荐）
- 如果你是“仿真/大 FPGA（例如 Zynq）”并且愿意扩 RAM，则：
  - 用上面的“三处一致”把 RAM 扩到 32kB/64kB
  - 然后按 CoreMark 的对象文件集合链接出 BRAM ELF，再转成 `.hex` 放到 `../firmware.hex`

如果你希望把 “coremark.bram.hex” 也做成一条 `make` 命令（像 `coremark.spiflash1.bin` 一样），可以继续扩展 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile) 加一个 `coremark.bram.elf` 目标，并在命令行用 `RAM_SIZE=...` 配合新的 `bram.ld` 即可。

---

## 5. 在你的 Zynq 板子（PL）上跑“正经” CoreMark：两条推荐路线

这里的核心区别是：**Zynq 板子怎么给 RISC-V 核心提供“可执行代码存放区”**。你常见有两种现实选择：

- A) 代码/数据都在 PL 的 BRAM（最简单，最稳）：扩 RAM → BRAM init（bitstream 内置固件）
- B) 代码在外部 SPI Flash（像 Step24 那样），数据在 BRAM：需要 PL 能访问一颗 SPI Flash，并且你有合适的烧录方式

### 5.1 路线 A（推荐）：Vivado + 大 BRAM + 固件内置（最少外设依赖）

#### 5.1.1 你需要准备什么

- Vivado（与你板子型号匹配的版本即可）
- 一份 Zynq 板子的 XDC（通常厂商会给）
- 一条“PL 侧 UART TX/RX”的物理连接方案（外置 USB-UART 转接到 PMOD 是最通用的）

#### 5.1.2 工程搭建（建议用 Step20 思路）

1) 选一个顶层 SoC

- 推荐以 Step20/21/22 的 SoC 为基础（顶层只有 `CLK/RESET/LEDS/RXD/TXD`，外设少，容易约束）
- 把 RAM 按 4.1 章扩到 64kB 或更多

2) 固件生成（CoreMark）

你要让 BRAM 里装得下 CoreMark，因此：

- 修改/复制 `bram.ld` 让 RAM 足够大
- 在生成 `.hex` 时用 `make RAM_SIZE=...`

3) BRAM 初始化方式（Vivado 下两种常用做法）

- 做法 1：保留 `$readmemh("firmware.hex", MEM);` 并把 `firmware.hex` 加入 Vivado 工程的 sources
  - 这依赖 Vivado 对“推断 RAM 的初始化”支持情况
  - 优点是改动小
- 做法 2（更正规）：用 Block Memory Generator IP
  - 把固件转成 `.coe`，在 IP 里指定 init file
  - 用 IP 替换掉 step20 里推断出来的 `reg [31:0] MEM[...]`

4) 时钟与复位

- 用板载时钟（例如 100MHz）直驱或经 PLL/MMCM
- 记下 CPU 真实频率，后面要用它修正 `CLOCKS_PER_SEC`

5) UART 波特率

Step24 的 UART 使用 `CPU_FREQ` 宏来推导波特率（见 [step24.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step24.v#L396-L405)），Step20 的 UART 也有类似宏机制。你需要确保：

- Verilog 里 `CPU_FREQ`（MHz）与你实际时钟一致
- 串口终端使用 `115200`

6) 生成 bitstream → 下载到 PL → 打开串口终端

跑起来后你会看到 CoreMark 输出。接着做“正经跑分”两件事：

- 把 CoreMark port 里的 `CLOCKS_PER_SEC` 改成真实频率（Hz）
- 把 `ITERATIONS` 调到更大的值，确保运行时长足够

### 5.2 路线 B（可选）：复用 Step24 的 SPI Flash XIP（更像“嵌入式跑法”，但对板子要求高）

前提条件：

- 你的 Zynq 板子上有一颗 SPI Flash，并且 **PL 侧能接到它的引脚**
  - 许多 Zynq 板子的 QSPI Flash 是 PS 用来启动的，PL 未必有直连
  - 即使有直连，也要小心与 PS 争用

如果满足前提，你可以：

1) 用本文第 3 章生成 `coremark.spiflash1.bin`
2) 复用 Step24 的 SPI Flash controller（[spi_flash.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/spi_flash.v)）与 SoC（[step24.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step24.v)）
3) 在 Vivado 里把 SPI Flash 引脚约束到实际 pin
4) 用 Vivado 的 “Program Flash” 或者你板子支持的烧录工具，把 `coremark.spiflash1.bin` 写到约定偏移（本教程默认从 `0x00820000` 映射的 flash 区读取）

这条路的优势是：PL RAM 不用很大（只放 `.data/.bss/stack`），劣势是工程集成与板级连线更复杂。

---

## 6. 常见故障排查（按现象定位）

- **编译/链接报 “section will not fit in region BRAM”**
  - 说明你还在 6kB RAM 的 bram.ld/step20.v 配置上尝试塞 CoreMark
  - 换 Step24(spiflash1) 或者扩 RAM（第 4 章）

- **spiflash1 链接报段重叠（.eh_frame/.data overlap）**
  - 确认你用的是更新后的 [spiflash1.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/spiflash1.ld#L38-L49)

- **串口输出是乱码**
  - `CPU_FREQ` 与实际时钟不一致，或者终端波特率不是 115200
  - Zynq 板上常见坑：你连的是 PS 的 USB-UART，但你的 SoC UART 在 PL 引脚上

- **CoreMark/MHz 明显离谱**
  - 先确认 `CLOCKS_PER_SEC` 是否等于 CPU 实际频率（Hz）
  - 再确认 `rdcycle` 的实现是否真的是“CPU 每周期 +1”（见 [perf.S](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/perf.S)）

---

## 7. 你可以直接复用的关键文件索引

- 固件构建入口：[FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile)
- CoreMark 端口与参数：[core_portme.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.c)、[core_portme.h](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK/core_portme.h)
- Step24（带 SPI Flash XIP）：[step24.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step24.v)
- Step20（BRAM+readmemh，方便做“固件内置”）：[step20.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/step20.v)
- 串口终端脚本：[terminal.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/terminal.sh)

