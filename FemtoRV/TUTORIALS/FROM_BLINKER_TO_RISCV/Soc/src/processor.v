module processor (
    input         clk,
    input         resetn,
    
    // 取指接口，外部通常接 ICache 或同步程序 ROM。
    output [31:0] inst_addr,
    input  [31:0] inst_rdata,
    output        inst_en,
    input         inst_valid,
    input         inst_ready,
    
    // 数据存储器接口采用读写分离风格，方便映射到双口 RAM。
    output [31:0] data_raddr,
    input  [31:0] data_rdata,
    output [31:0] data_waddr,
    output [31:0] data_wdata,
    output [3:0]  data_wmask,
    output        data_wen,

    // 地址命中 IO 空间时，通过这组信号和外设寄存器交互。
    output [31:0] IO_mem_addr,
    input [31:0]  IO_mem_rdata,
    output [31:0] IO_mem_wdata,
    output        IO_mem_wr,
    output        halt
);

`ifdef BENCH
`include "riscv_disassembly.v"
`endif

   // 这颗处理器保持了清晰的 F/D/E/M/W 五段结构：
   // F 负责发起取指并处理预测跳转，D 做译码与寄存器编号准备，
   // E 完成旁路、ALU、分支决策，M 负责真正的存储访问，W 统一写回。
   // CSR 目前只实现 cycle/instret 这两个最常用的统计计数器。
   wire [63:0] cycle;
   wire [63:0] instret;
   wire [31:0] M_CSR_data;

   csr_unit CSR(
      .clk(clk),
      .resetn(resetn),
      .valid_inst_retired(!MW_nop),
      .csrId_is(EM_csrId_is),
      .csr_rdata(M_CSR_data),
      .out_cycle(cycle),
      .out_instret(instret)
   );

   // hazard_unit 统一给出各级 stall/flush 控制。除法是多周期操作时，
   // E_stall 和 M_flush 会让后端保持一致状态直到结果准备好。

   wire F_stall;
   wire D_stall;
   wire D_flush;
   wire E_stall;
   wire E_flush;
   wire M_flush;
   wire dataHazard;
   wire rs1Hazard;
   wire rs2Hazard;
   wire aluBusy;
   wire E_correctPC;
   wire E_takeBranch;
   wire fetch_valid = inst_valid;
   wire icache_miss = resetn ? (F_pendingValid && !inst_valid && !inst_ready) : 1'b0;

   hazard_unit HZ (
      .d_is_load(D_isLoad),
      .d_is_store(D_isStore),
      .d_reads_rs1(D_readsRs1),
      .d_reads_rs2(D_readsRs2),
      .d_rs1_id(D_rs1Id),
      .d_rs2_id(D_rs2Id),
      .fd_nop(FD_nop),
      .de_rd_id(DE_rdId),
      .de_is_load(DE_isLoad),
      .de_is_csrrs(DE_isCSRRS),
      .de_is_store(DE_isStore),
      .e_correct_pc(E_correctPC),
      .alu_busy(aluBusy),
      .halt(halt),
      .icache_miss(icache_miss),
      .f_stall(F_stall),
      .d_stall(D_stall),
      .e_stall(E_stall),
      .d_flush(D_flush),
      .e_flush(E_flush),
      .m_flush(M_flush),
      .data_hazard(dataHazard),
      .rs1_hazard(rs1Hazard),
      .rs2_hazard(rs2Hazard)
   );

   // ----------------------------
   // F 级：取指与前端重定向
   // ----------------------------

   reg  [31:0] PC;
   reg  [31:0] F_pendingPC;
   reg  [1:0]  F_pendingEpoch;
   reg         F_pendingValid;
   reg  [1:0]  F_epoch;
   reg  [31:0] FB_PC;
   reg  [31:0] FB_instr;
   reg  [1:0]  FB_epoch;
   reg         FB_valid;

`ifdef CONFIG_PC_PREDICT
   wire [31:0] F_PC = E_correctPC ? E_PCcorrection :
                      D_predictPC ? D_PCprediction :
                                    PC;
`else
   wire [31:0] F_PC = E_correctPC ? E_PCcorrection : PC;
`endif
   wire [31:0] F_PCplus4 = F_PC + 4;
   wire        F_reqFire = inst_en && inst_ready;
   wire        F_redirect = E_correctPC ||
                            (D_predictPC && (F_pendingPC != D_PCprediction));
   wire [1:0]  F_epochNext = F_epoch + (F_redirect ? 2'd1 : 2'd0);
   wire        processor_unused_ok = &{
      1'b0,
      cycle,
      instret,
      F_stall,
      rs1Hazard,
      rs2Hazard,
      D_isSYSTEM,
      D_isLUIorAUIPC,
      D_Iimm,
      D_Simm,
      DE_funct3_is[3:2],
      DE_isALUimm,
      DE_isAUIPC,
      DE_isLUI,
      DE_isMUL,
      E_shamt,
      E_aluPlus[0],
      EE_quotient,
      EE_dividend,
      EE_div_sign,
      EM_rs1Id,
      EM_rs2Id,
      EM_correctPC,
      EM_PCcorrection,
      M_word_addr,
      MW_PC,
      nbRV32M
   };

   // F_PC 是当前真正送去取指的地址，可能来自顺序 PC、D 级预测或 E 级纠正。
   assign inst_addr = F_PC;
   assign inst_en   = resetn && !halt && !D_stall && !FB_valid;
   wire [31:0] PROGROM_rdata = inst_rdata;

   always @(posedge clk) begin

      if(!resetn) begin
         PC <= 0;
         F_pendingPC <= 32'b0;
         F_pendingEpoch <= 2'b0;
         F_pendingValid <= 1'b0;
         F_epoch <= 2'b0;
         FB_PC <= 32'b0;
         FB_instr <= 32'b0000000_00000_00000_000_00000_0110011;
         FB_epoch <= 2'b0;
         FB_valid <= 1'b0;
         FD_instr_reg <= 32'b0000000_00000_00000_000_00000_0110011;
         FD_PC <= 32'b0;
         FD_nop <= 1'b1;
      end else begin
         if(D_predictPC && !D_stall) begin
            PC <= D_PCprediction;
            FD_nop <= 1'b1;
            FB_valid <= 1'b0;
         end
         if(F_redirect) begin
            F_epoch <= F_epochNext;
            FB_valid <= 1'b0;
         end
         if(E_correctPC) begin
            PC <= E_PCcorrection;
            FD_nop <= 1'b1;
            FB_valid <= 1'b0;
         end

         if(fetch_valid && F_pendingValid) begin
            if((F_pendingEpoch != F_epochNext) || D_flush) begin
               if(!D_stall) begin
                  FD_nop <= 1'b1;
               end
            end else if(!D_stall || FD_nop) begin
               FD_PC <= F_pendingPC;
               FD_instr_reg <= PROGROM_rdata;
               FD_nop <= 1'b0;
            end else begin
               FB_PC <= F_pendingPC;
               FB_instr <= PROGROM_rdata;
               FB_epoch <= F_pendingEpoch;
               FB_valid <= 1'b1;
            end
         end else if(!D_stall) begin
            if(FB_valid && (FB_epoch == F_epochNext)) begin
               FD_PC <= FB_PC;
               FD_instr_reg <= FB_instr;
               FD_nop <= 1'b0;
               FB_valid <= 1'b0;
            end else if(FB_valid) begin
               FB_valid <= 1'b0;
               FD_nop <= 1'b1;
            end else begin
               FD_nop <= 1'b1;
            end
         end

         if(fetch_valid && !F_reqFire) begin
            F_pendingValid <= 1'b0;
         end

         if(F_reqFire) begin
            F_pendingPC <= F_PC;
            F_pendingEpoch <= F_epochNext;
            F_pendingValid <= 1'b1;
            PC <= F_PCplus4;
         end
      end
   end

   reg [31:0] FD_PC;
   reg [31:0] FD_instr_reg;
   wire [31:0] FD_instr = FD_instr_reg;
   reg        FD_nop;
   // ----------------------------
   // D 级：译码与分支预测
   // ----------------------------

   // 写回级结果会回送到寄存器堆，供下一拍译码/执行读取。
   wire        wbEnable;
   wire [31:0] wbData;
   wire [4:0]  wbRdId;

   // 指令字段译码与立即数生成已经拆到独立子模块，便于单测和复用。
   wire [4:0] D_rdId;
   wire [4:0] D_rs1Id;
   wire [4:0] D_rs2Id;
   wire [2:0] D_funct3;
   wire D_funct7_bit30;
   wire [1:0] D_csrId;
   wire [7:0] D_funct3_onehot;
   wire D_isALUreg;
   wire D_isALUimm;
   wire D_isBranch;
   wire D_isJALR;
   wire D_isJAL;
   wire D_isAUIPC;
   wire D_isLUI;
   wire D_isLoad;
   wire D_isStore;
   wire D_isSYSTEM;
   wire D_isCSRRS;
   wire D_isEBREAK;
   wire D_readsRs1;
   wire D_readsRs2;
   wire D_wbEnable_raw;
   wire D_isRV32M;
   wire D_isMUL;
   wire D_isDIV;
   wire D_isJALorJALR;
   wire D_isLUIorAUIPC;
   wire D_isJALorJALRorLUIorAUIPC;

   wire [31:0] D_Iimm;
   wire [31:0] D_Simm;
   wire [31:0] D_Bimm;
   wire [31:0] D_Uimm;
   wire [31:0] D_Jimm;
   wire [31:0] D_IorSimm;

   decoder DEC(
      .instr(FD_instr),
      .rd_id(D_rdId),
      .rs1_id(D_rs1Id),
      .rs2_id(D_rs2Id),
      .funct3(D_funct3),
      .funct7_bit30(D_funct7_bit30),
      .csr_id(D_csrId),
      .funct3_onehot(D_funct3_onehot),
      .is_alu_reg(D_isALUreg),
      .is_alu_imm(D_isALUimm),
      .is_branch(D_isBranch),
      .is_jalr(D_isJALR),
      .is_jal(D_isJAL),
      .is_auipc(D_isAUIPC),
      .is_lui(D_isLUI),
      .is_load(D_isLoad),
      .is_store(D_isStore),
      .is_system(D_isSYSTEM),
      .is_csrrs(D_isCSRRS),
      .is_ebreak(D_isEBREAK),
      .reads_rs1(D_readsRs1),
      .reads_rs2(D_readsRs2),
      .wb_enable_raw(D_wbEnable_raw),
      .is_rv32m(D_isRV32M),
      .is_mul(D_isMUL),
      .is_div(D_isDIV),
      .is_jal_or_jalr(D_isJALorJALR),
      .is_lui_or_auipc(D_isLUIorAUIPC),
      .is_jal_or_jalr_or_lui_or_auipc(D_isJALorJALRorLUIorAUIPC)
   );

   imm_gen IMM(
      .instr(FD_instr),
      .is_store(D_isStore),
      .imm_i(D_Iimm),
      .imm_s(D_Simm),
      .imm_b(D_Bimm),
      .imm_u(D_Uimm),
      .imm_j(D_Jimm),
      .i_or_s_imm(D_IorSimm)
   );

   wire D_predictPC;
   wire [31:0] D_PCprediction;
   wire D_predictBranch;
   wire [31:0] D_predictRA;
`ifdef CONFIG_BHT_ADDR_BITS
   localparam BP_BHT_ADDR_BITS = `CONFIG_BHT_ADDR_BITS;
`else
   localparam BP_BHT_ADDR_BITS = 12;
`endif
`ifdef CONFIG_BHT_HISTO_BITS
   localparam BP_BHT_HISTO_BITS = `CONFIG_BHT_HISTO_BITS;
`else
   localparam BP_BHT_HISTO_BITS = 9;
`endif
   wire [BP_BHT_ADDR_BITS-1:0] D_BHTindex;

`ifdef CONFIG_PC_PREDICT
   branch_predictor #(
      .BHT_ADDR_BITS(BP_BHT_ADDR_BITS),
      .BHT_HISTO_BITS(BP_BHT_HISTO_BITS),
`ifdef CONFIG_GSHARE
      .WITH_GSHARE(1),
`else
      .WITH_GSHARE(0),
`endif
`ifdef CONFIG_RAS
      .WITH_RAS(1)
`else
      .WITH_RAS(0)
`endif
   ) BP (
      .clk(clk),
      .resetn(resetn),
      .fd_pc(FD_PC),
      .fd_instr(FD_instr),
      .d_is_branch(D_isBranch),
      .d_is_jal(D_isJAL),
      .d_is_jalr(D_isJALR),
      .d_rd_id(D_rdId),
      .d_rs1_id(D_rs1Id),
      .d_bimm(D_Bimm),
      .d_jimm(D_Jimm),
      .d_stall(D_stall),
      .d_flush(D_flush),
      .fd_nop(FD_nop),
      .d_predict_branch(D_predictBranch),
      .d_predict_pc_valid(D_predictPC),
      .d_pc_prediction(D_PCprediction),
      .d_predict_ra(D_predictRA),
      .d_bht_index(D_BHTindex),
      .de_is_jal(DE_isJAL),
      .de_is_jalr(DE_isJALR),
      .de_rd_id(DE_rdId),
      .de_rs1_id(DE_rs1Id),
      .de_pcplus4(DE_PCplus4orUimm),
      .de_is_branch(DE_isBranch),
      .e_take_branch(E_takeBranch),
      .e_correct_pc(E_correctPC),
      .de_bht_index(DE_BHTindex),
      .e_stall(E_stall)
   );
`else
   assign D_predictPC = 1'b0;
   assign D_PCprediction = 32'b0;
   assign D_predictBranch = 1'b0;
   assign D_predictRA = 32'b0;
   assign D_BHTindex = {BP_BHT_ADDR_BITS{1'b0}};
`endif

   wire [31:0] RF_rs1_data;
   wire [31:0] RF_rs2_data;

   regfile RF(
      .clk(clk),
      .wen(wbEnable),
      .waddr(wbRdId),
      .wdata(wbData),
      .raddr1(DE_rs1Id),
      .raddr2(DE_rs2Id),
      .rdata1(RF_rs1_data),
      .rdata2(RF_rs2_data)
   );

   always @(posedge clk) begin


      if(!D_stall) begin

	 DE_rdId  <= D_rdId;
	 DE_rs1Id <= D_rs1Id;
	 DE_rs2Id <= D_rs2Id;

	 DE_funct3    <= D_funct3;
	 DE_funct3_is <= D_funct3_onehot;
	 DE_funct7    <= D_funct7_bit30;
	 DE_csrId     <= D_csrId;

`ifdef CONFIG_RV32M
	 DE_isRV32M <= D_isRV32M;
	 DE_isMUL   <= D_isMUL;
	 DE_isDIV   <= D_isDIV;
`endif

	 DE_nop <= 1'b0;


	 DE_isALUreg <= D_isALUreg;
	 DE_isALUimm <= D_isALUimm;
	 DE_isBranch <= D_isBranch;
	 DE_isJALR   <= D_isJALR;
	 DE_isJAL    <= D_isJAL;
	 DE_isAUIPC  <= D_isAUIPC;
	 DE_isLUI    <= D_isLUI;
	 DE_isLoad   <= D_isLoad;
	 DE_isStore  <= D_isStore;
	 DE_isCSRRS  <= D_isCSRRS;
	 DE_isEBREAK <= D_isEBREAK;

	 // 这里只记录“这类指令具备写回资格”，真正写回还要到后面再排除 rd=x0。
	 DE_wbEnable <= D_wbEnable_raw;

	 DE_IorSimm <= D_IorSimm;

`ifdef CONFIG_PC_PREDICT
     // 分支预测失误时，纠正地址需要在“落空”和“多跳”之间二选一。
     DE_PCplus4orBimm <= FD_PC + (D_predictBranch ? 4 : D_Bimm);
     DE_predictBranch <= D_predictBranch;
     DE_predictRA     <= D_predictRA;
 `ifdef CONFIG_GSHARE
     DE_BHTindex  <= D_BHTindex;
 `endif
`else
	 DE_PCplusBorJimm <= FD_PC + (D_isJAL ? D_Jimm : D_Bimm);
`endif

	 // 这条式子把 JAL/JALR/LUI/AUIPC 共用的“写回结果”统一成一套加法表达式。
	 DE_PCplus4orUimm <= ({32{FD_instr[6:5]!=2'b01}} & FD_PC) +
                             (D_isJALorJALR ? 4 : D_Uimm);

	 DE_isJALorJALRorLUIorAUIPC <= D_isJALorJALRorLUIorAUIPC;
      end

      if(E_flush | FD_nop) begin
	 DE_nop      <= 1'b1;
	 DE_isALUreg <= 1'b0;
	 DE_isALUimm <= 1'b0;
	 DE_isBranch <= 1'b0;
	 DE_isJALR   <= 1'b0;
	 DE_isJAL    <= 1'b0;
	 DE_isAUIPC  <= 1'b0;
	 DE_isLUI    <= 1'b0;
	 DE_isLoad   <= 1'b0;
	 DE_isStore  <= 1'b0;
	 DE_isCSRRS  <= 1'b0;
	 DE_isEBREAK <= 1'b0;
	 DE_wbEnable <= 1'b0;
`ifdef CONFIG_RV32M
	 DE_isRV32M <= 1'b0;
	 DE_isMUL   <= 1'b0;
	 DE_isDIV   <= 1'b0;

`endif
	 DE_isJALorJALRorLUIorAUIPC <= 1'b0;
      end

   end

   reg        DE_nop;
   reg [4:0]  DE_rdId;
   reg [4:0]  DE_rs1Id;
   reg [4:0]  DE_rs2Id;

   reg [1:0]  DE_csrId;
   reg [2:0]  DE_funct3;
   (* onehot *) reg [7:0] DE_funct3_is;
   reg [5:5]  DE_funct7;

   reg [31:0] DE_IorSimm;

   reg DE_isALUreg;
   reg DE_isALUimm;
   reg DE_isBranch;
   reg DE_isJALR;
   reg DE_isJAL;
   reg DE_isAUIPC;
   reg DE_isLUI;
   reg DE_isLoad;
   reg DE_isStore;
   reg DE_isCSRRS;
   reg DE_isEBREAK;

`ifdef CONFIG_RV32M
   reg DE_isRV32M;
   reg DE_isMUL;
   reg DE_isDIV;
`endif

   reg DE_wbEnable;

   reg DE_isJALorJALRorLUIorAUIPC;

`ifdef CONFIG_PC_PREDICT
   reg [31:0] DE_PCplus4orBimm;
   reg DE_predictBranch;
   reg [31:0] DE_predictRA;
 `ifdef CONFIG_GSHARE
   reg [BP_BHT_ADDR_BITS-1:0] DE_BHTindex;
 `endif
`else
   reg [31:0] DE_PCplusBorJimm;
`endif

   reg [31:0] DE_PCplus4orUimm;

   // ----------------------------
   // E 级：旁路、运算、分支决策
   // ----------------------------

   // 旁路优先级是 M 级优先于 W 级，这样能最快消费刚产生的新结果。

   wire E_M_fwd_rs1 = EM_wbEnable && (EM_rdId == DE_rs1Id);
   wire E_W_fwd_rs1 = MW_wbEnable && (MW_rdId == DE_rs1Id);

   wire E_M_fwd_rs2 = EM_wbEnable && (EM_rdId == DE_rs2Id);
   wire E_W_fwd_rs2 = MW_wbEnable && (MW_rdId == DE_rs2Id);

   wire [31:0] E_rs1 = E_M_fwd_rs1 ? EM_Eresult :
	               E_W_fwd_rs1 ? wbData     :
	                             RF_rs1_data;

   wire [31:0] E_rs2 = E_M_fwd_rs2 ? EM_Eresult :
	               E_W_fwd_rs2 ? wbData     :
	                             RF_rs2_data;

   wire [31:0] E_aluIn1 = E_rs1;
   wire [31:0] E_aluIn2 = (DE_isALUreg | DE_isBranch) ? E_rs2 : DE_IorSimm;
   wire [4:0]  E_shamt  = DE_isALUreg ? E_rs2[4:0] : DE_rs2Id;

   wire E_minus = DE_funct7[5] & DE_isALUreg;
   wire E_arith_shift = DE_funct7[5];

   // 加法结果既给普通算术，也给 JALR 目标地址复用。
   wire [31:0] E_aluPlus;
   wire        E_LT;
   wire        E_LTU;
   wire        E_EQ;
   wire [31:0] E_aluOut;
   
   wire EE_divBusy;
   wire EE_divFinished;
   wire [31:0] EE_quotient;
   wire [31:0] EE_dividend;
   wire EE_div_sign;
   wire [31:0] E_div_result;

`ifdef CONFIG_RV32M
   div_unit DIVU(
      .clk(clk),
      .resetn(resetn),
      .start(DE_isDIV & !dataHazard & !EE_divFinished),
      .is_div(DE_isDIV),
      .funct3(DE_funct3),
      .rs1(E_rs1),
      .rs2(E_rs2),
      .busy(EE_divBusy),
      .finished(EE_divFinished),
      .quotient_abs(EE_quotient),
      .remainder_abs(EE_dividend),
      .sign_flag(EE_div_sign),
      .result(E_div_result)
   );
   assign aluBusy = EE_divBusy | (DE_isDIV & !EE_divFinished);
`else
   assign E_div_result = 32'b0;
   assign aluBusy = 1'b0;
`endif

`ifdef CONFIG_RV32M
   wire [31:0] alu_div_result = E_div_result;
   wire        alu_is_rv32m   = DE_isRV32M;
   wire        alu_is_div     = DE_isDIV;
`else
   wire [31:0] alu_div_result = 32'b0;
   wire        alu_is_rv32m   = 1'b0;
   wire        alu_is_div     = 1'b0;
`endif

   alu #(
`ifdef CONFIG_RV32M
       .WITH_RV32M(1)
`else
       .WITH_RV32M(0)
`endif
   ) ALU (
       .in1(E_aluIn1),
       .in2(E_aluIn2),
       .funct3(DE_funct3),
       .is_minus(E_minus),
       .is_arith_shift(E_arith_shift),
       .is_rv32m(alu_is_rv32m),
       .is_div(alu_is_div),
       .div_result(alu_div_result),
       .alu_out(E_aluOut),
       .alu_plus(E_aluPlus),
       .eq(E_EQ),
       .lt(E_LT),
       .ltu(E_LTU)
   );

   // 条件分支在 E 级真正决定 taken/not-taken，并与前端预测结果比较。
   assign E_takeBranch =
     DE_isBranch &&
     (DE_funct3_is[0] ? E_EQ  :
      DE_funct3_is[1] ? !E_EQ :
      DE_funct3_is[4] ? E_LT  :
      DE_funct3_is[5] ? !E_LT :
      DE_funct3_is[6] ? E_LTU :
      DE_funct3_is[7] ? !E_LTU : 1'b0);

   wire [31:0] E_JALRaddr = {E_aluPlus[31:1],1'b0};

`ifdef CONFIG_PC_PREDICT
 `ifdef CONFIG_RAS
     assign E_correctPC = (
	   (DE_isJALR    && (DE_predictRA != E_JALRaddr)   ) ||
           (DE_isBranch  && (E_takeBranch^DE_predictBranch))
     );
 `else
     assign E_correctPC = DE_isJALR ||
	(DE_isBranch  && (E_takeBranch^DE_predictBranch));
 `endif
   wire [31:0] E_PCcorrection = DE_isBranch ? DE_PCplus4orBimm : E_JALRaddr;
`else
   assign E_correctPC = (
			   DE_isJAL || DE_isJALR ||
			  (DE_isBranch && E_takeBranch)
			 );
   wire [31:0] E_PCcorrection =
	       DE_isJALR ? E_JALRaddr : DE_PCplusBorJimm;
`endif

   wire [31:0] E_result =
	       DE_isJALorJALRorLUIorAUIPC ? DE_PCplus4orUimm : E_aluOut;

   wire [31:0] E_addr = E_rs1 + DE_IorSimm;

   always @(posedge clk) begin

      if(!resetn) begin
         EM_nop      <= 1'b1;
         EM_rdId     <= 5'b0;
         EM_rs1Id    <= 5'b0;
         EM_rs2Id    <= 5'b0;
         EM_funct3   <= 3'b0;
         EM_csrId_is <= 4'b0;
         EM_rs2      <= 32'b0;
         EM_Eresult  <= 32'b0;
         EM_addr     <= 32'b0;
         EM_isLoad   <= 1'b0;
         EM_isStore  <= 1'b0;
         EM_isCSRRS  <= 1'b0;
         EM_wbEnable <= 1'b0;
         EM_correctPC  <= 1'b0;
         EM_PCcorrection <= 32'b0;
      end else if(M_flush) begin
         EM_nop       <= 1'b1;
         EM_isLoad    <= 1'b0;
         EM_isStore   <= 1'b0;
         EM_isCSRRS   <= 1'b0;
         EM_wbEnable  <= 1'b0;
         EM_correctPC <= 1'b0;
      end else if(!E_stall) begin
         EM_nop      <= DE_nop;
         EM_rdId     <= DE_rdId;
         EM_rs1Id    <= DE_rs1Id;
         EM_rs2Id    <= DE_rs2Id;
         EM_funct3   <= DE_funct3;
         EM_csrId_is <= 4'b0001 << DE_csrId;
         EM_rs2      <= E_rs2;
         EM_Eresult  <= E_result;
         EM_addr     <= E_addr;
         EM_isLoad   <= DE_isLoad;
         EM_isStore  <= DE_isStore;
         EM_isCSRRS  <= DE_isCSRRS;
         EM_wbEnable <= DE_wbEnable && (DE_rdId != 0);
         EM_correctPC  <= E_correctPC;
         EM_PCcorrection <= E_PCcorrection;
      end
   end

   assign halt = resetn & DE_isEBREAK;

   reg        EM_nop;
   reg [4:0]  EM_rdId;
   reg [4:0]  EM_rs1Id;
   reg [4:0]  EM_rs2Id;
   (* onehot *) reg [3:0]  EM_csrId_is;
   reg [2:0]  EM_funct3;
   reg [31:0] EM_rs2;
   reg [31:0] EM_Eresult;
   reg [31:0] EM_addr;
   reg        EM_isStore;
   reg        EM_isLoad;
   reg        EM_isCSRRS;
   reg 	      EM_wbEnable;
   reg        EM_correctPC;
   reg [31:0] EM_PCcorrection;

   // ----------------------------
   // M 级：地址分类与访存对齐
   // ----------------------------

   // 负责任何非字访问的重排和符号扩展细节，主流水线只处理 32 位总线。
   wire [31:0] M_STORE_data;
   wire [3:0]  M_STORE_wmask;
   wire [31:0] M_Mdata;

   lsu_align LSU_ALIGN(
      .addr(EM_addr),
      .funct3(EM_funct3),
      .store_rs2(EM_rs2),
      .load_raw_word(M_raw_rdata),
      .store_data(M_STORE_data),
      .store_wmask(M_STORE_wmask),
      .load_data(M_Mdata)
   );


   wire  M_isIO         = EM_addr[22];
   wire  M_isRAM        = !M_isIO;

   assign IO_mem_addr  = EM_addr;
   assign IO_mem_wr    = resetn ? (EM_isStore && M_isIO) : 1'b0;
   assign IO_mem_wdata = EM_rs2;

   wire [3:0] M_wmask = {4{EM_isStore & M_isRAM}} & M_STORE_wmask;
   wire [13:0] M_word_addr = EM_addr[15:2];

   // 数据 RAM 读地址在当前实现里直接取执行级算出的地址，匹配同步读 RAM 的一拍延迟。
   assign data_raddr = E_addr;
   wire [31:0] M_raw_rdata = data_rdata;
   assign data_wmask = resetn ? M_wmask : 4'b0;
   assign data_waddr = EM_addr;
   assign data_wdata = M_STORE_data;
   assign data_wen   = resetn ? |M_wmask : 1'b0;

   always @(posedge clk) begin
      if(!resetn) begin
         MW_nop       <= 1'b1;
         MW_rdId      <= 5'b0;
         MW_wbData    <= 32'b0;
         MW_wbEnable  <= 1'b0;
      end else begin
         MW_nop       <= EM_nop;
         MW_rdId      <= EM_rdId;

         MW_wbData <=
             EM_isLoad  ? (M_isIO ? IO_mem_rdata : M_Mdata) :
             EM_isCSRRS ? M_CSR_data   :
             EM_Eresult;

         MW_wbEnable  <= EM_wbEnable;
      end
   end

   reg        MW_nop;
   reg [4:0]  MW_rdId;
   reg [31:0] MW_wbData;
   reg 	      MW_wbEnable;
   // ----------------------------
   // W 级：统一写回
   // ----------------------------

   assign wbData   = MW_wbData;
   assign wbEnable = MW_wbEnable;
   assign wbRdId   = MW_rdId;

`ifdef BENCH
`ifndef EXTERNAL_BENCH_FINISH
   always @(posedge clk) begin
      if(halt) $finish();
   end
`endif

   reg [31:0] DE_instr; reg [31:0] DE_PC;
   reg [31:0] EM_instr; reg [31:0] EM_PC;
   reg [31:0] MW_instr; reg [31:0] MW_PC;

   localparam NOP = 32'b0000000_00000_00000_000_00000_0110011;

   always @(posedge clk) begin
      if(!resetn) begin
         DE_instr <= NOP;
         DE_PC <= 32'b0;
         EM_instr <= NOP;
         EM_PC <= 32'b0;
         MW_instr <= NOP;
         MW_PC <= 32'b0;
      end else begin
         if(!D_stall) begin
            DE_instr <= FD_nop ? NOP : FD_instr;
            DE_PC    <= FD_PC;
         end
         if(E_flush) begin
            DE_instr <= NOP;
         end
         if(!E_stall) begin
            EM_instr <= DE_instr;
            EM_PC    <= DE_PC;
         end
         if(M_flush) begin
            EM_instr <= NOP;
         end
         MW_instr <= EM_instr;
         MW_PC    <= EM_PC;
      end
   end

`ifdef CONFIG_DEBUG

   wire pi_trace_window =
      (F_PC  >= 32'h0000022c && F_PC  <= 32'h00000920) ||
      (FD_PC >= 32'h0000022c && FD_PC <= 32'h00000920) ||
      (DE_PC >= 32'h0000022c && DE_PC <= 32'h00000920) ||
      (EM_PC >= 32'h0000022c && EM_PC <= 32'h00000920) ||
      (MW_PC >= 32'h0000022c && MW_PC <= 32'h00000920);

   always @(posedge clk) begin
      if(resetn & !halt & pi_trace_window) begin

         $write("     ");
	 $write("[W] PC=%h ", MW_PC);
	 $write("     ");
	 riscv_disasm(MW_instr,MW_PC);
	 if(wbEnable) $write(
            "    x%0d <- 0x%0h (%0d)",
	    riscv_disasm_rdId(MW_instr),wbData,wbData
         );
	 $write("\n");

         $write("( %c) ",M_flush?"f":" ");
	 $write("[M] PC=%h ", EM_PC);
	 $write("     ");
	 riscv_disasm(EM_instr,EM_PC);
	 $write("\n");

         $write("(%c%c) ", E_stall ? "s" : " ", E_flush ? "f":" ");
	 $write("[E] PC=%h ", DE_PC);

	 // 调试输出里用 M/W 标记展示 rs1、rs2 分别来自哪一级旁路。
	 if(DE_nop) $write("[  ] ");
	 else $write("[%s%s] ",
	         riscv_disasm_readsRs1(DE_instr) ?
		     (E_M_fwd_rs1 ? "M" : E_W_fwd_rs1 ? "W" : " ") : " ",
		 riscv_disasm_readsRs2(DE_instr) ?
		     (E_M_fwd_rs2 ? "M" : E_W_fwd_rs2 ? "W" : " ") : " "
	 );
	 riscv_disasm(DE_instr,DE_PC);
	 if(DE_instr != NOP) begin
	    $write("  rs1=0x%h (%0d) rs2=0x%h (%0d) ",E_rs1,E_rs1,E_rs2,E_rs2);
`ifdef CONFIG_PC_PREDICT
	    if(riscv_disasm_isBranch(DE_instr)) begin
	       $write(" taken:%0d  %s",
		       E_takeBranch,
		      (E_takeBranch == DE_predictBranch) ?
		             "predict hit" : "predict miss"
               );
	    end
`endif
	 end
`ifdef CONFIG_RV32M
	 if(DE_isRV32M) $write(" %d%d ",EE_divBusy, EE_divFinished);
	 if(aluBusy) $write(" [div-busy]");
`endif
	 $write("\n");

         $write("(%c%c) ",D_stall ? "s":" ",D_flush ? "f":" ");
	 $write("[D] PC=%h ", FD_PC);
	 $write("[%s%s] ",
		dataHazard && rs1Hazard?"*":" ",
		dataHazard && rs2Hazard?"*":" ");
	 riscv_disasm(FD_nop ? NOP : FD_instr,FD_PC);
`ifdef CONFIG_PC_PREDICT
	 if(riscv_disasm_isBranch(FD_instr)) begin
	    $write(" predict taken:%0d",D_predictBranch);
	 end
`endif
	 $write("\n");

         $write("(%c ) ",F_stall ? "s":" ");
	 $write("[F] PC=%h ", F_PC);
`ifdef CONFIG_PC_PREDICT
	 if(D_predictPC) begin
	    $write(" PC <- [D] 0x%0h (prediction)",D_PCprediction);
	 end
`endif
	 if(EM_correctPC) begin
	    $write(" PC <- [E] 0x%0h (correction)",EM_PCcorrection);
	 end
         $write(" pending=%0d ppc=%h pep=%0d fb=%0d fbe=%0d epoch=%0d ready=%0d valid=%0d", F_pendingValid, F_pendingPC, F_pendingEpoch, FB_valid, FB_epoch, F_epoch, inst_ready, fetch_valid);
	 $write("\n");
         if(D_predictPC && !FD_nop) begin
            $display("PI_PRED dpc=%h instr=%h pred=%h branch=%0d ra=%h", FD_PC, FD_instr, D_PCprediction, D_predictBranch, D_predictRA);
         end
         if(DE_isJALR) begin
            $display("PI_JALR epc=%h instr=%h rs1=%h rf=%h pred=%h actual=%h mfw=%0d wfw=%0d",
                     DE_PC, DE_instr, E_rs1, RF_rs1_data, D_predictRA, E_JALRaddr, E_M_fwd_rs1, E_W_fwd_rs1);
         end

	 $display("");
      end
   end

`ifdef verilator
`ifdef CONFIG_VERILATOR_DBG

   // 需要设断点时，直接把 breakpoint 的表达式改成想观察的条件即可。
   wire breakpoint = 1'b0;

   reg step = 1'b0;
   reg [31:0] dbg_cmd = 0;

   initial begin
      $display("");
      $display("\"Debugger\" commands:");
      $display("--------------------");
      $display("g       : go");
      $display("<return>: step");
      $display("see \"debugger\" section in source for breakpoints");
      $display("");
   end

   always @(posedge clk) begin
      if(resetn & !halt) begin
	 if(step) begin
	    $write("DBG>");
	    dbg_cmd <= $c32("getchar()");
	    $write("\n");
	 end
	 if(dbg_cmd == "g") begin
	    step <= 1'b0;
	 end
	 if(breakpoint) begin
	 end
      end
   end
`endif
`endif

`endif

`ifdef CONFIG_EVTLOG
   always @(posedge clk) begin
      if(resetn && !halt) begin
         if(EM_correctPC) begin
            $display("EVT_BRANCH_CORRECT pc_from=%h pc_to=%h", F_PC, EM_PCcorrection);
         end
         if(D_flush || E_flush || M_flush) begin
            $display("EVT_PIPE_FLUSH D=%0d E=%0d M=%0d pc=%h", D_flush, E_flush, M_flush, F_PC);
         end
      end
   end
`endif

   // 仿真统计信息集中在这里维护，方便观察预测命中率、CPI 和指令构成。

   localparam BR_PROFILE_SLOTS = 256;
   localparam PHT_PROFILE_SLOTS = 1 << BP_BHT_ADDR_BITS;

   integer nbBranch = 0;
   integer nbBranchHit = 0;
   integer nbBranchBackward = 0;
   integer nbBranchBackwardHit = 0;
   integer nbBranchForward = 0;
   integer nbBranchForwardHit = 0;
   integer nbJAL  = 0;
   integer nbJALR = 0;
   integer nbJALRhit = 0;
   integer nbLoad = 0;
   integer nbStore = 0;
   integer nbLoadHazard = 0;
   integer nbRV32M = 0;
   integer nbMUL = 0;
   integer nbDIV = 0;
   integer nbPHTSharedEntries = 0;
   integer nbPHTUniquePcOver1 = 0;
   integer brProfTotal [0:BR_PROFILE_SLOTS-1];
   integer brProfHit [0:BR_PROFILE_SLOTS-1];
   reg [31:0] brProfPC [0:BR_PROFILE_SLOTS-1];
   reg brProfValid [0:BR_PROFILE_SLOTS-1];
   reg reportUsed [0:BR_PROFILE_SLOTS-1];
   integer phtUseCount [0:PHT_PROFILE_SLOTS-1];
   integer phtAliasCount [0:PHT_PROFILE_SLOTS-1];
   integer phtUniqueCount [0:PHT_PROFILE_SLOTS-1];
   reg [31:0] phtPC0 [0:PHT_PROFILE_SLOTS-1];
   reg [31:0] phtPC1 [0:PHT_PROFILE_SLOTS-1];
   reg [31:0] phtPC2 [0:PHT_PROFILE_SLOTS-1];
   reg [31:0] phtPC3 [0:PHT_PROFILE_SLOTS-1];
   integer stats_i;
   integer stats_slot;
   integer stats_free;
   integer stats_idx;
   integer stats_best;
   integer stats_metric;
   integer stats_unique;
   integer stats_alias;

   initial begin
      for(stats_i = 0; stats_i < BR_PROFILE_SLOTS; stats_i = stats_i + 1) begin
         brProfTotal[stats_i] = 0;
         brProfHit[stats_i] = 0;
         brProfPC[stats_i] = 32'b0;
         brProfValid[stats_i] = 1'b0;
         reportUsed[stats_i] = 1'b0;
      end
      for(stats_i = 0; stats_i < PHT_PROFILE_SLOTS; stats_i = stats_i + 1) begin
         phtUseCount[stats_i] = 0;
         phtAliasCount[stats_i] = 0;
         phtUniqueCount[stats_i] = 0;
         phtPC0[stats_i] = 32'b0;
         phtPC1[stats_i] = 32'b0;
         phtPC2[stats_i] = 32'b0;
         phtPC3[stats_i] = 32'b0;
      end
   end

   always @(posedge clk) begin
      if(resetn) begin
	 if(riscv_disasm_isBranch(DE_instr)) begin
	    nbBranch <= nbBranch + 1;
`ifdef CONFIG_PC_PREDICT
	    if(E_takeBranch == DE_predictBranch) begin
	       nbBranchHit <= nbBranchHit + 1;
	    end
`endif
	    if(DE_instr[31]) begin
	       nbBranchBackward <= nbBranchBackward + 1;
`ifdef CONFIG_PC_PREDICT
	       if(E_takeBranch == DE_predictBranch) begin
	          nbBranchBackwardHit <= nbBranchBackwardHit + 1;
	       end
`endif
	    end else begin
	       nbBranchForward <= nbBranchForward + 1;
`ifdef CONFIG_PC_PREDICT
	       if(E_takeBranch == DE_predictBranch) begin
	          nbBranchForwardHit <= nbBranchForwardHit + 1;
	       end
`endif
	    end

	    stats_slot = -1;
	    stats_free = -1;
	    for(stats_i = 0; stats_i < BR_PROFILE_SLOTS; stats_i = stats_i + 1) begin
	       if(brProfValid[stats_i] && brProfPC[stats_i] == DE_PC) begin
	          stats_slot = stats_i;
	       end
	       if(!brProfValid[stats_i] && stats_free == -1) begin
	          stats_free = stats_i;
	       end
	    end
	    if(stats_slot == -1) begin
	       stats_slot = stats_free;
	       if(stats_slot != -1) begin
	          brProfValid[stats_slot] <= 1'b1;
	          brProfPC[stats_slot] <= DE_PC;
	          brProfTotal[stats_slot] <= 1;
`ifdef CONFIG_PC_PREDICT
	          brProfHit[stats_slot] <= (E_takeBranch == DE_predictBranch) ? 1 : 0;
`else
	          brProfHit[stats_slot] <= 0;
`endif
	       end
	    end else begin
	       brProfTotal[stats_slot] <= brProfTotal[stats_slot] + 1;
`ifdef CONFIG_PC_PREDICT
	       if(E_takeBranch == DE_predictBranch) begin
	          brProfHit[stats_slot] <= brProfHit[stats_slot] + 1;
	       end
`endif
	    end

`ifdef CONFIG_GSHARE
	    stats_idx = {{(32-BP_BHT_ADDR_BITS){1'b0}}, DE_BHTindex};
	    stats_unique = phtUniqueCount[stats_idx];
	    stats_alias = phtAliasCount[stats_idx];
	    phtUseCount[stats_idx] <= phtUseCount[stats_idx] + 1;
	    if(stats_unique == 0) begin
	       phtPC0[stats_idx] <= DE_PC;
	       phtUniqueCount[stats_idx] <= 1;
	    end else if(
	         (phtPC0[stats_idx] == DE_PC) ||
	         (stats_unique > 1 && phtPC1[stats_idx] == DE_PC) ||
	         (stats_unique > 2 && phtPC2[stats_idx] == DE_PC) ||
	         (stats_unique > 3 && phtPC3[stats_idx] == DE_PC)
	    ) begin
	    end else begin
	       phtAliasCount[stats_idx] <= stats_alias + 1;
	       if(stats_unique == 1) begin
	          phtPC1[stats_idx] <= DE_PC;
	          phtUniqueCount[stats_idx] <= 2;
	          nbPHTSharedEntries <= nbPHTSharedEntries + 1;
	          nbPHTUniquePcOver1 <= nbPHTUniquePcOver1 + 1;
	       end else if(stats_unique == 2) begin
	          phtPC2[stats_idx] <= DE_PC;
	          phtUniqueCount[stats_idx] <= 3;
	          nbPHTUniquePcOver1 <= nbPHTUniquePcOver1 + 1;
	       end else if(stats_unique == 3) begin
	          phtPC3[stats_idx] <= DE_PC;
	          phtUniqueCount[stats_idx] <= 4;
	          nbPHTUniquePcOver1 <= nbPHTUniquePcOver1 + 1;
	       end else if(stats_unique < 5) begin
	          phtUniqueCount[stats_idx] <= 5;
	          nbPHTUniquePcOver1 <= nbPHTUniquePcOver1 + 1;
	       end
	    end
`endif
	 end

	 if(riscv_disasm_isJAL(DE_instr)) begin
	    nbJAL <= nbJAL + 1;
	 end
	 if(riscv_disasm_isJALR(DE_instr)) begin
	    nbJALR <= nbJALR + 1;
`ifdef CONFIG_RAS
	    if(DE_predictRA == E_JALRaddr) begin
	       nbJALRhit <= nbJALRhit + 1;
	    end
`endif
	 end
      end

      if(riscv_disasm_isLoad(MW_instr)) begin
	 nbLoad <= nbLoad + 1;
      end
      if(riscv_disasm_isStore(MW_instr)) begin
	 nbStore <= nbStore + 1;
      end
      if(riscv_disasm_isRV32M(MW_instr)) begin
	 if(MW_instr[14]) begin
	    nbDIV <= nbDIV + 1;
	 end else begin
	    nbMUL <= nbMUL + 1;
	 end
      end
      if(dataHazard) begin
	 nbLoadHazard <= nbLoadHazard + 1;
      end
   end

   /* verilator lint_off WIDTH */
   always @(posedge clk) begin
      if(halt) begin
`ifndef EXTERNAL_BENCH_FINISH
	 $display("Simulated processor's report");
	 $display("----------------------------");
	 $display("Branch hit = %3.3f\%%",
		   nbBranchHit*100.0/nbBranch	 );
	 $display("Branch bwd = %3.3f\%% (%0d/%0d)",
		   nbBranchBackwardHit*100.0/nbBranchBackward,
		   nbBranchBackwardHit, nbBranchBackward);
	 $display("Branch fwd = %3.3f\%% (%0d/%0d)",
		   nbBranchForwardHit*100.0/nbBranchForward,
		   nbBranchForwardHit, nbBranchForward);
	 $display("JALR   hit = %3.3f\%%",
		   nbJALRhit*100.0/nbJALR	 );
	 $display("Load hzrds = %3.3f\%%", nbLoadHazard*100.0/nbLoad);
	 $display("CPI        = %3.3f",(cycle*1.0)/(instret*1.0));
`ifdef CONFIG_GSHARE
	 $display("PHT shared entries = %0d", nbPHTSharedEntries);
	 $display("PHT extra PC refs  = %0d", nbPHTUniquePcOver1);
`endif
	 $write("Instr. mix = (");
	 $write("Branch:%3.3f\%%",    nbBranch*100.0/instret);
	 $write(" JAL:%3.3f\%%",       nbJAL*100.0/instret);
	 $write(" JALR:%3.3f\%%",      nbJALR*100.0/instret);
	 $write(" Load:%3.3f\%%",      nbLoad*100.0/instret);
	 $write(" Store:%3.3f\%%",     nbStore*100.0/instret);
`ifdef CONFIG_RV32M
	 $write(" MUL(HSU):%3.3f\%% ", nbMUL*100.0/instret);
	 $write(" DIV/REM:%3.3f\%% ",   nbDIV*100.0/instret);
`endif
	 $write(")\n");
	 for(stats_i = 0; stats_i < BR_PROFILE_SLOTS; stats_i = stats_i + 1) begin
	    reportUsed[stats_i] = 1'b0;
	 end
	 $display("Top hot branches");
	 for(stats_i = 0; stats_i < 10; stats_i = stats_i + 1) begin
	    stats_best = -1;
	    for(stats_slot = 0; stats_slot < BR_PROFILE_SLOTS; stats_slot = stats_slot + 1) begin
	       if(brProfValid[stats_slot] && !reportUsed[stats_slot]) begin
	          if(stats_best == -1 || brProfTotal[stats_slot] > brProfTotal[stats_best]) begin
	             stats_best = stats_slot;
	          end
	       end
	    end
	    if(stats_best != -1) begin
	       reportUsed[stats_best] = 1'b1;
	       $display("  pc=%h count=%0d hit=%3.3f\%%",
	                brProfPC[stats_best],
	                brProfTotal[stats_best],
	                brProfHit[stats_best]*100.0/brProfTotal[stats_best]);
	    end
	 end
	 for(stats_i = 0; stats_i < BR_PROFILE_SLOTS; stats_i = stats_i + 1) begin
	    reportUsed[stats_i] = 1'b0;
	 end
	 $display("Worst hot branches");
	 for(stats_i = 0; stats_i < 10; stats_i = stats_i + 1) begin
	    stats_best = -1;
	    for(stats_slot = 0; stats_slot < BR_PROFILE_SLOTS; stats_slot = stats_slot + 1) begin
	       if(brProfValid[stats_slot] && !reportUsed[stats_slot] && brProfTotal[stats_slot] >= 20) begin
	          if(stats_best == -1) begin
	             stats_best = stats_slot;
	          end else if((brProfHit[stats_slot]*1000)/brProfTotal[stats_slot] <
	                   (brProfHit[stats_best]*1000)/brProfTotal[stats_best]) begin
	             stats_best = stats_slot;
	          end else if((brProfHit[stats_slot]*1000)/brProfTotal[stats_slot] ==
	                   (brProfHit[stats_best]*1000)/brProfTotal[stats_best] &&
	                   brProfTotal[stats_slot] > brProfTotal[stats_best]) begin
	             stats_best = stats_slot;
	          end
	       end
	    end
	    if(stats_best != -1) begin
	       reportUsed[stats_best] = 1'b1;
	       $display("  pc=%h count=%0d hit=%3.3f\%%",
	                brProfPC[stats_best],
	                brProfTotal[stats_best],
	                brProfHit[stats_best]*100.0/brProfTotal[stats_best]);
	    end
	 end
`ifdef CONFIG_GSHARE
	 $display("Hot shared PHT entries");
	 for(stats_i = 0; stats_i < 10; stats_i = stats_i + 1) begin
	    stats_best = -1;
	    for(stats_slot = 0; stats_slot < PHT_PROFILE_SLOTS; stats_slot = stats_slot + 1) begin
	       if(phtUniqueCount[stats_slot] > 1) begin
	          if(stats_best == -1 || phtUseCount[stats_slot] > phtUseCount[stats_best]) begin
	             stats_best = stats_slot;
	          end
	       end
	    end
	    if(stats_best != -1 && phtUseCount[stats_best] > 0) begin
	       $display("  idx=%0d uses=%0d unique=%0d alias_new=%0d pc0=%h pc1=%h",
	                stats_best,
	                phtUseCount[stats_best],
	                phtUniqueCount[stats_best],
	                phtAliasCount[stats_best],
	                phtPC0[stats_best],
	                phtPC1[stats_best]);
	       phtUseCount[stats_best] = -1;
	    end
	 end
`endif
	 $finish();
`endif
      end
   end
   /* verilator lint_on WIDTH */

`endif

endmodule
