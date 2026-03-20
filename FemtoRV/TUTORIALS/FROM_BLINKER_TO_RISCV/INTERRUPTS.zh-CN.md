# 从闪烁灯到 RISC-V·第三篇：中断（Interrupts）

本文目前仍是 WIP（Work In Progress）：暂时主要是笔记/草稿。

## 目标（Goals）

- 做一个循序渐进、一步步温和引入的教程：把第一篇末尾得到的处理器逐步“变形”为能运行 FreeRTOS 的版本（由 @jimmylu890303 提议）。
- 也许进一步覆盖更多 RISC-V 特权 ISA（privileged ISA）的内容，并尝试运行 Linux-noMMU（前提是不需要引入太多额外材料）。

我认为 @Mecrisp 的 `individua`（再加上“内存映射寄存器 + 中断源”）已经包含了我们需要的全部要素。

- 首先当然是把它跑起来。如何添加一个内存映射寄存器在
  [这里](https://github.com/BrunoLevy/learn-fpga/tree/master/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV#step-17-memory-mapped-device---lets-do-much-more-than-a-blinky-) 有说明。接下来需要把中断源也接进去。

- 然后需要写一份清晰的解释，说明 RISC-V 特权指令集是怎么工作的。这需要不少写作，因为我觉得官方规范
  [这里](https://riscv.org/wp-content/uploads/2017/05/riscv-privileged-v1.10.pdf)
  非常难读，原因包括：

  - 它列出了所有可能的 CSR，但我们实际上只需要解释其中很少几个；
  - 需要澄清“处理器内部 CSR（in-processor CSR）”与“内存映射 CSR（memory-mapped CSR）”的关系（我自己也觉得这里不够清晰）；
  - 需要解释：中断触发时到底发生了什么，以及从中断返回时发生了什么。

- 可能还需要解释 RISC-V 中断控制器规范（PLIC）：
  [PLIC](https://9p.io/sources/contrib/geoff/riscv/riscv-plic.pdf)。
  我不太确定 CLINT、PLIC 等概念分别是什么，需要进一步阅读。

为了保持第一篇的“逐步、小步改动”风格（step-by-step incremental modification），教程的情景（初稿）可以是：

- 从第一篇末尾得到的 `quark` 开始；

- 加一根 `interrupt_request` 线，并实现 `mstatus`、`mtvec` CSR；把 `interrupt_request` 接到一个物理按钮。写一个有趣的示例程序，例如：用 ASCII 动画显示一个弹跳的小球，主循环一直运行；中断发生时给小球加一个随机的“力”。如果有两个按钮，可以做个简化版 “pong” 或 “breakout”；

- 加计时器中断源。写一个最小化的多任务示例，演示上下文切换（@Mecrisp 已经做过）。例如：屏幕上有两三个弹跳球，每个球一个线程；

- 再写一个同时使用“计时器中断 + 按钮”的例子：多线程 pong（一个线程负责球，一个线程负责挡板，一个线程负责游戏逻辑）；

- 运行 FreeRTOS（可能需要几个中间步骤，尤其是仿真 / verilator 等相关内容）。

## 问题清单（List of questions）

- 运行 FreeRTOS 所需的最小 CSR 与指令集合是什么？初步猜测：
  - `mepc`：保存 PC
  - `mtvec`：中断/异常处理入口
  - `mstatus`：其中 `MIE` 位（bit3，中断全局使能）。还需要其它位吗？
  - `mcause`：其中 interrupt 位（bit31）。还需要其它位吗？（例如区分 timer 与 UART 中断）
  - `mtime`、`mtimecmp`
- FreeRTOS 里的 system call 是不是用 trap？我们是否需要区分“硬件中断”与 “system call”（也就是 `mcause` 里的额外位）？
- FreeRTOS 是否需要不同的特权级别？
- @Mecrisp 的实现里有 `interrupt_request_sticky` 触发器（当 `interrupt_request` 拉高就置 1，并在从异常处理返回后清 0）。它与规范里的 `MIP`（machine interrupt pending）位是什么关系？
- 规范提到了 “memory-mapped CSRs”。它们是否意味着：同一个 CSR 既在处理器内部实现、又投影到内存空间（或者两者任意组合）？
  我认为：只要 `CSRRx` 指令语义能工作，就符合规范——无论是通过处理器内部寄存器实现，还是通过 trap handler 软件模拟实现。
  因此一种可能的“最小内核”是：硬件只实现 `mepc`、`mtvec`、`mstatus`、`mcause`，而当访问其它 CSR 时触发异常，由 trap handler 处理。
  `mtime` 与 `mtimecmp` 可以：
  - 在核外实现（例如另一个硬件模块，负责触发 `interrupt_request`），或
  - 直接在核内实现。
  我不确定核外实现的收益是什么：它是否能显著简化 CPU？因为最后 “CPU + 中断控制逻辑” 的总成本可能差不多。
  但也许核外实现能简化 CSR 地址译码：例如把 memory-mapped CSR 页用 1-hot 编码，CSR 地址翻译由 trap handler 完成。
- 这又引出 PLIC/CLINT/CLIC 是什么的问题。我猜它们是在多核系统下、使用 memory-mapped CSRs 的中断体系结构规范，用于不同核互访/共享中断资源等，但对我来说仍然**非常不清晰**。
  哪个与我们相关？我们是否必须实现其中之一，还是可以做更简单的东西？
  我倾向于认为：PLIC 与 (A)CLINT 更偏多核系统；如果真要选一个可能相关的，也许是 CLIC。但也可能我们都不需要：如果把 `mtime/mtimecmp` 做在核内，且 FreeRTOS 不要求它们是 memory-mapped，就可以避开这套复杂体系。
  - [PLIC](https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc)
  - [(A)CLINT](https://github.com/riscv/riscv-aclint/blob/main/riscv-aclint.adoc)
  - [CLIC](https://github.com/riscv/riscv-fast-interrupt/blob/master/clic.adoc)

## Interrupts / Exception / Traps

### 定义（Definitions）

- Exception：与某条指令相关的运行时异常条件（unusual condition of run-time associated with an instruction）
- Trap：由异常条件导致的、同步转移到 trap handler（synchronous transfer to a trap handler caused by exceptional condition）
- Interrupt：异步发生的外部事件（external event that occurs asynchronously）

（如果我理解没错：trap 指的是通过 `xret` 返回的那种“陷入/陷出”机制；exception 是“由当前指令触发 trap”；interrupt 是“异步触发 trap”，例如定时器或某根外部中断线。）

## 现有 FemtoRV 内核中的中断

Matthias 做过三个支持中断的 FemtoRV：
- intermissum（RV32-IM）
- gracilis    （RV32-IMC）
- individua   （RV32-IMAC）

它们的中断逻辑是共通的：额外有一根 `interrupt_request` 线，拉高会触发中断。

它们实现了以下 CSR：
- `mepc`：保存的程序计数器（返回地址）
- `mtvec`：中断/异常处理入口地址
- `mstatus` bit x：中断使能
- `mcause` bit x：中断原因（以及锁：表示已经在中断处理里）
- 另外还有一个 `interrupt_request_sticky` 触发器

除了“读写新 CSR”（相对容易）之外，我们需要对内核做三类修改：
- 1）`interrupt_request` 如何与芯片其余部分交互
- 2）什么时候、以及如何跳转到 trap handler
- 3）如何从 trap handler 返回（也就是 `mret` 做什么）

### 1）`interrupt_request` 如何与 `interrupt_sticky` 交互

`interrupt_request` 只与 `interrupt_sticky` 直接交互，而芯片其余部分只看到 `interrupt_sticky`。

- 当 `interrupt_request` 拉高时，`interrupt_sticky` 置 1；
- `interrupt_sticky` 一旦为 1，会保持为 1，直到中断被处理（也就是执行到能够处理该中断的 `execute` 状态）。

### 2）什么时候、以及如何跳到 trap handler？

只需做三件事：
- 跳转到 trap handler：`PC = mtvec`
- 保存返回地址：`mepc = PC+4`（若是 RV32C 指令则为 `PC+2`）
- 标记“当前正在 trap handler 中”：设置 `mcause` 的 bit31（表示这是 interrupt）

它发生在 `EXECUTE` 阶段，并且需要同时满足三个条件：
- 有 pending 中断（`interrupt_request_sticky` 为 1）
- 中断使能打开（`MIE`，即 `mstatus[3]` 为 1）
- 当前不在中断处理里（`mcause[31]` 为 0）

### 3）如何从 trap handler 返回？

- 清 `mcause[31]` 为 0
- `PC = mepc`（跳回保存的返回地址）

这在 `EXECUTE` 阶段完成：当当前指令为 `mret` 时，`PC_next` 的 mux 选择 `mepc`。

### 从另一个角度总结一次“中断发生时发生了什么”

- 1）外部中断源拉高 `interrupt_request`
- 2）`interrupt_sticky` 置 1（并保持为 1，直到进入 `EXECUTE` 处理）
- 3）`EXECUTE` 设置 `mcause[31]`，把返回地址保存到 `mepc`，并跳到 trap handler；同时 `interrupt_sticky` 清 0
- 4）trap handler 中的指令执行，直到执行到 `mret`
- 5）`EXECUTE` 执行 `mret`（清 `mcause[31]`，并跳回 `mepc`）

问题：RISC-V 规范中 `mstatus` 还有 `mip`（machine interrupt pending）位。它与我们的 `interrupt_sticky` 是同一回事，还是不同概念？

## 我认为 FreeRTOS 需要什么

- 可能需要 `A` 扩展指令（可从 FemtoRV-individua 起步）
- 可能需要 `ECALL` 指令与 `mcause` 里的相关位
- 需要 `mtime`、`mtimeh`（可以复用 `mcycles`、`mcyclesh`？）
- 需要 `mtimecmp`、`mtimehcmp` 以及 `mcause` 里的相关位
- 可能需要 UART 的外部中断源（可用现有 `interrupt_request`）
- 可能需要 `mscratch` CSR
- 可能需要 `mtval`（bad address 或 bad instruction）

## 我认为 Linux-noMMU 需要什么

看看 @cnlohr 的 miniRV32。它实现了：
- `mstatus`, `mscratch`, `mtvec`, `mie`, `mip`, `mepc`, `mtval`, `mcause` 
- `cycle[l,h]`, `timer[l,h]`, `timermatch[l,h]`
  （问题：能不能直接用 cycle 当 timer？timer 是否可写？）
- `extraflags`：特权级（2 位）、WFI（1 位）、Load/Store reservation LSBs（这是什么？）

备注/问题：
- 看起来只需要 `m` 这一组 CSR，挺好。
- @cnlohr 的代码短且易读。
- load/store reservation 是什么？
- 需要仔细看看 extraflags
- mini-rv32ima.c 包含 SoC
- 最少需要多少 RAM？

## 能跑 Linux-noMMU 的最小硬件集合是什么？

@MrBossman 的 [kisc-v](https://github.com/Mr-Bossman/KISC-V) 有一个很有意思的“极简”实现：把特权 ISA 用 trap handler 软件模拟。它有一种 trap 机制：当遇到未知指令时，把 PC 与某个固定地址里存放的指针进行交换。

但这里有几个我需要搞懂的问题：
- 它如何区分 trap 与 interrupt？
  - trap handler 把所有寄存器复制到固定地址
  - trap handler 把栈指针改为 `_sstack`（系统栈？）
  - trap handler 调用 `entry`
  - trap handler 恢复寄存器
  - trap handler 跳到 `back`（那里是一条 opcode=1 的不支持指令，因此再次触发“交换 PC 与保存 PC”）
  - `cause` 从固定地址处的 `intc` 数组推导。`intc` 在哪里写？在 HDL 里？看起来像是中断控制器的 memory-mapped 寄存器。
- 它如何屏蔽中断？
- 它如何处理 pending 中断？
- 如果在 trap handler 里又发生 interrupt，会怎样？会被标记成 pending 吗？

## 新教程草案（Draft）

我想首先介绍一个概念：通过 trap handler 用软件实现新指令。因为我们后面会复用这一思路：用 trap handler 处理通过 SYSTEM 指令访问的 memory-mapped CSR。

- 1）简介与基本概念（traps / interrupts / exceptions）
- 2）用软件实现指令
  - 最小集合：`mtvec`, `mepc`, `mret`（也许把 `mtvec` 硬连）
  - 用软件实现 RV32M
  - 用软件实现 RV32F（在 RV32I 和 RV32M 上：涉及嵌套 traps）
  - RV32F 的软硬混合实现（FMA 与比较类在硬件，其余如 FDIV/FSQRT 在软件）
- 3）中断
  - 需要 `mcause` 来区分不同原因
  - 外部中断源：一根 `interrupt_request` 线
  - 计时器中断：基本 PLIC、把 CSR 投影到内存空间、在 trap handler 中访问 CSR
- 4）运行 FreeRTOS（可能单独一篇）
  - FreeRTOS 的内存环境：内存映射、memory-mapped CSR
  - 需要支持哪些指令，以及如何在 trap handler 中访问 CSR
- 5）Linux（可能单独一篇）
  - Linux 内存环境：内存映射、memory-mapped CSR
  - 需要支持哪些指令，以及如何在 trap handler 中访问 CSR

## 能跑（NoMMU）Linux 的最小内核

（待续）

##（补充）一种“极简/非标准”的硬件 trap 机制（kisc-v 思路）

在硬件中：一种非标准（non-standard）的 trap-handler 机制（Mr Bossman 的 kisc-v），大致工作方式如下：

- `mtvec` 是硬连线的常量地址（hardwired constant address）；
- 存在一个内存映射（memory-mapped）的 `mepc` CSR；
- 每当一条“无法识别的指令”进入 `EXECUTE` 阶段，就跳转到 `mepc`，并用 `PC+4` 覆盖 `mepc`；
- 另外还有类似 PLIC 的中断逻辑，使用内存映射的 `mip`、`mie`、`mstatus`、`mcause`。

## 链接（Links）

- Daniel Magum 关于定时器中断的[详细解释](https://danielmangum.com/posts/risc-v-bytes-timer-interrupts/)
- @cnlohr 的 [minirv32](https://github.com/cnlohr/mini-rv32ima)
- minirv32 引用的 Stack Overflow 问答：[here](https://stackoverflow.com/questions/61913210/risc-v-interrupt-handling-flow/61916199#61916199)
- 支持 Linux 的 @ultraembedded 模拟器：[exact-step](https://github.com/ultraembedded/exactstep/blob/master/cpu-rv32/rv32.cpp)
- @regymm 的 [quasi-soc](https://github.com/regymm/quasiSoC)
- @MrBossman 的 [kisc-v](https://github.com/Mr-Bossman/KISC-V)
- @splinedrive 的 [Kian risc-V](https://github.com/splinedrive/kianRiscV)
