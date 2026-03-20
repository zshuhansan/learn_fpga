# 从闪烁灯到 RISC-V

本教程是一段循序渐进的旅程：从一个最简单的 LED 闪烁电路（blinky）一路走到一个 RISC-V 内核。

它适用于以下开发板：
- IceStick
- IceBreaker
- ULX3S
- ARTY
- tang nano 9K

如果你没有开发板，也可以在仿真里完成全部内容（但确实没那么有趣）。

## 关于本教程

- 这是一个循序渐进的入门，每一步尽量只改动一件事。它是我在学习这些概念（2020-2022）时记录的日志的“精选版”。我也尽量保留我踩过的坑、走过的弯路与误区，通常会以旁注或提示的形式标出来；
- 我尽量把硬件门槛降到最低。即使用最小的 FPGA（IceStick Ice40HX1K），你也可以完成本教程的第一部分，并把它一步步变成一个完全可用的 RV32I 微控制器，能执行编译出来的 C 程序；
- 最终得到的处理器不是最高效的，但也绝不是玩具：它可以运行任意程序。你可能会问：能不能跑 DOOM？答案是可以的，它甚至能 [运行 DOOM](https://github.com/BrunoLevy/learn-fpga/tree/master/LiteX/software/Doom)！（但不是在 IceStick 上，你需要更大的 FPGA）。它借助了 LiteX 提供的不错的 SDRAM 控制器，因为 Doom 需要一定容量的 RAM；
- 本教程既讲硬件也讲软件：你会学到如何为你的内核编译汇编与 C 程序；
- 我尽量让所有示例程序“好玩且有意思”，同时保持足够短小。随教程附带的演示程序包括：
  - Mandelbrot 集（汇编与 C 两个版本）
  - rotozoom 图形效果
  - 绘制填充多边形
  - 光线追踪
  这些图形程序都通过 ANSI 转义序列在终端的文本模式下显示（没错，这样会得到很大的“像素”）。如果你想更好玩，也可以使用一块小 OLED 屏（以后会补充说明）。
- [第二篇](PIPELINE.md) 讲流水线：你会在那篇里学到如何把本教程末尾得到的基础处理器改造成更高效的流水线处理器，并加入分支预测；
- [第三篇](INTERRUPTS.md) 仍在编写中（WIP），内容是中断与 RISC-V 特权 ISA；
- 本教程使用 VERILOG 编写。目前也在移植到其他 HDL：
  - 由 @bl0x 提供的 [Amaranth/nMigen 版本](https://github.com/bl0x/learn-fpga-amaranth)
  - TODO：Silice 版本
  - TODO：SpinalHDL 版本

## 处理器设计入门与参考资料

为了理解处理器设计，我最先读的是 Stackoverflow 上的
[这个回答](https://stackoverflow.com/questions/51592244/implementation-of-simple-microprocessor-using-verilog/51621153#51621153)，它非常有启发性。@mithro 还推荐了 [这篇文章](http://www.fpgacpu.org/papers/xsoc-series-drafts.pdf)。
如果你想系统地学一门完整课程，我强烈推荐 MIT 的 [这门课](http://web.mit.edu/6.111/www/f2016/)，它也讲到了比本文更深入的内容（流水线等）。

关于 Verilog 基础与语法，我读的是 _Verilog by example by Blaine C. Readler_，它短小精悍、直达要点。

Stackoverflow 那个回答有两点特别好：
- 它只保留最核心的内容；
- 它选用的是一个 RISC 处理器示例，和 RISC-V 有不少相似之处（区别在于它有状态标志位，而 RISC-V 没有）。

你会从中学到：处理器里会有一个 _寄存器堆_（register file），用于存放所谓的 _通用寄存器_（general-purpose registers）。所谓“通用”，是指任意指令在读寄存器时可以读任意一个，在写寄存器时也可以写任意一个；不同于 x86（CISC）那样有很多 _专用_（specialized）寄存器。为了实现最通用的一类指令（`register <- register OP register`），寄存器堆在每个周期读取两个寄存器，并可选地写回一个寄存器。

还会有一个 _ALU_，对两个值执行运算。

以及一个 _译码器_（decoder），把当前指令的比特模式转换成所需的内部控制信号。

如果你想自己设计一个 RISC-V 处理器，我建议你先仔细读一遍
[那个 Stackoverflow 回答](https://stackoverflow.com/questions/51592244/implementation-of-simple-microprocessor-using-verilog/51621153#51621153)，然后自己画一画电路图，先把整体概念建立起来；当然你也可以选择直接跳进本教程，按步骤往下做。它会温和地把你从最最简单的 blinky 带到一个完全可用的 RISC-V 内核。

## 前置条件

第一步是克隆 learn-fpga 仓库：
```
$ git clone https://github.com/BrunoLevy/learn-fpga.git
```

开始之前，你需要安装以下软件：
- iverilog/icarus（仿真）
```
  $ sudo apt-get install iverilog
```
- yosys/nextpnr：适用于你的开发板的综合/布局布线工具链。见 [此链接](../toolchain.md)。

注意：仅安装 iverilog/icarus 也足以跑完教程的全部步骤并进行体验，但感受并不一样。我强烈建议你尽可能把每一步都跑在真实硬件上。当你第一次看到自己写的处理器运行起代码时，那种兴奋感是仿真完全比不了的！

## 第 1 步：你的第一个 blinky

先从最简单的 blinky 开始！我们的 blinky 作为一个 VERILOG 模块实现，连接到输入与输出，如下所示（[step1.v](step1.v)）：

```verilog
   module SOC (
       input  CLK,
       input  RESET,
       output [4:0] LEDS,
       input  RXD,
       output TXD
   );

   reg [4:0] count = 0;
   always @(posedge CLK) begin
      count <= count + 1;
   end
   assign LEDS = count;
   assign TXD  = 1'b0; // not used for now

   endmodule
```

我们把它叫 SOC（System On Chip，片上系统）。对于一个 blinky 来说这是个很“大”的名字，但教程走到最后你会发现，它确实会被一步步“变形”成一个真正的 SoC。这个 SOC 连接了如下信号：

- `CLK`（输入）：系统时钟。
- `LEDS`（输出）：连接到开发板的 5 个 LED。
- `RESET`（输入）：复位按钮。你可能会说 IceStick 没有按钮，但事实上……（后面会讲）
- `RXD` 与 `TXD`（输入/输出）：连接到 FTDI 芯片，通过 USB 模拟串口。后面也会讲。

你可以这样综合并把比特流下载到板子上：
```
$ BOARDS/run_xxx.sh step1.v
```
其中 `xxx` 对应你的开发板型号。

你会看到 5 个 LED 亮起来……但它们看起来并没有在闪。为什么？其实它们确实在闪，只是频率太高，人眼分辨不出来。

要看到变化，可以用仿真。为了仿真，我们写一个新的 VERILOG 文件 [bench_iverilog.v](bench_iverilog.v)，包含一个 `bench` 模块来封装我们的 `SOC`：

```verilog
module bench();
   reg CLK;
   wire RESET = 0;
   wire [4:0] LEDS;
   reg  RXD = 1'b0;
   wire TXD;

   SOC uut(
     .CLK(CLK),
     .RESET(RESET),
     .LEDS(LEDS),
     .RXD(RXD),
     .TXD(TXD)
   );

   reg[4:0] prev_LEDS = 0;
   initial begin
      CLK = 0;
      forever begin
	 #1 CLK = ~CLK;
	 if(LEDS != prev_LEDS) begin
	    $display("LEDS = %b",LEDS);
	 end
	 prev_LEDS <= LEDS;
      end
   end
endmodule
```

`bench` 模块驱动了 `SOC`（此处命名为 `uut`，意为 “unit under test”）的全部输入输出信号。`forever` 循环不断翻转 `CLK`，并在 LED 状态变化时显示出来。

现在可以启动仿真：
```
  $ iverilog -DBENCH -DBOARD_FREQ=10 bench_iverilog.v step1.v
  $ vvp a.out
```

不过这两条命令不太好记，所以我写了一个脚本，你更可能会用它：
```
  $ ./run.sh step1.v
```

你会看到 LED 在计数。仿真非常宝贵：它允许你在 VERILOG 代码里插入“打印语句”（`$display`），而这些在真实硬件上运行时是做不到的！

退出仿真：
```
  <ctrl><c>
  finish
```

_注：我开发 femtorv 的最早版本完全是在板子上完成的，当时不会用仿真，只能靠 LED 调试。别学我，那很蠢！_

**试一试** 你会如何修改 `step1.v`，把速度降到肉眼能看到 LED 闪烁的程度？

**试一试** 你能不能实现一个类似 “Knight Rider” 的左右来回扫描灯，而不是简单计数？

## 第 2 步：更慢的 blinky

你可能已经猜到了：想让 blinky 变慢，要么用更多位数计数（并把高位连到 LED），要么插入一个“分频器”（也叫 “gearbox”）：它用很多位计数，然后用最高位去驱动你的设计。第二种方式很有意思，因为你不用修改设计本身，只需要把分频器插到开发板的 `CLK` 与你的设计之间。这样即使在真实硬件上，也能看清 LED 的变化。

为此我在 [clockworks.v](clockworks.v) 里做了一个 `Clockworks` 模块，里面包含 gearbox，以及和 `RESET` 相关的一点机制（后面会解释）。`Clockworks` 的实现如下：

```verilog
module Clockworks
(
   input  CLK,   // clock pin of the board
   input  RESET, // reset pin of the board
   output clk,   // (optionally divided) clock for the design.
   output resetn // (optionally timed) negative reset for the design (more on this later)
);
   parameter SLOW;
...
   reg [SLOW:0] slow_CLK = 0;
   always @(posedge CLK) begin
      slow_CLK <= slow_CLK + 1;
   end
   assign clk = slow_CLK[SLOW];
...
endmodule
```

它会把时钟频率分成 `2^SLOW` 分之一。

接着我们把 `Clockworks` 插到开发板的 `CLK` 与设计之间：使用内部 `clk` 信号来驱动设计，如 [step2.v](step2.v) 所示：

```verilog
`include "clockworks.v"

module SOC (
    input  CLK,        // system clock
    input  RESET,      // reset button
    output [4:0] LEDS, // system LEDs
    input  RXD,        // UART receive
    output TXD         // UART transmit
);

   wire clk;    // internal clock
   wire resetn; // internal reset signal, goes low on reset

   // A blinker that counts on 5 bits, wired to the 5 LEDs
   reg [4:0] count = 0;
   always @(posedge clk) begin
      count <= !resetn ? 0 : count + 1;
   end

   // Clock gearbox (to let you see what happens)
   // and reset circuitry (to workaround an
   // initialization problem with Ice40)
   Clockworks #(
     .SLOW(21) // Divide clock frequency by 2^21
   )CW(
     .CLK(CLK),
     .RESET(RESET),
     .clk(clk),
     .resetn(resetn)
   );

   assign LEDS = count;
   assign TXD  = 1'b0; // not used for now
endmodule
```

它也处理了 `RESET` 信号。

现在可以在仿真里试试：
```
  $ ./run.sh step2.v
```

如你所见，计数器现在慢得多了。也试试在真实板子上跑：
```
  $ BOARDS/run_xxx.sh step2.v
```

是的，现在你能清晰看到 LED 的变化了！那 `RESET` 按钮呢？IceStick 不是没有按钮吗？其实它有一个！

![](IceStick_RESET.jpg)

把手指按在图里圈出的区域（大约在 47 号引脚附近）。

**试一试** 写一个 Knight Rider 扫描灯，并用 `RESET` 来切换方向。

如果你查看 [clockworks.v](clockworks.v)，你会看到它也能生成 `PLL`：它是一种可用来生成*更快*时钟的组件。例如 IceStick 的系统时钟是 12 MHz，但我们后面生成的内核会跑到 45 MHz。我们稍后再讲。

## 第 3 步：从 ROM 读取 LED 图案的闪烁灯

现在我们已经具备了需要的工具，所以来看看如何把这个 blinky 变成一个完全可用的 RISC-V 处理器。这个目标看起来很遥远，但第 16 步时我们做出来的处理器，VERILOG 代码长度甚至不到 200 行！我第一次发现做一个处理器居然可以这么简单时非常震惊。好，现在我们一步步来。

我们已经知道处理器需要一块内存，并且会以顺序方式从中取指（除非遇到跳转与分支）。先做一个更简单但相似的东西：一个预编程的“圣诞灯带”，把 LED 的显示图案存放在内存里并按顺序读出来（见 [step3.v](step3.v)）。灯带的内存里预先存了这些图案：

```verilog
   reg [4:0] MEM [0:20];
   initial begin
       MEM[0]  = 5'b00000;
       MEM[1]  = 5'b00001;
       MEM[2]  = 5'b00010;
       MEM[3]  = 5'b00100;
       ...
       MEM[19] = 5'b10000;
       MEM[20] = 5'b00000;
   end
```

_注意：`initial` 块里的内容在综合时不会生成电路，它会被直接转换成 FPGA BRAM 的初始化数据。_

我们还会有一个“程序计数器” `PC`，每个时钟递增，并用它去索引 `MEM` 来取出要显示的内容：

```verilog
   reg [4:0] PC = 0;
   reg [4:0] leds = 0;

   always @(posedge clk) begin
      leds <= MEM[PC];
      PC <= (!resetn || PC==20) ? 0 : (PC+1);
   end
```

_注意这里的 `PC==20` 测试是为了让它循环。_

现在请在仿真和板子上都试一试。

**试一试** 做出多个闪烁模式，并用 `RESET` 在模式之间切换。

## RISC-V 指令集架构

当然，最重要的信息来源是
[RISC-V 参考手册](https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf)。
你会在里面看到 RISC-V 有许多“口味”。我们先从最简单的（RV32I：32 位基础整数指令集）开始，然后再逐步添加扩展。RISC-V 的一个很棒的特点是：指令集是 _模块化_ 的。你可以从一个非常小、完全自洽的核心开始，而它依然符合标准。这意味着标准工具（编译器、汇编器、链接器）可以为这个核心生成代码。

我当时读的是第 2 章（第 13 页到第 30 页）。结合第 130 页的表，你会发现真正不同的指令类型其实只有 11 种！（这里把 AND/OR/ADD 等看作同一类指令：它们的“操作”只是额外的参数。）现在我们只需要对全局有个概念，不必钻太深。先总览一下这 11 类指令：

| 指令类型    | 说明                                  | 算法                                  |
|-------------|---------------------------------------|---------------------------------------|
| branch      | 条件跳转，6 种变体                     | `if(reg OP reg) PC<-PC+imm`           |
| ALU reg     | 三寄存器 ALU 运算，10 种变体            | `reg <- reg OP reg`                   |
| ALU imm     | 寄存器-立即数 ALU 运算，9 种变体        | `reg <- reg OP imm`                   |
| load        | 内存到寄存器，5 种变体                  | `reg <- mem[reg + imm]`               |
| store       | 寄存器到内存，3 种变体                  | `mem[reg+imm] <- reg`                 |
| `LUI`       | 装载高位立即数                          | `reg <- (im << 12)`                   |
| `AUIPC`     | PC 加上高位立即数                        | `reg <- PC+(im << 12)`                |
| `JAL`       | 跳转并链接                              | `reg <- PC+4 ; PC <- PC+imm`          |
| `JALR`      | 寄存器跳转并链接                         | `reg <- PC+4 ; PC <- reg+imm`         |
| `FENCE`     | 多核系统内存排序                         |（此处不展开，暂时跳过）               |
| `SYSTEM`    | 系统调用、断点等                         |（此处不展开，暂时跳过）               |

- 6 种 branch 变体：对两个寄存器进行比较，决定是否跳转。
- ALU 运算有两种形式：`register <- register OP register` 或 `register <- register OP immediate`。
- load/store 可以操作字节、16 位（half-word）或 32 位（word）。其中字节和 half-word 的 load 还可以做符号扩展。地址由“寄存器内容 + 立即数偏移”得到。
- 剩下的指令相对“特殊”（第一次阅读可以略过细节，只要知道它们用于实现无条件跳转、函数调用、多核内存排序、系统调用与断点）：
  - `LUI` 用来把常量的高 20 位装载进寄存器，低位可以再用 `ADDI` 或 `ORI` 补齐。乍看之下用两条指令装载 32 位常量很怪，但这是因为所有指令长度固定为 32 位，这是个聪明的权衡。
  - `AUIPC` 把一个常量加到当前 PC，并把结果放入寄存器。它通常与 `JALR` 配合，用于到达一个 32 位的 PC 相对地址。
  - `JAL`（jump and link）把一个偏移量加到 PC，并把跳转后下一条指令的地址写入寄存器，可用于函数调用。`JALR` 类似，但偏移量加在寄存器上。
  - `FENCE` 与 `SYSTEM` 分别用于多核系统的内存排序，以及系统调用/断点。

总结一下：我们有分支（条件跳转）、ALU 运算、load/store，以及少数用于无条件跳转与函数调用的特殊指令。还有两类用于内存排序与系统调用（暂时忽略）。好吧，听起来像是“只有 9 类指令”，应该做得到……

当时我也没完全理解所有细节，所以我会从我认为最简单的部分开始（指令译码、寄存器堆与 ALU），再逐步把跳转、分支和其它指令串起来。

## 第 4 步：指令译码器

现在的想法是：在内存里放一些 RISC-V 指令，像“圣诞灯带”那样顺序取指，把当前指令放进 `instr` 寄存器，并尝试识别 11 类指令中的哪一类（根据识别结果点亮不同的 LED）。每条指令用一个 32 位字编码，我们需要把这 32 位拆开，识别指令类型与参数。

[RISC-V 参考手册](https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf) 的第 130 页（RV32/64G Instruction Set Listings）有两张表，汇总了我们需要的全部信息。

先看大表：你会注意到最低的 7 个比特（7 LSBs）决定了指令“类型”（目前不考虑 `FENCE`，所以我们有 10 种可能）：

```verilog
   reg [31:0] instr;
   ...
   wire isALUreg  =  (instr[6:0] == 7'b0110011); // rd <- rs1 OP rs2
   wire isALUimm  =  (instr[6:0] == 7'b0010011); // rd <- rs1 OP Iimm
   wire isBranch  =  (instr[6:0] == 7'b1100011); // if(rs1 OP rs2) PC<-PC+Bimm
   wire isJALR    =  (instr[6:0] == 7'b1100111); // rd <- PC+4; PC<-rs1+Iimm
   wire isJAL     =  (instr[6:0] == 7'b1101111); // rd <- PC+4; PC<-PC+Jimm
   wire isAUIPC   =  (instr[6:0] == 7'b0010111); // rd <- PC + Uimm
   wire isLUI     =  (instr[6:0] == 7'b0110111); // rd <- Uimm
   wire isLoad    =  (instr[6:0] == 7'b0000011); // rd <- mem[rs1+Iimm]
   wire isStore   =  (instr[6:0] == 7'b0100011); // mem[rs1+Simm] <- rs2
   wire isSYSTEM  =  (instr[6:0] == 7'b1110011); // special
```

除了识别指令类型，我们还需要译码指令的参数。上方那张表把指令分成 6 种格式（`R-type`,`I-type`,`S-type`,`B-type`,`U-type`,`J-type`），取决于指令需要哪些参数，以及这些参数如何编码进 32 位指令字。

`R-type` 指令有两个源寄存器 `rs1` 与 `rs2`，对它们做运算并把结果写入第三个目的寄存器 `rd`（`ADD`, `SUB`, `SLL`, `SLT`, `SLTU`, `XOR`, `SRL`, `SRA`, `OR`, `AND`）。

由于 RISC-V 有 32 个寄存器，`rs1`,`rs2`,`rd` 各占 5 位。很有意思的是：对所有指令格式来说，这些字段占用的位位置都相同。因此“译码” `rs1`,`rs2`,`rd` 只需从指令字上拉几根线出来：

```verilog
   wire [4:0] rs1Id = instr[19:15];
   wire [4:0] rs2Id = instr[24:20];
   wire [4:0] rdId  = instr[11:7];
```

接着，要区分 10 种 R-type 指令，主要靠 `funct3` 字段（3 位编码）。3 位只能区分 8 种，所以还需要 `funct7`（指令字的最高 7 位）。其中第 30 位用于区分 `ADD`/`SUB` 与 `SRA`/`SRL`（算术右移/逻辑右移）。译码器里需要拉出 `funct3` 与 `funct7`：

```verilog
   wire [2:0] funct3 = instr[14:12];
   wire [6:0] funct7 = instr[31:25];
```

`I-type` 指令有一个寄存器 `rs1` 和一个立即数 `Iimm`，对它们做运算并把结果写入 `rd`（`ADDI`, `SLTI`, `SLTIU`, `XORI`, `ORI`, `ANDI`, `SLLI`, `SRLI`, `SRAI`）。

_等一下：_ 为什么 R-type 有 10 种，而 I-type 只有 9 种？仔细看你会发现没有 `SUBI`，因为可以用 `ADDI` 加一个负数立即数来实现。这是 RISC-V 的普遍原则：如果已有功能能实现，就不要再增加新功能。

和 R-type 类似，I-type 也通过 `funct3` 与 `funct7` 来区分（其中 `funct7` 只用到了指令字第 30 位，用于区分 `SRAI`/`SRLI` 的算术/逻辑右移）。

I-type 的立即数编码在指令字最高 12 位，所以我们再拉线得到它：

```verilog
   wire [31:0] Iimm={{21{instr[31]}}, instr[30:20]};
```

可以看到，指令字的第 31 位被重复了 21 次，这就是“符号扩展”（把 12 位有符号数扩展成 32 位）。

还有另外四种指令格式：`S-type`（Store）、`B-type`（Branch）、`U-type`（上位立即数，左移 12 位）、`J-type`（Jump）。它们在指令字里编码立即数的方式各不相同。

要理解这是什么意思，回到第 2 章第 16 页：不同指令类型的核心差异之一就是 _立即数_ 的编码方式。

| 指令类型    | 说明                                                                 | 立即数编码方式                                              |
|-------------|----------------------------------------------------------------------|-------------------------------------------------------------|
| `R-type`    | 寄存器-寄存器 ALU 运算。[更多说明](https://www.youtube.com/watch?v=pVWtI0426mU) | 无                                                           |
| `I-type`    | 寄存器-立即数整数 ALU 运算与 `JALR`                                   | 12 位，符号扩展                                              |
| `S-type`    | store                                                                | 12 位，符号扩展                                              |
| `B-type`    | branch                                                               | 12 位，符号扩展，高位为 `[31:1]`（最低位 bit0 为 0）         |
| `U-type`    | `LUI`,`AUIPC`                                                        | 20 位，占据 `[31:12]`（低 12 位 `[11:0]` 为 0）              |
| `J-type`    | `JAL`                                                                | 12 位，符号扩展，高位为 `[31:1]`（最低位 bit0 为 0）         |

注意：`I-type` 与 `S-type` 编码的是同一“类型”的数值（但来自 `instr` 的不同位置）；`B-type` 与 `J-type` 也同理。

不同格式的立即数可以这样译码：

```verilog
   wire [31:0] Uimm={    instr[31],   instr[30:12], {12{1'b0}}};
   wire [31:0] Iimm={{21{instr[31]}}, instr[30:20]};
   wire [31:0] Simm={{21{instr[31]}}, instr[30:25],instr[11:7]};
   wire [31:0] Bimm={{20{instr[31]}}, instr[7],instr[30:25],instr[11:8],1'b0};
   wire [31:0] Jimm={{12{instr[31]}}, instr[19:12],instr[20],instr[30:21],1'b0};
```

注意：`Iimm`, `Simm`, `Bimm`, `Jimm` 都做了符号扩展（通过复制 bit31 到高位）。

至此我们的指令译码器就完成了！总结一下，译码器从指令字里提取出：
- isXXX：识别 11 类（此处 10 类）RISC-V 指令的信号
- 源寄存器与目的寄存器：`rs1`,`rs2`,`rd`
- 功能码：`funct3` 与 `funct7`
- 5 种立即数格式（其中 `Iimm`,`Simm`,`Bimm`,`Jimm` 带符号扩展）

现在我们把内存初始化成几条 RISC-V 指令，并看看能否正确识别：根据指令类型点亮不同的 LED（见 [step4.v](step4.v)）。为此我们直接查第 130 页的大表，手工编码指令（有点痛苦，但后面会看到更舒服的方法）。此时用 `_` 分隔二进制常量的不同字段就非常方便。

```verilog
   initial begin
      // add x1, x0, x0
      //                    rs2   rs1  add  rd  ALUREG
      MEM[0] = 32'b0000000_00000_00000_000_00001_0110011;
      // addi x1, x1, 1
      //             imm         rs1  add  rd   ALUIMM
      MEM[1] = 32'b000000000001_00001_000_00001_0010011;
      ...
      // lw x2,0(x1)
      //             imm         rs1   w   rd   LOAD
      MEM[5] = 32'b000000000000_00001_010_00010_0000011;
      // sw x2,0(x1)
      //             imm   rs2   rs1   w   imm  STORE
      MEM[6] = 32'b000000_00001_00010_010_00000_0100011;
      // ebreak
      //                                        SYSTEM
      MEM[7] = 32'b000000000001_00000_000_00000_1110011;
   end
```

然后我们这样取指并识别：

```verilog
   always @(posedge clk) begin
      if(!resetn) begin
	 PC <= 0;
      end else if(!isSYSTEM) begin
	 instr <= MEM[PC];
	 PC <= PC+1;
      end
   end
   assign LEDS = isSYSTEM ? 31 : {PC[0],isALUreg,isALUimm,isStore,isLoad};
```

（第一个 LED 接到 `PC[0]`，这样即使连续多条相同指令，我们也能看到它在闪。）

可以看到：只要指令不是 `SYSTEM`，程序计数器就会递增。当前我们唯一支持的 `SYSTEM` 指令是 `EBREAK`，它用于停止执行。

在仿真模式下，我们还可以显示识别出的指令名称与各字段：

```verilog
`ifdef BENCH
   always @(posedge clk) begin
      $display("PC=%0d",PC);
      case (1'b1)
	isALUreg: $display("ALUreg rd=%d rs1=%d rs2=%d funct3=%b",rdId, rs1Id, rs2Id, funct3);
	isALUimm: $display("ALUimm rd=%d rs1=%d imm=%0d funct3=%b",rdId, rs1Id, Iimm, funct3);
	isBranch: $display("BRANCH");
	isJAL:    $display("JAL");
	isJALR:   $display("JALR");
	isAUIPC:  $display("AUIPC");
	isLUI:    $display("LUI");
	isLoad:   $display("LOAD");
	isStore:  $display("STORE");
	isSYSTEM: $display("SYSTEM");
      endcase
   end
`endif
```

**试一试** 在仿真和板子上运行 [step4.v](step4.v)。尝试用不同的 RISC-V 指令初始化内存，看看译码器是否能正确识别它们。

## 旁注：RISC-V 的优雅

这一段可以跳过。
这里主要是我阅读 [RISC-V 参考手册](https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf) 时的一些个人感想（灵感来自其中斜体的评论与问答）。

到这里我才真正理解“指令集架构”（ISA）的含义：它当然是一份“哪些比特模式代表什么行为”的规范（Instruction Set），但同时它也会被“最终如何落到电路连线”（Architecture）强烈影响。ISA 并不“抽象”；它是与具体实现无关的，但它的设计强烈考虑了实现的便利性。虽然不同实现的流水线、分支预测、多执行单元、缓存等可以完全不同，但它们的指令译码器很可能都非常相似。

一开始我觉得很奇怪的地方包括：这么多立即数格式变体、立即数在 `instr` 的不同位段里被“打散”、`zero` 寄存器、以及一些怪怪的指令 `LUI`,`AUIPC`,`JAL`,`JALR`。当你真正去写译码器时，会更能理解原因。这个 ISA 非常聪明，是长期演化的结果（之前还有 RISC-I、RISC-II 等）。在我看来它像是一次 _提炼_：到 2020 年，各种 ISA 的尝试已经非常多，RISC-V 显然吸收了前人的经验，做出许多优秀的选择并避免了不少次优方案。

我觉得这个 ISA 特别好的地方在于：
- 指令长度固定，事情一下简单很多。（当然也有可变长度指令的扩展，但至少核心指令集很简单）；
- `rs1`,`rs2`,`rd` 在所有指令里都位于相同位置；
- 需要符号扩展的立即数都从同一个比特（`instr[31]`）扩展；
- 那些“怪指令” `LUI`,`AUIPC`,`JAL`,`JALR` 可以组合出更高级的任务（装载 32 位常量、跳到任意地址、函数调用）。它们的存在让硬件设计更简单；而对汇编程序员来说，可以通过 _伪指令_ `CALL`,`RET` 等来简化使用。参见 [RISC-V 汇编手册](https://github.com/riscv/riscv-asm-manual/blob/master/riscv-asm.md) 末尾两张表。分支测试也一样：很多比较可以通过交换参数实现（例如 `a < b <=> b > a`），于是用伪指令帮你把这些细节处理掉。

换个角度，要理解 RISC-V 的优雅，不妨想象你的任务是 _发明它_：既要定义指令集合，又要定义它们的比特编码。约束条件包括：
- 指令长度固定（32 位）
- 越简单越好：最高级的复杂是简单 [Leonardo da Vinci]!!
- 源/目的寄存器始终编码在相同位置
- 一旦需要符号扩展，应从同一比特扩展
- 能方便地在寄存器中装载任意 32 位常量（允许多条指令）
- 能方便地跳到任意内存位置（允许多条指令）
- 能方便地实现函数调用（允许多条指令）

这样你就能理解为什么会有多个立即数格式。例如 `JAL` 相比 `JALR` 少了一个源寄存器字段，于是可以把多出来的位用来扩大可跳转的偏移范围。每一位都很“值钱”，因此立即数在指令字里会在固定的 5 位寄存器字段之间“穿梭拼接”。

同理，`LUI`,`AUIPC`,`JAL`,`JALR` 的设计目的就是提供一组可以组合的基本积木：装载任意 32 位数、跳转到任意位置、以及尽可能简单地实现函数调用约定。在这些约束下，之前看起来奇怪的选择就变得完全合理。并且这样做还能让译码器的逻辑深度较低：除了 7 位 opcode 译码外，大多只是连线与复制 bit31 做符号扩展。

最后再说一句 `zero` 寄存器：我认为它非常聪明。有了它你不需要 `MOV rd rs`（用 `ADD rd rs zero`），不需要 `NOP`（用 `ADD zero zero zero`），所有分支指令也都能和 `zero` 比较。我觉得 `zero` 是个伟大的发明（虽然比不上 `0` 本身），能让指令集更紧凑。

## 第 5 步：寄存器堆与状态机

寄存器堆的实现如下：

```verilog
   reg [31:0] RegisterBank [0:31];
```

更仔细地看看执行一条指令需要做什么。以一串 R-type 指令为例，每条指令需要完成以下四件事：

- 取指：`instr <= MEM[PC]`
- 取寄存器值：`rs1 <= RegisterBank[rs1Id]; rs2 <= RegisterBank[rs2Id]`  
  其中 `rs1` 与 `rs2` 是两个寄存器。我们需要这样做，是因为 `RegisterBank` 综合后会是 BRAM，而访问 BRAM 的内容需要一个周期。
- 计算 `rs1 OP rs2`（`OP` 由 `funct3` 与 `funct7` 决定）
- 把结果写回 `rd`：`RegisterBank[rdId] <= writeBackData`。如果 `OP` 由组合逻辑计算出来，那么这一步可以与上一步发生在同一个周期。

前三步用一个状态机实现，代码如下（见 [step5.v](step5.v)）：

```verilog
   localparam FETCH_INSTR = 0;
   localparam FETCH_REGS  = 1;
   localparam EXECUTE     = 2;
   reg [1:0] state = FETCH_INSTR;
   always @(posedge clk) begin
	 case(state)
	   FETCH_INSTR: begin
	      instr <= MEM[PC];
	      state <= FETCH_REGS;
	   end
	   FETCH_REGS: begin
	      rs1 <= RegisterBank[rs1Id];
	      rs2 <= RegisterBank[rs2Id];
	      state <= EXECUTE;
	   end
	   EXECUTE: begin
	      PC <= PC + 1;
	      state <= FETCH_INSTR;
	   end
	 endcase
      end
   end
```

第四步（寄存器写回）在这个块里实现：

```verilog
   wire [31:0] writeBackData = ... ;
   wire writeBackEn = ...;
   always @posedge(clk) begin
      if(writeBackEn && rdId != 0) begin
          RegisterBank[rdId] <= writeBackData;
      end
   end
```

记住：写寄存器 0 没有效果（所以有 `rdId != 0` 这个判断）。
当 `writeBackData` 需要写入 `rdId` 时，`writeBackEn` 会拉高。`writeBackData` 会来自 ALU，下一步会讲。

**试一试** 在仿真和板子上运行 [step5.v](step5.v)。你会看到“准 CPU”的状态机在 LED 上跳华尔兹（LED 显示当前状态）。

## 第 6 步：ALU

现在我们能从内存取指、译码并读取寄存器值了，但（准）CPU 仍然什么都做不了。接下来看看如何对寄存器值做实际计算。

_所以，你是不是要做一个 `ALU` 模块？顺便问问，为什么不做一个 `Decoder` 模块和一个 `RegisterBank` 模块？_

我最早的设计用了一堆模块和文件，总代码约 1000 行。后来 Matthias Koch 写了一个单体版本，只有 200 行左右。不仅更紧凑，而且当你把所有东西放在同一个地方时会更容易理解。
**经验法则：** 如果你的“盒子与盒子之间的线”比盒子里面的电路还多，那你可能分太多模块（盒子）了！

_但是等等，模块化设计不是好事吗？_

模块化设计既不“好”也不“坏”，只有当它能让事情更简单时才有意义。在这里并不是这样。当然也没有绝对答案，这取决于个人口味与风格。本教程采用（基本）单体设计。

现在我们想先实现两类指令：
- R-type：`rd` <- `rs1` `OP` `rs2`（由 `isALUreg` 识别）
- I-type：`rd` <- `rs1` `OP` `Iimm`（由 `isALUimm` 识别）

ALU 有两个输入 `aluIn1` 与 `aluIn2`，计算 `aluIn1 OP aluIn2` 并输出到 `aluOut`：

```verilog
   wire [31:0] aluIn1 = rs1;
   wire [31:0] aluIn2 = isALUreg ? rs2 : Iimm;
   reg [31:0] aluOut;
```

根据指令类型，`aluIn2` 要么是第二个源寄存器 `rs2` 的值，要么是 I-type 格式的立即数 `Iimm`。运算 `OP` 主要由 `funct3` 决定（也会用到 `funct7`）。请把 [RISC-V 参考手册](https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf) 的第 130 页打开放在旁边：

| funct3  | 运算                                          |
|--------|-----------------------------------------------|
| 3'b000 | `ADD` 或 `SUB`                                |
| 3'b001 | 左移                                           |
| 3'b010 | 有符号比较（<）                               |
| 3'b011 | 无符号比较（<）                               |
| 3'b100 | `XOR`                                         |
| 3'b101 | 逻辑右移或算术右移（带符号扩展）               |
| 3'b110 | `OR`                                          |
| 3'b111 | `AND`                                         |

- 对于 `ADD`/`SUB`：如果是 `ALUreg`（R-type），则通过检查 `funct7` 的第 5 位区分 `ADD` 与 `SUB`（1 表示 `SUB`）。如果是 `ALUimm`（I-type），只能是 `ADD`。在这个上下文里，可以通过检查 `instr` 的第 5 位来区分 `ALUreg`（为 1）与 `ALUimm`（为 0）。
- 对于逻辑/算术右移：同样通过 `funct7[5]` 区分，1 表示算术右移（带符号扩展），0 表示逻辑右移。
- 移位量（shift amount）：对 `ALUreg` 来自 `rs2` 的低 5 位，对 `ALUimm` 来自 `instr[24:20]`（也就是 `rs2Id` 那几位）。

把这些合在一起，可以得到如下 ALU 的 VERILOG 代码：

```verilog
   reg [31:0] aluOut;
   wire [4:0] shamt = isALUreg ? rs2[4:0] : instr[24:20]; // shift amount
   always @(*) begin
      case(funct3)
	3'b000: aluOut = (funct7[5] & instr[5]) ? (aluIn1-aluIn2) : (aluIn1+aluIn2);
	3'b001: aluOut = aluIn1 << shamt;
	3'b010: aluOut = ($signed(aluIn1) < $signed(aluIn2));
	3'b011: aluOut = (aluIn1 < aluIn2);
	3'b100: aluOut = (aluIn1 ^ aluIn2);
	3'b101: aluOut = funct7[5]? ($signed(aluIn1) >>> shamt) : (aluIn1 >> shamt);
	3'b110: aluOut = (aluIn1 | aluIn2);
	3'b111: aluOut = (aluIn1 & aluIn2);
      endcase
   end
```

_注意：_ 虽然 `aluOut` 被声明为 `reg`，但它仍会综合成组合逻辑（不会生成触发器），因为它在 `always @(*)` 组合块里被赋值，并且 `case` 把所有情况都覆盖了。

寄存器写回的配置如下：

```verilog
   assign writeBackData = aluOut;
   assign writeBackEn = (state == EXECUTE && (isALUreg || isALUimm));
```

**试一试** 在仿真与板子上运行 [step6.v](step6.v)。仿真里会打印每次写回操作的目标寄存器和写入值。板子上会在 LED 上显示 `x1` 的最低 5 位。你可以修改程序并观察寄存器值如何变化。

**你在这里！** 这是你需要实现的指令清单：现在你的“准 RISC-V”已经支持其中的 20 条。下一步：跳转，然后分支，然后……其余的。继续之前你可能已经注意到：把 RISC-V 程序手工翻成二进制（也就是手工汇编）非常痛苦。下一节会给出一个更舒服的方案。

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [ ] 2 | [ ] 6  | [ ] | [ ]   | [ ] 5 | [ ] 3 | [*] 1  |

## 第 7 步：使用 VERILOG 汇编器

为了避免手工把 RISC-V 汇编翻成二进制，我们可以用 GNU 汇编器生成二进制文件，再把它转换为十六进制，并用 VERILOG 的 `readmemh()` 初始化内存。后面会讲如何做。

但在我们的场景里，更方便的是：把小段汇编程序直接写在同一个 VERILOG 文件里。实际上可以做到：用 tasks 与 functions 在 VERILOG 里实现一个 RISC-V 汇编器，如 [riscv_assembly.v](riscv_assembly.v) 所示。

在 [step7.v](step7.v) 中，内存初始化成与 [step6.v](step6.v) 相同的汇编程序。现在它看起来像这样，读起来舒服多了，对吧？

```verilog
   `include "riscv_assembly.v"
   initial begin
      ADD(x0,x0,x0);
      ADD(x1,x0,x0);
      ADDI(x1,x1,1);
      ADDI(x1,x1,1);
      ADDI(x1,x1,1);
      ADDI(x1,x1,1);
      ADD(x2,x1,x0);
      ADD(x3,x1,x2);
      SRLI(x3,x3,3);
      SLLI(x3,x3,31);
      SRAI(x3,x3,5);
      SRLI(x1,x3,26);
      EBREAK();
   end
```

_注意：_ `riscv_assembly.v` 需要在使用汇编的模块内部被 `include`。

这一步我们还做了一个修改：前面步骤里 `PC` 是“当前指令的索引”。接下来我们希望 `PC` 表示“当前指令的地址”。由于每条指令是 32 位（4 字节），所以：
- `PC` 自增用 `PC <= PC + 4`（不再是 `PC <= PC + 1`）
- 取指用 `instr <= MEM[PC[31:2]];`（忽略 `PC` 的最低 2 位）

## 第 8 步：跳转

跳转指令有两条：`JAL`（jump and link）与 `JALR`（jump and link register）。所谓 “and link”，指的是可以把当前 PC 写入某个寄存器，因此 `JAL`/`JALR` 不仅能用来跳转，也能用来实现函数调用。两条指令应该实现如下语义：

| 指令              | 效果                    |
|------------------|-------------------------|
| JAL rd,imm       | rd<-PC+4; PC<-PC+Jimm   |
| JALR rd,rs1,imm  | rd<-PC+4; PC<-rs1+Iimm  |

为了实现它们，我们需要对内核做几处改动。首先是写回数据：跳转类指令写回的值可能是 `PC+4`，而不是 `aluOut`：

```verilog
   assign writeBackData = (isJAL || isJALR) ? (PC + 4) : aluOut;
   assign writeBackEn = (state == EXECUTE &&
			 (isALUreg ||
			  isALUimm ||
			  isJAL    ||
			  isJALR)
			 );
```

我们还需要声明一个 `nextPC`，涵盖 3 种情况：

```verilog
   wire [31:0] nextPC = isJAL  ? PC+Jimm :
	                isJALR ? rs1+Iimm :
	                PC+4;
```

然后在状态机里，把 `PC <= PC + 4;` 替换成 `PC <= nextPC;` 就可以了！

现在我们可以用一个简单的（无限）循环来测试新实现的跳转指令：

```verilog
`include "riscv_assembly.v"
      integer L0_=4;
      initial begin
	 ADD(x1,x0,x0);
      Label(L0_);
	 ADDI(x1,x1,1);
	 JAL(x0,LabelRef(L0_));
	 EBREAK();
	 endASM();
      end
```

整数 `L0_` 是一个标签。和真实汇编器不同，我们需要手工指定 `L0_` 的值。这里很容易：`L0_` 就在第一条指令之后，因此它对应 RAM 起始地址（0）加上一条 32 位指令的大小（4），也就是 4。
对于更长且有很多标签的程序，你可以让标签先不初始化（`integer L0_;`），第一次跑仿真时它会计算并打印出应该使用的标签值。这样做不算特别方便，但依然比手工汇编/手工算标签好太多。

`LabelRef()` 会计算相对当前 PC 的标签偏移。此外，在仿真模式下，它会显示当前地址（用来初始化标签）。如果标签已经被初始化（比如这里 `L0_=4`），它还会检查这个值是否与汇编器生成的当前地址一致。如果不一致，`endASM()` 会打印错误并退出。

_注 1：_ 我习惯在程序末尾总是放一条 `EBREAK()`。这里即使是无限循环也不需要，但如果我后来决定跳出循环，`EBREAK()` 已经在那儿了。

_注 2：_ `endASM();` 会检查所有标签的有效性，一旦发现无效标签就退出仿真。因此如果你使用 RISC-V VERILOG 汇编器，强烈建议你在综合前先跑仿真（因为综合时无法做这种检查）。

**试一试** 在仿真与板子上运行 [step8.v](step8.v)。是的，走到第 8 步，我们做出来的看起来仍然只是另一个“傻 blinky”！但这次它在执行一个真实的 RISC-V 程序！虽然还不是完整的 RISC-V 内核，但已经很有 RISC-V 的味道了。耐心一点，很快我们就能跑一些比 blinky 有趣得多的程序。

**你在这里！**
还有不少工作，但我们在稳步前进。

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [*] 2 | [ ] 6  | [ ] | [ ]   | [ ] 5 | [ ] 3 | [*] 1  |

**试一试** 在循环前面加几条指令，先在仿真里跑；按仿真器提示修正 label；再跑一次仿真；最后跑到板子上。

## 第 9 步：分支

分支类似跳转，不同之处在于它会比较两个寄存器，并根据比较结果更新 `PC`。另一个不同点是：分支相对 `PC` 的可达范围更小（12 位偏移）。分支共有 6 条指令：

| 指令               | 效果                                                  |
|-------------------|-------------------------------------------------------|
| BEQ rs1,rs2,imm   | if(rs1 == rs2) PC <- PC+Bimm                          |
| BNE rs1,rs2,imm   | if(rs1 != rs2) PC <- PC+Bimm                          |
| BLT rs1,rs2,imm   | if(rs1 <  rs2) PC <- PC+Bimm（有符号比较）            |
| BGE rs1,rs2,imm   | if(rs1 >= rs2) PC <- PC+Bimm（有符号比较）            |
| BLTU rs1,rs2,imm  | if(rs1 <  rs2) PC <- PC+Bimm（无符号比较）            |
| BGEU rs1,rs2,imm  | if(rs1 >= rs2) PC <- PC+Bimm（无符号比较）            |

_等等：_ 有 `BLT`，那 `BGT` 呢？还是 RISC-V 的同一个原则：能用已有功能实现就不要增加新功能。在这里，
`BGT rs1,rs2,imm` 等价于 `BLT rs2,rs1,imm`（交换前两个操作数即可）。在 RISC-V 汇编里写 `BGT` 也能工作（汇编器会替换成交换操作数后的 `BLT`）。`BGT` 称为“伪指令”（pseudo-instruction）。后面会看到很多伪指令用来简化汇编编程。

回到分支指令实现：我们需要在 ALU 中加入一些连线来计算比较结果：

```verilog
   reg takeBranch;
   always @(*) begin
      case(funct3)
	3'b000: takeBranch = (rs1 == rs2);
	3'b001: takeBranch = (rs1 != rs2);
	3'b100: takeBranch = ($signed(rs1) < $signed(rs2));
	3'b101: takeBranch = ($signed(rs1) >= $signed(rs2));
	3'b110: takeBranch = (rs1 < rs2);
	3'b111: takeBranch = (rs1 >= rs2);
	default: takeBranch = 1'b0;
      endcase
```

_注 1：_ 之后我们会看到如何做一个更紧凑的 ALU，综合后 LUT 用量会显著减少。但现在目标是先做出一个能工作、尽量容易理解的 RISC-V 处理器，优化先放一放。

_注 2：_ `funct3` 8 种可能里只有 6 种被分支使用，因此 `case` 里必须有 `default:`，否则综合器可能无法保持 `takeBranch` 为纯组合逻辑（会推导出锁存器），这不是我们想要的。

实现分支剩下唯一要做的事，就是把 `nextPC` 加一个分支情况：

```verilog
   wire [31:0] nextPC = (isBranch && takeBranch) ? PC+Bimm :
   	                isJAL                    ? PC+Jimm :
	                isJALR                   ? rs1+Iimm :
	                PC+4;
```

现在可以用下面的程序测试一个简单循环：从 0 数到 31，每次迭代把值显示在 LED 上（注意 LED 连的是 `x1`），然后停止：

```c++
`include "riscv_assembly.v"
      integer L0_ = 8;

      initial begin
         ADD(x1,x0,x0);
         ADDI(x2,x0,32);
      Label(L0_);
	 ADDI(x1,x1,1);
         BNE(x1, x2, LabelRef(L0_));
         EBREAK();

	 endASM();
      end
```

**试一试** 在仿真与板子上运行 [step9.v](step9.v)。试着修改程序：写一个 “Knight Rider” 扫描灯，包含一个外层循环和两个内层循环（一个从左到右，一个从右到左）。

**你在这里！**
哇，我们已经实现了 38 条指令里的 28 条！继续……

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [*] 2 | [ *] 6 | [ ] | [ ]   | [ ] 5 | [ ] 3 | [*] 1  |

## 第 10 步：LUI 与 AUIPC

我们还有两条“怪指令”需要实现。它们做什么？其实很简单：

| 指令            | 效果                 |
|----------------|----------------------|
| LUI rd, imm    | rd <= Uimm           |
| AUIPC rd, imm  | rd <= PC + Uimm      |

如果你看 `Uimm` 的格式，会发现它把立即数放在高位（`imm[31:12]`），低 12 位全是 0。这两条指令非常有用：其它立即数格式只能修改低位。配合 `LUI`/`AUIPC`，可以把任意 32 位常量装载进寄存器（最多两条指令）。

实现它们只需要修改 `writeBackData` 与 `writeBackEn`：

```verilog
   assign writeBackData = (isJAL || isJALR) ? (PC + 4) :
			  (isLUI) ? Uimm :
			  (isAUIPC) ? (PC + Uimm) :
			  aluOut;

   assign writeBackEn = (state == EXECUTE &&
			 (isALUreg ||
			  isALUimm ||
			  isJAL    ||
			  isJALR   ||
			  isLUI    ||
			  isAUIPC)
			 );
```

**你在这里！**
看起来我们快完成了！还剩 8 条指令……

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [*] 2 | [ *] 6 | [*] | [*]   | [ ] 5 | [ ] 3 | [*] 1  |

**试一试** 在仿真与板子上运行 [step10.v](step10.v)。

_啊！！_ 在我的 icestick 上居然放不下（需要 1283 个 LUT，而 IceStick 只有 1280）。怎么办？记住：我们到目前为止完全没考虑资源占用，只是先把“能工作”的设计写出来。其实我们的设计还有 _大量_ 优化空间，后面会讲。但在缩小处理器之前，我们先把 SOC 的结构整理得更清晰一点（然后再把处理器缩小）。

## 第 11 步：把内存拆成单独模块

在前面的设计里，我们把所有东西都塞在 `SOC` 模块里（内存 + 处理器）。这一步我们把它们拆开。

先看 `Memory` 模块：

```verilog
module Memory (
   input             clk,
   input      [31:0] mem_addr,  // address to be read
   output reg [31:0] mem_rdata, // data read from memory
   input   	     mem_rstrb  // goes high when processor wants to read
);
   reg [31:0] MEM [0:255];

`include "riscv_assembly.v"
   integer L0_=8;
   initial begin
                  ADD(x1,x0,x0);
                  ADDI(x2,x0,31);
      Label(L0_); ADDI(x1,x1,1);
                  BNE(x1, x2, LabelRef(L0_));
                  EBREAK();
      endASM();
   end

   always @(posedge clk) begin
      if(mem_rstrb) begin
         mem_rdata <= MEM[mem_addr[31:2]];
      end
   end
endmodule
```

接口里有 `clk` 连接系统时钟。处理器想读内存时，把地址放到 `mem_addr` 上并把 `mem_rstrb` 置 1；随后 `Memory` 把读出的数据放到 `mem_rdata`。

对称地，`Processor` 模块里 `mem_addr` 变成输出，`mem_rdata` 变成输入，`mem_rstrb` 也变成输出：

```verilog
module Processor (
    input 	      clk,
    input 	      resetn,
    output     [31:0] mem_addr,
    input      [31:0] mem_rdata,
    output 	      mem_rstrb,
    output reg [31:0] x1
);
...
endmodule
```

（我们还加了一个 `x1` 输出，用来把寄存器 `x1` 的内容导出来做可视化调试，把它连到 LED 上。）

状态机多了一个状态：

```verilog
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam FETCH_REGS  = 2;
   localparam EXECUTE     = 3;

   case(state)
     FETCH_INSTR: begin
       state <= WAIT_INSTR;
     end
     WAIT_INSTR: begin
       instr <= mem_rdata;
       state <= FETCH_REGS;
     end
     FETCH_REGS: begin
       rs1 <= RegisterBank[rs1Id];
       rs2 <= RegisterBank[rs2Id];
       state <= EXECUTE;
     end
     EXECUTE: begin
        if(!isSYSTEM) begin
  	   PC <= nextPC;
	end
	state <= FETCH_INSTR;
      end
   endcase
```

_注意：_ 之后我们会看到如何简化并回到 3 状态。

现在 `mem_addr` 与 `mem_rstrb` 可以这样驱动：

```verilog
   assign mem_addr = PC;
   assign mem_rstrb = (state == FETCH_INSTR);
```

最后在 `SOC` 里把它们连起来：

```verilog
module SOC (
    input  CLK,        // system clock
    input  RESET,      // reset button
    output [4:0] LEDS, // system LEDs
    input  RXD,        // UART receive
    output TXD         // UART transmit
);
   wire    clk;
   wire    resetn;
   Memory RAM(
      .clk(clk),
      .mem_addr(mem_addr),
      .mem_rdata(mem_rdata),
      .mem_rstrb(mem_rstrb)
   );

   wire [31:0] mem_addr;
   wire [31:0] mem_rdata;
   wire mem_rstrb;
   wire [31:0] x1;
   Processor CPU(
      .clk(clk),
      .resetn(resetn),
      .mem_addr(mem_addr),
      .mem_rdata(mem_rdata),
      .mem_rstrb(mem_rstrb),
      .x1(x1)
   );
   assign LEDS = x1[4:0];

   // Gearbox and reset circuitry.
   Clockworks #(
     .SLOW(19) // Divide clock frequency by 2^19
   ) CW (
     .CLK(CLK),
     .RESET(RESET),
     .clk(clk),
     .resetn(resetn)
   );

   assign TXD  = 1'b0; // not used for now
endmodule
```

现在在仿真里运行 [step11.v](step11.v)。如预期，它和上一节做同样的事（LED 上从 0 数到 31 然后停止）。那跑到板子上呢？
更糟了：1341 LUT（而 IceStick 只有 1280）。所以我们得开始缩小它！

## 第 12 步：体积优化：不可思议的缩小内核

_致敬经典电影 “The Incredible Shrinking Man”_

缩小内核有很多办法。先看 ALU：它需要支持加法、减法与比较。能不能复用减法结果来做比较？当然可以。但要做到这一点，我们需要做一次 33 位减法，并检查符号位。Matthias Koch（@Mecrisp）告诉了我这个技巧，它也用在 swapforth/J1（另一个能在 IceStick 上运行的小 RISC 内核）里。33 位减法可以写成：

```verilog
   wire [32:0] aluMinus = {1'b0,aluIn1} - {1'b0,aluIn2};
```

如果你想知道 Verilog 里的 `A-B` 实际做了什么：它相当于 `A+~B+1`（先逐位取反 B 再相加并加 1），这正是二进制补码减法的原理。例如 `4'b0000 - 4'b0001` 的结果是 `-1`，用 4 位补码编码为 `4'b1111`。按公式计算：`4'b0000 + ~4'b0001 + 1 = 4'b0000 + 4'b1110 + 1 = 4'b1111`。因此我们保留下面这个表达式（虽然上面的更直观，但知道底层发生了什么挺有意思）：

```verilog
   wire [32:0] aluMinus = {1'b1, ~aluIn2} + {1'b0,aluIn1} + 33'b1;
```

接着我们构造三个比较信号（这样能省掉三个 32 位加法器）：

```
   wire        EQ  = (aluMinus[31:0] == 0);
   wire        LTU = aluMinus[32];
   wire        LT  = (aluIn1[31] ^ aluIn2[31]) ? aluIn1[31] : aluMinus[32];
```

- `EQ`：当 `aluIn1` 与 `aluIn2` 相等时为 1（也就是 `aluMinus == 0`，无需看第 33 位）。
- `LTU`：无符号比较结果，由 33 位减法的符号位给出。
- `LT`：有符号比较分两种情况：若符号不同，则 `aluIn1` 为负时 `LT=1`；否则由 33 位减法的符号位给出。

当然加法还是需要一个加法器：

```verilog
   wire [31:0] aluPlus = aluIn1 + aluIn2;
```

然后 `aluOut` 可以这样计算：

```verilog
   reg [31:0]  aluOut;
   always @(*) begin
      case(funct3)
	3'b000: aluOut = (funct7[5] & instr[5]) ? aluMinus[31:0] : aluPlus;
	3'b001: aluOut = aluIn1 << shamt;;
	3'b010: aluOut = {31'b0, LT};
	3'b011: aluOut = {31'b0, LTU};
	3'b100: aluOut = (aluIn1 ^ aluIn2);
	3'b101: aluOut = funct7[5]? ($signed(aluIn1) >>> shamt) :
			 ($signed(aluIn1) >> shamt);
	3'b110: aluOut = (aluIn1 | aluIn2);
	3'b111: aluOut = (aluIn1 & aluIn2);
      endcase
   end
```

在 IceStick 上试试看：可以了！1167 LUT，能放下！但别急着停，还有不少缩小空间。再看 `takeBranch`：能不能复用 `EQ`,`LT`,`LTU`？当然可以：

```verilog
   reg takeBranch;
   always @(*) begin
      case(funct3)
	3'b000: takeBranch = EQ;
	3'b001: takeBranch = !EQ;
	3'b100: takeBranch = LT;
	3'b101: takeBranch = !LT;
	3'b110: takeBranch = LTU;
	3'b111: takeBranch = !LTU;
	default: takeBranch = 1'b0;
      endcase
   end
```

要让它生效，还需要确保分支指令时 `rs2` 也被送到 ALU 的第二输入：

```verilog
   wire [31:0] aluIn2 = isALUreg | isBranch ? rs2 : Iimm;
```

在板子上现在多少 LUT？1094。还不错，但继续：`JALR` 的目标地址是 `rs1+Iimm`，我们居然为它单独造了一个加法器，而 ALU 本来就算得出来。把它复用掉：

```verilog
   wire [31:0] nextPC = ((isBranch && takeBranch) || isJAL) ? PCplusImm  :
	                isJALR                              ? {aluPlus[31:1],1'b0}:
	                PCplus4;
```

现在是 1030 LUT。最大的吃 LUT 大户其实是移位器。我们的 ALU 里有三个（左移、逻辑右移、算术右移）。Matthias Koch（@mecrisp）指出可以把两个右移合并：做一个 33 位移位器，多出来的 1 位根据输入的 bit31 和移位类型决定是 0 还是 1：

```verilog
   wire [31:0] shifter =
               $signed({instr[30] & aluIn1[31], shifter_in}) >>> aluIn2[4:0];
```

更进一步：甚至可以用一个移位器同时支持左移与右移：如果是左移，就把输入比特翻转、移位后再把输出翻转回来：

```verilog
   wire [31:0] shifter_in = (funct3 == 3'b001) ? flip32(aluIn1) : aluIn1;
   wire [31:0] leftshift = flip32(shifter);
```

此时 ALU 变成这样：

```verilog
   reg [31:0]  aluOut;
   always @(*) begin
      case(funct3)
	3'b000: aluOut = (funct7[5] & instr[5]) ? aluMinus[31:0] : aluPlus;
	3'b001: aluOut = leftshift;
	3'b010: aluOut = {31'b0, LT};
	3'b011: aluOut = {31'b0, LTU};
	3'b100: aluOut = (aluIn1 ^ aluIn2);
	3'b101: aluOut = shifter;
	3'b110: aluOut = (aluIn1 | aluIn2);
	3'b111: aluOut = (aluIn1 & aluIn2);
      endcase
   end
```

现在多少 LUT？887！

_注 1：_ 移位还可以更省：每个时钟只移 1 位，把 ALU 做成多周期。这样 ALU 会复杂一点，但会小很多（Femtorv32-quark 用了这个技巧）。后面会讲。

_注 2：_ 如果 ALU 是多周期的，我们甚至可以只用一个 33 位加法器：用三个周期完成减法（分别计算 `~aluIn2`、`aluIn1+(~aluIn2)`、`aluIn1+(~aluIn2)+1`）。

在继续之前，还有一个容易的优化：把用于地址计算的加法器做因式分解：

```verilog
   wire [31:0] PCplusImm = PC + ( instr[3] ? Jimm[31:0] :
				  instr[4] ? Uimm[31:0] :
				             Bimm[31:0] );
   wire [31:0] PCplus4 = PC+4;
```

然后 `nextPC` 与 `writeBackData` 都能复用这两个加法器：

```verilog

   assign writeBackData = (isJAL || isJALR) ? (PCplus4) :
			  (isLUI) ? Uimm :
			  (isAUIPC) ? PCplusImm :
			  aluOut;

   assign writeBackEn = (state == EXECUTE && !isBranch);

   wire [31:0] nextPC = (isBranch && takeBranch || isJAL) ? PC+Imm  :
	                isJALR                   ? {aluPlus[31:1],1'b0} :
	                PCplus;
```

结果呢？839 LUT（又省了 50 个左右）。当然还可以继续省（例如移位多周期化、地址计算用更少位宽），但现在已经足够给后面的步骤留出空间了，我们先继续往下。

## 第 13 步：子程序（版本 1：只用基础 RISC-V 指令）

好，现在我们有了一个（仍不完整但已经很像样的）RISC-V 处理器和一个 SOC，并且它们能放进 IceStick。回忆一下，我们只剩 8 条指令没实现了（5 个 Load 变体、3 个 Store 变体）。

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [*] 2 | [ *] 6 | [*] | [*]   | [ ] 5 | [ ] 3 | [*] 1  |

在动手实现它们之前，我们先多了解一点 RISC-V 汇编与函数调用。到目前为止我们一直用 gearbox 来把 CPU 放慢，以便观察程序运行。能不能改成实现一个 `wait` 函数并调用它呢？来试试。

第一件事是把 `Clockworks` 实例化里的 `#(.SLOW(nnn))` 参数去掉：

```verilog
   Clockworks CW(
     .CLK(CLK),
     .RESET(RESET),
     .clk(clk),
     .resetn(resetn)
   );
```

这样就不再生成分频器，而是把开发板的 `CLK` 直接连到内部 `clk`。

好，现在我们需要搞清楚两件事：
- 如何写一个“等待一段时间”的函数
- 如何调用它

_等等_ 你在说函数调用，但我们还没有 `Load` / `Store` 指令。我们没法把返回地址压栈（栈在内存里，而我们还读写不了内存），那怎么调用函数？

其实用 RISC-V 指令实现函数调用有很多方式。为了确保大家采用同一套约定，RISC-V 定义了 **应用二进制接口**（ABI），规定了如何调用函数、如何传参、哪些寄存器用于什么目的。更多细节见
[这个文档](https://github.com/riscv-non-isa/riscv-asm-manual/blob/master/riscv-asm.md)。

**调用函数** ABI 规定返回地址保存在 `x1`，因此可以用 `JAL(x1,offset)` 调用函数，其中 `offset` 是（有符号）“当前 PC 与被调用函数地址的差”。这要求偏移能放进 20 位（Jimm 格式）。
_注：_ 对于更远的函数，可以用 `AUIPC` 与 `JALR` 组合到达任意偏移。

**函数返回** 则跳回 `x1` 中保存的地址，可以用 `JALR(x0,x1,0)`。

**函数参数与返回值** 前 6 个参数通过 `x10`..`x16` 传递，返回值放在 `x10`（会覆盖第一个参数）。

这很有意思：虽然我们还没有 `Load`/`Store`，但仍然能写“有函数”的程序；只是我们没法写“函数里面再调用别的函数”的函数，因为那需要把 `x1` 保存到栈上（当然也可以把 `x1` 临时放到另一个寄存器里，但很快会变得混乱，所以这里不这么做）。

还有个小细节：我们刚学到 ABI 里 `x1` 是返回地址，而之前我们把 `x1` 连到了 LED 上。既然要遵守 ABI，就得换一个寄存器。从现在开始，我们把 `x10` 连到 LED。

好，现在我们可以再写一个“新的 blinky”了：选一个 `slow_bit` 常量，写一个 `wait` 函数计数到 `2^slow_bit`，并在主循环里调用它来减速：

```verilog
`ifdef BENCH
   localparam slow_bit=15;
`else
   localparam slow_bit=19;
`endif


`include "riscv_assembly.v"
   integer L0_   = 4;
   integer wait_ = 20;
   integer L1_   = 28;

   initial begin
      ADD(x10,x0,x0);
   Label(L0_);
      ADDI(x10,x10,1);
      JAL(x1,LabelRef(wait_)); // call(wait_)
      JAL(zero,LabelRef(L0_)); // jump(l0_)

      EBREAK(); // I keep it systematically
               // here in case I change the program.

   Label(wait_);
      ADDI(x11,x0,1);
      SLLI(x11,x11,slow_bit);
   Label(L1_);
      ADDI(x11,x11,-1);
      BNE(x11,x0,LabelRef(L1_));
      JALR(x0,x1,0);

      endASM();
   end

   always @(posedge clk) begin
      if(mem_rstrb) begin
         mem_rdata <= MEM[mem_addr[31:2]];
      end
   end
endmodule
```

在仿真与板子上运行 [step13.v](step13.v)。

**试一试** 做一个 Knight Rider 扫描灯：一个子程序负责从左到右，另一个负责从右到左，再加上 `wait` 子程序。_提示：_ 你需要把 `x1` 先保存到另一个寄存器里。

## 第 14 步：子程序（版本 2：使用 RISC-V ABI 与伪指令）

有了 ABI，我们有了一套标准的写程序方式，但需要记住的事情也不少：
- 虽然所有寄存器本质一样，但 ABI 规定了某些寄存器承担某些任务（例如 `x1` 保存返回地址，`x10`..`x16` 传参等）；
- 函数调用用 `JAL` 或 `AUIPC`+`JALR`，函数返回用 `JALR`。

在 CISC 处理器上，常常有专门的 `CALL` 与 `RET` 指令；寄存器也常常“看起来”是专用的（返回地址、栈指针、参数寄存器）。这让程序员更省心，因为需要记住的规则少一些。对 RISC 也没有理由不这么做：我们可以“假装”寄存器不一样，给它们起不同的别名。这些别名在
[这里](https://github.com/riscv-non-isa/riscv-asm-manual/blob/master/riscv-asm.md#general-registers) 列出来了。

| ABI 名称           | 寄存器 | 用途                                       |
|-------------------|--------|--------------------------------------------|
| `zero`            | `x0`   | 读为 0，写入忽略                            |
| `ra`              | `x1`   | 返回地址                                   |
| `t0`...`t6`       | ...    | 临时寄存器                                 |
| `fp`,`s0`...`s11` | ...    | 保存寄存器，`fp`=`so`：帧指针               |
| `a0`...`a7`       | ...    | 函数参数与返回值（`a0`）                    |
| `sp`              | `x2`   | 栈指针                                     |
| `gp`              | `x3`   | 全局指针                                   |

保存寄存器（`s0`...`s11`）按约定应当被函数保留，或由函数负责保存/恢复。你可以把局部变量放在这些寄存器里。如果你写一个函数且使用了它们，按 ABI 你应该把用到的保存寄存器压栈，并在返回前弹栈恢复。

其它寄存器则不能指望在函数调用前后保持不变。

全局指针 `gp` 可以作为“捷径”来在一条指令内访问较远的内存区域。我们后面会看到（等我们有了 `Load`/`Store`）。

在我们的 VERILOG 汇编器 [riscv_assembly.v](riscv_assembly.v) 里，只需要声明这些寄存器别名：

```verilog
   localparam zero = x0;
   localparam ra   = x1;
   localparam sp   = x2;
   localparam gp   = x3;
   ...
   localparam t4   = x29;
   localparam t5   = x30;
   localparam t6   = x31;
```

除了别名，还有很多常用任务对应的 _伪指令_，例如：

| 伪指令                | 作用                                    |
|----------------------|-----------------------------------------|
| `LI(rd,imm)`         | 在寄存器里装载一个 32 位常量              |
| `CALL(offset)`       | 调用函数                                |
| `RET()`              | 从函数返回                              |
| `MV(rd,rs)`          | 等价于 `ADD(rd,rs,zero)`                |
| `NOP()`              | 等价于 `ADD(zero,zero,zero)`            |
| `J(offset)`          | 等价于 `JAL(zero,offset)`               |
| `BEQZ(rd1,offset)`   | 等价于 `BEQ(rd1,x0,offset)`             |
| `BNEZ(rd1,offset)`   | 等价于 `BNE(rd1,x0,offset)`             |
| `BGT(rd1,rd2,offset)`| 等价于 `BLT(rd2,rd1,offset)`            |

如果常量在 [-2048,2047] 范围内，`LI` 用 `ADDI(rd,x0,imm)` 实现；否则用 `LUI`+`ADDI` 组合实现（如果你想了解细节，可看 [这个 stackoverflow 回答](https://stackoverflow.com/questions/50742420/risc-v-build-32-bit-constants-with-lui-and-addi)，里面涉及符号扩展的一些微妙点）。

使用 ABI 别名与伪指令后，我们的程序可以写成：

```verilog
   integer L0_   = 4;
   integer wait_ = 24;
   integer L1_   = 32;

   initial begin
      LI(a0,0);
   Label(L0_);
      ADDI(a0,a0,1);
      CALL(LabelRef(wait_));
      J(LabelRef(L0_));

      EBREAK();

   Label(wait_);
      LI(a1,1);
      SLLI(a1,a1,slow_bit);
   Label(L1_);
      ADDI(a1,a1,-1);
      BNEZ(a1,LabelRef(L1_));
      RET();

      endASM();
   end
```

它看起来改进不算巨大，但在更长的程序里，清晰度会好很多：它更能体现程序员的意图（这里是函数调用，那里是跳到标签等）。否则所有指令都长得差不多，读起来会更困难。

这很有趣：RISC-V 标准本身的指令集非常简单，但直接用它写程序并不轻松；于是 ABI 与伪指令“假装”指令集更复杂一点，让程序员更省心。同时 ABI 也确保了不同人、不同语言写出来的函数可以互相调用。后面我们会看到如何用 GNU 汇编器与 C 编译器为我们的 CPU 编译程序。

但在开始玩软件与工具链之前，别忘了硬件还剩 8 条指令要实现（5 条 `Load` 变体与 3 条 `Store` 变体）。

**试一试** 发明一个（或从 [别处](https://github.com/riscv-collab/riscv-gcc/blob/5964b5cd72721186ea2195a7be8d40cfe6554023/libgcc/config/riscv/muldi3.S) 抄一个）两数相乘的例程，在仿真里用不同输入测试它，然后在板子上跑跑看。

## 第 15 步：Load（读内存）

现在来看 load 指令的实现。load 一共有 5 条：

| 指令            | 效果                                                            |
|----------------|-----------------------------------------------------------------|
| LW(rd,rs1,imm) | 从地址 (rs1+imm) 读取一个 word 到 rd                             |
| LBU(rd,rs1,imm)| 从地址 (rs1+imm) 读取一个 byte 到 rd（零扩展）                   |
| LHU(rd,rs1,imm)| 从地址 (rs1+imm) 读取一个 half-word 到 rd（零扩展）              |
| LB(rd,rs1,imm) | 读取一个 byte 到 rd，并做符号扩展                                 |
| LH(rd,rs1,imm) | 读取一个 half-word 到 rd，并做符号扩展                            |

_注意_：`LW` 的地址要求按 word 边界对齐（4 的倍数），`LH`/`LHU` 要求按 halfword 边界对齐（2 的倍数）。这对我们是好事，能让实现容易很多……

但仍然有不少工作要做！首先要做一段电路来决定最终装载的值（我们称它为 `LOAD_data`）。

如上所示，我们可以装载 word、halfword、byte，而 halfword/byte 又分两种：是否符号扩展。

例如一个有符号 byte 值 `-1`，编码为 `8'b11111111`。用 `LBU` 装载到 32 位寄存器，会得到 `32'b0000000000000000000000011111111`；而用 `LB` 装载则会得到 `32'b11111111111111111111111111111111`，也就是 32 位的 `-1`。

因此我们有一个“二维”的分支：装载 byte/halfword/word，以及是否符号扩展。但其实更复杂：我们的内存按 word 组织，因此装载 byte 时要知道它是该 word 的哪个 byte（4 个之一），装载 halfword 时要知道是哪个 halfword（2 个之一）。这可以通过查看数据地址（`rs1 + Iimm`）的低两位实现：

```verilog
   wire [31:0] loadstore_addr = rs1 + Iimm;
   wire [15:0] LOAD_halfword =
	       loadstore_addr[1] ? mem_rdata[31:16] : mem_rdata[15:0];

   wire  [7:0] LOAD_byte =
	       loadstore_addr[0] ? LOAD_halfword[15:8] : LOAD_halfword[7:0];
```

好，现在我们需要在 `mem_rdata`（`LW`）、`LOAD_halfword`（`LH`,`LHU`）与 `LOAD_byte`（`LB`,`LBU`）之间选择。查
[RISC-V 参考手册](https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf)
第 130 页，这由 `funct3` 的低两位决定：

```verilog
   wire mem_byteAccess     = funct3[1:0] == 2'b00;
   wire mem_halfwordAccess = funct3[1:0] == 2'b01;

   wire [31:0] LOAD_data =
         mem_byteAccess ? LOAD_byte     :
     mem_halfwordAccess ? LOAD_halfword :
                          mem_rdata     ;
```

接下来把符号扩展加进去。要写入 `rd` 高位的符号位 `LOAD_sign`，取决于是否需要符号扩展（`LB`,`LH`，即 `funct3[2]=0`），以及被装载值的最高位：

```verilog
   wire LOAD_sign =
	!funct3[2] & (mem_byteAccess ? LOAD_byte[7] : LOAD_halfword[15]);

   wire [31:0] LOAD_data =
         mem_byteAccess ? {{24{LOAD_sign}},     LOAD_byte} :
     mem_halfwordAccess ? {{16{LOAD_sign}}, LOAD_halfword} :
                          mem_rdata ;
```

呼——有点绕，但最终并不算太复杂。我最初的设计更复杂，Matthias Koch（@mecrisp）把它简化成了上面这种（而且确实还算容易理解）。

但我们还没完：现在要修改状态机。需要再加两个状态：`LOAD` 与 `WAIT_DATA`：

```verilog
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam FETCH_REGS  = 2;
   localparam EXECUTE     = 3;
   localparam LOAD        = 4;
   localparam WAIT_DATA   = 5;
   reg [2:0] state = FETCH_INSTR;
```

_注 1_：当然也可以用更少状态，但此处目标仍然是先做一个“能工作且尽量易懂”的版本。后面会讲怎么简化状态机。
_注 2_：别忘了 `state` 的位宽要够！（从之前的 `reg [1:0] state` 变为 `reg [2:0] state`）。

然后把新状态接入状态机：

```verilog
     ...
	   EXECUTE: begin
	      if(!isSYSTEM) begin
		 PC <= nextPC;
	      end
	      state <= isLoad ? LOAD : FETCH_INSTR;
	   end
	   LOAD: begin
	      state <= WAIT_DATA;
	   end
	   WAIT_DATA: begin
	      state <= FETCH_INSTR;
	   end

     ...
```

最后，驱动与内存接口相关的信号：`mem_addr`（读取地址）与 `mem_rstrb`（读请求）：

```verilog
   assign mem_addr = (state == WAIT_INSTR || state == FETCH_INSTR) ?
		     PC : loadstore_addr ;
   assign mem_rstrb = (state == FETCH_INSTR || state == LOAD);
```

现在用下面的程序测试新指令：

```verilog
   integer L0_   = 8;
   integer wait_ = 32;
   integer L1_   = 40;

   initial begin
      LI(s0,0);
      LI(s1,16);
   Label(L0_);
      LB(a0,s0,400); // LEDs are plugged on a0 (=x10)
      CALL(LabelRef(wait_));
      ADDI(s0,s0,1);
      BNE(s0,s1, LabelRef(L0_));
      EBREAK();

   Label(wait_);
      LI(t0,1);
      SLLI(t0,t0,slow_bit);
   Label(L1_);
      ADDI(t0,t0,-1);
      BNEZ(t0,LabelRef(L1_));
      RET();

      endASM();

      // Note: index 100 (word address)
      //     corresponds to
      // address 400 (byte address)
      MEM[100] = {8'h4, 8'h3, 8'h2, 8'h1};
      MEM[101] = {8'h8, 8'h7, 8'h6, 8'h5};
      MEM[102] = {8'hc, 8'hb, 8'ha, 8'h9};
      MEM[103] = {8'hff, 8'hf, 8'he, 8'hd};
   end
```

这个程序在地址 400 处初始化了 4 个 word 的数据，然后在循环里把它们逐个 byte 装载到 `a10`。还有一个延时循环（`wait` 函数）方便观察。

**试一试** 在仿真与板子上运行。测试其它装载指令。做一个可编程灯带（类似第 3 步）。

**你在这里！** 还剩 3 条指令就完成了！

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [*] 2 | [*] 6  | [*] | [*]   | [*] 5 | [ ] 3 | [*] 1  |

## 第 16 步：Store（写内存）

我们快到终点了，但还需要实现以下 3 条指令：

| 指令             | 效果                                        |
|-----------------|---------------------------------------------|
| SW(rs2,rs1,imm) | 把 rs2 写到地址 rs1+imm                      |
| SB(rs2,rs1,imm) | 把 rs2 的低 8 位写到地址 rs1+imm             |
| SH(rs2,rs1,imm) | 把 rs2 的低 16 位写到地址 rs1+imm            |

为此我们需要做三件事：
- 修改处理器与内存之间的接口，让处理器能写内存；
- 内存按 word 编址，每次写都会修改一个 word。但 `SB`/`SH` 需要能写单个字节，因此除了写入 word 数据外，还要计算写掩码（4 位 mask）决定哪些 byte 生效；
- 修改状态机。

`Memory` 模块需要这样修改：

``` verilog
module Memory (
   input             clk,
   input      [31:0] mem_addr,
   output reg [31:0] mem_rdata,
   input   	     mem_rstrb,
   input      [31:0] mem_wdata,
   input      [3:0]  mem_wmask
);

   reg [31:0] MEM [0:255];

   initial begin
      ...
   end

   wire [29:0] word_addr = mem_addr[31:2];
   always @(posedge clk) begin
      if(mem_rstrb) begin
         mem_rdata <= MEM[word_addr];
      end
      if(mem_wmask[0]) MEM[word_addr][ 7:0 ] <= mem_wdata[ 7:0 ];
      if(mem_wmask[1]) MEM[word_addr][15:8 ] <= mem_wdata[15:8 ];
      if(mem_wmask[2]) MEM[word_addr][23:16] <= mem_wdata[23:16];
      if(mem_wmask[3]) MEM[word_addr][31:24] <= mem_wdata[31:24];
   end
```

新增了两个输入：`mem_wdata`（32 位写数据）与 `mem_wmask`（4 位写掩码，指示哪些 byte 需要写）。

_注意_：你可能会问这种“按 byte 掩码写 BRAM”在 FPGA 上如何实现。多数 FPGA 的 BRAM 通过厂商原语直接支持 masked write。Yosys 有个非常聪明的步骤叫 “technology mapping”：它会识别 Verilog 里的某些模式，并实例化最适合目标器件的厂商原语。其实我们之前已经用过 technology mapping：寄存器堆每周期要读两个寄存器 `rs1` 与 `rs2`。IceStick 的 BRAM 每周期只能读一个值，于是 yosys 会自动复制寄存器堆：写回 `rd` 时同时写两份，读的时候分别从两份里读出两个寄存器。借助 yosys 的“魔法”，你不用自己处理这些细节，它会自动选择最佳映射（复制 BRAM、目标支持双读口时用双读口、或者干脆用触发器阵列等）。Ice40HX1K 有 8kB BRAM（8 个 1kB block）。其中 2kB 用于（复制的）寄存器堆，剩下 6kB 用来综合系统 RAM。

`Processor` 模块也要相应更新：

```verilog
module Processor (
    input 	      clk,
    input 	      resetn,
    output [31:0]     mem_addr,
    input [31:0]      mem_rdata,
    output 	      mem_rstrb,
    output [31:0]     mem_wdata,
    output [3:0]      mem_wmask,
    output reg [31:0] x10 = 0
);
```

（并在 `SOC` 里把新增信号接起来。）

接着计算要写入的 word 数据与 mask。写地址依旧是 `rs1 + imm`，但 `Load` 与 `Store` 的立即数格式不同：`Load` 用 `Iimm`，`Store` 用 `Simm`：

```
   wire [31:0] loadstore_addr = rs1 + (isStore ? Simm : Iimm);
```

写入数据取决于写 byte/halfword/word，以及写地址的低两位。很有意思的是：我们并不需要显式判断写的是 byte/halfword/word，因为写掩码会在 byte/halfword 写时忽略高字节：

```
   assign mem_wdata[ 7: 0] = rs2[7:0];
   assign mem_wdata[15: 8] = loadstore_addr[0] ? rs2[7:0]  : rs2[15: 8];
   assign mem_wdata[23:16] = loadstore_addr[1] ? rs2[7:0]  : rs2[23:16];
   assign mem_wdata[31:24] = loadstore_addr[0] ? rs2[7:0]  :
			     loadstore_addr[1] ? rs2[15:8] : rs2[31:24];
```

最后是 4 位写掩码 `STORE_wmask`，指示 `mem_wdata` 的哪些 byte 实际写入：

| 写掩码                                         | 指令                                      |
|-----------------------------------------------|-------------------------------------------|
| `4'b1111`                                     | `SW`                                      |
| `4'b0011` 或 `4'b1100`                        | `SH`，取决于 `loadstore_addr[1]`          |
| `4'b0001`,`4'b0010`,`4'b0100` 或 `4'b1000`    | `SB`，取决于 `loadstore_addr[1:0]`        |

推导表达式有点痛苦。最终我们得到如下实现：

```verilog
   wire [3:0] STORE_wmask =
	      mem_byteAccess      ?
	            (loadstore_addr[1] ?
		          (loadstore_addr[0] ? 4'b1000 : 4'b0100) :
		          (loadstore_addr[0] ? 4'b0010 : 4'b0001)
                    ) :
	      mem_halfwordAccess ?
	            (loadstore_addr[1] ? 4'b1100 : 4'b0011) :
              4'b1111;
```

再给状态机增加一个 `STORE` 状态：

```verilog
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam FETCH_REGS  = 2;
   localparam EXECUTE     = 3;
   localparam LOAD        = 4;
   localparam WAIT_DATA   = 5;
   localparam STORE       = 6;

   ...

   always @(posedge clk) begin
   ...
       case(state)
           ...
	   EXECUTE: begin
	      if(!isSYSTEM) begin
		 PC <= nextPC;
	      end
	      state <= isLoad  ? LOAD  :
		       isStore ? STORE :
		       FETCH_INSTR;
	   LOAD: begin
	      state <= WAIT_DATA;
	   end
	   WAIT_DATA: begin
	      state <= FETCH_INSTR;
	   end
	   STORE: begin
	      state <= FETCH_INSTR;
	   end
	 endcase
      end
   end
```

与内存接口相关信号这样驱动：

```verilog
   assign mem_addr = (state == WAIT_INSTR || state == FETCH_INSTR) ?
		     PC : loadstore_addr ;
   assign mem_rstrb = (state == FETCH_INSTR || state == LOAD);
   assign mem_wmask = {4{(state == STORE)}} & STORE_wmask;
```

最后一点：`Store` 指令不应写回寄存器堆！

```verilog
   assign writeBackEn = (state==EXECUTE && !isBranch && !isStore && !isLoad) ||
			(state==WAIT_DATA) ;
```

_注_：条件里的 `!isLoad`（防止在 `EXECUTE` 阶段写回 `rd`）其实可以去掉，因为在 `WAIT_DATA` 时会立刻覆盖。但这里保留它是为了让仿真更容易理解。

**试一试** 在仿真与板子上运行 [step16.v](step16.v)。它会把 16 个字节从地址 400 复制到地址 800，然后显示复制后的字节。

**你在这里！** 恭喜！你已经完成了第一个 RV32I RISC-V 内核！

| ALUreg | ALUimm | Jump  | Branch | LUI | AUIPC | Load  | Store | SYSTEM |
|--------|--------|-------|--------|-----|-------|-------|-------|--------|
| [*] 10 | [*] 9  | [*] 2 | [*] 6  | [*] | [*]   | [*] 5 | [*] 3 | [*] 1  |

_但是等等_：我们确实费了很大劲实现了一个 RISC-V 内核，但我现在看到的还是第 1 步那种“傻 blinky”一样的效果！我想看到更多东西！

要做到这一点，我们需要让设备与外部世界以比 5 个 LED 更丰富的方式通信。

## 第 17 步：内存映射设备——做点（远）不止 blinky 的事情！

现在的想法是给 SOC 加“外设”。我们已经有 LED，并且它们目前直接连到寄存器 `a0`（`x10`）。把设备直接连到寄存器上不算优雅：更好的方式是给外设在地址空间里分配一个特殊地址，它并不是真正的 RAM，而是有一个寄存器连接到 LED。这样就能通过给不同外设分配不同“虚拟地址”来扩展出很多外设；SOC 里只需要一些地址译码硬件，把读写请求路由到对应外设。你会看到：除了去掉处理器里从 `x10` 到 LED 的那根线，其它只需要对 SOC 做一些小改动。

在改 SOC 之前，先决定“内存映射”（memory map）：哪些地址范围对应什么。在我们的系统里有 6kB RAM，所以可以说地址 0 到 2^13-1（8kB，为了对齐到 2 的幂）对应 RAM。我这里给 RAM 分配更大的地址空间（因为也有更大 BRAM 的 FPGA）：0 到 2^22-1（4MB）都视为 RAM。

然后规定：如果地址的第 22 位为 1，则该地址对应外设（IO）。接着要在多个外设之间选择。自然想法是用 bit0..21 作为“设备索引”，但那需要多个 22 位比较器，在 IceStick 上会吃掉不少 LUT。Matthias Koch（@mecrisp）提出了更好的主意：用 1-hot 编码——当地址里第 n 位为 1，就路由到第 n 个外设。我们只考虑 word 地址（忽略最低两位）。这样最多能挂 20 个外设，已经够用了，而且地址译码简单很多，从而仍能放进 IceStick。

为了决定某次内存访问是去 RAM 还是去 IO，我们在 SOC 里加入如下电路：

```verilog
   wire [31:0] RAM_rdata;
   wire [29:0] mem_wordaddr = mem_addr[31:2];
   wire isIO  = mem_addr[22];
   wire isRAM = !isIO;
   wire mem_wstrb = |mem_wmask;
```

RAM 这样连接：

```verilog
   Memory RAM(
      .clk(clk),
      .mem_addr(mem_addr),
      .mem_rdata(RAM_rdata),
      .mem_rstrb(isRAM & mem_rstrb),
      .mem_wdata(mem_wdata),
      .mem_wmask({4{isRAM}}&mem_wmask)
   );
```

（注意写掩码与了 `isRAM`）

接下来加 LED 的逻辑。LED 在 `SOC` 模块接口里声明为 `reg`：

```verilog
module SOC (
    input 	     CLK,
    input 	     RESET,
    output reg [4:0] LEDS,
    input 	     RXD,
    output 	     TXD
);
```

并用如下块驱动：

```verilog
   localparam IO_LEDS_bit = 0;

   always @(posedge clk) begin
      if(isIO & mem_wstrb & mem_wordaddr[IO_LEDS_bit]) begin
	 LEDS <= mem_wdata;
      end
   end
```

现在我们可以这样写（又一个版本的）老朋友 blinky：

```verilog
      LI(gp,32'h400000);
      LI(a0,0);
   Label(L1_);
      SW(a0,gp,4);
      CALL(LabelRef(wait_));
      ADDI(a0,a0,1);
      J(LabelRef(L1_));
```

首先把 IO 页基地址装到 `gp`（也就是 `2^22`）。要写 LED，就把 `a0` store 到 IO 页的 word 地址 1（也就是字节地址 4）。
为了后面挂多个外设更方便，我们写几个辅助函数：

```verilog
   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   localparam IO_LEDS_bit      = 0;  // W five leds

   // Converts an IO_xxx_bit constant into an offset in IO page.
   function [31:0] IO_BIT_TO_OFFSET;
      input [31:0] bit;
      begin
	 IO_BIT_TO_OFFSET = 1 << (bit + 2);
      end
   endfunction
```

于是写 LED 就变成：

```verilog
   SW(a0,gp,IO_BIT_TO_OFFSET(IO_LEDS_bit));
```

_好吧，说了这么多还是 blinky？第 17 步了还只是 blinky？！_

是的，你说得对。那我们加一个 UART，让内核能往虚拟终端输出文本。
IceStick（以及很多 FPGA 板子）都有一个特殊芯片（如果你想知道型号：FTDI2232H），把传统的 RS232 串口协议与 USB 互相转换。对我们来说这是好消息：RS232 协议很简单，比 USB 容易实现得多。我们的内核只需通过两个引脚与外界通信（发送 `TXD`，接收 `RXD`），FTDI 芯片会帮你把它变成 USB。再者，没必要重复造轮子：已经有很多 UART（Universal Asynchronous Receiver Transmitter，实现 RS232 协议）的 VERILOG 实现。当前我们只实现其中一半：只实现发送端，让处理器能够发数据到终端模拟器显示文本。

Olof Kindren 写了一个 [Tweet-size UART](https://twitter.com/OlofKindgren/status/1409634477135982598)，还有一个更易读的版本在 [这里](https://gist.github.com/olofk/e91fba2572396f55525f8814f05fb33d)。

把它加进 SOC 并连接起来：

```verilog
   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   localparam IO_LEDS_bit      = 0;  // W five leds
   localparam IO_UART_DAT_bit  = 1;  // W data to send (8 bits)
   localparam IO_UART_CNTL_bit = 2;  // R status. bit 9: busy sending
   ...

   wire uart_valid = isIO & mem_wstrb & mem_wordaddr[IO_UART_DAT_bit];
   wire uart_ready;

   corescore_emitter_uart #(
      .clk_freq_hz(`BOARD_FREQ*1000000),
      .baud_rate(115200)
   ) UART(
      .i_clk(clk),
      .i_rst(!resetn),
      .i_data(mem_wdata[7:0]),
      .i_valid(uart_valid),
      .o_ready(uart_ready),
      .o_uart_tx(TXD)
   );

   wire [31:0] IO_rdata =
	       mem_wordaddr[IO_UART_CNTL_bit] ? { 22'b0, !uart_ready, 9'b0}
	                                      : 32'b0;

   assign mem_rdata = isRAM ? RAM_rdata :
	                      IO_rdata ;

```

UART 被映射到两个不同地址：一个只能写，用来发送一个字符；另一个只能读，用来查询 UART 状态（第 9 位：0 表示 ready，1 表示 busy 正在发送）。

现在我们的处理器与外界通信方式就不止 5 个 LED 了！来实现一个发字符的函数：

```verilog
   Label(putc_);
      // Send character to UART
      SW(a0,gp,IO_BIT_TO_OFFSET(IO_UART_DAT_bit));
      // Read UART status, and loop until bit 9 (busy sending)
      // is zero.
      LI(t0,1<<9);
   Label(putc_L0_);
      LW(t1,gp,IO_BIT_TO_OFFSET(IO_UART_CNTL_bit));
      AND(t1,t1,t0);
      BNEZ(t1,LabelRef(putc_L0_));
      RET();
```

它先把字符写到 IO 空间里的 UART 地址，然后不断读状态寄存器，直到 UART 不再 busy。

**试一试** 在仿真里运行 [step17.v](step17.v)。

_等等_ 仿真里它怎么知道该显示什么？

因为我“作弊”了：我在 SOC 里加了这段：

```verilog
`ifdef BENCH
   always @(posedge clk) begin
      if(uart_valid) begin
	 $write("%c", mem_wdata[7:0] );
	 $fflush(32'h8000_0001);
      end
   end
`endif
```

（传给 `$fflush()` 的魔法常量对应 `stdout`。否则你可能要等 stdout 缓冲区满了才会看到输出。）这意味着仿真时我们完全绕过 UART，我相信 Olof 的 UART 没问题，但要更严谨的话，应该在仿真的 `TXD` 上接一个 RS232 解码器来显示字符（后面会看到这类仿真的示例）。

**试一试** 在板子上运行 [step17.v](step17.v)。

要显示 UART 输出，用：

```
  $ ./terminal.sh
```

_说明_
- 编辑 `terminal.sh` 选择你喜欢的终端模拟器，并根据本机配置修改 `DEVICE=/dev/ttyUSB1`。
- 如果显示乱码，试试发送 break（picocom 下按 `<ctrl><A>` `<crtl><\>`）。
- 默认波特率是 `115200`（用于基本测试与 demo 足够）。后面一些 demo 会用 ANSI 序列在终端做“图形”，更高波特率会更顺滑（试试 `1000000`）。需要同时修改 `emitter_uart.v` 与 `terminal.sh`（大多数 FPGA 没问题，但 GOWIN 板子常见的不是 FTDI 而是 BL702，可能会影响这点）。

## 第 18 步：计算 Mandelbrot 集

现在我们有一个能跑的 RISC-V 处理器、带 UART 的 SOC，可以往虚拟终端输出字符。来休息一下，做一个纯软件步骤：写一段 RISC-V 汇编，计算一个粗糙的 ASCII-art Mandelbrot 集。

我们的“图像”用 80x80 个字符表示。先写一个程序把图像全部填充成 `*`。我们用两个嵌套循环：Y 坐标在 `s0`，X 坐标在 `s1`，上界（80）放在 `s11`。程序如下：

```verilog
      LI(gp,32'h400000); // IO page
      LI(s1,0);
      LI(s11,80);

   Label(loop_y_);
      LI(s0,0);

   Label(loop_x_);
      LI(a0,"*");
      CALL(LabelRef(putc_));

      ADDI(s0,s0,1);
      BNE(s0,s11,LabelRef(loop_x_));

      LI(a0,13);
      CALL(LabelRef(putc_));
      LI(a0,10);
      CALL(LabelRef(putc_));

      ADDI(s1,s1,1);
      BNE(s1,s11,LabelRef(loop_y_));

      EBREAK();
```

输出大概是这样：

```
          ***********
        ***************
       ******************
     *********************
    ***********************
    ************************
   *************************
  ***************************
  ***************************
 *****************************
 *****************************
 *****************************
 *****************************
 *****************************
 *****************************
 *****************************
 *****************************
 *****************************
 *****************************
  ***************************
  ***************************
   *************************
   *************************
    ***********************
     *********************
      *******************
        ***************
          ***********
```

接下来计算 Mandelbrot 集，需要迭代如下操作：

```
   Z <- 0; iter <- 0
   do
      Z <- Z^2 + C
      iter <- iter + 1
   while |Z| < 2
```

其中 `Z` 与 `C` 是复数，`C = x + iy` 对应当前像素。根据复数乘法规则（`i*i = -1`），有
`Z^2 = (Zr + i*Zi)^2 = Zr^2-Zi^2 + 2*i*Zr*Zi`。计算迭代的循环大致如下：

```verilog
   Label(loop_Z_);
      MV(a0,s4); // Zrr  <- (Zr*Zr) >> mandel_shift
      MV(a1,s4);
      CALL(LabelRef(mulsi3_));
      SRLI(s6,a0,`mandel_shift);
      MV(a0,s4); // Zri <- (Zr*Zi) >> (mandel_shift-1)
      MV(a1,s5);
      CALL(LabelRef(mulsi3_));
      SRAI(s7,a0,`mandel_shift-1);
      MV(a0,s5); // Zii <- (Zi*Zi) >> (mandel_shift)
      MV(a1,s5);
      CALL(LabelRef(mulsi3_));
      SRLI(s8,a0,`mandel_shift);
      SUB(s4,s6,s8); // Zr <- Zrr - Zii + Cr
      ADD(s4,s4,s2);
      ADD(s5,s7,s3); // Zi <- 2Zri + Cr

      ADD(s6,s6,s8); // if norm > norm max, exit loop
      LI(s7,`norm_max);
      BGT(s6,s7,LabelRef(exit_Z_));

      ADDI(s10,s10,-1);  // iter--, loop if non-zero
      BNEZ(s10,LabelRef(loop_Z_));

   Label(exit_Z_);
```

最后根据退出循环时的 `iter`（`s10`）选择不同字符显示：

```
   Label(exit_Z_);
      LI(a0,colormap_);
      ADD(a0,a0,s10);
      LBU(a0,a0,0);
      CALL(LabelRef(putc_));
```

其中 “colormap” 是一个字符数组，用来模拟从暗到亮的不同“强度”：

```
   Label(colormap_);
      DATAB(" ",".",",",":");
      DATAB(";","o","x","%");
      DATAB("#","@", 0 , 0 );
```

![](https://github.com/BrunoLevy/learn-fpga/blob/master/FemtoRV/TUTORIALS/Images/mandelbrot_terminal.gif)

**试一试** 在仿真与板子上运行 [step18.v](step18.v)。修改它来绘制你自己的图形（例如用 colormap 画“同心圆”）。

## 第 19 步：用 Verilator 加速仿真

你在第 18 步已经看到：仿真比在真实板子上跑慢得多。但有一个工具叫 `verilator`，可以把 VERILOG 设计转换成 C++，再编译成可执行文件，仿真速度比 icarus/iverilog 快很多。先安装：

```
  $ apt-get install verilator
```

在转换前需要写一个 C++ “bench”：生成设计的输入信号并提供 `main()`。`main` 的主要工作是创建由 `SOC` 模块生成的 `VSOC` 类对象，然后不断翻转 `CLK`。每次 `CLK` 变化都要调用 `eval()`。`sim_main.cpp` 如下：

```c++
#include "VSOC.h"
#include "verilated.h"
#include <iostream>

int main(int argc, char** argv, char** env) {
   VSOC top;
   top.CLK = 0;
   while(!Verilated::gotFinish()) {
      top.CLK = !top.CLK;
      top.eval();
   }
   return 0;
}
```

另外，在 [sim_main.cpp](sim_main.cpp) 里还有一些代码：当 LED 改变时解码并打印它们的状态。

把设计转换成 C++：

```
  $ verilator -DBENCH -DBOARD_FREQ=12 -Wno-fatal --top-module SOC -cc -exe sim_main.cpp step18.v
```

编译并运行生成的程序：

```
  $ cd obj_dir
  $ make -f VSOC.mk
  $ ./VSOC
```

你会看到它比 icarus/iverilog 快很多！对小设计差别不算巨大，但相信我：当你开发一个 RV32IMFC（带 FPU）的内核时，高效仿真非常重要。

为了更方便，提供了 `run_verilator.sh` 脚本：

```
  $ run_verilator.sh step18.v
```

## 第 20 步：使用 GNU 工具链编译程序——汇编

到这里你可能觉得我们的 RISC-V 设计只是一个教学玩具，离“真实世界”很远。但从这一节开始你会感觉：你做出来的东西其实和任何其它 RISC-V 处理器一样真实！处理器的价值在于能跑什么软件；如果我们的东西能运行任何为（RV32I）RISC-V 写的软件，那它就是一个 RV32I RISC-V 处理器。

_等等_ 我们之前写软件用的是 VERILOG 汇编器，它不就是个玩具，和真实世界不同吗？

实际上，VERILOG 汇编器生成的机器码与任何其它 RISC-V 汇编器生成的是完全相同的。我们也可以用其它 RISC-V 汇编器生成机器码，再加载到设计里运行。

为此 VERILOG 提供了 `$readmemh()`，可以从外部文件初始化内存。在 [step20.v](step20.v) 中这样用：

```verilog
   initial begin
       $readmemh("firmware.hex",MEM);
   end
```

其中 `firmware.hex` 是一个 ASCII 文件，包含 `MEM` 的初始内容（十六进制）。

如果要使用外部汇编器，需要解决三件事：
- 如何用 GNU 工具编译 RISC-V 汇编代码
- 如何告诉 GNU 工具我们做的“设备”是什么样（RAM 起始地址、大小等）
- 如何把 GNU 工具输出转换成 `$readmemh()` 能读的格式

先从一个简单 blinky 开始，见 [blinker.S](FIRMWARE/blinker.S)：

```
# Simple blinker

.equ IO_BASE, 0x400000
.equ IO_LEDS, 4

.section .text

.globl start

start:
        li   gp,IO_BASE
	li   sp,0x1800
.L0:
	li   t0, 5
	sw   t0, IO_LEDS(gp)
	call wait
	li   t0, 10
	sw   t0, IO_LEDS(gp)
	call wait
	j .L0

wait:
        li t0,1
	slli t0, t0, 17
.L1:
        addi t0,t0,-1
	bnez t0, .L1
	ret
```

如你所见，它与我们之前在 VERILOG 汇编里写的代码非常相似。这个程序包含三部分：
- **主程序**
- **工具函数**（这里是 `wait`）
- **启动初始化**（初始化 `gp` 与 `sp`）

所以我们把它拆成 3 个文件：
- [FIRMWARE/blinker.S](FIRMWARE/blinker.S)：`main`
- [FIRMWARE/wait.S](FIRMWARE/wait.S)：`wait`
- [FIRMWARE/start.S](FIRMWARE/start.S)：启动代码，最后会调用 `main`

要编译它，你需要安装 RISC-V 工具链（编译器、汇编器、链接器）。本仓库的 makefile 可以帮你做：

```
  $ cd learn-fpga/FemtoRV
  $ make ICESTICK.firmware_config
```

_注意：_ 即使你用的是更大的板子，也请始终使用 `ICESTICK.firmware_config`，因为它会把 makefile 配置成 RV32I 构建（这正是我们的处理器支持的）。

这会下载并解压到 `learn-fpga/FemtoRV/FIRMWARE/TOOLCHAIN`。把 `riscv64-unknown-elf-gcc..../bin/` 加到 PATH。

然后编译程序：

```
  $ cd learn-fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE
  $ riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -mno-relax start.S -o start.o
  $ riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -mno-relax blinker.S -o blinker.o
  $ riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 -mno-relax wait.S -o wait.o
```

这里指定了架构（`rv32i`）与 ABI（`ilp32`）。`no-relax` 与我们把 `gp` 用作 IO 页基地址有关（避免汇编器对它做“放松优化”并挪作它用）。

这会生成 `.o` 目标文件。接着用链接器把它们链接成可执行文件。链接器负责决定代码/数据在内存中的布局，因此要提供一个链接脚本（[FIRMWARE/bram.ld](FIRMWARE/bram.ld)）描述设备内存：

```
MEMORY
{
   BRAM (RWX) : ORIGIN = 0x0000, LENGTH = 0x1800  /* 6kB RAM */
}
SECTIONS
{
    everything :
    {
	. = ALIGN(4);
	start.o (.text)
        *(.*)
    } >BRAM
}
```

链接脚本里有 `MEMORY` 描述内存段。这里我们只有一个名为 `BRAM` 的 6kB 段，从 `0x0000` 开始。`SECTIONS` 描述“哪些内容放到哪里”。这里很简单：所有东西都放到 BRAM。我们还指定了 `start.o` 的内容要最先放进内存。

链接器调用如下：

```
  $ riscv64-unknown-elf-ld blinker.o wait.o -o blinker.bram.elf -T bram.ld -m elf32lriscv -nostdlib -norelax
```

它生成一个 elf 可执行文件（Executable and Linkable Format，和 Linux 系统的可执行文件格式一样）。`-T bram.ld` 指定链接脚本；`-m elf32lriscv` 表示生成 32 位；`-nostdlib` 表示暂时不使用 C 标准库；`-norelax` 保留 `gp` 的用法。这里不需要在命令行显式列出 `start.o`，因为链接脚本 `bram.ld` 里已经包含它了。

还没结束：我们需要从 elf 里提取出机器码，并生成 `$readmemh()` 能读的十六进制 ASCII 文件。我写了一个 `firmware_words` 工具：它能解析 elf，抽取我们需要的部分并输出十六进制：

```
  $ make blinker.bram.hex
```

_提示_：你也可以直接 `make xxxx.bram.hex`，它会自动调用汇编、链接与转换工具。

现在可以在仿真与板子上运行例子：

```
  $ cd ..
  $ ./run_verilator.sh step20.v
  $ BOARDS/run_xxx.sh step20.v
```

既然写程序更方便了，就可以写更复杂的程序。比如经典的 “hello world”。我们需要一个 `putstring` 例程把字符串输出到 tty。它输入为 `a0`（字符串首地址），循环读取每个字符，遇到 0 结束，并对每个字符调用 `putchar`：

```
# Warning, buggy code ahead !
putstring:
	mv t2,a0
.L2:    lbu a0,0(t2)
	beqz a0,.L3
	call putchar
	addi t2,t2,1
	j .L2
.L3:    ret
```

看到注释了吗？上面这段有 bug，你能找出来吗？

提示：`putstring` 是一个“调用函数的函数”。这种情况下是不是要做点额外处理？

还记得 `call` 与 `ret` 的含义吗？`call` 会把 `PC+4` 写入 `ra`，然后跳到目标函数；`ret` 会跳到 `ra`。假设有人调用了 `putstring`：刚进入时 `ra` 保存的是 `putstring` 结束后应该返回的地址。但 `putstring` 内部又 `call putchar`，于是 `ra` 被覆盖成 “call 后的下一条指令地址”，供 `putchar` 返回使用。结果 `putchar` 返回没问题，但 `putstring` 在执行 `ret` 时也会跳到那个地址——这显然不对。解决办法是在 `putstring` 开头把 `ra` 保存到栈上，结束时再恢复：

```
putstring:
	addi sp,sp,-4 # save ra on the stack
	sw ra,0(sp)   # (need to do that for functions that call functions)
	mv t2,a0
.L2:    lbu a0,0(t2)
	beqz a0,.L3
	call putchar
	addi t2,t2,1
	j .L2
.L3:    lw ra,0(sp)  # restore ra
	addi sp,sp,4 # resptore sp
	ret
```

使用方式：

```
   la   a0, hello
   call putstring

   ...

hello:
	.asciz "Hello, world !\n"
```

`la`（load address）伪指令把字符串地址装载到 `a0`。字符串用标签声明，并用 `.asciz` 生成以 0 结尾的字符串。

**试一试** 编译 `hello.S`（`cd FIRMWARE; make hello.bram.hex`）并在仿真与板子上测试。也试试 `mandelbrot.S`。你会发现 [FIRMWARE/mandelbrot.S](FIRMWARE/mandelbrot.S) 没有 `__mulsi` 函数：如果你看 [FIRMWARE/Makefile](FIRMWARE/Makefile)，它会链接 RV32I 版本的 `libgcc.a`，其中包含了这个函数。

现在你会更强烈地感觉到自己的处理器是“真实”的：当你运行 Mandelbrot 示例时，它在 *你* 的处理器上运行了 *别人* 写的代码。那我们能不能更进一步，跑由标准工具生成的代码？

## 第 21 步：使用 GNU 工具链编译程序——C

现在来看如何为我们的处理器写 C 代码。到这里我们已经能生成 `.o` 并用链接器得到 elf，可通过链接脚本把内容放到内存正确位置，然后处理器从地址 0 的 `start.S` 开始执行，最后调用 `main`。之前程序完全用汇编写。ABI 的好处（第 13、14 步讲过）是：只要遵守 ABI，就能把不同工具生成的 `.o` 混合链接——C 编译器当然也是遵守 ABI 的。

[FIRMWARE/sieve.c](FIRMWARE/sieve.c)（来自 picorv 的例子）是个不错的测试：它做乘法、除法和取模。我们的 RV32I 内核并不直接支持这些指令，但编译器会通过 `libgcc.a` 中的函数实现；只要链接上 `libgcc.a`，就能工作。

但是它还用了 `printf()` 输出结果，而 `printf()` 在 `libc.a` 里。理论上可以用，但 `printf()` 支持的格式太多，代码太大，放不进 6kB RAM。所以我们用一个小得多、简单得多的替代实现：
[FIRMWARE/print.c](FIRMWARE/print.c)（也来自 picorv），并把它一起链接进可执行文件。

![](mandel_and_riscvlogo.png)

还有两个示例：Mandelbrot 的 C 版本 [FIRMWARE/mandel_C.c](FIRMWARE/mandel_C.c)，用
[ANSI 颜色](https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences) 在终端显示低分辨率“图形”；以及 [FIRMWARE/riscv_logo.c](FIRMWARE/riscv_logo.c)，显示一个旋转的 RISC-V 标志（90 年代 demo scene 风格！）。

**试一试** 编译 `sieve.c`（`cd FIRMWARE; make sieve.bram.hex`）并在仿真（`./run_verilator.sh step20.v`）与板子上（`BOARDS/run_xxx.sh step20.v; ./terminal.sh`）测试。也试试其它程序，写你自己的程序（比如生命游戏等）。

注意：Verilator 框架在仿真里可以直接加载 ELF（无需重新生成 `firmware.hex`）。你可以生成所有 demo：`cd FIRMWARE; make hello.bram.elf mandelbrot.bram.elf mandel_C.bram.elf riscv_logo.bram.elf;cd ..`，然后用 `./run_verilator.sh step20.v FIRMWARE/mandel_C.bram.elf` 或 `./obj_dir/FIRMWARE/mandel_C.bram.elf` 运行指定程序。

现在你可以看到：你的处理器不仅不是玩具，它还是一个真正的 RISC-V 处理器，可以运行标准工具（gcc 等）生成的程序！

_注意_：IceStick 只有 `6kB` RAM，所以只能运行很小的程序。若编译出的程序大于 `6kB` 会报错。更麻烦的是：程序几乎占满 BRAM 时，栈空间几乎没有，栈会覆盖其它内容，导致 CPU 进入异常状态甚至卡死。这种情况很难理解与调试，所以 `firmware_words` 会在生成的代码占用超过 95% BRAM 时打印一个很大的警告。

## 第 22 步：存储数据：我能不能不止 6kB 内存？

_以及处理器的一些优化_

![](IceStick_SPIFLASH.jpg)

IceStick 只有 8 个 1kB BRAM block。我们需要其中 2 个用于寄存器堆（因为会被复制以支持双读口），所以只剩 6kB RAM 给程序用。对 Mandelbrot 这类小程序够用，但很快会碰到极限。

IceStick 上还有一颗 4MB 的 FLASH（见图；其它板子也有类似芯片）。当你综合一个设计时，它被写入这个 FLASH；上电后 FPGA 会从这个 FLASH 读取配置。好消息是：FPGA 配置只占用几十 KB，FLASH 里剩下大把空间可存我们自己的数据。坏消息是：我们得做额外硬件来访问它。

你可能会问：这颗芯片只有 8 个引脚，怎么寻址 4MB？它用的是 _串行协议_（SPI）：读数据时，把地址按位送入，然后芯片按位把数据送出（我们这里是每次两个 bit）。如果你想了解 SPI flash，我的笔记在
[这里](https://github.com/BrunoLevy/learn-fpga/blob/master/FemtoRV/TUTORIALS/spi_flash.md)，Verilog 实现在 [spi_flash.v](spi_flash.v)。它支持不同模式（取决于用几根线、是否双向等）。

`MappedSPIFlash` 模块接口如下：

```verilog
module MappedSPIFlash(
    input wire 	       clk,
    input wire 	       rstrb,
    input wire [19:0]  word_address,

    output wire [31:0] rdata,
    output wire        rbusy,

    output wire        CLK,
    output reg         CS_N,
    inout  wire [1:0]  IO
);
```

| 信号          | 说明                                                      |
|--------------|-----------------------------------------------------------|
| clk          | 系统时钟                                                   |
| rstrb        | 读请求（处理器想读一个 word 时拉高）                         |
| word_address | 要读的 word 地址                                           |
| rdata        | 读出的数据                                                 |
| rbusy        | 忙信号（正在接收数据时拉高）                                 |
| CLK          | SPI flash 的时钟引脚                                       |
| CS_N         | SPI flash 的片选（低有效）                                  |
| IO           | 两根双向线，用于发送与接收数据                               |

现在要修改 SOC：让某些地址映射到 SPI flash。先决定它在处理器地址空间里的映射方式：用地址 bit23 选择 SPI flash；bit22 选择 IO（LED、UART）。对 IO 还要保证 bit23 为 0；若 bit23 与 bit22 都为 0，则访问 BRAM。也就是说地址空间按 bit23..22 分成四个象限，我们用其中三个。

于是用如下信号区分区域：

```verilog
   wire isSPIFlash  = mem_addr[23];
   wire isIO        = mem_addr[23:22] == 2'b01;
   wire isRAM       = mem_addr[23:22] == 2'b00;
```

`MappedSPIFlash` 这样连接：

```verilog
   wire SPIFlash_rdata;
   wire SPIFlash_rbusy;
   MappedSPIFlash SPIFlash(
      .clk(clk),
      .word_address(mem_wordaddr),
      .rdata(SPIFlash_rdata),
      .rstrb(isSPIFlash & mem_rstrb),
      .rbusy(SPIFlash_rbusy),
      .CLK(SPIFLASH_CLK),
      .CS_N(SPIFLASH_CS_N),
      .IO(SPIFLASH_IO)
   );
```

（`SPIFLASH_CLK`、`SPIFLASH_CS_N`、`SPIFLASH_IO[0]`、`SPIFLASH_IO[1]` 在 `BOARDS` 子目录的约束文件里声明。）

返回给处理器的数据用三选一 mux：

```verilog
   assign mem_rdata = isRAM      ? RAM_rdata :
                      isSPIFlash ? SPIFlash_rdata :
	                           IO_rdata ;
```

现在处理器只要访问 bit23=1 的地址，就能触发 SPI flash 读。但数据按 bit 到达，处理器怎么知道数据准备好了？`MappedSPIFlash` 提供了 `SPIFlash_rbusy`：只要还在接收数据就为 1。我们需要把这个信号纳入处理器状态机：给处理器加一个输入 `mem_rbusy`，并这样修改状态机：

```verilog
   ...
   WAIT_DATA: begin
      if(!mem_rbusy) begin
	 state <= FETCH_INSTR;
      end
   end
   ...
```

然后在 SOC 中把它连到 `SPIFlash_rbusy`：

```verilog
   wire mem_rbusy;
   ...
   Processor CPU(
     ...
     .mem_rbusy(mem_rbusy),
     ...
   );
   ...
   assign mem_rbusy = SPIFlash_rbusy;
```

既然我们正在回看状态机，还有个明显的优化点。还记得这段吗？是不是可以更快？

```verilog
   WAIT_INSTR: begin
      instr <= mem_rdata;
      state <= FETCH_REGS;
   end
   FETCH_REGS: begin
      rs1 <= RegisterBank[rs1Id];
      rs2 <= RegisterBank[rs2Id];
      state <= EXECUTE;
   end
```

是的：`rs1Id`/`rs2Id` 只是从 `instr` 拉出来的 5 根线，因此可以直接从 `mem_rdata` 得到它们，并在 `WAIT_INSTR` 阶段就把寄存器读出来：

```verilog
   WAIT_INSTR: begin
      instr <= mem_rdata;
      rs1 <= RegisterBank[mem_rdata[19:15]];
      rs2 <= RegisterBank[mem_rdata[24:20]];
      state <= EXECUTE;
   end
```

这样每条指令省一个周期，是个很划算的优化！

再来一个：为什么需要 `LOAD` 与 `STORE` 状态？能不能在 `EXECUTE` 状态里直接发起内存传输？可以。于是把读/写 strobes 改成：

```verilog
   assign mem_rstrb = (state == FETCH_INSTR || (state == EXECUTE & isLoad));
   assign mem_wmask = {4{(state == EXECUTE) & isStore}} & STORE_wmask;
```

这样状态机只剩 4 个状态：

```verilog
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam EXECUTE     = 2;
   localparam WAIT_DATA   = 3;
   reg [1:0] state = FETCH_INSTR;
   always @(posedge clk) begin
      if(!resetn) begin
	 PC    <= 0;
	 state <= FETCH_INSTR;
      end else begin
	 if(writeBackEn && rdId != 0) begin
	    RegisterBank[rdId] <= writeBackData;
	 end
	 case(state)
	   FETCH_INSTR: begin
	      state <= WAIT_INSTR;
	   end
	   WAIT_INSTR: begin
	      instr <= mem_rdata;
	      rs1 <= RegisterBank[mem_rdata[19:15]];
	      rs2 <= RegisterBank[mem_rdata[24:20]];
	      state <= EXECUTE;
	   end
	   EXECUTE: begin
	      if(!isSYSTEM) begin
		 PC <= nextPC;
	      end
	      state <= isLoad  ? WAIT_DATA : FETCH_INSTR;
	   end
	   WAIT_DATA: begin
	      if(!mem_rbusy) begin
		 state <= FETCH_INSTR;
	      end
	   end
	 endcase
      end
   end
```

还有别的可优化点。比如你可能注意到 RV32I 指令最低两位永远是 `2'b11`，因此没必要存它们：

```verilog
   reg [31:2] instr;
   ...
   instr <= mem_rdata[31:2];
   ...
   wire isALUreg  =  (instr[6:2] == 5'b01100);
   ...
```

再比如：我们一直用 32 位做地址计算，但地址空间只有 24 位，这里能省不少资源：

```verilog
   localparam ADDR_WIDTH=24;
   wire [ADDR_WIDTH-1:0] PCplusImm = PC + ( instr[3] ? Jimm[31:0] :
				  instr[4] ? Uimm[31:0] :
				             Bimm[31:0] );
   wire [ADDR_WIDTH-1:0] PCplus4 = PC+4;

   wire [ADDR_WIDTH-1:0] nextPC = ((isBranch && takeBranch) || isJAL) ? PCplusImm   :
	                                  isJALR   ? {aluPlus[31:1],1'b0} :
	                                             PCplus4;

   wire [ADDR_WIDTH-1:0] loadstore_addr = rs1 + (isStore ? Simm : Iimm);
```

最新的 Verilog 文件在 [step22.v](step22.v)。现在用下面的 [程序](FIRMWARE/read_spiflash.c) 测试能否从处理器访问 SPI flash：

```C
#include "io.h"
#define SPI_FLASH_BASE ((char*)(1 << 23))
int main()  {
   for(int i=0; i<16; ++i) {
      IO_OUT(IO_LEDS,i);
      int lo = (int)SPI_FLASH_BASE[2*i  ];
      int hi = (int)SPI_FLASH_BASE[2*i+1];
      print_hex_digits((hi << 8) | lo,4); // print four hexadecimal digits
      printf(" ");
   }
   printf("\n");
}
```

SPI flash 通过地址 bit23 映射到地址空间（`SPI_FLASH_BASE = 1 << 23`）。程序逐个读取 byte，并把它们按 16 位一组显示（低地址 byte 是低有效字节，因为 RISC-V 是 little-endian）。`print_hex_digits()` 在 [FIRMWARE/print.c](FIRMWARE/print.c) 里（第二个参数指定输出多少个十六进制字符）。

编译、综合并下载到板子：

```
 $ cd FIRMWARE
 $ make read_spiflash.bram.hex
 $ cd ..
 $ BOARDS/run_icestick.sh step22.v
 $ ./terminal.sh
```

……然后你什么也看不到。为什么？因为程序在你打开终端之前就结束了。按一下“隐形复位键”（第 2 步提到的那个，见 [step 2](README.md#step-2-slower-blinky)），每按一次，它都会在终端显示 SPI flash 里前 16 个 word。
在 IceStick 上你可能会看到：

```
00FF FF00 AA7E 7E99 0051 0501 0092 6220 4B01 0072 8290 0000 0011 0101 0000 0000
```

这些值来自哪里？想想 SPI flash 芯片存在的原因：它存放你的设计。FPGA 上电后从 SPI flash 读取配置。配置对应 `yosys/nextpnr/icepack` 流水线最后生成的 `SOC.bin`：
- `yosys` 把 Verilog 变成电路（netlist）
- `nextpnr` 把电路映射到 FPGA 逻辑资源
- `icepack` 把结果转成 FPGA 能直接理解的二进制 bitstream

看看 `SOC.bin` 的前 16 个 word：

```
  $ od -x -N 32 SOC.bin
```

你会看到类似：

```
0000000 00ff ff00 aa7e 7e99 0051 0501 0092 6220
0000020 4b01 0072 8290 0000 0011 0101 0000 0000
0000040
```

正好对应终端里显示的内容。所以我们的 CPU 现在能读到自己 FPGA 配置的二进制表示，就像生物学家在测序自己的 DNA 一样！这听起来很“递归”，但实际用途不大。不过它启发我们：`SOC.bin` 并不大：

```
$ ls -al SOC.bin
-rw-rw-r-- 1 blevy blevy 32220 Jan  7 07:31 SOC.bin
```

大约 `32KB`，而 SPI flash 有 `4MB`，空间非常充裕！我们要做的只是不要覆盖 FPGA 配置（也就是把我们的数据写到 `SOC.bin` 大小之后的区域）。为简单起见，我们用 `1MB` 偏移作为数据存储起点（你可能会说 32KB 到 1MB 之间浪费了很多，但后续步骤会用到这块空间做别的事）。

**试一试** 创建一个文本文件 `hello.txt`，把它写到 SPI flash 的 `1MB` 偏移处（见下方命令），再写一个程序把它读出来显示。为了知道何时停止，你可能需要一个结束符，或事先编码文件长度。

对 ICE40 系列板子（IceStick、IceBreaker 等）：

```
 $ iceprog -o 1M hello.txt
```

对 ECP5 板子（ULX3S）：

```
 $ cp hello.txt hello.img
 $ ujprog -j flash -f 1048576 hello.img
```

（使用从 [https://github.com/kost/fujprog](https://github.com/kost/fujprog) 编译的最新版 `ujprog`。）

![](ST_NICCC_tty.png)

现在我们有了更多存储空间，可以做更有意思的事情：在终端显示一个动画。这个动画是 90 年代的一个 demo：它把多边形数据流送到软件多边形渲染器。多边形数据是一个 640kB 的二进制文件，位于 `learn_fpga/FemtoRV/FIRMWARE/EXAMPLES/DATA/scene1.dat`（同目录还有其它文件说明格式）。先把文件写入 SPI flash 的 1MB 偏移处。ICE40 系列板子：

```
 $ iceprog -o 1M learn_fpga/FemtoRV/FIRMWARE/EXAMPLES/DATA/scene1.dat
```

ECP5（ULX3S）：

```
 $ cp learn_fpga/FemtoRV/FIRMWARE/EXAMPLES/DATA/scene1.dat scene1.img
 $ ujprog -j flash -f 1048576 scene1.img
```

（使用从 [https://github.com/kost/fujprog](https://github.com/kost/fujprog) 编译的最新版 `ujprog`。）

然后编译程序：

```
 $ cd FIRMWARE
 $ make ST_NICCC.bram.hex
 $ cd ..
```

并把设计与程序下载到板子：

```
 $ BOARDS/run_xxx.sh step22.v
 $ ./terminal.sh
```

**试一试** 把一张图片存到 SPI flash（选一个易读格式），写程序把它显示出来。你可以用 `printf("\033[48;2;%d;%d;%dm ",R,G,B);` 输出一个像素（`R`,`G`,`B` 在 0..255），每行结束后用 `printf("\033[48;2;0;0;0m\n");`。

## 第 23 步：从 SPI flash 运行程序（第一步）

上一节我们已经能从 SPI flash 读取数据，也有足够空间存放数据，但代码与变量仍只能共享 6kB RAM，这依然很紧张！如果能把代码放在 SPI flash，并直接从那里执行，那就更棒了：想象你有 2MB 存代码，而完整的 6kB RAM 都给变量和栈使用！

要做到从 SPI flash 取指，硬件上几乎只需要一处改动：在 `WAIT_INSTR` 状态里，如果 `mem_rbusy` 还没清零，就不要进入 `EXECUTE`。也就是在切换状态前检查 `mem_rbusy`：

```verilog
   WAIT_INSTR: begin
      instr <= mem_rdata[31:2];
      rs1 <= RegisterBank[mem_rdata[19:15]];
      rs2 <= RegisterBank[mem_rdata[24:20]];
      if(!mem_rbusy) begin
	 state <= EXECUTE;
      end
   end
```

并把 BRAM 初始化成一个跳到地址 `0x00820000` 的小程序：

```verilog
   initial begin
      LI(a0,32'h00820000);
      JR(a0);
   end
```

这个地址等于 SPI flash 映射基址 `0x00800000`（=1<<23）加上 128kB 偏移 `0x20000`。为什么要加 128kB？因为 SPI flash 同时存放 FPGA 配置，我们不能覆盖那部分。

硬件部分基本就这些。现在看看能否真的从那里执行代码。我们需要一个新链接脚本（[FIRMWARE/spiflash0.ld](FIRMWARE/spiflash0.ld)）：

```
MEMORY {
   FLASH (RX)  : ORIGIN = 0x00820000, LENGTH = 0x100000 /* 4 MB in flash */
}
SECTIONS {
    everything : {
	. = ALIGN(4);
	start.o (.text)
        *(.*)
    } >FLASH
}
```

它与之前类似，但把所有内容放到 flash。先用一个不写全局变量的程序测试，比如 [FIRMWARE/hello.S](FIRMWARE/hello.S)。用新链接脚本链接：

```
  $ riscv64-unknown-elf-ld -T spiflash0.ld -m elf32lriscv -nostdlib -norelax hello.o putchar.o -o hello.spiflash0.elf
```

Makefile 里也提供了封装：

```
  $ make hello.spiflash0.elf
```

接着把 ELF 转成平坦二进制：

```
  $ riscv64-unknown-elf-objcopy hello.spiflash0.elf hello.spiflash0.bin -O binary
```

或：

```
  $ make hello.spiflash0.bin
```

然后写入 SPI flash 的 128k 偏移处：

```
  $ iceprog -o 128k hello.spiflash0.bin
```

或：

```
  $ make hello.spiflash0.prog
```

最后：

```
  $ ./terminal.sh
```

## 第 24 步：从 SPI flash 运行程序（更好的链接脚本）

开始前先对内核做个小改动：按复位按钮时 CPU 会跳到地址 0（那里是跳到 flash 的指令），但程序运行后 RAM 很可能被改写过，地址 0 未必还保留那条跳转指令。为避免这个问题，可以让 CPU 每次复位都直接跳到 flash：

```verilog
   if(!resetn) begin
     PC    <= 32'h00820000;
     state <= WAIT_DATA;
   end
```

注意状态设为 `WAIT_DATA`，这样会等待 `mem_rbusy` 变低后再继续。

现在我们能在 flash 里放代码并从那里运行。只读数据（例如 `.asciz "Hello, world !\n"`）也能放在 flash。局部变量在栈上，栈在 6kB RAM 里，所以也能工作。栈在哪里？由 [FIRMWARE/start.S](FIRMWARE/start.S) 设置：`sp` 初始化为 RAM 末尾 `0x1800`。

但下面这种程序怎么办？

```C
  int x = 3;
  void main() {
     x = x + 1;
     printf("%d\n",x);
  }
```

全局变量 `x` 有初始值，应存于 flash；但运行时又会被修改，因此必须放在 RAM。怎么做到？需要把“全局变量的初始化数据”放在 flash 中，并在启动时拷贝到 RAM。为此我们需要：
- 一个新的链接脚本：既定义变量在 RAM 的地址，也定义其初值在 flash 的存放位置（LMA）
- 一个新的启动代码 `start.S`：负责把初值从 flash 拷贝到 RAM，并把未初始化区清零

编译 C 时，编译器会用 section 指令标注不同内容。你可以用 `readelf` 看看：

```
$ cd FIRMWARE
$ make ST_NICCC.o
$ readelf -S ST_NICCC.o
```

常见 section：

| section     | 说明             |
|-------------|------------------|
| text        | 可执行代码        |
| bss, sbss   | 未初始化数据      |
| data, sdata | 可读写数据        |
| rodata      | 只读数据          |

（BSS 这个名字有很古早的历史原因：60 年代 IBM 704 汇编器里有个伪指令叫 Block Started by Symbol。）

`readelf` 输出里还有 `type` 字段：`PROGBIT` 表示需要从文件加载数据（`text`,`data`,`rodata`），`NOBITS` 表示无需从文件加载（`bss`）。`Addr` 表示该 section 在内存中的地址（对 `.o` 通常为 0，对最终链接后的 elf 则有意义），`Offs` 是 section 数据在 `.o` 文件中的偏移，`Size` 是字节数。

因此链接脚本需要表达：
- `text` 放到 flash
- `bss` 放到 BRAM，并在启动时清零
- `data` 放到 BRAM，但其初值存放在 flash（LMA），启动时拷贝到 BRAM

`text` 与 `bss` 我们已经会了。`data` 需要链接脚本支持 LMA（Load Memory Address）：用 `AT` 指定初值存放地址。例如：

```
  MEMORY {
      FLASH (rx)  : ORIGIN = 0x00820000, LENGTH = 0x100000
      RAM   (rwx) : ORIGIN = 0x00000000, LENGTH = 0x1800
  }
  SECTIONS {

    .data: AT(address_in_spi_flash) {
      *(.data*)
      *(.sdata*)
    } > RAM

    .text : {
      start_spiflash1.o(.text)
      *(.text*)
      *(.rodata*)
      *(.srodata*)
    } >FLASH

    .bss : {
      *(.bss*)
      *(.sbss*)
    } >RAM
  }
```

每个输出 section（`.data`, `.text`, `.bss`）把输入 `.o` 里的相应 section 聚合起来，并映射到 RAM/FLASH。`.text` 里我们确保 `start_spiflash1.o` 的 `.text` 放在最前面，因为复位后 CPU 会跳到那里。只读数据（`.rodata`, `.srodata`）放 flash。

`.data` 的 `AT` 表示其初值在 flash 的存放位置，而 `.data` 本身运行地址在 RAM。链接器在引用 `.data` 符号时使用 RAM 地址。

那系统如何知道要从 flash 拷贝初始化数据到 BRAM？拷贝多少、拷到哪里？BSS 如何清零？这些需要在启动代码 `start_spiflash1.S` 里手工完成：

```asm
.equ IO_BASE, 0x400000

.text
.global _start
.type _start, @function

_start:
.option push
.option norelax
     li  gp,IO_BASE
.option pop

     li   sp,0x1800

# zero-init bss section:
     la a0, _sbss
     la a1, _ebss
     bge a0, a1, end_init_bss
loop_init_bss:
     sw zero, 0(a0)
     addi a0, a0, 4
     blt a0, a1, loop_init_bss
end_init_bss:

# copy data section from SPI Flash to BRAM:
     la a0, _sidata
     la a1, _sdata
     la a2, _edata
     bge a1, a2, end_init_data
loop_init_data:
     lw a3, 0(a0)
     sw a3, 0(a1)
     addi a0, a0, 4
     addi a1, a1, 4
     blt a1, a2, loop_init_data
end_init_data:

     call main
     ebreak
```

- 先初始化 `sp` 与 `gp`（这里 `gp` 指向 IO 页）。
- 第一个循环清零 `_sbss` 到 `_ebss` 的区域。
- 第二个循环把 `_sidata` 处的数据拷贝到 `_sdata`..`_edata`。
- 最后 `call main`。

_等等_：`_sbss`,`_ebss`,`_sidata`,`_sdata`,`_edata` 的值怎么得到？

链接脚本可以生成它们。`.data` 段类似：

```
    .data : AT ( _sidata ) {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        *(.sdata*)
        . = ALIGN(4);
        _edata = .;
    } > RAM
```

其中 `.` 表示当前位置。`ALIGN(4)` 确保地址对齐到 4 字节边界，因为启动代码按 word 拷贝/清零。

`.text` 段类似：

```
    .text : {
        . = ALIGN(4);
        start_spiflash1.o(.text)
        *(.text*)
        . = ALIGN(4);
        *(.rodata*)
        *(.srodata*)
        _etext = .;
        _sidata = _etext;
    } >FLASH
```

注意它把 `_sidata` 定义在 `.text` 末尾，这样 `.data` 的初值就紧跟在代码之后放在 flash 中。

现在用一个例子试试：

```
  $ cd FIRMWARE
  $ make mandel_C.spiflash1.prog
  $ cd ..
  $ ./terminal.sh
```

能跑！但 _等等_：它明显比之前慢，为什么？

因为 flash 是串行存储：地址与数据按 bit（这里按 2 bit）传输，比 BRAM 每周期直接给 32 位慢很多。

能不能优化？当然：把关键函数放到 BRAM 就行。修改链接脚本（见 [FIRMWARE/spiflash2.ld](FIRMWARE/spiflash2.ld)）：

```
    .data_and_fastcode : AT ( _sidata ) {
        . = ALIGN(4);
        _sdata = .;

	/* Initialized data */
        *(.data*)
        *(.sdata*)

	/* integer mul and div */
	*/libgcc.a:muldi3.o(.text)
	*/libgcc.a:div.o(.text)

	putchar.o(.text)
	print.o(.text)

	/* functions with attribute((section(".fastcode"))) */
	*(.fastcode*)

        . = ALIGN(4);
        _edata = .;
    } > RAM
```

这样就把某些特定函数（libgcc 的整数乘除、IO 函数）放进 RAM。链接器会把这些代码与初始化数据放在同一个段里，而启动代码会一起拷贝到 RAM，酷！

试试看：

```
  $ cd FIRMWARE
  $ make mandel_C.spiflash2.prog
  $ cd ..
  $ ./terminal.sh
```

速度会好很多。

注意 `*(.fastcode*)`：你也可以把自己的函数放进 BRAM，只要把它们放到 `.fastcode` section。C 里可以这样：

```C
 void my_function(my args ...) __attribute((section(".fastcode")));
 void my_function(my args ...) {
      ...
 }
```

**试一试** 运行 `ST_NICCC` demo（`make ST_NICCC.spiflash2.prog`）。然后把 `ST_NICCC.c` 里定义 `RV32_FASTCODE` 的那行取消注释，再跑一次。

![](tinyraytracer_tty.png)

现在我们可以在设备上跑更大的程序了：
- [FIRMWARE/pi.c](FIRMWARE/pi.c)（Fabrice Beillard：计算圆周率小数）
- [FIRMWARE/tinyraytracer.c](FIRMWARE/tinyraytracer.c)（Dmitry Sokolov：光线追踪）

它们都用浮点数。对 RV32I 内核来说，浮点运算通过 `libgcc` 的例程实现，因此可执行文件更大（`pi` 约 17kB、`tinyraytracer` 约 25kB），在 6kB RAM 时根本跑不了。SPI flash 提供的额外存储让设备有了更多可能性！

到这里，我们的设备不仅能跑标准工具（gcc）生成的代码，还能跑独立开发的现成代码（`libgcc` 的数学例程）。在你自己做的处理器上跑现成二进制，真的很令人兴奋！

## 下一篇教程

[Pipelining](PIPELINE.md)

## 所有步骤对应的文件

- [step 1](step1.v)：Blinky，太快看不出来
- [step 2](step2.v)：带 clockworks 的 blinky
- [step 3](step3.v)：从 ROM 读取图案的 blinky
- [step 4](step4.v)：指令译码器
- [step 5](step5.v)：寄存器堆与状态机
- [step 6](step6.v)：ALU
- [step 7](step7.v)：使用 VERILOG 汇编器
- [step 8](step8.v)：跳转
- [step 9](step9.v)：分支
- [step 10](step10.v)：LUI 与 AUIPC
- [step 11](step11.v)：把内存拆成单独模块
- [step 12](step12.v)：体积优化：不可思议的缩小内核！
- [step 13](step13.v)：子程序 1（标准 RISC-V 指令集）
- [step 14](step14.v)：子程序 2（使用 RISC-V 伪指令）
- [step 15](step15.v)：Load
- [step 16](step16.v)：Store
- [step 17](step17.v)：内存映射外设
- [step 18](step18.v)：Mandelbrot 集
- step 19：用 Verilator 加速仿真
- [step 20](step20.v)：用 GNU 工具链编译汇编程序
- step 21：用 GNU 工具链编译 C 程序
- [step 22](step22.v)：更多内存：使用 SPI flash
- [step 23](step23.v)：从 SPI flash 运行程序（第一步）
- [step 24](step24.v)：从 SPI flash 运行程序（更好的链接脚本）

_WIP_

- step 25：更多外设（LED 矩阵、OLED 屏幕……）
（并把上一节的 `putc` 函数拷贝过来。）

**定点数** 现在要算 Mandelbrot，我们需要处理实数。但我们的极简 RISC-V 内核不能直接做浮点运算。C 编译器的支持库 `libgcc` 里有浮点支持函数，但我们稍后再用。这里先用定点数：在一个整数里用一部分位表示小数部分（这里用 10 位），另一部分位表示整数部分（22 位）。也就是说，要表示实数 `x`，我们在寄存器里存 `x*2^10` 的整数部分。它类似浮点数，只是指数固定为 10。我们在程序里用以下常量：

```verilog
   `define mandel_shift 10
   `define mandel_mul (1 << `mandel_shift)
```

对加减来说并无区别，因为两边都有同样的 `2^10` 缩放因子；但乘法不同：你计算的是 `x*2^10*y*2^10 = (x*y)*2^20`，而你希望得到 `(x*y)*2^10`，因此要再除以 `2^10`（右移 10 位）。

那如何做乘法？我们的处理器没有 `MUL` 指令！`MUL` 属于 RV32M，我们后面会看到如何添加它，但它放不进 IceStick 的资源预算。怎么办？我们可以实现一个函数，输入 `a0`、`a1`，输出它们的乘积到 `a0`。`libgcc` 里就有这样的函数：当编译 C 给不带 `MUL` 的 RV32I 内核时就会用它。源码在
[这里](https://github.com/riscv-collab/riscv-gcc/blob/5964b5cd72721186ea2195a7be8d40cfe6554023/libgcc/config/riscv/muldi3.S)。
把它移植到我们的 VERILOG 汇编器（语法稍有不同；后面会看到如何直接用 gcc/gas）：

```verilog
      // Mutiplication routine,
      // Input in a0 and a1
      // Result in a0
   Label(mulsi3_);
      MV(a2,a0);
      LI(a0,0);
   Label(mulsi3_L0_);
      ANDI(a3,a1,1);
      BEQZ(a3,LabelRef(mulsi3_L1_));
      ADD(a0,a0,a2);
   Label(mulsi3_L1_);
      SRLI(a1,a1,1);
      SLLI(a2,a2,1);
      BNEZ(a1,LabelRef(mulsi3_L0_));
      RET();
```

（别忘了在 `initial` 块之前声明新标签。）

在真正画 Mandelbrot 之前，先用定点数思路画一个更简单的形状：把 `[-2.0,2.0]x[-2.0,2.0]` 映射到 30x30 字符区域，画一个以原点为中心、半径为 2 的圆盘。我们先计算（定点）坐标 `x,y`（存到 `s2`,`s3`），再算 `x^2+y^2`。这需要调用 `mulsi3` 两次（并把结果右移 10 位）。最后把结果与 `4<<10` 比较（4 是半径平方，`<<10` 是定点缩放），决定显示不同字符。程序如下：

```verilog
   `define mandel_shift 10
   `define mandel_mul (1 << `mandel_shift)
   `define xmin (-2*`mandel_mul)
   `define xmax ( 2*`mandel_mul)
   `define ymin (-2*`mandel_mul)
   `define ymax ( 2*`mandel_mul)
   `define dx ((`xmax-`xmin)/30)
   `define dy ((`ymax-`ymin)/30)
   `define norm_max (4 << `mandel_shift)

   integer    loop_y_      = 28;
   integer    loop_x_      = 36;
   integer    in_disk_     = 92;

   initial begin
      LI(gp,32'h400000); // IO page

      LI(s1,0);
      LI(s3,`xmin);
      LI(s11,30);
      LI(s10,`norm_max);

   Label(loop_y_);
      LI(s0,0);
      LI(s2,`ymin);

   Label(loop_x_);

      MV(a0,s2);
      MV(a1,s2);
      CALL(LabelRef(mulsi3_));
      SRLI(s4,a0,`mandel_shift); // s4 = x*x
      MV(a0,s3);
      MV(a1,s3);
      CALL(LabelRef(mulsi3_));
      SRLI(s5,a0,`mandel_shift); // s5 = y*y
      ADD(s6,s4,s5);             // s6 = x*x+y*y
      LI(a0,"*");
      BLT(s6,s10,LabelRef(in_disk_)); // if x*x+y*y < 4
      LI(a0," ");
  Label(in_disk_);
      CALL(LabelRef(putc_));

      ADDI(s0,s0,1);
      ADDI(s2,s2,`dx);
      BNE(s0,s11,LabelRef(loop_x_));

      LI(a0,13);
      CALL(LabelRef(putc_));
      LI(a0,10);
      CALL(LabelRef(putc_));

      ADDI(s1,s1,1);
      ADDI(s3,s3,`dy);
      BNE(s1,s11,LabelRef(loop_y_));

      EBREAK();
```
