# 当前 SoC 设计报告

## 1. 报告范围

这份报告面向当前仓库里的 SoC RTL 实现，核心依据是这些源文件：

- [Soc/src/Soc.v](Soc/src/Soc.v)
- [Soc/src/processor.v](Soc/src/processor.v)
- [Soc/src/branch_predictor.v](Soc/src/branch_predictor.v)
- [Soc/src/hazard_unit.v](Soc/src/hazard_unit.v)
- [Soc/src/decoder.v](Soc/src/decoder.v)
- [Soc/src/imm_gen.v](Soc/src/imm_gen.v)
- [Soc/src/regfile.v](Soc/src/regfile.v)
- [Soc/src/alu.v](Soc/src/alu.v)
- [Soc/src/div_unit.v](Soc/src/div_unit.v)
- [Soc/src/lsu_align.v](Soc/src/lsu_align.v)
- [Soc/src/csr_unit.v](Soc/src/csr_unit.v)
- [Soc/src/icache.v](Soc/src/icache.v)

从功能上看，这是一颗单发射、顺序执行的 RV32I + RV32M SoC，外加最小 CSR 子集和少量 MMIO 外设。处理器采用清晰的五级流水：F、D、E、M、W。

当前默认 RTL 还保留了若干可配置路径：

- 默认启用 PC 预测、RAS、gshare 分支预测、RV32M 扩展。
- 顶层支持两种取指路径：
  - 直接从同步 PROGROM 取指。
  - 通过 ICache 走 AXI 只读回填。
- 当前 Zynq Vivado 综合脚本 [Soc/vivado/run_soc_zynq7020_synth.tcl](Soc/vivado/run_soc_zynq7020_synth.tcl) 定义了 BYPASS_ICACHE，因此当前 FPGA 实现结果对应的是“直接 PROGROM 取指”版本。

如果只想先记住一句话，可以这样概括当前设计：

> 这是一颗带分支预测、返回地址栈、旁路和 RV32M 的五级顺序流水 CPU，SoC 顶层把它接到程序存储、数据 RAM、UART、LED 和仿真 mailbox 上。

## 2. 顶层结构总览

从 SoC 顶层看，当前系统可以抽象成下面这张图：

```text
                +----------------------+
                |        Soc.v         |
                |                      |
CLK/RESET ----> |  +----------------+  |
                |  |   processor     |  |
                |  +----------------+  |
                |     | inst/data/io   |
                |     v                |
                |  +----------------+  |
                |  | PROGROM /      |  |
                |  | ICache + DDR   |  |
                |  +----------------+  |
                |  +----------------+  |
                |  |   DATARAM      |  |
                |  +----------------+  |
                |  +----------------+  |
                |  | UART / LED /   |  |
                |  | Mailbox MMIO   |  |
                |  +----------------+  |
                +----------------------+
```

顶层职责很集中：

1. 实例化处理器核心。
2. 给处理器接上取指存储系统。
3. 给处理器接上数据 RAM。
4. 把地址落在 IO 区间的访存转成外设寄存器访问。
5. 在仿真模式下，把 UART 输出和 benchmark mailbox 结果直接打印出来。

## 3. 模块划分

### 3.1 SoC 顶层模块

#### Soc

文件： [Soc/src/Soc.v](Soc/src/Soc.v)

职责：

- 连接 CPU、指令存储、数据存储、UART、LED、mailbox。
- 统一定义 MMIO 解码。
- 在 BENCH 模式下处理 CoreMark 等软件回写的 mailbox 结果。

#### progrom_ip_model

文件： [Soc/src/progrom_ip_model.v](Soc/src/progrom_ip_model.v)

职责：

- 在 BYPASS_ICACHE 路径下作为同步程序 ROM。
- 当前直接用 inst_addr[15:2] 作为字地址。

#### dataram_ip_model

文件： [Soc/src/dataram_ip_model.v](Soc/src/dataram_ip_model.v)

职责：

- 提供数据存储体。
- 支持按字节写掩码写入。

#### emitter_uart

文件： [Soc/src/emitter_uart.v](Soc/src/emitter_uart.v)

职责：

- 实现最小 UART 发送器。
- MMIO 对数据寄存器写入后立即触发串行发送。

#### icache

文件： [Soc/src/icache.v](Soc/src/icache.v)

职责：

- 在非 BYPASS_ICACHE 配置下充当取指缓存。
- 对 CPU 提供简化 ready/valid 接口。
- 对外发起 AXI4 只读 burst 回填。

### 3.2 处理器核心内部模块

#### processor

文件： [Soc/src/processor.v](Soc/src/processor.v)

职责：

- 实现 F/D/E/M/W 五级主流水。
- 实现前端重定向、译码、旁路、分支决策、访存、写回。
- 集成 branch_predictor、hazard_unit、ALU、乘法器、除法器、LSU 对齐模块和 CSR 模块。

#### decoder

文件： [Soc/src/decoder.v](Soc/src/decoder.v)

职责：

- 把 32 位指令拆成 rd、rs1、rs2、funct3、funct7 等字段。
- 生成各类语义控制信号，例如 ALU、Load、Store、Branch、JALR、CSR、RV32M 等。

#### imm_gen

文件： [Soc/src/imm_gen.v](Soc/src/imm_gen.v)

职责：

- 统一生成 I、S、B、U、J 五种立即数。

#### regfile

文件： [Soc/src/regfile.v](Soc/src/regfile.v)

职责：

- 提供两读一写寄存器堆。
- 保证 x0 恒为 0。

#### branch_predictor

文件： [Soc/src/branch_predictor.v](Soc/src/branch_predictor.v)

职责：

- 在 D 级对 Branch、JAL、JALR 做前端预测。
- 维护全局历史、BHT 饱和计数器和 RAS。
- 在 E 级根据真实执行结果更新预测器状态。

#### hazard_unit

文件： [Soc/src/hazard_unit.v](Soc/src/hazard_unit.v)

职责：

- 统一产生 stall 和 flush。
- 处理 load-use、CSR 读后相关、除法/乘法忙、跳转修正和 ICache miss。

#### alu

文件： [Soc/src/alu.v](Soc/src/alu.v)

职责：

- 负责基础整数算术、比较、逻辑、移位。
- 在启用 RV32M 时负责最终选择乘法或除法结果。

#### mul_unit

文件： [Soc/src/processor.v](Soc/src/processor.v)

职责：

- 实现乘法类指令。
- 采用两段内部流水：先锁存操作数并计算乘积，再输出低 32 位或高 32 位结果。

#### div_unit

文件： [Soc/src/div_unit.v](Soc/src/div_unit.v)

职责：

- 实现 DIV、DIVU、REM、REMU。
- 采用逐位试商的串行算法。

#### lsu_align

文件： [Soc/src/lsu_align.v](Soc/src/lsu_align.v)

职责：

- 负责字节、半字、整字的存取对齐。
- 负责 store 写掩码生成和 load 符号扩展。

#### csr_unit

文件： [Soc/src/csr_unit.v](Soc/src/csr_unit.v)

职责：

- 当前只实现 cycle 和 instret 两组 64 位只读 CSR。

## 4. 地址空间与 MMIO 视图

### 4.1 指令空间

- 在 BYPASS_ICACHE 路径下，程序 ROM 直接使用 inst_addr[15:2] 作为字地址。
- 这意味着当前直接 ROM 取指视图覆盖 64KB 指令空间。

### 4.2 数据空间

- DATARAM 的局部地址由 data_raddr[16:2] 和 data_waddr[16:2] 减去 0x4000 字得到。
- 换成字节地址，就是把 DATARAM 映射到 0x0001_0000 附近。
- 当前 14 位字地址对应 64KB 数据 RAM。

### 4.3 MMIO 空间

当前处理器用地址 bit22 区分 RAM 和 IO：

- bit22 = 0：访问普通 RAM。
- bit22 = 1：访问 IO 空间。

软件里定义的 IO_BASE 是 0x0040_0000，对应文件 [FIRMWARE/io.h](FIRMWARE/io.h)。

一个容易忽略的细节是：当前 MMIO 解码不是“地址等于某常数”，而是“看低地址的某一位是否为 1”。因此各寄存器偏移是 2 的幂：

| 资源 | 访问地址 | 说明 |
| --- | --- | --- |
| LEDS | 0x0040_0004 | 写低 5 位更新板载 LED |
| UART_DAT | 0x0040_0008 | 写入一个字节后开始发送 |
| UART_CNTL | 0x0040_0010 | 读取 UART busy 状态 |
| MBOX0 | 0x0040_0020 | benchmark 数值 0 |
| MBOX1 | 0x0040_0040 | benchmark 数值 1 |
| MBOX2 | 0x0040_0080 | benchmark 数值 2 |
| MBOXD | 0x0040_0100 | 写入后打印 mailbox 并结束仿真 |

其中 mailbox 偏移可以在 [FIRMWARE/soc_smoke.S](FIRMWARE/soc_smoke.S) 里看到对应定义。

### 4.4 MMIO 与普通存储访问的差别

普通 RAM 访问：

- 写路径有字节掩码。
- 读路径经过 lsu_align 做对齐和符号扩展。

MMIO 访问：

- 当前按 32 位寄存器风格处理。
- 写 IO 时直接把 EM_rs2 送到 IO_mem_wdata，没有字节掩码概念。
- 也就是说，外设接口是“字寄存器语义”，不是“可细粒度 byte-enable 的总线语义”。

## 5. 处理器主流程

### 5.1 总体流水结构

当前核心是标准五级流水：

- F：取指与前端重定向。
- D：译码与预测。
- E：旁路、ALU、分支真假判断、JALR 目标计算。
- M：真正执行访存，并完成数据对齐。
- W：统一写回寄存器堆。

对应的级间寄存器主要有：

- F/D：FD_PC、FD_instr_reg、FD_nop。
- D/E：DE_xxx。
- E/M：EM_xxx。
- M/W：MW_xxx。

除此之外，前端还额外有两组不是传统流水寄存器的结构：

- F_pending：记录已经发出去但尚未消化的取指请求。
- FB：一项取指回包缓冲，用来处理“取指回来了，但 D 级当前不能接收”的情况。

这两组结构的存在，使当前前端即使面对同步取指存储或带 ready/valid 的 ICache，也能保持顺序一致性。

### 5.2 F 级：取指、等待返回、丢弃旧路径

F 级的关键不是“把 PC+4”，而是“谁来决定下一次真的去取哪个地址”。

当前优先级是：

1. 如果 E 级发现 JALR 预测错误，就立刻转去真实 JALR 目标。
2. 否则如果 E 级发现条件分支预测方向错误，就转去纠正地址。
3. 否则如果 D 级已经给出预测目标，就按预测目标继续前冲。
4. 否则按顺序 PC 前进。

也就是下面这条逻辑：

- E 级纠正优先级最高。
- D 级预测次之。
- 顺序 PC 最后兜底。

### 5.3 epoch 机制：为什么旧取指不会污染新路径

只要存在预测和纠正，就一定会遇到一个问题：

> 旧路径上发出去的取指请求，可能在“已经改道”之后才回来。

当前设计用 epoch 来解决：

- 每次发生前端重定向，epoch 加 1。
- F_pending 记录这次请求属于哪个 epoch。
- 取指回包到来时，如果发现它的 epoch 已经过期，就直接丢掉。

这样做的好处是：

- 不需要依赖外部存储“撤销请求”。
- 即使 ICache 或 ROM 在错误路径上返回了数据，也不会再进入 FD。

### 5.4 FB 缓冲：为什么 D 级 stall 时不会丢指令

还有一个前端小机制很重要：

- 如果取指刚好返回，但 D 级这拍正在 stall，无法接收新指令。
- 当前实现会把返回值暂存进 FB_PC、FB_instr、FB_epoch、FB_valid。
- 等 D 级恢复后再把它送入 FD。

因此当前前端不是“要么接住，要么丢掉”，而是允许保留一个回包缓冲。

### 5.5 D 级：译码、预测、准备 E 级所需信息

D 级主要做三件事：

1. 通过 decoder 产生指令类别和控制信号。
2. 通过 imm_gen 生成各种立即数。
3. 通过 branch_predictor 给出条件分支、JAL、JALR 的前端预测。

译码模块会生成下面这些高价值控制信息：

- 是否是 ALU reg / ALU imm。
- 是否是 Branch / JAL / JALR。
- 是否是 Load / Store。
- 是否是 CSRRS / EBREAK。
- 是否使用 rs1、rs2。
- 是否是 RV32M，并进一步区分乘法类和除法类。

这些信号在 D 级生成后，被锁存进 DE_xxx，供 E 级使用。

### 5.6 寄存器堆读取是怎样接入 E 级的

当前寄存器堆是两读一写：

- 写回来自 W 级。
- 读地址来自已经锁存到 D/E 边界的 rs1、rs2 编号。

因此更准确地说：

- D 级决定“下一条进入 E 级的指令要读哪两个寄存器”。
- E 级组合地拿到寄存器值，再叠加旁路结果形成最终操作数。

### 5.7 E 级：真正的执行中心

E 级是当前处理器最核心的一拍，几乎所有关键控制都在这里收口：

- 旁路选择。
- ALU 运算。
- 分支真假判断。
- JALR 真实目标计算。
- RV32M 单元启动与 busy 控制。

#### 5.7.1 旁路

当前旁路策略是：

- 优先从 M 级前递。
- 再从 W 级前递。
- 否则使用寄存器堆读值。

也就是说，M 级优先级高于 W 级。这样刚产生的新结果能尽快被下一条指令消费。

#### 5.7.2 ALU 输入

当前 ALU 输入规则比较直接：

- in1 默认来自 rs1。
- in2 对于寄存器型 ALU 指令和 Branch 取 rs2。
- 其他情况取 I/S 型立即数。

JALR 的目标地址也复用了加法器结果，只是在最低位再强制清零。

#### 5.7.3 JAL、JALR、LUI、AUIPC 的结果复用

当前实现没有给这些指令各造一套独立写回路径，而是把它们合并成一类：

- JAL / JALR：写回 PC+4。
- LUI / AUIPC：写回 U 型结果。

这部分通过 DE_PCplus4orUimm 预先准备，最后在 E_result 统一选择。

### 5.8 M 级：访存和地址分类

M 级主要做三件事：

1. 判定本次地址是 RAM 还是 IO。
2. 用 lsu_align 生成写数据、写掩码和 load 回读结果。
3. 形成最终写回候选值。

一个很重要的实现细节是：

- data_raddr 直接等于 E_addr。

这样做是为了适配同步读 RAM：

- E 级先算出地址。
- 下一拍 M 级正好拿到返回字数据。

这也是为什么 load-use hazard 不能靠普通旁路解决，而必须停一拍。

### 5.9 W 级：统一写回

W 级非常简单，职责就是把以下三类结果统一写回：

- Load 返回值。
- CSR 读返回值。
- 普通 ALU 或 JAL/JALR/LUI/AUIPC 结果。

处理器只在 rd 不等于 x0 时才真正打开写回。

### 5.10 停机条件

当前 halt 信号来自 E 级的 EBREAK 检测：

- halt = resetn && DE_isEBREAK

也就是说，软件执行到 ebreak 时，处理器会对外报告 HALT。

在 BENCH 仿真里，如果没有定义 EXTERNAL_BENCH_FINISH，halt 还会直接触发仿真结束。

## 6. 分支预测、跳转预测与修正机制

这一部分是当前 SoC 最值得单独展开的内容。

### 6.1 预测器总体策略

当前 branch_predictor 同时处理三类控制流：

- 条件分支 Branch。
- JAL。
- JALR。

其中：

- JAL 是无条件跳转，D 级总是可以直接给出预测目标。
- 条件分支依赖 BHT 或 BTFNT 决定是否预测 taken。
- JALR 在启用 RAS 时走返回地址栈预测路径。

### 6.2 BHT 与全局历史

预测器内部有两类状态：

- branch_history：全局历史寄存器。
- BHT：2 位饱和计数器表。

当前 processor 里默认参数是：

- BHT_ADDR_BITS = 12
- BHT_HISTO_BITS = 9

因此默认情况下：

- BHT 共有 4096 项。
- 全局历史宽度为 9 bit。

### 6.3 gshare 的索引方式

在启用 gshare 时，BHT 索引不是单纯的 PC，而是：

- PC 的索引位
- 与 branch_history 的哈希值异或

这样做的目的，是让“同一条分支在不同历史上下文下”尽量落到不同表项，减少简单 PC 索引带来的混叠。

如果禁用 gshare，当前实现会退化成 BTFNT：

- 向后跳默认 taken。
- 向前跳默认 not taken。

### 6.4 条件分支预测何时生效

在 D 级：

- 预测器先根据 BHT 或 BTFNT 得到 d_predict_branch。
- 如果当前指令是条件分支并且预测为 taken，就给出 d_pc_prediction。
- 如果预测为 not taken，则前端继续走顺序 PC。

真实真假在 E 级由比较器决定：

- EQ、LT、LTU 都由 ALU 相关逻辑给出。
- 根据 funct3 选择 BEQ、BNE、BLT、BGE、BLTU、BGEU 的判断条件。

### 6.5 条件分支错误时如何纠正

当前 E 级只关心一件事：

> 实际真假是否和 D 级预测方向一致。

如果不一致，就产生 E_branchRedirect。

纠正目标没有重新临时计算，而是提前在 D 级准备好：

- 如果当时预测 taken，那么纠正地址应该是 PC+4。
- 如果当时预测 not taken，那么纠正地址应该是 branch target。

这就是 DE_PCplus4orBimm 的作用。

### 6.6 JAL 的处理

JAL 最简单：

- D 级总是认为它要跳。
- 预测目标就是 PC + J immediate。
- 写回值是 PC + 4。

如果目标寄存器是 x1，预测器还会把返回地址压入 RAS。

### 6.7 JALR 与 RAS

JALR 是当前设计里最有意思的部分之一。

启用 RAS 后，预测器维护两套 4 项返回地址栈：

- ras_spec_x：投机栈。
- ras_arch_x：架构栈。

这样设计的原因是：

- D 级需要立刻做投机预测，所以必须有一套“可以先动”的栈。
- 一旦 E 级发现走错路，又需要快速回滚到确定正确的状态，所以必须保留一套“提交态”的栈。

### 6.8 call / return 识别规则

当前实现把下面两类模式当作 call / return：

- call：jal x1, target
- return：jalr x0, x1, 0 或 jalr x0, x5, 0

对应行为是：

- D 级遇到 call，就把 fd_pc + 4 压入投机 RAS。
- D 级遇到 return，就把投机 RAS 弹出一层。
- E 级在指令真正向后推进时，对架构 RAS 做同样的 push / pop。

### 6.9 为什么需要两套 RAS

如果只有一套栈，那么分支预测一旦走错，前面若干拍对 call / return 的投机 push / pop 就已经把真实状态破坏了。

当前实现的回滚方法是：

- 只要 e_correct_pc 拉高。
- 投机 RAS 立即整体回滚到架构 RAS。

这样前端就能在下一拍重新从正确路径继续预测。

### 6.10 JALR 的现实含义

当前预测器在启用 RAS 时，会让 JALR 走 RAS 目标预测路径。

这意味着：

- 对真正的函数返回，往往能提前命中。
- 对普通间接跳转或非标准形式的 JALR，预测不一定准，最后由 E 级通过比较预测返回地址和真实 JALR 目标来修正。

E 级的 JALR 修正条件是：

- 预测 RA 不等于真实 E_JALRaddr。

一旦不相等，就触发 E_jalrRedirect。

### 6.11 预测器更新时机

当前 BHT 和 branch_history 不是在 D 级预测时更新，而是在 E 级确认分支真实结果后更新。

这是很重要的设计选择：

- 学习的是“真实已执行结果”。
- 不是沿着投机路径先改状态，后面再补救。

这样预测器更稳，调试也更清晰。

## 7. 数据冒险、控制冒险与停顿策略

### 7.1 哪些相关会直接靠旁路解决

对于普通 ALU 结果，当前策略是：

- 尽量旁路。
- 只有“本拍根本还拿不到数据”的情况才 stall。

因此普通算术依赖通常不会停。

### 7.2 哪些相关必须停一拍

hazard_unit 当前只把这些情况当作真正的数据冒险：

- E 级上一条是 Load。
- 或 E 级上一条是 CSRRS。
- 且 D 级当前指令要读取同一个 rs1 或 rs2。

原因很直接：

- Load/CSR 读结果要到后面一拍才真正可用。
- 单靠 M/W 旁路已经赶不上紧邻下一条指令的 E 级取数时间。

于是当前策略是：

- d_stall = 1
- e_flush = 1

也就是冻结前面，并向 E 级注入一个气泡。

### 7.3 ICache miss 为什么只冻结 F 级

hazard_unit 里对 ICache miss 的处理很克制：

- f_stall 会被 icache_miss 拉高。
- d_stall 不受 icache_miss 影响。

含义是：

- F 级拿不到新指令时，先别继续发请求。
- D 级已经在手里的老指令仍然可以继续向后推进。

这种处理对吞吐更友好，也更符合前端 miss 的实际语义。

### 7.4 多周期乘除法如何冻结流水

当乘法器或除法器忙时：

- aluBusy 拉高。
- f_stall、d_stall、e_stall 一起拉高。
- m_flush 也拉高。

这说明当前策略不是“让后端带着不稳定状态继续流”，而是把 E 级牢牢钉住，直到乘除结果准备好。

## 8. RV32M 实现细节

### 8.1 乘法器

当前 mul_unit 是单独模块，但定义在 [Soc/src/processor.v](Soc/src/processor.v) 末尾。

实现方式可以概括为两段：

1. 启动时锁存 funct3、rs1、rs2。
2. 下一阶段计算 64 位乘积。
3. 最后一阶段根据 MUL、MULH、MULHSU、MULHU 选择低半或高半结果。

所以它不是一拍组合直出，而是一个短多周期单元。

### 8.2 除法器

div_unit 使用的是逐位试商法：

- 启动时把除数左移到最高对齐位。
- 每拍判断当前位能不能减。
- 同时右移除数和商位掩码。
- 最后根据 DIV/DIVU/REM/REMU 恢复商或余数的符号。

从行为上看，它更像一个小状态机，而不是组合除法器。

### 8.3 为什么 hazard_unit 必须认识 aluBusy

因为对于多周期单元来说：

- E 级那条指令这拍并没有“真的执行完”。
- 如果不冻结前端，后续指令会错误地踩进来。

所以 aluBusy 不只是性能信号，也是正确性信号。

## 9. ICache 路径

虽然当前 Zynq 综合脚本绕过了 ICache，但 RTL 本身已经有一颗完整的取指缓存，结构值得单独说明。

### 9.1 组织方式

当前 ICache 规格：

- 总容量 8KB。
- Cache line 大小 32B。
- 2 路组相联。
- 地址划分：Tag[31:12]、Index[11:5]、Offset[4:0]。

### 9.2 命中流程

在 IDLE 态：

- 如果当前请求命中某一路。
- 直接从对应 256 bit cache line 里切出 32 bit 指令字。
- 同时更新 LRU。

### 9.3 miss 流程

miss 时状态机会经过三步：

1. AR_REQ：发起 AXI 读地址请求。
2. R_WAIT：接收 8 个 32 位 beat，拼成整条 cache line。
3. REFILL_END：把 line 写回选中的 way，并把请求字立即回给 CPU。

这条 miss 路径说明当前 ICache 设计更偏“功能正确和结构清晰”，而不是极限低延迟复杂前端。

### 9.4 替换策略

每个 set 有一位 lru_array：

- 命中哪一路，就把另一路标成下次优先替换对象。
- miss 时优先替换 invalid way，否则按 lru_array 选路。

### 9.5 当前验证结论如何理解

因为当前 Zynq Vivado 综合脚本定义了 BYPASS_ICACHE：

- 当前 100 MHz 时序结果不代表“带 ICache 的前端也已同样闭合”。
- 它代表的是当前 CPU + PROGROM 直接取指路径已经获得实现层证据。

这一点在读 QoR 结论时必须分清。

## 10. 调试、统计与 BENCH 机制

当前 RTL 里埋了大量只在仿真有效的观测逻辑，这对理解系统非常有帮助。

### 10.1 调试输出

在 BENCH + CONFIG_DEBUG 下：

- 可以打印 F/D/E/M/W 五级反汇编。
- 可以看到 rs1、rs2 是来自寄存器堆还是 M/W 旁路。
- 可以看到 branch predict hit/miss。
- 可以看到 JALR 预测地址与真实地址比较。

### 10.2 事件日志

在 CONFIG_EVTLOG 下：

- 分支修正会打印 EVT_BRANCH_CORRECT。
- 流水冲刷会打印 EVT_PIPE_FLUSH。

### 10.3 运行结束后的统计

处理器在 BENCH 模式结束时还能打印：

- Branch 总命中率。
- 前向分支与后向分支命中率。
- JALR 命中率。
- Load hazard 比例。
- CPI。
- 指令构成占比。
- 热分支和低命中热分支。
- gshare 表项共享程度。

### 10.4 ICache 统计

icache 在 BENCH 结束时会打印命中次数和 miss 次数。

### 10.5 mailbox 的真实语义

BENCH 模式下，MBOX0、MBOX1、MBOX2 只是缓存 benchmark 数值。

真正触发打印和结束的是：

- 对 MBOXD 地址写入。

一旦写入：

- SoC 顶层会打印 COREMARK_TICKS、COREMARK_MHZ_X1000、COREMARK_CPI_X1000。
- 然后直接结束仿真。

这也是为什么某些 benchmark 的尾部 UART 文本可能还没完全刷完，仿真就已经结束了。

### 10.6 FAST_UART_STATUS 的影响

在当前 Vivado / BENCH 常用配置里经常会定义 FAST_UART_STATUS。

它会让 UART 状态寄存器读出来恒为“可写”。

优点：

- 软件不会因为轮询 UART busy 而拖慢仿真或实现路径。

代价：

- 末尾若软件刚写完 mailbox 就 finish，最后几字符的 UART 打印可能来不及完整出现在日志里。

## 11. 当前验证状态

这一节专门回答“当前 SoC 到底被验证到什么程度了”。

### 11.1 功能验证

#### riscv-tests

当前回归日志 [ROUTE_A/riscv_tests_soc_20260410_postfix.log](ROUTE_A/riscv_tests_soc_20260410_postfix.log) 给出的汇总是：

- PASS = 44
- FAIL = 0

这说明当前 RTL 在该批 ISA 测试覆盖下是通过的。

#### CoreMark

显式指定 CoreMark 固件运行的当前日志是 [coremark_soc_current.log](coremark_soc_current.log)，结果为：

- Ticks = 29983640
- CoreMark/MHz = 1.000545
- CPI = 1.351
- SIM_DONE finish=1 cycles=30020008

这说明当前 SoC 至少在当前显式固件路径下可以正常完成 CoreMark。

需要额外强调一点：

- riscv-tests 脚本会复用 SoC 公共固件文件名。
- 因此如果 benchmark 运行时没有显式指定固件路径，可能会把前一次测试残留镜像误当成当前程序。

也就是说，CoreMark 是否正常，应优先看显式固件运行日志，而不是看某次混用镜像后的 mailbox 数字。

### 11.2 实现验证

当前最佳实现证据来自这个变体目录：

- [Soc/vivado/reports_zynq7020_clg400_2_mulpipe_fanout32_redirectsplit_clean_postphys/post_route_timing_summary.rpt](Soc/vivado/reports_zynq7020_clg400_2_mulpipe_fanout32_redirectsplit_clean_postphys/post_route_timing_summary.rpt)

结论是：

- WNS = 0.007 ns
- TNS = 0.000 ns
- 100 MHz 约束满足

利用率来自：

- [Soc/vivado/reports_zynq7020_clg400_2_mulpipe_fanout32_redirectsplit_clean_postphys/post_route_utilization.rpt](Soc/vivado/reports_zynq7020_clg400_2_mulpipe_fanout32_redirectsplit_clean_postphys/post_route_utilization.rpt)

关键数字：

- Slice LUTs = 2759
- Slice Registers = 2264
- Block RAM Tile = 31
- DSP = 4

### 11.3 当前实现证据的边界

这里必须把“功能正确”和“实现闭合”分开看：

- riscv-tests 和 CoreMark 证明的是当前 RTL 在仿真里的功能和性能。
- Vivado WNS/TNS 证明的是当前综合实现版本在目标时钟下具备时序可行性。
- 这两者都重要，但谁也不能替代谁。

### 11.4 板级落地仍然缺什么

当前 DRC 报告在：

- [Soc/vivado/reports_zynq7020_clg400_2_mulpipe_fanout32_redirectsplit_clean_postphys/post_route_drc.rpt](Soc/vivado/reports_zynq7020_clg400_2_mulpipe_fanout32_redirectsplit_clean_postphys/post_route_drc.rpt)

可见：

- Violations found = 15
- NSTD-1 = 1
- UCIO-1 = 1
- PLBUFGOPT-1 = 2

其中：

- NSTD-1 表示顶层端口还没有指定 IOSTANDARD。
- UCIO-1 表示顶层端口还没有指定实际管脚 LOC。
- 这两项是板级约束问题，不是 CPU 内部 datapath 时序错误。
- 但它们会阻止严肃的 bitstream 生成和真实上板交付。

因此当前结论应当写成：

> 当前 SoC 已经具备仿真功能证据，也拿到了 100 MHz 的实现证据；但真正上板前仍需要补齐目标板 XDC 约束，并重新走 bitstream 流程确认。

## 12. 关键设计特点总结

如果把当前 SoC 的设计重点浓缩成几个最重要的判断，可以总结为：

1. 它不是一个“只有五级流水骨架”的教学 CPU，而是已经加入了比较完整的前端预测、返回地址栈、旁路和 RV32M 处理。
2. 它的前端实现比表面看起来更成熟，因为用了 epoch 和单项 fetch buffer 去处理预测改道和返回时序。
3. 它把 load-use / CSR-use 与普通 ALU 依赖严格区分，说明旁路和 hazard 策略已经经过明确分工。
4. 它的分支预测不是停留在静态 not-taken，而是已经具备 gshare + RAS 的结构化实现。
5. 它当前的 FPGA QoR 证据对应的是绕过 ICache 的取指版本，这一点在后续继续演进前端时必须始终记住。

## 13. 建议的阅读顺序

如果你打算继续深入 RTL，建议按下面顺序读：

1. 先读 [Soc/src/Soc.v](Soc/src/Soc.v)，把地址空间和外设边界搞清楚。
2. 再读 [Soc/src/processor.v](Soc/src/processor.v)，重点抓 F 级、E 级和流水寄存器更新。
3. 然后读 [Soc/src/branch_predictor.v](Soc/src/branch_predictor.v)，理解 gshare 和 RAS 的协作方式。
4. 最后读 [Soc/src/hazard_unit.v](Soc/src/hazard_unit.v)、[Soc/src/lsu_align.v](Soc/src/lsu_align.v)、[Soc/src/div_unit.v](Soc/src/div_unit.v)，把停顿、访存和多周期执行补完整。

这样读，最容易从“我知道它能跑”走到“我知道它为什么这样跑”。