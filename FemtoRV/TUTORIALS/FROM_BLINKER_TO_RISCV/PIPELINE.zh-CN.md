# 从闪烁灯到 RISC-V·第二篇：流水线

在[第一篇](README.md)里，我们学习了如何在 FPGA 上做出一个完全可用的 RISC-V 处理器。我们的处理器并不是最高效的：每条指令需要 3 到 4 个周期。现代处理器借助多种技术可以做到更高效率，甚至在一个周期里执行多条指令。本篇将演示如何把我们这个“超级简单”的处理器逐步改造成更高效的流水线处理器（pipelined processor）。

本篇需要至少 128kB BRAM 的 FPGA（例如 ULX3S）。当然也可以只在仿真中完成。

## 第 1 步：分离指令存储与数据存储

我们上一阶段的处理器 [step24.v](step24.v) 使用“统一内存”（unified memory）：取指与访存都走同一组线。对流水线处理器来说，内部结构往往不同：它会把“指令存储”和“数据存储”分开。实际的现代处理器里，它们通常是 I-cache 和 D-cache，背后通过一个统一的外部总线与主存交互。这里为了简化（因为后面才会引入 cache），我们先在处理器内部做一个“程序 ROM”和一个“数据 RAM”（各 64kB），并从 `.hex` 文件直接初始化（后面会讲如何从 `ELF` 生成这些 `.hex` 文件）：

```verilog
   reg [31:0] PROGROM [0:16383];
   reg [31:0] DATARAM [0:16383];   
   initial begin
      $readmemh("PROGROM.hex",PROGROM);
      $readmemh("DATARAM.hex",DATARAM);      
   end
```

- `PROGROM`：存放指令；
- `DATARAM`：存放变量。`LOAD`/`STORE` 只能读写这块内存（不能访问程序存储）。

之前的内存总线被内部线网替代：

```verilog
   wire [31:0] mem_addr;
   wire [31:0] mem_rdata;
   wire [31:0] mem_wdata;
   wire [3:0]  mem_wmask;
```

和之前相比，这里不再有 `mem_rstrobe` 与 `mem_rbusy`：内部存储器的访问延迟固定为 1 个周期，即“在下一拍得到 `mem_addr` 对应的数据”。

为了与外部世界通信，处理器仍然保留“映射 IO 页”的总线（用来和 `UART` 等设备通信）：

```verilog
module Processor (
    ...
    output [31:0] IO_mem_addr,  
    input [31:0]  IO_mem_rdata, 
    output [31:0] IO_mem_wdata, 
    output 	  IO_mem_wr     
);
```

现在需要把访问路由到内部 RAM 或外部 IO。我们沿用之前的 IO 页约定（以便复用同一套软件），用地址 bit22 表示“是否访问 IO 页”：

```verilog
   wire isIO  = mem_addr[22];
   wire isRAM = !isIO;
```

数据 RAM 每拍读一次，并且可选按 byte 写入（`mem_wmask`）：

```verilog
   wire [13:0] mem_word_addr = mem_addr[15:2];
   reg [31:0] dataram_rdata;
   wire [3:0] dataram_wmask = mem_wmask & {4{isRAM}};
   always @(posedge clk) begin
      dataram_rdata <= DATARAM[mem_word_addr];
      if(dataram_wmask[0]) DATARAM[mem_word_addr][ 7:0 ] <= mem_wdata[ 7:0 ];
      if(dataram_wmask[1]) DATARAM[mem_word_addr][15:8 ] <= mem_wdata[15:8 ];
      if(dataram_wmask[2]) DATARAM[mem_word_addr][23:16] <= mem_wdata[23:16];
      if(dataram_wmask[3]) DATARAM[mem_word_addr][31:24] <= mem_wdata[31:24];
   end
```

然后把外部 IO 总线接上：

```verilog
   assign mem_rdata = isRAM ? dataram_rdata : IO_mem_rdata;
   assign IO_mem_addr  = mem_addr;
   assign IO_mem_wdata = mem_wdata;
   assign IO_mem_wr    = isIO & mem_wmask[0];
```

最后还需要几处简单修改：
- 在 `FETCH_INSTR` 状态从 `PROGROM` 取指：`instr <= PROGROM[PC[15:2]];`
- 不再有 `mem_rbusy`（`DATARAM`/`PROGROM` 都是固定 1 周期访问），因此状态机可简化。代价是：我们不能像之前那样从 SPI flash 直接执行程序（当 `PROGROM` 被替换成“指令 cache”后又能做到了）。

更新后的 VERILOG 在这里：[pipeline1.v](pipeline1.v)

我们也需要为新内核写软件。软件以两个 ASCII 十六进制文件形式提供：`PROGROM.hex` 与 `DATARAM.hex`，分别对应程序存储内容与数据存储初值。第一步是写一个新的链接脚本，描述内存映射：

```
MEMORY {
   PROGROM (RX) : ORIGIN = 0x00000, LENGTH = 0x10000  /* 64kB ROM */
   DATARAM (RW) : ORIGIN = 0x10000, LENGTH = 0x10000  /* 64kB RAM */   
}
```

也就是：前 64kB 为 ROM，后 64kB 为 RAM。

然后描述段布局：把文本段放入 `PROGROM`，其余段放入 `DATARAM`：

```
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

文本段以 `start_pipeline.S` 开头：

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

使用这个链接脚本，就能生成一个 ELF：代码在前 64kB，数据在后 64kB。下面是编译 Fabrice Bellard 的 `pi`（计算圆周率小数）示例的方式：

```
$ cd FIRMWARE
$ riscv64-unknown-elf-gcc -Os -fno-pic -march=rv32i -mabi=ilp32 -fno-stack-protector -w -Wl,--no-relax   -c pi.c
$ riscv64-unknown-elf-as -march=rv32i -mabi=ilp32   start_pipeline.S -o start_pipeline.o 
$ riscv64-unknown-elf-as -march=rv32i -mabi=ilp32   putchar.S -o putchar.o 
$ riscv64-unknown-elf-as -march=rv32i -mabi=ilp32   wait.S -o wait.o 
$ riscv64-unknown-elf-gcc -Os -fno-pic -march=rv32i -mabi=ilp32 -fno-stack-protector -w -Wl,--no-relax   -c print.c
$ riscv64-unknown-elf-gcc -Os -fno-pic -march=rv32i -mabi=ilp32 -fno-stack-protector -w -Wl,--no-relax   -c memcpy.c
$ riscv64-unknown-elf-gcc -Os -fno-pic -march=rv32i -mabi=ilp32 -fno-stack-protector -w -Wl,--no-relax   -c errno.c
$ riscv64-unknown-elf-ld -T pipeline.ld -m elf32lriscv -nostdlib -norelax pi.o putchar.o wait.o print.o memcpy.o errno.o -lm libgcc.a -o pi.pipeline.elf
```

`Makefile` 已经封装好了：

```
$ make pi.pipeline.elf
```

你可以查看内存布局：

```
$ readelf -a pi.pipeline.elf | more
```

会看到 `.text` 与 `.data` 位于预期的位置：

```
Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        00000000 000074 003fd8 00  AX  0   0  4
  [ 2] .data             PROGBITS        00010000 004050 0002c0 00  WA  0   0  8
```

要生成 `PROGROM.hex` 与 `DATARAM.hex`，可以使用 `firmware_words` 工具。它支持 `-from_addr` 与 `-to_addr` 来选择要导出的地址范围：

```
$ firmware_words pi.pipeline.elf -ram 0x20000 -max_addr 0x20000 -out pi.PROGROM.hex -from_addr 0 -to_addr 0xFFFF
$ firmware_words pi.pipeline.elf -ram 0x20000 -max_addr 0x20000 -out pi.DATARAM.hex -from_addr 0x10000 -to_addr 0x1FFFF
```

同样，`Makefile` 会自动处理（并把 `.hex` 文件复制到需要的位置）：

```
$ make clean
$ make pi.pipeline.hex
```

_(必须先 `make clean`，否则已有的 `.elf` 会让它“搞混”)_

现在可以在仿真中运行 `pi`：

```
$ cd ..
$ ./run_verilator.sh pipeline1.v
```

也可以试试其它程序（例如 `tinyraytracer`）：

```
$ cd FIRMWARE
$ make tinyraytracer.pipeline.hex
$ cd ..
$ ./run_verilator.sh pipeline1.v
```

如果你有 ULX3S，把它插到 USB 口，然后：

```
$ BOARDS/run_ulx3s.sh pipeline1.v
$ ./terminal.sh
```

## 第 2 步：性能计数器

流水线的目标是提升性能，因此我们需要一种衡量性能的方法。RISC-V ISA 定义了一组特殊寄存器，称为 CSR（Control and Status Registers）。CSR 很多，用于中断、权限级别、虚拟内存等；这里我们只实现其中两个（或四个）：

- `CYCLE`：统计时钟周期数
- `INSTRET`：统计已退休（retired）的指令数

这样就能通过 `CYCLE/INSTRET` 计算 CPI（cycles per instruction，每条指令平均周期数）。

`CYCLE` 与 `INSTRET` 可由以下指令读取：
- `RDCYCLE`
- `RDCYCLEH`
- `RDINSTRET`
- `RDINSTRETH`

之所以每个计数器有两条指令，是因为它们是 64 位宽：低 32 位用 `RDCYCLE`（或 `RDINSTRET`）读到 `rd`，高 32 位用 `RDCYCLEH`（或 `RDINSTRETH`）读到 `rd`。

事实上这四条都是伪指令，最终都由一条真实指令 `CSRRS` 实现，编码如下：

| instr[31:20] | instr[19:15] | instr[14:12] | instr[11:7] | instr[6:0] |
|--------------|--------------|--------------|-------------|------------|
|   CSR Id     |     rs1      |  funct3      |     rd      |   SYSTEM   |
|              |   5'b00000   |  3'b010      |             | 7'b1110011 |

`CSRRS` 会原子性地把 CSR 的当前值读到 `rd`，并把 `rs1` 指定的位 OR 进该 CSR。如果 `rs1` 为 `x0`，则只是单纯“读 CSR 到 rd”（这正是 `RDXXX` 伪指令的用法）。

- 指令字的高 12 位编码 CSR Id
- 这里 `rs1` 恒为 `x0`
- `funct3=3'b010` 表示 `CSRRS`
- `rd` 的位置与其它指令一致
- opcode 为 `SYSTEM`（与我们已用到的 `EBREAK` 相同）。我们通过 `funct3` 区分 `EBREAK`（`3'b000`）与 `CSRRS`（`3'b010`）。

我们需要识别的 4 个 CSR Id 为：

| CSR     | Id  | Id (binary)  |
|---------|-----|--------------|
| CYCLE   | C00 | 110000000000 |
| CYCLEH  | C80 | 110010000000 |
| INSTRET | C02 | 110000000010 |
| INSTETH | C82 | 110010000010 |

在设计里，声明两个 64 位寄存器。`cycle` 每拍自增：

```verilog
   reg [63:0] cycle;   
   reg [63:0] instret;
   
   always @(posedge clk) begin
      cycle <= cycle + 1;
   end
```

而 `instret` 在每次从程序存储取到一条指令时递增（这里用“取指”作为退休的近似）：

```verilog
     ...
     WAIT_INSTR: begin
        rs1 <= RegisterBank[instr[19:15]];
        rs2 <= RegisterBank[instr[24:20]];
        state <= EXECUTE;
        instret <= instret + 1;
     end
     ...
```     

在译码器里需要区分同为 `SYSTEM` opcode 的 `EBREAK` 与 `CSRRS`：

```verilog
   wire isEBREAK     = isSYSTEM & (funct3 == 3'b000);
   wire isCSRRS      = isSYSTEM & (funct3 == 3'b010);
```

然后定义读 CSR 的返回值：

```verilog
   wire [31:0] CSR_data =
	       ( instr[27] & instr[21]) ? instret[63:32]:
	       (!instr[27] & instr[21]) ? instret[31:0] :
	             instr[27]          ? cycle[63:32]  :
 	                                  cycle[31:0]   ;
```

（这里只检查能区分这 4 个 CSR 的若干个 bit。）

最后，当指令为 `CSRRS` 时，把它接到 `writeBackData`：

```verilog
   assign writeBackData = (isJAL || isJALR) ? PCplus4   :
			      isLUI         ? Uimm      :
			      isAUIPC       ? PCplusImm :
			      isLoad        ? LOAD_data :
			      isCSRRS       ? CSR_data  :
			                      aluOut;
```

对应的 VERILOG 版本在这里：[pipeline2.v](pipeline2.v)。

现在需要写一些工具函数，方便从 C 里读取计数器。实现位于 [FIRMWARE/perf.S](FIRMWARE/perf.S)。例如 `rdcycle()`：

```asm
rdcycle:
.L0:  
   rdcycleh a1
   rdcycle a0
   rdcycleh t0
   bne a1,t0,.L0
   ret
```

这里有两点需要知道：
- RISC-V RV32 ABI 规定：返回 64 位值时，用 `a1`（高 32）与 `a0`（低 32）返回；
- 读 64 位计数器需要两条指令，读低 32 位时可能发生回绕。按 RISC-V programmer’s manual 的建议，可以读两次高 32 位并比较，直到两次一致为止。

因为遵守 ABI，该函数可从 C 调用。我们在 [perf.h](perf.h) 里这样声明：

```C
#include <stdint.h>
extern uint64_t rdcycle();
```

（`rdinstret()` 同理。）

现在用一个简单的 C 程序 [FIRMWARE/test_rdcycle.c](FIRMWARE/test_rdcycle.c) 测试：

```C
#include "perf.h"

int main() {
   for(int i=0; i<100; ++i) {
      uint64_t cycles = rdcycle();
      uint64_t instret = rdinstret();      
      printf("i=%d    cycles=%d     instret=%d\n", i, (int)cycles, (int)instret);
   }
   uint64_t cycles = rdcycle();
   uint64_t instret = rdinstret();      
   printf("cycles=%d     instret=%d    100CPI=%d\n", (int)cycles, (int)instret, (int)(100*cycles/instret));
   
}
```

编译、生成 ROM/RAM 初始化内容并仿真：

```
$ cd FIRMWARE
$ make test_rdcycle.pipeline.hex
$ cd ..
$ ./run_verilator.sh pipeline2.v
```

_注 1：_ 如果你好奇：`perf.S` 已经被 `Makefile` 列入编译与链接列表。  
_注 2：_ 我们的 `printf()` 无法打印浮点数，所以输出 `100*CPI`。

对这个简单循环，我们会得到大约 `3.14` CPI。现在用更现实的程序（比如 `tinyraytracer`）测试。我们在 [FIRMWARE/raystones.c](FIRMWARE/raystones.c) 里加入计算 CPI 与 “raystones” 指标的代码。运行：

```
$ cd FIRMWARE
$ make raystones.pipeline.hex
$ cd ..
$ ./run_verilator.sh pipeline2.v
```

它会计算并显示一个简单光线追踪场景，并给出 CPI 与 “raystones” 分数。

也可以在设备上跑（ULX3S）：

```
$ BOARDS/run_ulx3s.sh pipeline1.v
$ ./terminal.sh
```

该程序是 Dmitry Sokolov 的 [tinyraytracer](https://github.com/ssloy/tinyraytracer) 的 C 版本。光线追踪很适合做内核性能基准，因为它大量使用浮点运算（可以是软件实现，也可以由 FPU 实现）。

除了 CPI，程序还会给出 “raystones” 分数。

“raystones”（`pixels/s/MHz` 或 `pixels/Mticks`）是一个衡量浮点性能的有趣指标。对于我们的内核，得到的典型数值为：

| CPI   | RAYSTONES |
|-------|-----------|
| 3.034 | 2.614     |

我们的内核平均略高于 3 CPI：多数指令 3 周期，load 需要 4 周期。对更“数据密集”的程序，平均 CPI 会更接近 4。

下面表格给出若干常见内核的 raystones：

 | core                 | instr set  | raystones |
 |----------------------|------------|-----------|
 | serv                 | rv32i      |   0.111   |
 |                      |            |           |
 | picorv32-minimal     | rv32i      |   1.45    |
 | picorv32-standard    | rv32im     |   2.352   |
 |                      |            |           |
 | femtorv-quark        | rv32i      |   1.99    |
 | femtorv-electron     | rv32im     |   3.373   |
 | femtorv-gracilis     | rv32imc    |   3.516   |
 | femtorv-petitbateau  | rv32imfc   |  45.159   |
 |                      |            |           |
 | vexriscv imac        | rv32imac   |   7.987   |
 | vexriscv_smp         | rv32imafd  | 124.121   |

本内核比 `femtorv-quark` 快，因为它有桶形移位器（barrel shifter），移位 1 周期完成；但比 `femtorv-electron` 慢，后者支持 `rv32im`，乘法 1 周期，除法 32 周期。

流水线能带来多少？理想情况下流水线可以做到 1 CPI，但实际会因数据相关与分支产生停顿（stall），后面会讲。对比 `femtorv-gracilis`（`rv32imc`）与流水线的 `vexriscv imac`，可以看到 `vexriscv` 速度超过 2 倍。

因此我们的目标是把当前处理器改造成流水线版本，类似但比 `vexriscv` 更简单：目前 ALU（仅支持 `rv32i`）是单周期；程序存储与数据存储都在 BRAM 里，读写 1 周期。后面我们会再引入 cache。

_关于 RAYSTONES 的注记：_ 所有 RAYSTONES 统计应当在 [FIRMWARE/raystones.c](FIRMWARE/raystones.c) 中取消注释 `#define NO_GRAPHIC` 后获取。原因是 [FIRMWARE/putchar.S](FIRMWARE/putchar.S) 的 `putchar()` 会发送字符并等待 UART 不忙；等待循环迭代次数会随着 CPU 频率与 UART 波特率比值而变化非常大。

## 第 3 步：顺序驱动的 5 级流水线

流水线处理器可以看作是“多周期处理器 + 状态机”的推广：多周期处理器把一条指令拆成多个阶段，并用状态机按顺序执行；流水线则让每个阶段拥有自己的电路，并在不同指令上**并行**工作，阶段之间通过寄存器传递信息（称为 stage，而不是 state）。

由于流水线会遇到一些棘手情况（相关、分支等），这里采用循序渐进的方式：先把 5 个阶段都做出来，但仍用一个状态机“每拍只激活一个阶段”，也就是让阶段顺序运行。这样我们先把结构搭对、把数据在阶段间传递跑通；下一步再让所有阶段并行运行。

按照处理器设计经典教材（Patterson & Hennessy, Harris & Harris）的套路，我们从最经典的 5 级流水线开始：

| 缩写 | 全称                  | 作用                                |
|------|-----------------------|-------------------------------------|
| I**F**  | Instruction fetch     | 从程序存储读取指令                  |
| I**D**  | Instruction decode    | 指令译码并提取立即数                |
| **E**X  | Execute               | 计算 ALU、比较、地址等              |
| **M**EM | read or write memory  | load/store 访问数据存储             |
| **W**B  | Write back            | 写回寄存器堆                        |

（下文使用单字母阶段名 `F,D,E,M,W`，而不是更常见的 `IF,ID,EX,MEM,WB`。原因是后面我们会给“流水线寄存器”命名：例如 `FD_` 表示 `F` 写出的寄存器、`D` 读入的寄存器。）

每个阶段从一组寄存器读取输入，并把输出写到另一组寄存器。这些寄存器称为“流水线寄存器”（pipeline registers），不同于 RISC-V ISA 的通用寄存器（architectural registers），它们只用于内部传递与记账。

本步为了便于调试，我们让每个阶段都保留一份 PC 与 instr（真实实现里后级可以不再保留，但这里先全保留更直观）。

### 用状态机顺序执行 5 个阶段

我们先用一个 one-hot 状态机依次激活 `F,D,E,M,W`：

```verilog
   localparam F_bit = 0; localparam F_state = 1 << F_bit;
   localparam D_bit = 1; localparam D_state = 1 << D_bit;
   localparam E_bit = 2; localparam E_state = 1 << E_bit;
   localparam M_bit = 3; localparam M_state = 1 << M_bit;
   localparam W_bit = 4; localparam W_state = 1 << W_bit;
   reg [4:0] 	  state;
   wire           halt;
   always @(posedge clk) begin
      if(!resetn) begin
	 state  <= F_state;
      end else if(!halt) begin
	 state <= {state[3:0],state[4]};
      end
   end
```

下一步我们会移除这个状态机，让阶段并行运行。现在先顺序运行阶段，有助于逐步引入复杂性，并用 `RAYSTONES` 每一步验证 CPU 仍能工作。

### “规则”：流水线寄存器命名与存储器访问约束

为了避免混乱，我们强制命名规则：阶段 `B` 从 `AB_` 前缀寄存器读输入，写输出到 `BC_` 前缀寄存器。

```verilog
   always @(posedge clk) begin
     if(state[B_bit]) begin
         BC_xxx <= AB_aaa;
	 BC_yyy <= AB_bbb;
	 ...
     end
   end
```

阶段内部可以有组合逻辑中间线（都是 `wire`），用 `B_` 前缀；其输入只能来自 `AB_` 寄存器或 `B_` 线。

对存储器访问还有一个关键约束：如果某阶段读存储器，那么该阶段只能把“读出的 word 原样”写入下一阶段寄存器，不能在同拍对读出值做额外处理。这能让综合器把 RAM/ROM 映射到带寄存输出口的 BRAM。

但有两个“例外信号”会违反“只从上一阶段读，只写到下一阶段”的单向规则：
- 跳转/分支：后级（通常 `E`）才能得出新 PC，需要通过 `jumpOrBranch` 与 `jumpOrBranchAddress` 回传到 `F`；
- 写回寄存器堆：后级（`W`）产生写回信息，需要通过 `wbEnable/wbRdId/wbData` 回写到 `D` 的寄存器堆写口。

这两条“逆流信号”正是后续控制冒险/数据冒险的根源。

为了调试，我写了一个 [RISC-V 反汇编器](riscv_disassembly.v)，可在仿真中显示流水线各阶段的指令，定位问题非常有用。

### Fetch（取指）

```verilog
   reg  [31:0] F_PC;
   reg  [31:0] PROGROM[0:16383]; 

   initial begin
      $readmemh("PROGROM.hex",PROGROM);
   end

   always @(posedge clk) begin
      if(!resetn) begin
	 F_PC    <= 0;
      end else if(state[F_bit]) begin
	 FD_instr <= PROGROM[F_PC[15:2]];
	 FD_PC    <= F_PC;
	 F_PC     <= F_PC+4;
      end else if(jumpOrBranch) begin
	 F_PC  <= jumpOrBranchAddress;	 
      end      
   end
   
/******************************************************************************/
   reg [31:0] FD_PC;   
   reg [31:0] FD_instr;
/******************************************************************************/

```

`F` 阶段维护 `F_PC` 并从程序 ROM 取指，输出 `FD_PC/FD_instr` 给 `D` 阶段。它还会响应后级发来的 `jumpOrBranch`，把 PC 改成 `jumpOrBranchAddress`。

### Decode（译码与读寄存器）

`D` 阶段负责识别 opcode、提取立即数、从寄存器堆取操作数。为简化起见，此处“需要时再译码”，并把 `PC` 与 `instr` 原样传递到后续阶段，以便每一级都能用反汇编器显示当前指令（代价是会出现多个译码器，后面再优化）。

```verilog
   reg [31:0] RegisterBank [0:31];
   
   always @(posedge clk) begin
      if(state[D_bit]) begin
	 DE_PC    <= FD_PC;
	 DE_instr <= FD_instr;
	 DE_rs1 <= RegisterBank[rs1Id(FD_instr)];
	 DE_rs2 <= RegisterBank[rs2Id(FD_instr)];
      end
   end

   always @(posedge clk) begin
      if(wbEnable) begin
	 RegisterBank[wbRdId] <= wbData;
      end
   end
   
/******************************************************************************/
   reg [31:0] DE_PC;
   reg [31:0] DE_instr;
   reg [31:0] DE_rs1;
   reg [31:0] DE_rs2;
/******************************************************************************/
```

为可读性定义了一些从指令字提取字段/立即数的 helper functions（后续阶段也会用）：

```verilog
   function isALUreg; input [31:0] I; isALUreg=(I[6:0]==7'b0110011); endfunction
   function isALUimm; input [31:0] I; isALUimm=(I[6:0]==7'b0010011); endfunction
   function isBranch; input [31:0] I; isBranch=(I[6:0]==7'b1100011); endfunction
   ...
   function [4:0] rs1Id; input [31:0] I; rs1Id = I[19:15];      endfunction
   function [4:0] rs2Id; input [31:0] I; rs2Id = I[24:20];      endfunction
   function [4:0] rdId;  input [31:0] I; rdId  = I[11:7];       endfunction
   ...
   function [2:0] funct3; input [31:0] I; funct3 = I[14:12]; endfunction
   function [6:0] funct7; input [31:0] I; funct7 = I[31:25]; endfunction
   ...
   function [31:0] Uimm; input [31:0] I; Uimm={I[31:12],{12{1'b0}}}; endfunction
   ...
```

### Execute（执行）

`E` 阶段与第一篇类似：组合逻辑计算 `E_aluOut`（运算结果）与 `E_takeBranch`（分支比较结果）。在此基础上构造“是否跳转/分支”和“跳转/分支目标地址”，以及本条指令最终“结果值”（用于写回或传递）：

```verilog
   wire E_JumpOrBranch = (
         isJAL(DE_instr)  || 
         isJALR(DE_instr) || 
         (isBranch(DE_instr) && E_takeBranch)
   );

   wire [31:0] E_JumpOrBranchAddr =
	isBranch(DE_instr) ? DE_PC + Bimm(DE_instr) :
	isJAL(DE_instr)    ? DE_PC + Jimm(DE_instr) :
	/* JALR */           {E_aluPlus[31:1],1'b0} ;

   wire [31:0] E_result = 
	(isJAL(DE_instr) | isJALR(DE_instr)) ? DE_PC+4                :
	isLUI(DE_instr)                      ? Uimm(DE_instr)         :
	isAUIPC(DE_instr)                    ? DE_PC + Uimm(DE_instr) : 
        E_aluOut                                                      ;
```

然后把 PC、instr、rs2、Eresult、访存地址等锁存到 `EM_` 流水线寄存器：

```verilog
   always @(posedge clk) begin
      if(state[E_bit]) begin
	 EM_PC      <= DE_PC;
	 EM_instr   <= DE_instr;
	 EM_rs2     <= DE_rs2;
	 EM_Eresult <= E_result;
	 EM_addr    <= isStore(DE_instr) ? DE_rs1 + Simm(DE_instr) : 
                                           DE_rs1 + Iimm(DE_instr) ;
      end
   end
   
/******************************************************************************/
   reg [31:0] EM_PC;
   reg [31:0] EM_instr;
   reg [31:0] EM_rs2;
   reg [31:0] EM_Eresult;
   reg [31:0] EM_addr;
/******************************************************************************/
```

并把“改 PC”的例外信号送回 `F`：

```verilog
   assign jumpOrBranchAddress = E_JumpOrBranchAddr;
   assign jumpOrBranch        = E_JumpOrBranch & state[M_bit];
```

### Memory 与 WriteBack（访存与写回）

`M` 阶段会访问数据 RAM。由于“读存储器的阶段只能把读出的 word 原样写入下一阶段寄存器”，load 的对齐与符号扩展被推迟到 `W` 阶段完成；而 store 的对齐与写掩码可以在 `M` 阶段准备，因为那是写入存储器的输入，不违反规则。

`W` 阶段通过 mux 选择最终写回数据并产生 `wbData/wbEnable/wbRdId`：

```verilog
   assign wbData = 
	       isLoad(MW_instr)  ? (W_isIO ? MW_IOresult : W_Mresult) :
	       isCSRRS(MW_instr) ? MW_CSRresult :
	       MW_Eresult;

   assign wbEnable =
        !isBranch(MW_instr) && !isStore(MW_instr) && (rdId(MW_instr) != 0);

   assign wbRdId = rdId(MW_instr);
```

完整实现位于 [pipeline3.v](pipeline3.v)。

运行基准：

```
$ cd FIRMWARE
$ make raystones.pipeline.hex
$ cd ..
$ ./run_verilator.sh pipeline3.v
```

得到的结果（与 pipeline2 对比）：

| 版本        | 描述                    | CPI   | RAYSTONES |
|-------------|-------------------------|-------|-----------|
|pipeline2.v  | 3-4 状态多周期           | 3.034 | 2.614     |
|pipeline3.v  | “顺序流水线”（阶段顺序跑）| 5     | 1.589     | 

虽然 CPI 变差了，但如果你综合对比会发现 fmax 变高了：阶段更简单、关键路径更短。

**小结**：我们把第一篇的 3-4 状态核心改造成 5 段结构（Fetch/Decode/Execute/Memory/WriteBack），并遵守“阶段只依赖上一段、输出写入下一段”的规则（除跳转/写回两条例外）。下一步将让 5 个阶段并行运行，真正进入“流水线”。

## 第 4 步：真正的流水线（并行运行 5 个阶段）

我们从上一节的核心出发，对它做一点“手术”：
- 去掉状态机；
- 去掉所有类似 `if(state[F_bit])`、`if(state[D_bit])`、`if(state[E_bit])`… 的条件；
- 去掉 `IO_mem_wr` 与 `M_wmask` 里对 `state[M_bit]` 的引用；
- 然后开始思考：这样改完会出什么问题？

会遇到两类典型问题：
- **控制冒险（control hazards）**：跳转/分支的目标 PC 在流水线里太晚才知道；
- **数据冒险（data hazards）**：寄存器写回太晚，后面的指令读到了旧值。

### 控制冒险：跳转/分支导致的“错误取指”

先看一个简单程序（helloC）：

```
$ cd FIRMWARE
$ make helloC.pipeline.hex
```

它会生成 `helloC.pipeline.elf.list`，开头大致如下：

```asm
00000000 <start>:
   0:	004001b7          	lui	x3,0x400
   4:	00020137          	lui	x2,0x20
   8:	008000ef          	jal	x1,10 <main>
   c:	00100073          	ebreak

00000010 <main>:
  10:	ff010113          	addi	x2,x2,-16 # 1fff0 <val.1514+0xffb8>
  14:	00112623          	sw	x1,12(x2)
  18:	004007b7          	lui	x15,0x400
  1c:	0ff00713          	li	x14,255
  20:	00010537          	lui	x10,0x10
  ...
```

如果流水线并行运行，指令会像下面这样进入各级（示意）：

| clk  | F              | D            | E            | M            | W            |
|------|----------------|--------------|--------------|--------------|--------------|
|  1   | lui x3,0x400   | nop          | nop          | nop          | nop          |
|  2   | lui x2,0x20    | lui x3,0x400 | nop          | nop          | nop          |
|  3   | jal x1,0x10    | lui x2,0x20  | lui x3,0x400 | nop          | nop          | 
|  4   | ebreak         | jal x1,0x10  | lui x2,0x20  | lui x3,0x400 | nop          |
|  5   | addi x2,x2,-16 | ebreak       | jal x1,0x10  | lui x2,0x20  | lui x3,0x400 |

在 clk=3，`jal` 进入 `F`；clk=4 进入 `D`；clk=5 进入 `E`，此时 `E` 才能确定目标地址 `0x10`，希望 clk=6 起 `F` 应该从 `0x10` 取指。但问题是：在此之前已经有两条“顺序流”的指令进了流水线（`PC+4` 在 D，`PC+8` 在 F），它们其实不该存在（除非恰好跳转目标正好等于其中之一——这里 `addi` 恰好在 0x10 是“巧合”）。

如果什么都不做，clk=6 会出现这种状态：

| clk  | F              | D              | E            | M            | W            |
|------|----------------|----------------|--------------|--------------|--------------|
|  6   | addi x2,x2,-16 | addi x2,x2,-16 | ebreak       | jal x1,0x10  | lui x2,0x20  | 

- `E` 里的 `ebreak` 不该执行（它来自 PC+4，被跳转“跨过去”了）；
- `D` 里的那条 `addi` 也不该在这里（它来自 PC+8）；
- `F` 里的 `addi` 才是正确的（它来自目标地址 0x10）。

这种“下一个 PC 值知道得太晚”的现象就是控制冒险。

解决思路很直接：把不该存在的那两条指令替换成 NOP（也叫 bubble），也就是“冲刷流水线”（flush）：

| clk  | F              | D              | E            | M            | W            |
|------|----------------|----------------|--------------|--------------|--------------|
|  6   | addi x2,x2,-16 | nop            | nop          | jal x1,0x10  | lui x2,0x20  | 

实现方式：声明 `D_flush` 与 `E_flush`，表示下一拍需要把 `D`（或 `E`）里的指令变成 `NOP`（触发条件：`E` 阶段执行的是 jump 或 taken-branch）。

最直觉的写法可能是这样的（先给出“错误做法”，因为它会引出一个重要综合陷阱）：

```verilog
   localparam NOP = 32'b0000000_00000_00000_000_00000_0110011;
   wire D_flush;
   wire E_flush;

   ...
   /* F */
   always @(posedge clk) begin
      FD_instr <= PROGROM[F_PC[15:2]]; 
      FD_PC <= F_PC;
      F_PC <= F_PC+4;

      if(jumpOrBranch) begin
	 F_PC     <= jumpOrBranchAddress;
      end

      if(D_flush | !resetn) begin
         FD_instr <= NOP;
      end
      
      if(!resetn) begin
	 F_PC <= 0;
      end
   end
   ...
   /* D */
   always @(posedge clk) begin
      DE_PC    <= FD_PC;
      DE_instr <= E_flush ? NOP : FD_instr;
      DE_rs1 <= RegisterBank[rs1Id(FD_instr)];
      DE_rs2 <= RegisterBank[rs2Id(FD_instr)];
      if(wbEnable) begin
	 RegisterBank[wbRdId] <= wbData;
      end
   end
```

并把 `D_flush/E_flush` 接到 `E_JumpOrBranch`：

```verilog
   assign D_flush = E_JumpOrBranch;
   assign E_flush = E_JumpOrBranch;
```

但这段代码有一个大问题：你能看出来吗？

回忆规则：访问存储器时，必须只把读出的值锁存到输出寄存器，不能再做其它逻辑。这里我们在 `F` 阶段读了 `PROGROM`，同时又在同一个 always 块里“条件性把 FD_instr 改成 NOP”。仿真中看似没问题，但综合时会非常糟糕：综合器可能无法把 `PROGROM` 映射成 BRAM 的寄存输出口，进而尝试用巨大的组合存储（flip-flop 阵列 + 巨型译码）来实现 64kB ROM，导致面积爆炸、布线不收敛。

正确做法是：不要直接覆盖 `FD_instr`，而是增加一个标志寄存器 `FD_nop`，表示“这一拍取到的指令在下一阶段应当视为 NOP”。`F` 阶段只负责把 ROM 数据原样写进 `FD_instr`，并把 `FD_nop` 置位：

```verilog
   /* F */
   always @(posedge clk) begin
      FD_instr <= PROGROM[F_PC[15:2]]; 
      FD_PC    <= F_PC;
      F_PC     <= F_PC+4;
      if(jumpOrBranch) begin
	 F_PC     <= jumpOrBranchAddress;
      end

      FD_nop <= D_flush | !resetn;
      
      if(!resetn) begin
	 F_PC <= 0;
      end
   end
   ...
   reg FD_nop;
```

然后 `D` 阶段根据 `FD_nop`/`E_flush` 决定是否把进入 `E` 的指令变成 NOP：

```verilog
   /* D */
   always @(posedge clk) begin
      ...
      DE_instr <= (E_flush | FD_nop) ? NOP : FD_instr;
      ...
   end
```

（并且要记得：每当使用 `FD_instr` 时，都要考虑 `FD_nop` 的存在。）

这样我们就解决了 jump/branch 的控制冒险：每次 jump 或 taken-branch 会向流水线注入两个 bubble，因此它们的代价是 3 个周期（指令本身 1 个周期 + 两个 NOP）。

但并行流水线还会遇到第二类问题：数据冒险。

### 数据冒险：后续指令读到了旧寄存器值

继续看上面 helloC 的片段。修好控制冒险后，流水线推进大致如下：

| clk  | F              | D              | E              | M              | W              |
|------|----------------|----------------|----------------|----------------|----------------|
|  7   | sw   x1,12(x2) | addi x2,x2,-16 | nop            | nop            | jal x1,0x10    |
|  8   | lui  x15,0x400 | sw   x1,12(x2) | addi x2,x2,-16 | nop            | nop            |
|  9   | li   x14,255   | lui  x15,0x400 | sw   x1,12(x2) | addi x2,x2,-16 | nop            |
|  10  | lui  x10,0x10  | li   x14,255   | lui  x15,0x400 | sw   x1,12(x2) | addi x2,x2,-16 |

问题在 clk=8：`sw x1,12(x2)` 在 `D` 阶段读寄存器 `x2`，而 `x2` 是上一条 `addi x2,x2,-16` 刚刚要写的结果。但该结果要到 clk=10（`addi` 进入 `W` 阶段）才真正写回寄存器堆。所以 clk=8 时 `sw` 读到的是旧的 `x2`，写到错误地址——这就是数据冒险。

最朴素的解决办法：让 `sw` 在 `D` 阶段“停住”等待，直到前面的 `addi` 写回完成；同时在 `E` 阶段插入 NOP（bubble）。也就是说：
- `D` stage stall（停顿）
- `F` 也必须 stall（因为前面阶段不能继续推进，否则会丢指令）
- `E` flush（插 bubble）

示意：

| clk  | F              | D              | E              | M              | W              |
|------|----------------|----------------|----------------|----------------|----------------|
|  8   | lui  x15,0x400 | sw   x1,12(x2) | addi x2,x2,-16 | nop            | nop            |
|  9   | lui  x15,0x400 | sw   x1,12(x2) | nop            | addi x2,x2,-16 | nop            |
|  10  | lui  x15,0x400 | sw   x1,12(x2) | nop            | nop            | addi x2,x2,-16 | 
|  11  | lui  x15,0x400 | sw   x1,12(x2) | nop            | nop            | nop            |
|  12  | li   x14,255   | lui  x15,0x400 | sw   x1,12(x2) | nop            | nop            | 

实现上声明两个 stall 信号 `F_stall` 与 `D_stall`。当一个阶段 stall 时，它不更新自己的输出寄存器（保持上一拍值）；但如果同时 flush，则 flush 优先（把指令置为 NOP）。

`F` 与 `D` 更新如下：

```verilog

   ...
   /* F */
   always @(posedge clk) begin
      if(!F_stall) begin
	 FD_instr <= PROGROM[F_PC[15:2]]; 
	 FD_PC    <= F_PC;
	 F_PC     <= F_PC+4;
      end

      if(jumpOrBranch) begin
	 F_PC     <= jumpOrBranchAddress;
      end

      FD_nop <= D_flush | !resetn;
      
      if(!resetn) begin
	 F_PC <= 0;
      end
   end
   ...
   /* D */
   always @(posedge clk) begin
      if(!D_stall) begin
	 DE_PC    <= FD_PC;
	 DE_instr <= (E_flush | FD_nop) ? NOP : FD_instr;
      end
      
      if(E_flush) begin
	 DE_instr <= NOP;
      end
      
      DE_rs1 <= RegisterBank[rs1Id(FD_instr)];
      DE_rs2 <= RegisterBank[rs2Id(FD_instr)];
      
      if(wbEnable) begin
	 RegisterBank[wbRdId] <= wbData;
      end
   end
```

数据冒险检测逻辑如下（当 `FD_instr` 读到的 `rs1/rs2` 与 `DE/EM/MW` 中某条将要写回的 `rd` 相同，就需要 stall）：

```verilog
   wire rs1Hazard = !FD_nop && readsRs1(FD_instr) && rs1Id(FD_instr) != 0 && (
               (writesRd(DE_instr) && rs1Id(FD_instr) == rdId(DE_instr)) ||
               (writesRd(EM_instr) && rs1Id(FD_instr) == rdId(EM_instr)) ||
	       (writesRd(MW_instr) && rs1Id(FD_instr) == rdId(MW_instr)) ) ;

   wire rs2Hazard = !FD_nop && readsRs2(FD_instr) && rs2Id(FD_instr) != 0 && (
               (writesRd(DE_instr) && rs2Id(FD_instr) == rdId(DE_instr)) ||
               (writesRd(EM_instr) && rs2Id(FD_instr) == rdId(EM_instr)) ||
	       (writesRd(MW_instr) && rs2Id(FD_instr) == rdId(MW_instr)) ) ;
   
   wire dataHazard = rs1Hazard || rs2Hazard;
   
   assign F_stall = dataHazard;
   assign D_stall = dataHazard;
   
   assign D_flush = E_JumpOrBranch;
   assign E_flush = E_JumpOrBranch | dataHazard;
```

这里用到三个辅助函数，用来判断一条指令是否写 `rd`、是否读 `rs1`、是否读 `rs2`：

```verilog
   function writesRd;
      input [31:0] I;
      writesRd = !isStore(I) && !isBranch(I);
   endfunction

   function readsRs1;
      input [31:0] I;
      readsRs1 = !(isJAL(I) || isAUIPC(I) || isLUI(I));
   endfunction

   function readsRs2;
      input [31:0] I;
      readsRs2 = isALUreg(I) || isBranch(I) || isStore(I);
   endfunction
```

对照下面表，你会发现：
- 除 `Store/Branch` 外几乎都写 `rd`
- 除 `JAL/AUIPC/LUI` 外几乎都读 `rs1`
- 只有 `ALUreg/Branch/Store` 会读 `rs2`

| instruction | what it does                   |
|-------------|--------------------------------|
| ALUreg      | `rd <- rs1 OP rs2`             |
| ALUimm      | `rd <- rs1 OP Iimm`            |
| Branch      | `if(rs1 OP rs2) PC<-PC+Bimm`   |
| JALR        | `rd <- PC+4; PC<-rs1+Iimm`     |
| JAL         | `rd <- PC+4; PC<-PC+Jimm`      |
| AUIPC       | `rd <- PC + Uimm`              | 
| LUI         | `rd <- Uimm`                   |
| Load        | `rd <- mem[rs1+Iimm]`          |
| Store       | `mem[rs1+Simm] <- rs2`         |
| SYSTEM      | special                        |

有了 stall 机制后，也可以用它来实现“停机”：当执行到 `EBREAK` 时让流水线停止推进：

```verilog
   wire halt = resetn & isEBREAK(DE_instr);

   assign F_stall = dataHazard | halt;
   assign D_stall = dataHazard | halt;
```

修复控制冒险与数据冒险后的版本是 [pipeline4.v](pipeline4.v)。如果你把 [pipeline4.v](pipeline4.v) 里 `//define VERBOSE` 取消注释，然后：

```
   $ cd FIRMWARE
   $ make helloC.pipeline.hex
   $ cd ..
   $ ./run_verilator.sh pipeline4.v 
```

查看 `log.txt`，你会看到每个阶段的 PC 与指令、stall/flush 情况、写回信息等，这个工具对调试帮助非常大（我也不是一次就写对的）。

现在跑 RAYSTONES：

```
$ cd FIRMWARE
$ make raystones.pipeline.hex
$ cd ..
$ ./run_verilator.sh pipeline4.v &> log.txt
```

结果：

| 版本        | 描述                    | CPI   | RAYSTONES |
|-------------|-------------------------|-------|-----------|
|pipeline2.v  | 3-4 状态多周期           | 3.034 | 2.614     |
|pipeline3.v  | “顺序流水线”             | 5     | 1.589     | 
|pipeline4.v  | stall/flush             | 2.193 | 3.734     |

已经好很多：比上一节快超过 2 倍，也略快于最初的 3-4 状态多周期核心。不过流水线里仍有很多 bubble（NOP）。能不能少吹点泡泡？

## 第 5 步：同周期写回并读取寄存器堆（旁路/组合读）

经典教材里会说：`W` 阶段写入寄存器堆的数据，可以在**同一周期**被 `D` 阶段读到。那么为什么我们要一直 stall 到指令离开 `W`？

原因是：我们的寄存器堆此前假设映射到 BRAM，并且读有 1 周期延迟，因此 `W` 写入的值要到下一拍才读得到。这使得我们不得不把 `W` 阶段也纳入 hazard 检测。

寄存器堆也可以用触发器阵列实现，从而实现“组合读”（读无延迟）。我们后面会讲怎么做。现在先用一种折中：在 `D` 阶段读寄存器时，增加一个旁路（bypass）：如果 `D` 读的寄存器恰好就是 `W` 正在写回的寄存器，就直接用 `wbData`，不要去读寄存器堆：

```verilog
/* D */
always @(posedge clk) begin
  ...
  if(wbEnable && rdId(MW_instr) == rs1Id(FD_instr)) begin
      DE_rs1 <= wbData;
  end else begin
      DE_rs1 <= RegisterBank[rs1Id(FD_instr)];
  end

  if(wbEnable && rdId(MW_instr) == rs2Id(FD_instr)) begin
      DE_rs2 <= wbData;
  end else begin
      DE_rs2 <= RegisterBank[rs2Id(FD_instr)];
  end
  ...
end  
```

这样 `D` 就能“看到” `W` 的写回值，于是数据冒险检测可以不再测试 `W` 阶段：

```
   wire rs1Hazard = !FD_nop && readsRs1(FD_instr) && rs1Id(FD_instr) != 0 && (
               (writesRd(DE_instr) && rs1Id(FD_instr) == rdId(DE_instr)) ||
	       (writesRd(EM_instr) && rs1Id(FD_instr) == rdId(EM_instr)) );
   

   wire rs2Hazard = !FD_nop && readsRs2(FD_instr) && rs2Id(FD_instr) != 0 && (
               (writesRd(DE_instr) && rs2Id(FD_instr) == rdId(DE_instr)) ||
	       (writesRd(EM_instr) && rs2Id(FD_instr) == rdId(EM_instr)) );
```

对应源码在 [pipeline5.v](pipeline5.v)。

接下来我们再做一次“真正的组合读寄存器堆”：把 `DE_rs1/DE_rs2` 改成 wire 直接连到寄存器堆（寄存器堆仍在时钟沿写入），从而 `D` 同周期既能写回又能读：

```verilog
   reg [31:0] RegisterBank [0:31];
   always @(posedge clk) begin
      if(!D_stall) begin
	 DE_PC    <= FD_PC;
	 DE_instr <= (E_flush | FD_nop) ? NOP : FD_instr;
      end
      
      if(E_flush) begin
	 DE_instr <= NOP;
      end

      if(wbEnable) begin
	 RegisterBank[wbRdId] <= wbData;
      end
   end
   
/******************************************************************************/
   reg [31:0] DE_PC;
   reg [31:0] DE_instr;
   wire [31:0] DE_rs1 = RegisterBank[rs1Id(DE_instr)];
   wire [31:0] DE_rs2 = RegisterBank[rs2Id(DE_instr)];
/******************************************************************************/
```

完整版本在 [pipeline5_bis.v](pipeline5_bis.v)。

此时性能为：

| 版本        | 描述                   | CPI   | RAYSTONES |
|-------------|------------------------|-------|-----------|
|pipeline2.v  | 3-4 状态多周期          | 3.034 | 2.614     |
|pipeline3.v  | “顺序流水线”            | 5     | 1.589     | 
|pipeline4.v  | stall/flush            | 2.193 | 3.734     |
|pipeline5.v  | stall/flush + 组合读 RF | 1.889 | 4.330     |

还不算“爆炸式提升”，但你可以看到我们在慢慢接近 1 CPI。

## 第 6 步：寄存器前递（register forwarding）

上一节我们通过“让 `D` 直接看到 `W` 的写回值”减少了 bubble。这个思路可以推广：如果我们需要的数据已经在流水线中某个更靠后的阶段产生了，就不要等它写回寄存器堆再读，而是直接把它送到需要它的地方——这就是**前递/旁路（forwarding/bypass）**。

对 `E` 阶段来说，所需的 `rs1/rs2` 可能来自：
- `M` 阶段：也就是 `EM_Eresult`
- `W` 阶段：也就是 `wbData`

我们在 `E` 阶段入口处加两组 3 路 mux，把 `DE_rs1/DE_rs2` 替换成可能被前递覆盖的 `E_rs1/E_rs2`：

```verilog
   wire E_M_fwd_rs1 = rdId(EM_instr) != 0 && writesRd(EM_instr) && 
	              (rdId(EM_instr) == rs1Id(DE_instr));
   
   wire E_W_fwd_rs1 = rdId(MW_instr) != 0 && writesRd(MW_instr) && 
	              (rdId(MW_instr) == rs1Id(DE_instr));

   wire E_M_fwd_rs2 = rdId(EM_instr) != 0 && writesRd(EM_instr) && 
	              (rdId(EM_instr) == rs2Id(DE_instr));
   
   wire E_W_fwd_rs2 = rdId(MW_instr) != 0 && writesRd(MW_instr) && 
	              (rdId(MW_instr) == rs2Id(DE_instr));
   
   wire [31:0] E_rs1 = E_M_fwd_rs1 ? EM_Eresult :
	               E_W_fwd_rs1 ? wbData     :
	               DE_rs1;
	       
   wire [31:0] E_rs2 = E_M_fwd_rs2 ? EM_Eresult :
	               E_W_fwd_rs2 ? wbData     :
	               DE_rs2;
```

之后在 `E` 阶段所有用到 `DE_rs1/DE_rs2` 的地方都要替换为 `E_rs1/E_rs2`（非常容易漏，漏了就会诡异地坏掉——要仔细）。

有了前递以后，真正无法避免的数据冒险主要只剩一种：**load 紧跟着使用其结果的指令**。因为 load 的数据要到 `M/W` 才出现，紧跟的下一条在 `E` 阶段需要该值时还拿不到；但如果隔了一条指令，load 的结果就能从 `W` 前递到 `E`，无需 stall。

因此 pipeline 控制逻辑可以显著简化：只在 `DE` 是 load（或 `CSRRS` 这种类似“从特殊寄存器读取”的指令）且下一条指令需要用到其 rd 时，才产生 bubble：

```verilog
   wire rs1Hazard = readsRs1(FD_instr) && (rs1Id(FD_instr) == rdId(DE_instr)) ;
   wire rs2Hazard = readsRs2(FD_instr) && (rs2Id(FD_instr) == rdId(DE_instr)) ;
   wire dataHazard = !FD_nop && (isLoad(DE_instr)||isCSRRS(DE_instr)) && (rs1Hazard || rs2Hazard);
   
   assign F_stall = dataHazard | halt;
   assign D_stall = dataHazard | halt;
   assign D_flush = E_JumpOrBranch;
   assign E_flush = E_JumpOrBranch | dataHazard;
```

注：
- 严格来说应测试 `rdId(DE_instr) != 0`，但 load/CSR 读到 x0 的情况很少；即使出现也只是多插 bubble，不会错；
- 甚至可以更保守：只要 `DE` 是 load/CSR 就固定插一个 bubble（更简单，但 CPI 更差）。很多时候“更保守但更短的组合逻辑”反而能提高 fmax，这是常见权衡。

新版本在 [pipeline6.v](pipeline6.v)。基准结果：

| 版本        | 描述                       | CPI   | RAYSTONES |
|-------------|----------------------------|-------|-----------|
|pipeline2.v  | 3-4 状态多周期              | 3.034 | 2.614     |
|pipeline3.v  | “顺序流水线”                | 5     | 1.589     | 
|pipeline4.v  | stall/flush                | 2.193 | 3.734     |
|pipeline5.v  | stall/flush + 组合读 RF     | 1.889 | 4.330     |
|pipeline6.v  | stall/flush + 前递          | 1.426 | 5.714     |

很好，我们的新流水线内核比最初的 3-4 状态多周期内核快超过 2 倍。

## 第 7 步：一点点分支预测（静态 BTFNT）

目前 jumps/branches 仍然会引入两条 bubble。对 `JAL` 来说，这有点“傻”，因为 `JAL` 的目标地址 `PC+Jimm` 完全可以在 `D` 阶段就计算出来，提前送给 `F`，从而让 `JAL` 只需要 1 个周期。

做法：在 `F` 阶段把“本拍取指用的 PC”改为一个 mux 输出 `F_PC`，优先使用 `D` 阶段给出的预测地址（`D_JumpOrBranchNow/D_JumpOrBranchAddr`），其次才用原来的 `PC`：

```verilog
   reg  [31:0] PC;
   wire [31:0] F_PC = D_JumpOrBranchNow ? D_JumpOrBranchAddr : PC;
   
   always @(posedge clk) begin
      
      if(!F_stall) begin
	 FD_instr <= PROGROM[F_PC[15:2]]; 
	 FD_PC    <= F_PC;
	 PC       <= F_PC+4;
      end
      
      if(E_JumpOrBranch) begin 
	 PC     <= E_JumpOrBranchAddr;
      end

      FD_nop <= D_flush | !resetn;
      
      if(!resetn) begin
	 PC <= 0;
      end
   end
```

这样 `F` 可以从 `D` 得到“预测的当前 PC”，从 `E` 得到“需要纠正时的 PC”。

既然对 `JAL` 做了这个机制，我们顺手对 branch 做一个极简静态预测：分支更常见的模式是“向后跳（循环）更可能 taken，向前跳更可能 not taken”。只需看 `Bimm` 的符号位就能判断向前/向后，于是：
- backward branch：预测 taken（发给 `F` 分支目标）
- forward branch：预测 not taken（不发给 `F`，让它顺序取 `PC+4`）

也就是经典静态策略 BTFNT（Backwards Taken, Forwards Not Taken）：

```verilog

   wire D_predictBranch = FD_instr[31];

   wire D_JumpOrBranchNow = !FD_nop && (
           isJAL(FD_instr) || 
           (isBranch(FD_instr) && D_predictBranch))
        );
   
   wire [31:0] D_JumpOrBranchAddr =  
               FD_PC + (isJAL(FD_instr) ? Jimm(FD_instr) : Bimm(FD_instr)); 
```

并把预测结果通过 `DE_predictBranch` 传给 `E`，使得 `E` 能判断预测是否正确：

```verilog
/* D */
   always @(posedge clk) begin
      ...
      if(!D_stall) begin
         ...
	 DE_predictBranch <= D_predictBranch;
	 ...
      end
      ...
   end      
```

`E` 阶段的“纠正条件”变为：
- `JALR`：仍然只能在 `E` 知道目标（因为依赖 `rs1`），需要纠正；
- branch：当 `E_takeBranch` 与 `DE_predictBranch` 不一致时才纠正：

```verilog
   wire E_JumpOrBranch = (
         isJALR(DE_instr) || 
         (isBranch(DE_instr) && (E_takeBranch^DE_predictBranch))
   );

   wire [31:0] E_JumpOrBranchAddr =
	isBranch(DE_instr) ? 
                     (DE_PC + (DE_predictBranch ? 4 : Bimm(DE_instr))) :
	/* JALR */   {E_aluPlus[31:1],1'b0} ;
```

预测不一致时（`E_takeBranch^DE_predictBranch`），如果原先预测 taken 但实际 not taken，就纠正到 `PC+4`；反之纠正到 `PC+Bimm`。

此时 pipeline 控制仍然不变：

```verilog
   assign F_stall = dataHazard | halt;
   assign D_stall = dataHazard | halt;
   
   assign D_flush = E_JumpOrBranch;
   assign E_flush = E_JumpOrBranch | dataHazard;
```

注意：
- `D_flush/E_flush` 仍连接到未寄存的 `E_JumpOrBranch`（必须同周期决定 flush）；
- `E_JumpOrBranch` 比之前少很多：`JAL` 不再需要纠正；branch 只有预测失败才纠正；
- `JALR` 仍然会产生两条 bubble，因为要等到 `E` 才知道目标；
- 此时存在较长关键路径：`PC` 前的 mux 由 `E_JumpOrBranch` 驱动，而 `E_JumpOrBranch` 来自 ALU 与前递逻辑。一个办法是把 `E_JumpOrBranch` 与地址寄存到 `EM`，然后 mux 到 `F_PC`（而不是直接 mux 到 `PC`），从而把关键路径切断：

```verilog
   /* E */
   always @(posedge clk) begin
      ...
      EM_JumpOrBranchNow  <= E_JumpOrBranch;
      EM_JumpOrBranchAddr <= E_JumpOrBranchAddr;
   end		  
```

`F` 阶段的 `F_PC` 选择变为：

```verilog

   /* F */
   
   wire [31:0] F_PC = 
	       D_JumpOrBranchNow  ? D_JumpOrBranchAddr  :
	       EM_JumpOrBranchNow ? EM_JumpOrBranchAddr :
	                            PC;
   
   always @(posedge clk) begin
      if(!F_stall) begin
	 FD_instr <= PROGROM[F_PC[15:2]]; 
	 FD_PC    <= F_PC;
	 PC       <= F_PC+4;
      end
      
      FD_nop <= D_flush | !resetn;
      
      if(!resetn) begin
	 PC <= 0;
      end
   end
```

新版本在 [pipeline7.v](pipeline7.v)。性能如何？

| 版本        | 描述                    | CPI   | RAYSTONES |
|-------------|-------------------------|-------|-----------|
|pipeline2.v  | 3-4 状态多周期           | 3.034 | 2.614     |
|pipeline3.v  | “顺序流水线”             | 5     | 1.589     | 
|pipeline4.v  | stall/flush             | 2.193 | 3.734     |
|pipeline5.v  | stall/flush + 组合读 RF  | 1.889 | 4.330     |
|pipeline6.v  | stall/flush + 前递       | 1.426 | 5.714     |
|pipeline7.v  | 基础分支预测（BTFNT）     | 1.226 | 6.077     |

有趣的是：我们的核心已经明显快于实现 RV32IM 的 femtorv-electron（3.373 raystones）。

## 第 8 步：动态分支预测（BHT / 2-bit 饱和计数 / gshare）

现在我们换一个更一般的视角来描述分支预测机制：
- 在 `D` 阶段，根据预测策略产生“预测是否 taken”的信号 `D_predictBranch`，并据此把“预测的下一条 PC（预测目标）”送给 `F`；
- 同时 `D` 把预测结果传给 `E`；
- `E` 阶段计算真实的分支结果，若与预测不一致，就发出纠正 PC，并冲刷两条指令（两条 bubble）。

之前的 BTFNT 是静态预测：只看当前指令，不维护状态。更聪明的办法是让预测器“学习”历史：为每个分支记录最近的结果（taken/not taken）。当然不可能为所有 PC 都记录完整历史，因此通常用 PC 的低位做哈希，构造一个小表——分支历史表 BHT（Branch History Table）。

推荐阅读（动态分支预测入门）：
- [link1](https://danluu.com/branch-prediction/)
- [link2](https://people.engr.ncsu.edu/efg/521/f02/common/lectures/notes/lec16.pdf)
- [Onur Mutlu's ETH Zurich lectures](https://www.youtube.com/watch?v=hl4eiN8ZMJg)（感谢 Luke Wren）
- [ALF](https://team.inria.fr/alf/members/andre-seznec/branch-prediction-research/)

### 1-bit BHT

我们先做最简单的：每个表项只存 1 bit（上次结果）。例如 4096 项：

```verilog
   localparam BP_ADDR_BITS=12;
   localparam BHT_SIZE=1<<BHT_INDEX_BITS;
   reg BHT[BHT_SIZE-1:0];
```

为了从 PC 生成索引，定义函数（忽略低两位，因为指令按 32 位对齐）：

```verilog
   function [BP_ADDR_BITS-1:0] BHT_index;
      input [31:0] PC;
      BHT_index = PC[BP_ADDR_BITS+1:2];
   endfunction
```

于是 `D_predictBranch` 取自 BHT：

```verilog
   /* D */
   wire D_predictBranch = BHT[BHT_index(FD_PC)][1];
   ...
   always @(posedge clk) begin
      ...
      if(!D_stall) begin
         ...
	 DE_predictBranch <= D_predictBranch;
	 DE_BHTindex <= BHT_index(FD_PC);
      end
      ...
   end
```

`E` 阶段除了纠正 PC 外，还要用真实结果更新 BHT：

```verilog
  /* E */
  wire E_JumpOrBranch = (
         isJALR(DE_instr) || 
         (isBranch(DE_instr) && (E_takeBranch^DE_predictBranch))
  );
  ...  
  always @(posedge clk) begin
     ...
     if(isBranch(DE_instr)) begin
	BHT[DE_BHTindex] <= E_takeBranch;
     end
     ...
  end     
```

作者还在 VERILOG 里加了命中率统计，用于比较不同策略在不同程序上的效果。对 `RAYSTONES` 与 `DHRYSTONES` 的命中率可能差异很大：

| 预测策略                    | raystones | dhrystones |
|----------------------------|-----------|------------|
| 总是预测 taken              | 68.6%     | 89.2%      |
| BTFNT（后向 taken 前向 not）| 63%       | 93.4%      |
| 1 bit BHT                  | 74%       | 92.7%      |

可以看到：更“聪明”的策略不一定在所有程序都更好。

### 2-bit BHT（饱和计数器）

经典的改进是用 2 bit 饱和计数器（saturating counter），预测位取最高位：

```verilog
   reg [1:0] BHT[BHT_SIZE-1:0];
   ...
   wire D_predictBranch = BHT[BHT_index(FD_PC)][1]; 
```

更新规则：taken 就“向上加一”，not taken 就“向下减一”，但在 00/11 处饱和。用函数实现：

```verilog

   /* E */
   function [1:0] incdec_sat;
      input [1:0] prev;
      input dir;
      incdec_sat = 
 	   {dir, prev} == 3'b000 ? 2'b00 :
           {dir, prev} == 3'b001 ? 2'b00 :
	   {dir, prev} == 3'b010 ? 2'b01 :
	   {dir, prev} == 3'b011 ? 2'b10 :		
	   {dir, prev} == 3'b100 ? 2'b01 :
	   {dir, prev} == 3'b101 ? 2'b10 :
	   {dir, prev} == 3'b110 ? 2'b11 :
	                           2'b11 ;
   endfunction;

   ...
   always @(posedge clk) begin
      if(isBranch(DE_instr)) begin
	 BHT[DE_BHTindex] <= incdec_sat(BHT[DE_BHTindex], E_takeBranch);
      end
   end   
```

命中率提升（两个基准都更好）：

| 预测策略     | raystones | dhrystones |
|--------------|-----------|------------|
| 2 bits BHT   | 76.8%     | 95.97%     |

### gshare：引入全局分支历史

进一步提升可以用 “gshare”：维护一个短的全局分支历史 shift register（记录最近 N 次分支结果），与 PC 的低位 XOR 作为 BHT 索引：

```verilog
   localparam BP_HISTO_BITS=9;
   reg [BP_HISTO_BITS-1:0] branch_history;

   /* E */
   always @(posedge clk) begin
      ...
      if(isBranch(DE_instr)) begin
	 branch_history <= {E_takeBranch,branch_history[BP_HISTO_BITS-1:1]};
	 BHT[DE_BHTindex] <= incdec_sat(BHT[DE_BHTindex], E_takeBranch);
      end
   end
```

索引函数：

```verilog
   function [BHT_INDEX_BITS-1:0] BHT_index;
      input [31:0] PC;
   /* verilator lint_off WIDTH */
      BHT_index = PC[BP_ADDR_BITS+1:2] ^ 
                  (branch_history << (BP_ADDR_BITS - BP_HISTO_BITS));
   /* verilator lint_on WIDTH */      
   endfunction
```

对应版本 [pipeline8.v](pipeline8.v)。命中率示例：

| 预测策略     | raystones | dhrystones |
|--------------|-----------|------------|
| gshare       | 82%       | 96.3%      |

对 RAYSTONES 的提升往往更明显，因为它的分支模式更不规则，全局历史能捕捉更多相关性。

## 第 9 步：返回地址栈（RAS, Return Address Stack）

一个相对容易的性能提升来自优化 `JALR`（函数调用/返回常用）。思路：在处理器内部维护一个小的返回地址栈 RAS（通常深度 4）。当检测到函数调用时，把返回地址压栈；当检测到函数返回（`JALR` 形式）时，栈顶就是预测的返回目标地址，可以直接在 `D` 阶段把它送给 `F`，减少 bubble。

RAS 可以用 4 个 32 位寄存器实现：

```verilog
/* D */
   reg [31:0] RAS_0;
   reg [31:0] RAS_1;
   reg [31:0] RAS_2;
   reg [31:0] RAS_3;
```

同时把 `D_JumpOrBranchNow/D_JumpOrBranchAddress` 改名为更贴切的 `D_predictPC/D_PCprediction`：

```verilog
/* D */

   wire D_predictPC = !FD_nop && (
      D_isJAL || D_isJALR || (D_isBranch && D_predictBranch) 
   );
   wire [31:0] D_PCprediction = 
                D_isJALR ? RAS_0 : 
	        (FD_PC + (D_isJAL ? D_Jimm : D_Bimm));
```

把预测的返回地址传给 `E`，用于判断预测是否正确：

```verilog
/* D */
always @(posedge clk) begin
   ...
   DE_predictRA <= RAS_0;
end
```

`E` 阶段判断是否需要纠正 PC：

```verilog
/* E */
  wire [31:0] E_JALRaddr = {E_aluPlus[31:1],1'b0};
  wire E_correctPC = (
     (DE_isJALR    && (DE_predictRA != E_JALRaddr)   ) || 
     (DE_isBranch  && (E_takeBranch^DE_predictBranch))
  );
  wire [31:0] E_PCcorrection = DE_isBranch ? DE_PCplus4orBimm : E_JALRaddr;
```

并更新 RAS（检测 call/ret 形态）：

```verilog
/* E */
 always @(posedge clk) begin
    ...
    if(!FD_nop && !D_flush) begin
       if(D_isJAL && (D_rdId==1 || D_rdId==5)) begin
          RAS_3 <= RAS_2;
          RAS_2 <= RAS_1;
          RAS_1 <= RAS_0;
          RAS_0 <= FD_PC + 4;
        end 
        if(D_isJALR && D_rdId==0 && (D_rs1Id == 1 || D_rs1Id==5)) begin
           RAS_0 <= RAS_1;
           RAS_1 <= RAS_2;
           RAS_2 <= RAS_3;
        end
    end
    ...
  end    
```

我们认为 `JAL x1,addr` 或 `JAL x5,addr` 是函数调用，而 `JALR x0,x1,imm` 或 `JALR x0,x5,imm` 是函数返回。为什么 `x5`？因为规范允许把 `x1` 或 `x5` 用作 link register（见 RISC-V 规范表 2.1，第 17 页；感谢 Bruce Hoult）。

对应实现 [pipeline9.v](pipeline9.v)。该版本支持通过宏开关启用/禁用特性：

| flag                | description                                          |
|---------------------|------------------------------------------------------|
| `CONFIG_PC_PREDICT` | 启用 `D` -> `F` 的预测路径                           |
| `CONFIG_RAS`        | 返回地址栈（需要 `CONFIG_PC_PREDICT`）                |
| `CONFIG_GSHARE`     | gshare 分支预测（需要 `CONFIG_PC_PREDICT`）           |
| `CONFIG_INITIALIZE` | 若置位，则寄存器初始化为 0（某些综合器需要）          |

RAS 的效果示例：

| 预测策略     | raystones| CPI   | dhrystones(DMIPS/MHz) |  CPI  |
|--------------|----------|-------|-----------------------|-------|
| gshare       |  7.185   | 1.121 | 1.562                 | 1.116 |
| gshare+RAS   |  7.374   | 1.092 | 1.606                 | 1.086 |

## 用 VERILOG 写的调试器

![](verilog_riscv_debugger.png)

这一版本还内置了一个“用 VERILOG 写的调试器”（仅 Verilator 可用）。设置 `CONFIG_DEBUG` 即可启用。它允许你逐时钟运行核心并观察流水线各阶段，同时显示前递与控制信号。

调试器只有两个命令：
- 按 `<return>`：前进一步（一个时钟）
- 输入 `g`：运行到下一个断点

断点通过断言 `breakpoint` 信号设置，见 [pipeline9.v](pipeline9.v) 第 840 行附近。例如：

当执行到某个地址时断下：

```verilog
  wire breakpoint = (DE_PC   == 32'h000000); // break on address reached
```

当访问某个数据地址时断下：

```verilog
   wire breakpoint = (EM_addr == 32'h......);
```

对 LED 输出断下：

```verilog
   wire breakpoint = (EM_addr == 32'h400004);
```

对 UART 输出断下：

```verilog
   wire breakpoint = (EM_addr == 32'h400008);
```

下一步我们会实现 RV32M，并引入需要多周期的除法器，这会让流水线控制稍微更复杂，因此有这样一个调试器会很有帮助。

## 第 10 步：RV32M（硬件乘除）

通过流水线、前递、分支预测和 RAS，我们已经达到 7.374 raystones。现在看看能否通过支持更多指令进一步提速：加入整数乘法/除法/取模，这些属于 `RV32M` 扩展。一旦内核支持它们，我们只需要把软件重新按 RV32IM 编译，编译器就会直接生成 `MUL` 指令，而不必调用 `libgcc` 里的 `__mulsi3` 软件例程。

我们需要支持 8 条新指令：

| instruction       | description        | comment                  | funct3 |
|-------------------|--------------------|--------------------------|--------|
| MUL rd,rs1,rs2    | rd <- rs1 * rs2    | signed, 32 lsbs          | 000    |
| MULH rd,rs1,rs2   | rd <- rs1 * rs2    | signed, 32 msbs          | 001    |
| MULHSU rd,rs1,rs2 | rd <- rs1 * rs2    | signed*unsigned, 32 msbs | 010    |
| MULHU rd,rs1,rs2  | rd <- rs1 * rs2    | unsigned, 32 msbs        | 011    |
| DIV rd,rs1,rs2    | rd <- rs1 / rs2    | signed version           | 100    |
| DIVU rd,rs1,rs2   | rd <- rs1 / rs2    | unsigned                 | 101    |
| REM rd,rs1,rs2    | rd <- rs1 % rs2    | signed                   | 110    |
| REMU rd,rs1,rs2   | rd <- rs1 % rs2    | unsigned                 | 111    |

它们都使用 `opcode=7'b0110011`（也就是 ALUreg）。区别在于 `funct7`：RV32M 的 `funct7=0000001`。`funct3` 则区分 8 条指令（刚好 8 种取值）。注意 `funct3[2]` 区分“乘法类”和“除法/取模类”。

### 乘法：用 FPGA DSP（1 周期）

多数 FPGA 有 DSP 乘法器块，只要写 `A <= B * C;`（并给综合工具正确参数）就能映射成 1 周期乘法。否则用位串行会需要约 32 周期，收益不大。这里计算 33x33 位乘法：通过给操作数加一个“符号位”来统一处理 MULH/MULHSU/MULHU 等变体（Matthias Koch 的技巧）：

```verilog
   /* E */
   wire E_isMULH   = DE_funct3_is[1];
   wire E_isMULHSU = DE_funct3_is[2];
   
   wire E_mul_sign1 = E_rs1[31] &  E_isMULH;
   wire E_mul_sign2 = E_rs2[31] & (E_isMULH | E_isMULHSU);

   wire signed [32:0] E_mul_signed1 = {E_mul_sign1, E_rs1};
   wire signed [32:0] E_mul_signed2 = {E_mul_sign2, E_rs2};
   wire signed [63:0] E_multiply = E_mul_signed1 * E_mul_signed2;
```

### 除法/取模：多周期（约 33 周期）

除法/取模无法靠 DSP 一拍完成，需要做多周期逻辑。这里用经典算法（移位减法），并参考了 Claire Wolf 的 picorv 与 Matthias Koch 的一些想法：

```verilog

   /* E */
   reg [31:0] EE_dividend;
   reg [62:0] EE_divisor;
   reg [31:0] EE_quotient;
   reg [31:0] EE_quotient_msk;

   reg  EE_div_sign;
   reg 	EE_divBusy     = 1'b0;
   reg 	EE_divFinished = 1'b0;

   wire E_divstep_do = (EE_divisor <= {31'b0, EE_dividend});
   
   always @(posedge clk) begin
      if (!EE_divBusy) begin
	 if(DE_isDIV & !dataHazard & !EE_divFinished) begin
	    EE_quotient_msk <= 1 << 31;
	    EE_divBusy     <= 1'b1;	    
	 end
	 EE_dividend <=   ~DE_funct3[0] & E_rs1[31] ? -E_rs1 : E_rs1;
	 EE_divisor  <= {(~DE_funct3[0] & E_rs2[31] ? -E_rs2 : E_rs2), 31'b0};
	 EE_quotient <= 0;
	 EE_div_sign <= ~DE_funct3[0] & (DE_funct3[1] ? E_rs1[31] : 
                         (E_rs1[31] != E_rs2[31]) & |E_rs2)       ;
      end else begin
	 EE_dividend <= E_divstep_do ? EE_dividend-EE_divisor[31:0]:EE_dividend;
	 EE_divisor  <= EE_divisor >> 1;
	 EE_quotient <= E_divstep_do ? EE_quotient|EE_quotient_msk :EE_quotient;
	 EE_quotient_msk <= EE_quotient_msk >> 1;
	 EE_divBusy <= EE_divBusy & !EE_quotient_msk[0];
      end 
      EE_divFinished <= EE_quotient_msk[0];
   end 
```

注意：除法在 `E` 阶段内部保存了一些状态寄存器（前缀 `EE_`），以区别于纯组合的 `E_` 线网。

最后用 mux 组合 RV32M 的输出，并给出 `aluBusy`（表示除法正在运行）：

```verilator
   wire [2:0] E_divsel = {DE_isDIV,DE_funct3[1],EE_div_sign};
   
   wire [31:0] E_aluOut_muldiv =
     (  DE_funct3_is[0]    ? E_multiply[31: 0] : 32'b0) | // 0:MUL
     ( |DE_funct3_is[3:1]  ? E_multiply[63:32] : 32'b0) | // 1:MH, 2:MHSU, 3:MHU
     (  E_divsel == 3'b100 ?  EE_quotient      : 32'b0) | // DIV
     (  E_divsel == 3'b101 ? -EE_quotient      : 32'b0) | // DIV (negative)
     (  E_divsel == 3'b110 ?  EE_dividend      : 32'b0) | // REM
     (  E_divsel == 3'b111 ? -EE_dividend      : 32'b0) ; // REM (negative)
   
   wire [31:0] E_aluOut = DE_isRV32M ? E_aluOut_muldiv : E_aluOut_base;

   wire aluBusy = EE_divBusy | (DE_isDIV & !EE_divFinished);
```

当除法在跑时，该指令必须留在 `E` 阶段（`E` stall），也意味着 `F`/`D` 也要 stall，而 `M` 需要 flush。新增 pipeline 控制信号 `E_stall` 与 `M_flush`，并这样连接：

```verilator
   assign F_stall = aluBusy | dataHazard | halt;
   assign D_stall = aluBusy | dataHazard | halt;
   assign E_stall = aluBusy;
   
   assign D_flush = E_correctPC;
   assign E_flush = E_correctPC | dataHazard;
   assign M_flush = aluBusy;
```

新版本是 [pipeline10.v](pipeline10.v)。你需要把系统配置为 RV32IM（而不是 RV32I）：

```
  $ cd learn-fpga/FemtoRV
  $ make ICEBREAKER.firmware_config
  $ cd TUTORIALS/FROM_BLINKER_TO_RISCV/FIRMWARE
  $ make clean raystones.pipeline.hex
```

然后运行：

```
  $ cd ..
  $ ./run_verilator.sh pipeline10.v
```

该核心达到 18.215 RAYSTONES。即使光线追踪更适合用 FPU，但 1 周期硬件整数乘法仍能显著提升软件浮点/整数运算性能。

如果想观察除法器工作过程，打开 `CONFIG_DEBUG`，用 Verilator 运行。按 `g` 到下一个断点（默认设在 `DIV` 指令），再按回车逐周期推进，可以看到除法 mask 的变化。

## 第 10 步（续）：面向 fmax 的优化思路

前面我们主要优化 CPI，但 fmax（最高频率）以及 LUT/FF 也很重要。ULX3S 上的典型数据如下：

| Version        | Description            | Fmax   | LUTs  | FFs  |
|----------------|------------------------|--------|-------|------|
|pipeline2.v     | 3-4 states multicycle  | 50 MHz | 2100  | 532  |
|pipeline3.v     | "sequential pipeline"  | 80 MHz | 1877  | 1092 |
|pipeline4.v     | stall/flush            | 50 MHz | 2262  | 1148 |
|pipeline5.v     | stall/flush RF bypas   | 55 MHz | 2115  | 1040 | 
|pipeline5_bis.v | stall/flush comb. RF   | 55 MHz | 1903  | 1040 | 
|pipeline6.v     | stall/flush+reg fwding | 40 MHz | 2478  | 1040 |

观察：
- “顺序流水线” fmax 较高，体现了“阶段更简单”的价值；
- 加入 pipeline 控制逻辑后 fmax 会下降（pipeline4）；
- 组合寄存器堆（或旁路）能简化控制逻辑，fmax 回升（pipeline5）；
- 加入前递逻辑后关键路径变长，fmax 又下降（pipeline6）。

接下来一些常见优化方向：

### 在 `E` 读 DATARAM，在 `M` 做对齐/符号扩展

把访存相关的组合逻辑分散到多个阶段，减少关键路径。

### 把 `wbEn` 做成流水线寄存

前递的比较逻辑里包含 `writesRd(I)`（又会调用 `isStore/isBranch` 做 7bit opcode 检测）以及 `rdId` 比较等，表达式很“宽”。可以在更早阶段把“该指令是否会写回”计算好并流水线传递，从而减少后级的组合逻辑。

### 让 `D` 统一译码，并在后级去掉 instr/PC 的传播

目前我们把 `instr` 传到每一级并重复译码，是为了调试方便，但这会造成冗余逻辑。更优化的做法是在 `D` 译码出各种 `is_xxx` flag 并流水线传递；后级不再需要完整 instr（甚至 PC 也可部分省掉）。

### 在 `D` 做一些“更聪明的位模式利用”

RISC-V 的 10 类主要指令 opcode 编码不是随意的，很多地方可以只测少量 bit。例如 `JAL` 是唯一 bit3 为 1 的主类指令，这可以用来简化 PC 预测逻辑。

### 前递比较器流水线化

`E` 阶段入口的 3 路 mux 由 `rs1Id/rs2Id` 与 `rdId` 比较驱动。可以在 `D` 阶段提前算出比较结果并存入少量触发器（例如 `DE_rs1Id_eq_EM_rdId` 等），以缩短 `E` 阶段关键路径。

“最终产品” `TordBoyau` 在这个项目里：[https://github.com/BrunoLevy/TordBoyau](https://github.com/BrunoLevy/TordBoyau)。在 ARTY 上用 Vivado 可达 100-125 MHz，并可超频到 140 MHz：
- RV32I 配置：光追约 7.375 raystones（1.092 CPI，gshare+RAS 很有效）
- RV32IM 配置：约 18.215 raystones

## 尾声

希望你喜欢这个系列。还有很多主题值得继续学习，我计划（等我理解后）写一些后续教程：

- **优化**：例如 store->load 前递让 `memcpy()` 达到每 word 1 周期；以及对 RV32IM 的 `TordBoyau` 做 fmax 优化（目前 80 MHz 左右但可超频到 140 MHz，可能存在需要排除的 false path）。
- **cache**：当前流水线核用 `PROGROM` 与 `DATARAM`。下一步是接入 cache 接口与 SDRAM 控制器，在 LiteX 系统里实现（LiteX 有成熟 SDRAM 控制器、帧缓冲等）。这样就能在流水线核上运行 DOOM（第一篇的核之所以能跑 DOOM，是因为 LiteX 已经做了 cache 控制器接口）。
- **中断**：RISC-V 有特权 ISA（privileged ISA），包含控制中断与 trap 的寄存器和指令。官方文档较难读，我想写一份“跑 Linux-noMMU 所需的最小集合”的说明。
- **MMU**：MMU 也是一个很有意思的主题，@ultraembedded 说它加起来很简单。
- **乱序执行（OoO）**：本篇末尾借助 gshare+RAS 达到 1.092 CPI，已经非常接近“光速”（1 CPI）。但理论上可以做到“超光速”：多执行单元 + 乱序调度，一周期执行多条指令。现有内核例如 @dolu1990 的 NaxRiscV、@ultraembedded 的 BiRiscV。其微架构依然是流水线，但组织成树，并由中心调度器（Tomasulo）分发指令。如果能有一个框架（例如基于 LiteX）自动生成“中心调度器”，让用户拼出自己的流水线树，会很有趣。

## 参考资料

- [Intel, AMD and VIA microarchitecture](https://www.agner.org/optimize/microarchitecture.pdf)
- [course at upenn](https://acg.cis.upenn.edu/milom/cse372-Spring06/labs/lab4.html)
- [slides](https://acg.cis.upenn.edu/milom/cis501-Fall05/lectures/06_pipeline.pdf)

## 乱序执行参考（Charles Papon 推荐）

- [Henry thesis](https://www.stuffedcow.net/files/henry-thesis-phd.pdf)
- [Boom](https://docs.boom-core.org/en/latest/index.html)
- [Mashimo](https://www.rsg.ci.i.u-tokyo.ac.jp/members/shioya/pdfs/Mashimo-FPT'19.pdf)
