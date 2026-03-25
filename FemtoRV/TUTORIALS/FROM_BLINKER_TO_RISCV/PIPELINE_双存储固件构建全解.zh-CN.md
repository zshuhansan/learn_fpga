# pipeline1 双存储固件构建全解（只懂 Verilog 也能完全跑通）

本文把 `pipeline1.v` 这套“程序 ROM + 数据 RAM”的固件生成链路，从零讲到你能独立改链接脚本/启动代码/Makefile 并解释每个文件格式在干什么。

目标：看完后你应该能回答这些问题并亲手操作：

- 为什么要生成两份 `PROGROM.hex` / `DATARAM.hex`，它们分别装什么？
- `pipeline.ld` 的 `MEMORY/SECTIONS` 到底是什么语法，怎么控制段放置？
- `start_pipeline.S` 为什么要写 `gp/sp`，`call main` 到底发生了什么？
- `riscv64-unknown-elf-gcc/as/ld/readelf/objdump/objcopy` 各自负责什么？
- `firmware_words` 为什么能把 ELF 变成 Verilog 可用的 `.hex`？
- `make pi.pipeline.hex` 背后到底执行了哪些步骤？

文档基于本仓库当前版本的这些文件：

- 处理器/双存储仿真顶层：[pipeline1.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/pipeline1.v)
- 链接脚本：[FIRMWARE/pipeline.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pipeline.ld)
- 启动代码：[FIRMWARE/start_pipeline.S](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/start_pipeline.S)
- Makefile 规则（含 `.pipeline.elf` / `.PROGROM.hex` / `.DATARAM.hex`）：[FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L68-L87)
- 运行 Verilator 仿真脚本：[run_verilator.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/run_verilator.sh)

---

## 1. 总体图景：你在做一个“把软件塞进硬件存储”的流程

`pipeline1.v` 里 CPU 内部有两块存储阵列（都用 `$readmemh()` 初始化）：

- `PROGROM`：装指令（只读）
- `DATARAM`：装数据（可读写，含 .data/.bss/.rodata/栈等）

对应代码位置：

- `$readmemh("PROGROM.hex",PROGROM);`  
- `$readmemh("DATARAM.hex",DATARAM);`  
见 [pipeline1.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/pipeline1.v#L53-L60)

因此你最终要得到两份文件：

- `PROGROM.hex`：地址范围 0x00000..0x0FFFF（64kB）
- `DATARAM.hex`：地址范围 0x10000..0x1FFFF（64kB）

这两份 `.hex` 都是 **“32 位 word 序列”**，供 Verilog 的 `$readmemh` 读入到 `reg [31:0] mem[]`。

关键：**软件里看到的是一个统一的地址空间**（0..0x1FFFF），只是我们用链接脚本把不同段映射到不同区域；硬件里则把它拆成两块阵列，各负责地址空间的一半。

---

## 2. 你会用到的工具链（每个工具到底做什么）

### 2.1 `riscv64-unknown-elf-gcc`：编译 C / 驱动链接

它可以做三类事：

- 编译：`.c` → `.o`（目标文件）
- 也可以驱动链接：把一堆 `.o` + `.a` 交给 `ld`
- 也能生成汇编：`.c` → `.s`（可选）

本仓库常用编译参数（在 Makefile 里封装好了）大致是：

- `-march=rv32i -mabi=ilp32`：目标 ISA/ABI（32 位寄存器，RV32I）
- `-fno-pic`：不生成位置无关代码（裸机更常见）
- `-fno-stack-protector`：关闭栈保护（避免依赖 libc runtime）
- `-Wl,--no-relax`：禁止链接器 relaxation（本教程用 `gp` 做 IO 映射，relax 可能改变寻址假设）
- `-O2/-Os`：优化等级（影响体积与速度）

### 2.2 `riscv64-unknown-elf-as`：汇编器

把 `.S/.s` 汇编成 `.o`。  

- `.S`（大写）通常表示会先经过 C 预处理器（能用 `#define/#include/#if`），`.s` 则不会。

### 2.3 `riscv64-unknown-elf-ld`：链接器

把多个 `.o` / `.a` 连接成最终 ELF，并按照链接脚本决定：

- 每个 section（`.text/.data/.bss/.rodata/...`）放到哪个地址
- 入口点在哪里
- 符号（symbol）地址是多少

本流程最核心的“布局控制”全在 `ld -T pipeline.ld ...`。

### 2.4 `readelf`：看 ELF 的“段/节布局”

你用它验证 `.text` 和 `.data` 是否落在预期地址。

重要输出字段解释（你会频繁用）：

- `Type`：
  - `PROGBITS`：文件里真的有内容（比如 `.text/.rodata/.data`）
  - `NOBITS`：文件里没有内容但运行时要占空间（典型 `.bss`）
- `Addr`：运行时地址（VMA）
- `Off`：在 ELF 文件中的偏移
- `Size`：大小
- `Flg`：
  - `A`：alloc（装载到内存）
  - `X`：可执行
  - `W`：可写

### 2.5 `objdump`：反汇编/检查生成的指令

在 Makefile 里链接后会自动生成 `*.list`，就是 `objdump -D` 的输出，见 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L70-L73)。

### 2.6 `objcopy`：ELF → 裸二进制（本流程不强依赖）

常用于把 ELF 变成 flat binary 写入 flash，但 `pipeline1` 的 ROM/RAM 初始化走的是 `firmware_words`，不是 `objcopy`。

### 2.7 `firmware_words`：ELF → Verilog `$readmemh` 可读的 word-hex

它会：

- 解析 ELF 的 loadable segment/section
- 把指定地址范围内的内容导出成 “每行 4 个 32-bit word”的十六进制文本
- 未定义的洞（hole）会填 0（等价于 memory 初始化为 0）

在 pipeline 规则里它还支持：

- `-from_addr` / `-to_addr`：只导出某段地址范围  
用于切出 `PROGROM` 和 `DATARAM` 两份 hex。

Makefile 中对应规则见：

- `%.PROGROM.hex`：[FIRMWARE/Makefile:L74-L79](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L74-L79)
- `%.DATARAM.hex`：[FIRMWARE/Makefile:L80-L85](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L80-L85)

### 2.8 `make`：把“正确的参数组合”固化成一条命令

你贴的那串手写命令，本仓库用 `make pi.pipeline.hex` 已封装。  
理解 make 的关键是：它不是脚本语言，它是“依赖图 + 规则”，会根据时间戳判断是否需要重建。

这也是为什么有时需要 `make clean`：如果某个中间产物存在且时间戳让 make 认为“已经最新”，它就不会重跑你期待的步骤（尤其你改了链接脚本/工具参数但目标文件没变时）。

### 2.9 Make/Makefile 彻底复习（结合本流程与本仓库）

这一节的目标是：你看到一条命令 `make pi.pipeline.hex`，能够在脑子里自动展开它到底做了什么、为什么有时需要 `make clean`、怎么让 make “强制重建”、怎么定位 make 为什么没执行你以为会执行的命令。

#### 2.9.1 Makefile 最核心的三件事

Make 只有三个概念必须吃透：

- 目标（target）：你想生成的东西，比如 `pi.pipeline.hex`
- 依赖（prerequisites）：生成目标前必须准备好的文件/目标
- 配方（recipe）：把依赖变成目标的命令序列

语法形态是：

```make
目标: 依赖1 依赖2 ...
	命令1
	命令2
```

两条你必须记死的“坑”：

- **命令行必须用 Tab 开头**（不是空格）。这是 make 的历史遗留设计，写错会出现非常迷惑的报错。
- Make 默认按“文件时间戳”判断要不要执行：如果目标文件存在，且比所有依赖都新，则认为目标已最新，不执行 recipe。

#### 2.9.2 为什么 Make 不是脚本：它在构建“依赖图”

当你执行：

```bash
make pi.pipeline.hex
```

make 的思路不是“按 Makefile 从上到下执行”，而是：

1) 找到名叫 `pi.pipeline.hex` 的规则
2) 递归展开它的依赖，直到展开到“源文件/已有文件”或“有规则可生成的文件”
3) 按拓扑顺序执行必须执行的 rule（只执行需要更新的那部分）

这就是为什么 make 很适合这种“多产物 + 增量编译”的构建链路。

#### 2.9.3 用本仓库规则，把 `pi.pipeline.hex` 展开成完整依赖链

在本仓库 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L68-L87) 里，pipeline 相关规则可以在脑中展开成这条链：

- `pi.pipeline.hex`
  - 依赖 `pi.PROGROM.hex` 和 `pi.DATARAM.hex`
    - 它们都依赖 `pi.pipeline.elf`
      - 依赖 `pi.o`、`start_pipeline.o`、以及 `putchar.o wait.o print.o memcpy.o errno.o perf.o`（这些来自 `$(LIBOBJECTS)`）
        - `pi.o` 来自 `pi.c`
        - `start_pipeline.o` 来自 `start_pipeline.S`
        - 其它 `.o` 分别来自 `*.c`/`*.S`

所以你执行一次 `make pi.pipeline.hex`，典型的执行顺序会是：

1) 编译/汇编各个 `.o`
2) `ld -T pipeline.ld ...` 链接出 `pi.pipeline.elf`
3) `objdump -D` 导出 `pi.pipeline.elf.list`（便于你查指令）
4) `firmware_words ... -from_addr 0 -to_addr 0xFFFF` → `pi.PROGROM.hex`
5) `firmware_words ... -from_addr 0x10000 -to_addr 0x1FFFF` → `pi.DATARAM.hex`
6) 把这两份 hex 复制到 `..` 与 `../obj_dir`，供 `$readmemh()` 和 Verilator 工程读取

#### 2.9.4 变量（variables）：Makefile 为什么看起来“像模板语言”

Makefile 里经常看到：

- `LIBOBJECTS=putchar.o wait.o ...`
- `$(RVLD)`、`$(RVTOOLCHAIN_LIB_DIR)` 这类变量

规则：

- `NAME=value`：定义变量
- `$(NAME)`：引用变量
- 命令行可覆盖变量：`make RAM_SIZE=65536 foo.hex`（本仓库有大量这种用法）
- 环境变量也会被 make 当作变量（可能造成“你以为没设置但其实被环境影响”的情况）

在本仓库里：

- 教程目录的 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile) 会 `include` 上层的 [makefile.inc](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/makefile.inc)
- `makefile.inc` 定义了工具链路径（`RVGCC/RVAS/RVLD/...`）、通用 CFLAGS、以及 `get_riscv_toolchain` 自动下载逻辑

因此你看到 `$(RVLD)` 实际就是某个 `.../bin/riscv64-unknown-elf-ld` 的完整路径，而不是系统 PATH 里的 `ld`。

#### 2.9.5 模式规则（pattern rule）：`%.pipeline.elf` 这种写法怎么读

本仓库 pipeline 的关键规则是这种形式：

```make
%.pipeline.elf: %.o start_pipeline.o $(LIBOBJECTS) $(RV_BINARIES)
	$(RVLD) -T pipeline.ld ... -o $@
```

理解要点：

- `%` 是通配符，代表“任意基名”
- 当你请求 `pi.pipeline.elf` 时，`%` 会被替换为 `pi`
  - 于是依赖就变成 `pi.o start_pipeline.o ...`
- `$@` 是自动变量，表示“当前正在生成的目标文件名”

常用自动变量（你用它们读 Makefile 会快很多）：

- `$@`：目标（target）
- `$<`：第一个依赖
- `$^`：所有依赖（去重）

#### 2.9.6 `.PHONY`：为什么 `clean` 不会被时间戳影响

如果一个 target 不对应真实文件（例如 `clean`、`all`、`get_riscv_toolchain`），应声明为伪目标（phony）。

否则如果目录里刚好有个叫 `clean` 的文件，make 会认为目标已存在且可能是最新的，导致 `make clean` 不执行。

你可以在上层 [makefile.inc](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/FIRMWARE/makefile.inc#L171-L176) 看到 `.PHONY` 的使用方式。

#### 2.9.7 为什么你经常需要 `make clean`（以及更“专业”的替代方式）

你在这个流程里常见的改动有：

- 改链接脚本 `pipeline.ld`
- 改启动代码 `start_pipeline.S`
- 改编译选项（`-O2/-Os`、`-march`、`-D` 宏）

这些改动不一定会让你想要的目标被重建，原因是：

- make 只看“文件依赖”与“时间戳”
- 如果规则没有把 `pipeline.ld` 写进依赖列表，改了 `pipeline.ld` 后，make 未必会重新链接 `pi.pipeline.elf`

本仓库 `%.pipeline.elf` 规则里没有显式写 `pipeline.ld` 作为依赖，因此你改了它，最稳的方式确实是 `make clean`。

更通用的替代办法（你以后可以优先用它们）：

- 强制重建某个目标：`make -B pi.pipeline.hex`（忽略时间戳，全做一遍）
- 只打印将要执行的命令，不执行：`make -n pi.pipeline.hex`（排查“make 认为会做什么”）
- 打印更详细的决策过程：`make -d pi.pipeline.hex`（非常啰嗦，但能定位“为什么没重建”）

#### 2.9.8 一次指定多个目标：`make clean pi.pipeline.hex`

你可以在同一条命令里写多个 target：

```bash
make clean pi.pipeline.hex
```

make 会按从左到右依次构建这些目标。  
在本仓库里这等价于：

1) 先删除旧产物（避免时间戳/依赖误判）
2) 再从头生成 `.elf` 与两份 `.hex`

这也是我推荐的“改链接脚本/启动代码后的一条龙命令”。

#### 2.9.9 `-C`：为什么文档里常写 `make -C FIRMWARE ...`

`make -C DIR target` 表示“切到 DIR 目录里执行 make”，等价于：

```bash
cd DIR
make target
```

优点是：你在任何目录都能写一条命令跑构建，不怕当前目录错了导致找不到 Makefile 或输出落错位置。

#### 2.9.10 读 Makefile 的建议顺序（避免迷路）

当你面对一个复杂 Makefile（尤其本仓库这种 `include` 多层配置）时，按这个顺序读会最快：

1) 先找你关心的目标名（例如 `%.pipeline.elf`、`%.pipeline.hex`）
2) 看它依赖了哪些文件/变量（`$(LIBOBJECTS)`、`$(RV_BINARIES)`）
3) 追踪变量来自哪里（通常在 `include ../../../FIRMWARE/makefile.inc`）
4) 最后再关心“工具链安装/环境检测”这种辅助逻辑（例如 `get_riscv_toolchain`）

---

## 3. 关键文件类型（你会在整个流程里看到哪些后缀）

按生成链路排序：

### 3.1 `.c`：C 源码

例如 [FIRMWARE/pi.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pi.c)。

注意：这里的 `main` 是 `void main()`（见 [pi.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pi.c#L180-L186)），裸机环境下不依赖返回值。

### 3.2 `.S`：汇编源码（可预处理）

例如 [start_pipeline.S](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/start_pipeline.S)。

### 3.3 `.o`：目标文件（未链接）

`gcc -c` 或 `as` 的输出。里面已经有 section（`.text/.data/...`）但地址通常还不是最终地址。

### 3.4 `.a`：静态库（很多 `.o` 打包）

例如 `libgcc.a`（编译器运行时支持，比如 32 位除法、64 位乘法辅助等）以及 `-lm` 需要的 `libm.a`。

### 3.5 `.elf`：最终可执行镜像（仍然是结构化格式）

`*.pipeline.elf` 是最终链接结果，里面包含：

- 程序段：`.text`
- 数据段：`.data/.bss/.rodata/...`
- 符号表、重定位信息（某些会被 strip 掉）

### 3.6 `.hex`：给 Verilog 初始化用的“文本十六进制”

这里的 `.hex` 不是 Intel HEX；它是 `$readmemh` 期望的那种“按 word 写出来的十六进制文本”。

你可以把它理解成：把一个地址范围内的 32-bit 内存逐 word dump 到文本里。

---

## 4. 链接脚本 `pipeline.ld`：把地址空间切成两半

链接脚本原文：[pipeline.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pipeline.ld)

### 4.1 `MEMORY` 语法：定义“内存区域”

```ld
MEMORY {
   PROGROM (RX) : ORIGIN = 0x00000, LENGTH = 0x10000  /* 64kB ROM */
   DATARAM (RW) : ORIGIN = 0x10000, LENGTH = 0x10000  /* 64kB RAM */
}
```

- `PROGROM` / `DATARAM` 是你起的“区域名字”
- `(RX)/(RW)` 是权限标记（读/写/执行），主要用于链接器检查
- `ORIGIN` 是这个区域的起始地址（字节地址）
- `LENGTH` 是区域长度

因此这张地址图就固定了：

- `0x0000_0000 .. 0x0000_FFFF` → 程序 ROM（64kB）
- `0x0001_0000 .. 0x0001_FFFF` → 数据 RAM（64kB）

### 4.2 `SECTIONS` 语法：把输入 section “归并成输出 section”，并放入区域

```ld
SECTIONS {
    .text : {
        . = ALIGN(4);
        start_pipeline.o (.text)
        *(.text*)
    } > PROGROM

    .data : {
        . = ALIGN(4);
        *(.data*)
        *(.sdata*)
        *(.rodata*)
        *(.srodata*)
        *(.bss*)
        *(.sbss*)
        *(COMMON)
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.init_array*)
        *(.gcc_except_table*)
    } > DATARAM
}
```

先讲 5 个你必须掌握的语法点：

1) **输出 section 名字**：比如 `.text`、`.data`  
它决定了最终 ELF 里会出现哪些 section。

2) **输入 section 选择器**：比如 `*(.text*)`  
含义是：从所有输入文件（`*`）里，把名字匹配 `.text*` 的 section 全部拿出来拼到这里。

3) **指定某个文件的 section 必须排在最前**：  
`start_pipeline.o (.text)` 这行的作用是：确保启动代码的 `.text` 在整个 `.text` 段最开头。

这非常关键，因为你的 CPU 复位后通常从 PC=0 取第一条指令，如果 `.text` 开头不是启动代码，程序就跑飞。

4) **位置计数器 `.` 与对齐 `ALIGN()`**：  
`. = ALIGN(4);` 表示把当前位置对齐到 4 字节边界（RISC-V 32 位指令要求 4 字节对齐）。

5) **`> REGION`：把输出 section 放入某个 MEMORY 区域**：  
`} > PROGROM` / `} > DATARAM` 就是最终“落地址”的地方。

### 4.3 一个很重要的取舍：把 `.rodata` 放在 DATARAM

很多系统会把 `.rodata` 放进 ROM/Flash（只读更省 RAM），但这里**故意把 `.rodata` 放进 DATARAM**，原因是：

- `pipeline1.v` 的 `PROGROM` 只是一个简单 ROM（只读），你的 load 指令读数据默认走 DATARAM/IO 路径
- 让 `.rodata` 在 DATARAM 可以避免你再做“从 ROM 读常量”的额外数据通路

代价是：常量也占用 RAM。

---

## 5. 启动代码 `start_pipeline.S`：把裸机运行环境搭起来

原文：[start_pipeline.S](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/start_pipeline.S)

```asm
.equ IO_BASE, 0x400000
.section .text
.globl start
start:
        li   gp,IO_BASE
        li   sp,0x20000
        call main
        ebreak
```

逐行解释（只站在“跑起来程序”角度）：

- `.equ IO_BASE, 0x400000`：定义一个常量符号  
  软件侧访问 MMIO 一般会用 `gp` 作为基址寄存器，后续的 `putchar` 等函数会假设 `gp` 已经指向 IO 页。

- `.section .text`：接下来的指令放进 `.text` 段

- `.globl start`：导出符号 `start`（让链接器/其他文件能引用它）

- `li gp, IO_BASE`：把全局指针 `gp(x3)` 指向 IO 基址 `0x0040_0000`  
  这与 `io.h` 一致：`IO_BASE = 0x400000`（见 [io.h](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/io.h#L3-L9)）。

- `li sp, 0x20000`：把栈顶设置在 0x20000  
  注意这里栈顶在 `0x20000`，刚好在 0..0x1FFFF 之外。对仿真/简化模型来说，只要 CPU 的数据通路没做越界检查，栈向下增长会落入 0x1FFFF 以下，就能工作。  
  如果你想更严格地把栈放在 DATARAM 内，可改成 `li sp, 0x20000` 的其它值（例如 `0x1FFFC`），但需要确保与程序的栈需求匹配。

- `call main`：跳转到 C 的 `main()`，并按 RISC-V 调用约定保存返回地址到 `ra(x1)`

- `ebreak`：用来“结束”程序  
  在仿真/bench 模式下通常会把 `ebreak` 当作停止条件（不同版本实现略有差异）。

对只懂 Verilog 的你来说，最重要的就是：**启动代码负责把寄存器初始化到 C 运行所需的最小集合**（至少 `gp`、`sp`），然后把控制权交给 `main`。

---

## 6. Makefile 封装：从 `pi.c` 到两份 `.hex`

核心规则在 [FIRMWARE/Makefile](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/Makefile#L68-L87)。

### 6.1 生成 ELF：`pi.pipeline.elf`

规则（简化解释）：

- 依赖：`pi.o start_pipeline.o putchar.o wait.o print.o memcpy.o errno.o` 等
- 链接：`ld -T pipeline.ld ... -lm ... libgcc.a`

这一步解决两件事：

1) 把所有 `.o` 拼成一个可执行镜像  
2) 按 `pipeline.ld` 规定，把 `.text` 放 0..64k，把 `.data` 放 64k..128k

### 6.2 从 ELF 导出两份 hex：`pi.PROGROM.hex` / `pi.DATARAM.hex`

规则直接把地址范围切开：

- `pi.PROGROM.hex`：`-from_addr 0 -to_addr 0xFFFF`
- `pi.DATARAM.hex`：`-from_addr 0x10000 -to_addr 0x1FFFF`

并把它们复制到教程目录（`..`）和 `obj_dir`，保证仿真时 `$readmemh` 能找到。

### 6.3 为什么有时要 `make clean`

你贴的原文里说“必须先 make clean，否则已有 .elf 会让它搞混”，更准确的说法是：

- make 的判断依据是时间戳
- 如果你改了 `pipeline.ld` / `start_pipeline.S` / 或改变了 `CFLAGS`，但目标文件时间戳没触发重建，make 可能不会重新链接生成新的 `.pipeline.elf`，进而导致 `.hex` 仍是旧布局

最稳的方法：

- 改了链接脚本/启动代码后，先 `make clean` 再 `make xxx.pipeline.hex`

---

## 7. 你应该怎么操作（推荐的最短正确路径）

### 7.1 生成工具链（一次性）

```bash
cd FIRMWARE
make get_riscv_toolchain
```

### 7.2 生成 `pi` 的双 hex（推荐：让 Makefile 全自动）

```bash
cd FIRMWARE
make clean
make pi.pipeline.hex
```

执行完成后，在教程目录（上一级）会看到：

- `PROGROM.hex`
- `DATARAM.hex`

### 7.3 跑 Verilator 仿真

```bash
cd ..
./run_verilator.sh pipeline1.v
```

说明：

- [run_verilator.sh](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/run_verilator.sh) 会定义 `-DBENCH`、`-DCPU_FREQ=10` 等宏，并编译 `sim_main.cpp`
- `pipeline1.v` 里的 `$readmemh("PROGROM.hex")` 默认从当前工作目录读文件；Makefile 已把 hex 复制到这里

---

## 8. 用 `readelf` 验证你真的理解了布局

生成 `pi.pipeline.elf` 后，你应该能看懂类似输出：

```text
Section Headers:
  [ 1] .text  PROGBITS  Addr=00000000 ...
  [ 2] .data  PROGBITS  Addr=00010000 ...
```

含义：

- `.text` 从 0 开始 → 对应 `PROGROM` 区
- `.data` 从 0x10000 开始 → 对应 `DATARAM` 区

如果你看到 `.text` 跑到了 0x10000 之后，或者 `.data` 掉回到 0，说明链接脚本没有按你想的工作（通常是你没用 `-T pipeline.ld` 或者没重建）。

---

## 9. 你可以立刻做的三个“必会改动练习”（验证真的学会）

1) 把 PROGROM 扩到 128kB，把 DATARAM 扩到 128kB  
修改 [pipeline.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pipeline.ld#L1-L4)：

- `LENGTH` 改成 `0x20000`
- `DATARAM ORIGIN` 改成 `0x20000`

然后同步修改：

- `pipeline1.v` 里 PROGROM/DATARAM 的数组深度（word 数量）
- Makefile 里导出范围（`-to_addr` 也要扩大）

2) 把 `.rodata` 挪回 PROGROM（省 DATARAM）  
在 `.text` 输出段里加上 `*(.rodata*)`/`*(.srodata*)`，并从 `.data` 输出段里移除它们。  
然后你会发现：如果 CPU 的 load 通路不支持从 PROGROM 读常量，程序会因为取常量失败而崩（这是“硬件/软件协同”的典型案例）。

3) 把栈指针放回 DATARAM 内  
把 [start_pipeline.S](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/start_pipeline.S#L5-L8) 的 `sp` 改成 `0x1FFFC` 并解释为什么这样更“自洽”。

---

## 10. 常见问题与排错

- **仿真报找不到 `PROGROM.hex` / `DATARAM.hex`**
  - 确认你执行了 `make pi.pipeline.hex` 且 Makefile 已把文件复制到教程目录
  - 确认你运行 `./run_verilator.sh pipeline1.v` 的当前目录就是 `FROM_BLINKER_TO_RISCV`（不是 `FIRMWARE/`）

- **改了 `pipeline.ld` 但布局没变**
  - `make clean` 后再 `make pi.pipeline.hex`
  - 用 `readelf -S pi.pipeline.elf` 验证 `Addr`

- **程序跑着跑着卡死/不输出**
  - 先用更小的程序（例如 hello/sieve）验证 UART
  - 再检查 `gp` 是否正确指向 `0x400000`（IO_BASE）
  - 再检查栈是否越界（`sp` 起始位置与 DATARAM 的边界关系）

---

## 11. 以 `pi.c` 为例：每一步发生了什么、产物是什么

这一节对应你最常用的一条命令：

```bash
cd FIRMWARE
make clean
make pi.pipeline.hex
```

你可以把它理解成“用 Makefile 把一条手写命令链自动跑完”。下面按步骤解释每个命令/产物的意义。

### 11.1 编译/汇编阶段：`.c/.S` → `.o`

Makefile 会先把所有需要的源文件变成目标文件（`.o`）。典型包含：

- `pi.c` → `pi.o`
- `start_pipeline.S` → `start_pipeline.o`
- `putchar.S` → `putchar.o`
- `wait.S` → `wait.o`
- `perf.S` → `perf.o`
- `print.c` → `print.o`
- `memcpy.c` → `memcpy.o`
- `errno.c` → `errno.o`

这些 `.o` 的共同点：

- 它们已经是 RISC-V 机器码了，但里面的跳转/全局变量引用很多还是“占位符”，需要链接时才能确定最终地址
- 每个 `.o` 自己带着若干 section（`.text/.data/.rodata/.bss/...`），但地址大多还没落到最终内存映射

### 11.2 链接阶段：`.o` → `pi.pipeline.elf`（按 `pipeline.ld` 固定布局）

接下来会执行一条类似的命令：

- `riscv64-unknown-elf-ld -T pipeline.ld ... -o pi.pipeline.elf`

这一刻发生了三件关键的事：

1) **符号解析**：把 `call main` 解析到 `pi.c` 里的 `main`，把 `printf` 解析到 `print.o` 的实现，把 `putchar` 解析到 `putchar.o`。
2) **重定位**：把所有“需要填真实地址”的地方填上最终地址（跳转目标、全局变量地址、常量池引用等）。
3) **段布局**：按 [pipeline.ld](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pipeline.ld) 把：
   - `.text` 放到 `PROGROM`（0x00000..0x0FFFF）
   - 其余（`.data/.bss/.rodata/...`）放到 `DATARAM`（0x10000..0x1FFFF）

此时 `pi.pipeline.elf` 才是真正意义上的“结构化可执行镜像”。

### 11.3 为什么 `pi` 链接时要 `-lm` 和 `libgcc.a`

看 [pi.c](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/pi.c#L19-L35)：

- `#include <math.h>`：会用到 `fmod`（以及可能的 `sqrt`），通常在 `libm.a`，所以链接要加 `-lm`。
- 即使你主要用整数算法，仍可能出现 64 位运算（`long long`）或 RV32I 缺失硬件乘除导致的辅助函数调用，这些通常在 `libgcc.a`。

一句话：`-lm` 给数学函数，`libgcc.a` 给编译器运行时兜底。

### 11.4 反汇编产物：`pi.pipeline.elf.list`

Makefile 会把 ELF 反汇编输出到 `pi.pipeline.elf.list`，用途是：

- 你做了 CPU 改动后，检查编译器是否生成了你不支持的指令（例如你没做 M 却出现 `mul/div`）
- 检查启动代码是否确实放在 `.text` 开头

### 11.5 导出两份 `$readmemh` 可用的 hex：`PROGROM.hex` / `DATARAM.hex`

最后 make 调用 `firmware_words` 把 ELF 地址空间切成两段：

- `pi.PROGROM.hex`：导出 0..0xFFFF
- `pi.DATARAM.hex`：导出 0x10000..0x1FFFF

并复制到教程目录，成为：

- `../PROGROM.hex`
- `../DATARAM.hex`

这就是 `pipeline*.v` 里 `$readmemh("PROGROM.hex")/$readmemh("DATARAM.hex")` 实际加载的文件。

---

## 12. 当你改 CPU 时，软件侧通常要改什么（以 M 扩展为例）

把“CPU 改动 → 软件要改什么”分成 6 类，你基本不会漏改。

### 12.1 ISA 改动：RV32I ↔ RV32IM（M 扩展）

你做了 M 扩展，硬件新增支持 `mul/div/rem` 等指令。软件侧必须让编译器“敢用它”：

- 用 `-march=rv32im` 编译（本仓库里就是 `make ARCH=rv32im ...`）

否则你会看到两种常见现象：

- 编译仍按 RV32I 生成：乘除会变成对 `libgcc.a` 里的软件例程调用，你的硬件 M 跑不出加速
- 反过来：硬件没做 M，但你用 `ARCH=rv32im` 编译，程序会因为遇到非法指令而卡死/跑飞

### 12.2 CSR/性能计数器支持（跑 CoreMark 必看）

[perf.S](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/perf.S) 用的是 `rdcycle/rdinstret` CSR 指令读取计数器。

- `pipeline1.v` 只把 SYSTEM 当 `ebreak` 用，没有 CSR 读，不适合跑依赖 `rdcycle` 的基准
- `pipeline9.v/pipeline10.v` 已经实现了 CSR 读 `cycle/instret`，适合跑 CoreMark

### 12.3 内存映射/大小改动（PROGROM/DATARAM）

你改了 PROGROM/DATARAM 的大小或地址范围，至少同步改三处：

- Verilog 里 ROM/RAM 的深度（word 数）
- 链接脚本 `pipeline.ld` 的 `ORIGIN/LENGTH`
- Makefile 里 `firmware_words` 的 `-from_addr/-to_addr`（导出范围）

### 12.4 启动约定改动（入口地址、栈、gp）

你改了 IO_BASE 或栈位置，至少同步改：

- `start_pipeline.S`（`gp/sp/call main`）
- `io.h`（C 侧 MMIO 宏）
- SoC 的地址译码（Verilog）

### 12.5 字节序/对齐改动（很少改，但一改就很致命）

如果你改了 load/store 的对齐策略或字节序，需要同步检查：

- Verilog 里 load 的拼接/符号扩展与 store 的 `wmask/wdata` 重排
- `firmware_words` 导出的 word 顺序与 `$readmemh` 装载方式

### 12.6 外设时钟改动（UART 乱码的根源）

你改了 `CPU_FREQ` 或时钟源：

- UART 波特率计算要匹配，否则输出乱码
- 基准里的 “每秒 tick / Coremark/MHz” 也要用正确时钟频率换算（上硬件测量时尤其重要）

---

## 13. 在此基础上跑 CoreMark（直接可跑通）

CoreMark 源码在：

- [FIRMWARE/COREMARK](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE/COREMARK)

现在可以一键生成 pipeline 的双 hex：

```bash
cd FIRMWARE
make clean
make coremark.pipeline.hex
```

这会把 `PROGROM.hex/DATARAM.hex` 复制到教程目录，供仿真加载。

### 13.1 用哪个 pipeline 顶层跑

- RV32I（无 M）：在教程目录运行

```bash
./run_verilator.sh pipeline9.v
```

- RV32IM（有 M，且希望编译器发出 `mul/div`）：先用 `ARCH=rv32im` 重新编译固件，再跑 pipeline10

```bash
cd FIRMWARE
make clean
make ARCH=rv32im coremark.pipeline.hex
cd ..
./run_verilator.sh pipeline10.v
```

### 13.2 为什么不建议用 pipeline1 跑 CoreMark

CoreMark 端口层会读 `rdcycle/rdinstret`，而 `pipeline1.v` 不支持 CSR 读，因此会在基准运行中遇到不支持的指令。
