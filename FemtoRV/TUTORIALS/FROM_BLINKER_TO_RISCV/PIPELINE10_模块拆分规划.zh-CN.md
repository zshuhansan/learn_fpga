# pipeline10 模块拆分规划（按耦合度/可测试性，而非强制按 5 级拆）

本文以 [pipeline10.v](file:///home/zshuhansan/git_project/learn_fpga/FemtoRV/TUTORIALS/FROM_BLINKER_TO_RISCV/pipeline10.v) 为参考，给出一套“更工程化、便于单独测试、耦合度更低”的模块划分规划。目标不是形式上把 F/D/E/M/W 五级各写成一个文件，而是把 **天然可复用/可独立验证** 的单元拆出来，同时保留流水线寄存器与控制在顶层统一仲裁，避免端口爆炸与语义混乱。

## 1. 设计目标与拆分原则

**目标**
- 保持 pipeline10 的微架构特性：RV32IM、5 级流水、前递、PC 预测（gshare 或 BTFNT）、RAS、RV32M（MUL 近似 1-cycle、DIV/REM 多周期 + stall/flush）。
- 拆出“容易单测”的功能块：decoder、imm、regfile、alu、mul/div、LSU 对齐、分支预测器、hazard/forward 控制、CSR/counters、写回 mux。
- 允许综合工具跨模块优化：默认不 `dont_touch`，仅对 debug 网/ILA 需要观察的信号做 `mark_debug`/`keep`。

**拆分原则**
- **强耦合留在顶层**：流水级寄存器（FD/DE/EM/MW）、stall/flush/redirect 的仲裁，放在 `core_top`，否则接口会爆炸。
- **纯组合/纯同步单元尽量独立**：例如 `decoder`、`imm_gen`、`alu`、`lsu_align`、`regfile`，可单测、可复用。
- **“多周期执行单元”独立**：DIV/REM 状态机（pipeline10 的 `EE_*`）天然适合拆为 `div_unit`，并通过 `busy/finished/result` 与顶层握手。
- **预测器独立**：gshare BHT + global history、RAS 的 push/pop 规则独立后更好测（给定指令流，预测是否稳定）。

## 2. 推荐层级结构（示意）

```
core_top
 ├─ ifetch_unit         (PC选择/取指请求/FD寄存器装载)
 ├─ decode_unit         (decoder + imm_gen + 分支预测“D端决策”)
 ├─ regfile             (2读1写，同步写)
 ├─ hazard_unit         (load-use/CSR/DIV busy 等产生 stall/flush)
 ├─ forward_unit        (EX端 rs1/rs2 选择旁路源)
 ├─ execute_unit        (ALU + branch/jal/jalr + 结果选择)
 │   ├─ mul_unit        (可用DSP推断，组合或1拍寄存版本)
 │   └─ div_unit        (多周期，busy/finished/result)
 ├─ lsu_unit            (地址计算 + load/store对齐 + 数据存储接口)
 │   └─ lsu_align       (wmask/wdata重排、load数据符号扩展)
 ├─ csr_unit            (cycle/instret + CSR read mux)
 └─ wb_unit             (写回选择、wb通道输出)
```

说明：
- `ifetch_unit/decode_unit/execute_unit/lsu_unit/wb_unit` 不是强制“流水级模块”，而是为了把代码从一个超大 always 块拆成更易读/易测的块；**FD/DE/EM/MW 寄存器仍建议在 core_top 内统一维护**（让 flush/stall 优先级集中）。

## 3. 顶层约定：通用信号与优先级

为了避免模块化后语义不一致，建议全核统一约定：

- `valid`：该级/该通道是否为有效指令（无效等价 NOP）
- `stall`：保持本级寄存器不变（不推进）
- `flush/kill`：把下一级寄存器强制置为 NOP/invalid
- `redirect`：PC 纠错（含地址）
- `wb` 通道：`wb_en/wb_rd/wb_data`

优先级（建议）：
1) reset
2) flush/kill（分支纠错、异常扩展预留）
3) stall（load-use、div busy、外设等待等）
4) 正常推进

## 4. 模块清单与输入输出（规划版）

下面列出建议拆分的模块、职责，以及推荐的 I/O（以“信号束”方式描述，便于你用 Verilog `wire/reg` 或 SystemVerilog `struct` 打包实现）。

### 4.1 core_top（流水线寄存器与全局仲裁）

**职责**
- 保存并更新 FD/DE/EM/MW 级寄存器。
- 仲裁并产生 `F_stall/D_stall/E_stall`、`D_flush/E_flush/M_flush`、`redirect`。
- 连接各功能模块，完成整核闭环。

**输入**
- `clk, resetn`
- `imem_rsp`（取指返回：`instr`, `valid`）
- `dmem_rsp`（数据返回：`rdata`, `ready/valid`）
- `io_rsp`（MMIO读返回，可选）

**输出**
- `imem_req`：`addr`, `en`
- `dmem_req`：`addr`, `wdata`, `wmask`, `ren`, `wen`
- `io_req`：`addr`, `wdata`, `wen`, `ren`
- `wb`：`wb_en`, `wb_rd`, `wb_data`
- debug（可选）：`pc`, `stage_valid[]`, `stage_instr[]`, `stall/flush` 等

### 4.2 regfile（寄存器堆）

**职责**
- 32×32 通用寄存器
- 2 读 1 写（读通常组合，写同步；或读同步也可但要配合流水级）

**输入**
- `clk`
- `raddr1[4:0], raddr2[4:0]`
- `wen, waddr[4:0], wdata[31:0]`

**输出**
- `rdata1[31:0], rdata2[31:0]`

**单测建议**
- 写后读同拍行为（write-first/read-first/no-change）要明确并锁定，避免后续旁路逻辑对不上。

### 4.3 decoder（指令译码）

**职责**
- 从 `instr` 解出：指令类型、rd/rs1/rs2、funct3/funct7、是否写回、是否读 rs1/rs2、是否 load/store/branch/jal/jalr/csr/ebreak 等。
- 输出一个“控制束”（control bundle），尽量让后级不再反复解析 opcode。

**输入**
- `instr[31:0]`

**输出**
- `rd, rs1, rs2`
- `funct3, funct7_bit30`
- `is_alu_reg/is_alu_imm/is_load/is_store/is_branch/is_jal/is_jalr/is_lui/is_auipc/is_csrrs/is_ebreak`
- `wb_enable`（含 rd!=0 的策略可以统一放顶层）
- `reads_rs1/reads_rs2`
- `rv32m_flags`：`is_rv32m, is_mul, is_div`（对应 pipeline10 的 `DE_isRV32M/DE_isMUL/DE_isDIV`）

**单测建议**
- 直接用已知指令编码（从 `.elf.list` 抄机器码）做表驱动测试。

### 4.4 imm_gen（立即数生成）

**职责**
- 生成 `Iimm/Simm/Bimm/Uimm/Jimm`（或 pipeline10 的 `DE_IorSimm`、`DE_PCplus4orUimm`、`DE_PCplus4orBimm` 所需的一切）。

**输入**
- `instr[31:0]`
- `pc[31:0]`（用于 PC+imm 类）
- `decoder` 的类型信号（或直接由 instr[6:2] 决定）

**输出（建议最小集）**
- `imm_i[31:0]`
- `imm_s[31:0]`
- `imm_b[31:0]`
- `imm_u[31:0]`
- `imm_j[31:0]`
- `pc_plus4[31:0]`
- `pc_plus_uimm_or_4[31:0]`（对应 AUIPC/LUI/JAL/JALR 的共享加法优化可以留在 execute_unit）

### 4.5 branch_predictor（预测器：gshare/BTFNT + RAS）

**职责**
- 在 D 端产生 `predict_pc_valid` 与 `predicted_pc`。
- 在 E 端接收“真实分支结果/真实目标”，更新 BHT/history，并在需要时给出“纠错 PC”。

**输入（D 端）**
- `d_pc[31:0]`
- `d_instr[31:0]`
- `d_valid`
- `d_is_jal/d_is_jalr/d_is_branch`
- `d_bimm/d_jimm`（可由 imm_gen 提供）
- `d_rs1_id/d_rd_id`（RAS push/pop 规则需要）

**输出（D 端）**
- `d_predict_pc_valid`
- `d_predicted_pc[31:0]`
- `d_predict_taken`（分支预测 taken 与否）
- `d_predict_ra`（RAS 顶，供 JALR 预测用，可选）

**输入（E 端更新）**
- `e_valid`
- `e_pc[31:0]`
- `e_is_branch/e_is_jalr`
- `e_take_branch`（真实是否跳）
- `e_jalr_target[31:0]`（真实目标）
- `e_predicted_taken`（从 D 端随流水带下来的预测）
- `e_predicted_ra`（若启用 RAS）
- `e_bht_index`（若启用 gshare，可在 D 端算好随流水带下去）

**输出（纠错）**
- `redirect_valid`
- `redirect_pc[31:0]`

说明：
- pipeline10 把 “D->F 预测路径” 与 “E->F 纠错路径” 都放在顶层 mux（见 `F_PC` 选择）。模块化后依然建议保持这个结构：预测器只产出 `predicted_pc/redirect_pc`，顶层负责优先级与 stall/flush 的配合。

### 4.6 hazard_unit（冒险检测与 stall/flush 生成）

**职责**
- 产生 `F_stall/D_stall/E_stall`、`D_flush/E_flush/M_flush`。
- 覆盖三类典型来源：
  - load-use hazard（E 阶段 load，其数据要到 M/W 才能用）
  - CSR read hazard（如果 CSR 读结果在后级才可用）
  - DIV/REM 多周期执行（pipeline10 的 `aluBusy`）
  - 可选：结构冒险（如果你把 dmem/io 合成单端口 BRAM 也会出现）

**输入**
- `fd_valid/fd_rs1/fd_rs2/fd_reads_rs1/fd_reads_rs2`
- `de_valid/de_rd/de_is_load/de_is_csrrs/de_is_div`（或更一般的“E 级结果何时可用”标志）
- `alu_busy`（来自 div_unit）
- `redirect_valid`（来自 branch_predictor 或 execute_unit）
- `halt`（ebreak）

**输出**
- `F_stall, D_stall, E_stall`
- `D_flush, E_flush, M_flush`

### 4.7 forward_unit（前递/旁路选择）

**职责**
- 计算 EX 级（E）使用的 `rs1/rs2` 实际值来自哪个源：
  - `regfile`
  - `EM` 结果（上条指令的 EX/MEM）
  - `MW` 写回数据（MEM/WB）

**输入**
- `de_rs1_id/de_rs2_id`
- `de_rs1_data/de_rs2_data`（来自 regfile）
- `em_wb_en/em_rd/em_value`
- `mw_wb_en/mw_rd/mw_value`

**输出**
- `e_rs1[31:0], e_rs2[31:0]`

说明：
- pipeline10 里前递选择分散在 E 段逻辑中，独立模块能显著降低 execute_unit 的复杂度。

### 4.8 execute_unit（ALU + 分支/跳转 + 结果选择）

**职责**
- 基于译码控制束与 `e_rs1/e_rs2/imm` 计算：
  - `alu_out`（算术/逻辑/移位/比较）
  - 分支是否成立 `take_branch`
  - JALR 目标地址
  - “E 级结果”（对应 pipeline10 的 `E_result`，包含 JAL/JALR 写回 PC+4、LUI/AUIPC、普通 ALU）

**输入**
- `pc`
- `instr/control bundle`
- `rs1, rs2`（来自 forward_unit）
- `imm`（来自 imm_gen 或随流水携带）
- `mul_result`（来自 mul_unit，可选）
- `div_result/div_busy/div_finished`（来自 div_unit）

**输出**
- `e_result[31:0]`
- `e_take_branch`
- `e_jalr_target[31:0]`
- `e_is_load/e_is_store`（给 lsu_unit）
- `e_addr_base[31:0]`（通常是 rs1）
- `e_imm_for_addr[31:0]`（Iimm/Simm）
- `alu_busy`（如果 DIV 正在跑）

### 4.9 mul_unit（乘法）

**职责**
- 提供 MUL/MULH/MULHSU/MULHU 的 64-bit 乘积或高低位选择。

**输入**
- `a[31:0], b[31:0]`
- `mode`：选择 signed/unsigned 组合与取高/低位

**输出**
- `mul64[63:0]` 或 `mul_out[31:0]`

实现提示：
- 若要 Vivado 更倾向 DSP：可以在内部乘法线上加 `(* use_dsp = "yes" *)`。
- 若要严格 1-cycle 频率：可用 XPM/Multiplier Generator 做 1-stage pipeline，并由顶层把 MUL 当作 1-cycle 或 2-cycle 单元处理（这会影响 stall/forward 语义）。

### 4.10 div_unit（除法/取模，多周期）

**职责**
- 复制 pipeline10 的 `EE_*` 状态机（迭代除法），对 DIV/DIVU/REM/REMU 提供结果。

**输入**
- `clk, resetn`
- `start`（进入 DIV 指令且无 hazard 时拉高一次）
- `funct3[2:0]`（区分 DIV/DIVU/REM/REMU）
- `rs1[31:0], rs2[31:0]`

**输出**
- `busy`
- `finished`（可 1-cycle 脉冲）
- `quotient[31:0], remainder[31:0]` 或直接 `result[31:0]`

顶层配合点：
- `busy` 期间需要 `F/D/E stall`，并对后级适当 flush/bubble（pipeline10 的 `E_stall/M_flush` 思路）。

### 4.11 lsu_align（load/store 对齐与字节使能）

**职责**
- 根据 `addr[1:0]` 与 `funct3`：
  - 生成 store 的 `wmask[3:0]` 与重排后的 `wdata[31:0]`
  - 对 load 的 `rdata` 做 byte/halfword 选择与符号扩展/零扩展

**输入**
- `addr[31:0]`
- `funct3[2:0]`
- `store_rs2[31:0]`
- `load_rdata_raw[31:0]`（从 dmem 读回的原始 32-bit word）

**输出**
- `store_wmask[3:0]`
- `store_wdata[31:0]`
- `load_data[31:0]`

### 4.12 lsu_unit（数据存储访问单元）

**职责**
- 在 E/M 边界生成并寄存 `addr/rs2/funct3`，在 M 级发起 dmem/io 访问。
- 把 dmem 与 io 的读数据在 W 级选择好，交给 wb_unit。

**输入**
- `e_is_load/e_is_store`
- `e_addr = rs1 + imm`（来自 execute_unit）
- `e_rs2`（store data 原始值）
- `funct3`
- `dmem_rsp/io_rsp`

**输出**
- `dmem_req`（addr/ren/wen/wdata/wmask）
- `io_req`（addr/ren/wen/wdata）
- `mem_to_wb`：`w_is_io`、`w_load_data` 等（或直接交给 wb_unit）

### 4.13 csr_unit（CSR + 计数器）

**职责**
- 维护 `cycle/instret`（pipeline10 顶层已有）
- 对 CSRRS（以及未来可扩展 CSR 指令）给出读结果

**输入**
- `clk, resetn`
- `instret_inc`（一条指令退休时 +1，通常在 W 级且 valid 且非 bubble）
- `csr_read_en`
- `csr_id`（pipeline10 使用 2-bit 压缩 csrId：对应 cycle/instret 的低/高 32 位）

**输出**
- `csr_rdata[31:0]`
- `cycle[63:0], instret[63:0]`（可选导出用于 debug/统计）

### 4.14 wb_unit（写回选择）

**职责**
- 统一写回数据源优先级：
  - load：来自 dmem/io + 对齐后的 `load_data`
  - csr：来自 csr_unit
  - 其他：来自 execute_unit 的 `e_result`
- 生成 regfile 的写端口信号。

**输入**
- `mw_control`（是否 load/csr/普通）
- `mw_e_result`
- `w_load_data`
- `mw_csr_data`
- `rd`

**输出**
- `wb_en/wb_rd/wb_data`（直接连 regfile）

## 5. 模块化落地建议（实现路线）

**路线 A（最省事，适合比赛开发节奏）**
- 先把 `decoder/imm_gen/regfile/lsu_align/div_unit` 拆出来。
- `core_top` 里仍然保留 pipeline10 的大部分流水控制与寄存器更新。
- 这样能把 60% 的“可单测逻辑”独立出来，同时风险最小。

**路线 B（更彻底，但端口更多）**
- 再把 `branch_predictor/hazard_unit/forward_unit/execute_unit/lsu_unit/wb_unit` 拆出。
- 顶层只保留流水寄存器与仲裁，所有组合逻辑都在子模块中。

## 6. 与 Xilinx BRAM IP 存放 PROGROM/DATARAM 的接口建议

如果你计划把程序与数据都改成 Xilinx BRAM IP：
- 建议把 `imem_req/imem_rsp`、`dmem_req/dmem_rsp` 作为 core_top 的标准端口束，便于之后从仿真 `$readmemh` 版本替换到 IP 版本。
- BRAM 默认同步读（读延迟 1 拍），这与 pipeline10 现在的取指/访存节拍是兼容的；关键在于你要把“读数据有效的拍”定义清楚，并让 `lsu_unit` 与 `wb_unit` 按这个节拍写回。

