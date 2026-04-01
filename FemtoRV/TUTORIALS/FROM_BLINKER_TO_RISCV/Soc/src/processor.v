module processor (
    input         clk,
    input         resetn,
    
    // Instruction Memory Interface (1-cycle latency)
    output [31:0] inst_addr,
    input  [31:0] inst_rdata,
    output        inst_en, // Used to stop fetching if F_stall
    
    // Data Memory Interface (Dual-port: Port A for read, Port B for write)
    // Port A: Read
    output [31:0] data_raddr,
    input  [31:0] data_rdata,
    // Port B: Write
    output [31:0] data_waddr,
    output [31:0] data_wdata,
    output [3:0]  data_wmask,
    output        data_wen,

    // IO memory interface
    output [31:0] IO_mem_addr,  // IO memory address
    input [31:0]  IO_mem_rdata, // data read from IO memory
    output [31:0] IO_mem_wdata, // data written to IO memory
    output        IO_mem_wr     // IO write flag
);

`ifdef BENCH
`include "riscv_disassembly.v"
`endif

/******************************************************************************/

 /*
   Reminder for the 10 RISC-V codeops
   ----------------------------------
   5'b01100 | ALUreg  | rd <- rs1 OP rs2
   5'b00100 | ALUimm  | rd <- rs1 OP Iimm
   5'b11000 | Branch  | if(rs1 OP rs2) PC<-PC+Bimm
   5'b11001 | JALR    | rd <- PC+4; PC<-rs1+Iimm
   5'b11011 | JAL     | rd <- PC+4; PC<-PC+Jimm
   5'b00101 | AUIPC   | rd <- PC + Uimm
   5'b01101 | LUI     | rd <- Uimm
   5'b00000 | Load    | rd <- mem[rs1+Iimm]
   5'b01000 | Store   | mem[rs1+Simm] <- rs2
   5'b11100 | SYSTEM  | special
 */

/******************************************************************************/

`ifdef CONFIG_INITIALIZE
   // Iteration variable for the "initial" blocks
   integer i;
`endif

   // CSRs (cycle and retired instructions counters)
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

   // Pipeline control
   // Note: E_stall and M_flush are only used if RV32M is configured
   // (multicycle ALU).

   wire F_stall;
   wire D_stall;
   wire D_flush;
   wire E_stall;
   wire E_flush;
   wire M_flush;
   wire dataHazard;
   wire rs1Hazard;
   wire rs2Hazard;
   wire aluBusy; // Declare before use in hazard_unit
   wire E_correctPC; // Declare before use in hazard_unit
   wire E_takeBranch; // Declare before use in branch_predictor

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

   wire halt; // Halt execution (on ebreak)

/******************************************************************************/

                      /***  F: Instruction fetch ***/

   reg  [31:0] PC;

   // 外部指令存储器接口连线
   assign inst_addr = F_PC;
   assign inst_en   = resetn ? !F_stall : 1'b0;
   wire [31:0] PROGROM_rdata = inst_rdata;

`ifdef CONFIG_PC_PREDICT
   wire [31:0] F_PC =
	       D_predictPC  ? D_PCprediction  :
	       EM_correctPC ? EM_PCcorrection :
	                      PC;
`else
   wire [31:0] F_PC = EM_correctPC ? EM_PCcorrection :
	              PC;
`endif

   wire [31:0] F_PCplus4 = F_PC + 4;

   always @(posedge clk) begin

      if(!resetn) begin
         PC <= 0;
         FD_nop <= 1'b1;
      end else begin
         if(!F_stall) begin
            FD_PC    <= F_PC;
            PC       <= F_PCplus4;
         end
         FD_nop <= D_flush;
      end
   end

/******************************************************************************/
/******************************************************************************/
   reg [31:0] FD_PC;
   wire [31:0] FD_instr = PROGROM_rdata;
   reg        FD_nop; // Needed because I cannot directly write NOP to FD_instr
                      // because FD_instr is plugged to PROGROM's output port.
/******************************************************************************/
/******************************************************************************/

                     /*** D: Instruction decode ***/

   /** These three signals come from the Writeback stage **/
   wire        wbEnable;
   wire [31:0] wbData;
   wire [4:0]  wbRdId;

   // 路线A拆分：把译码与立即数拼接从主核心剥离到独立模块
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
   wire [11:0] D_BHTindex;

`ifdef CONFIG_PC_PREDICT
   branch_predictor #(
      .BHT_ADDR_BITS(12),
      .BHT_HISTO_BITS(9),
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
      .d_flush(D_flush),
      .fd_nop(FD_nop),
      .d_predict_branch(D_predictBranch),
      .d_predict_pc_valid(D_predictPC),
      .d_pc_prediction(D_PCprediction),
      .d_predict_ra(D_predictRA),
      .d_bht_index(D_BHTindex),
      .de_is_branch(DE_isBranch),
      .e_take_branch(E_takeBranch),
      .de_bht_index(DE_BHTindex),
      .e_stall(E_stall)
   );
`endif // `CONFIG_PC_PREDICT

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

	 // wbEnable = !isBranch & !isStore
	 // Note: EM_wbEnable = DE_wbEnable && (rdId != 0)
	 DE_wbEnable <= D_wbEnable_raw;

	 DE_IorSimm <= D_IorSimm;

`ifdef CONFIG_PC_PREDICT
     // Used in case of misprediction:
     //    PC+Bimm if predict not taken, PC+4 if predict taken
     DE_PCplus4orBimm <= FD_PC + (D_predictBranch ? 4 : D_Bimm);
     DE_predictBranch <= D_predictBranch;
     DE_predictRA     <= D_predictRA;
 `ifdef CONFIG_GSHARE
     DE_BHTindex  <= D_BHTindex;
 `endif
`else
	 DE_PCplusBorJimm <= FD_PC + (D_isJAL ? D_Jimm : D_Bimm);
`endif

	 // Code below is equivalent to:
	 // DE_PCplus4orUimm =
	 //    ((isLUI ? 0 : FD_PC)) + ((isJAL | isJALR) ? 4 : Uimm)
	 // (knowing that isLUI | isAUIPC | isJAL | isJALR)
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

/******************************************************************************/
/******************************************************************************/
   reg        DE_nop; // Needed by instret in W stage
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

   reg DE_wbEnable; // !isBranch && !isStore && rdId != 0

   reg DE_isJALorJALRorLUIorAUIPC;

`ifdef CONFIG_PC_PREDICT
   reg [31:0] DE_PCplus4orBimm;
   reg DE_predictBranch;
   reg [31:0] DE_predictRA;
 `ifdef CONFIG_GSHARE
   reg [11:0] DE_BHTindex;
 `endif
`else
   reg [31:0] DE_PCplusBorJimm;
`endif

   reg [31:0] DE_PCplus4orUimm;

/******************************************************************************/
/******************************************************************************/
                     /*** E: Execute ***/

   /*********** Registrer forwarding ************************************/

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

   /*********** the ALU *************************************************/

   wire [31:0] E_aluIn1 = E_rs1;
   wire [31:0] E_aluIn2 = (DE_isALUreg | DE_isBranch) ? E_rs2 : DE_IorSimm;
   wire [4:0]  E_shamt  = DE_isALUreg ? E_rs2[4:0] : DE_rs2Id;

   wire E_minus = DE_funct7[5] & DE_isALUreg;
   wire E_arith_shift = DE_funct7[5];

   // The adder is used by both arithmetic instructions and JALR.
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

   /*********** Branch, JAL, JALR ***********************************/

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

   /**************************************************************/

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

/******************************************************************************/
/******************************************************************************/
   reg        EM_nop; // Needed by instret in W stage
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

/******************************************************************************/
/******************************************************************************/

                     /*** M: Memory ***/

   // 路线A拆分：Load/Store 的对齐和字节掩码交给独立模块
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
   assign IO_mem_wr    = resetn ? (EM_isStore && M_isIO) : 1'b0; // && M_STORE_wmask[0];
   assign IO_mem_wdata = EM_rs2;

   wire [3:0] M_wmask = {4{EM_isStore & M_isRAM}} & M_STORE_wmask;
   wire [13:0] M_word_addr = EM_addr[15:2];

   // 外部数据存储器接口连线
   assign data_raddr = E_addr;
   // ren is !E_stall implicitly, but we can just use !E_stall or let memory read always.
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

/******************************************************************************/
/******************************************************************************/
   reg        MW_nop; // Needed by instret in W stage
   reg [4:0]  MW_rdId;
   reg [31:0] MW_wbData;
   reg 	      MW_wbEnable;
/******************************************************************************/
/******************************************************************************/

                     /*** W: WriteBack ***/

   assign wbData   = MW_wbData;
   assign wbEnable = MW_wbEnable;
   assign wbRdId   = MW_rdId;

/******************************************************************************/

   // we do not test rdId == 0 because in general, one loads data to
   // a register, not to zero !

   // Hazard unit controls F_stall, D_stall, E_stall, D_flush, E_flush, M_flush, and dataHazard.
   // Note: E_stall and M_flush are only used with the
   // multi-cycle ALU (RV32M)

/******************************************************************************/

`ifdef BENCH
   always @(posedge clk) begin
      if(halt) $finish();
   end

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

   always @(posedge clk) begin
      if(resetn & !halt) begin

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

	 // Register forwarding
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
	 $write("\n");

	 $display("");
      end
   end

/* "debugger" */

`ifdef verilator

   // wire breakpoint = 1'b0; // no breakpoint
   // wire breakpoint = (EM_addr == 32'h400004); // break on LEDs output
   // wire breakpoint = (EM_addr == 32'h400008); // break on character output
   // wire breakpoint = (DE_PC   == 32'h000000); // break on address reached
   // wire breakpoint = DE_isRV32M && DE_isALUreg;
   wire breakpoint = 1'b0;

   reg step = 1'b1;
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
	    step <= 1'b1;
	 end
      end
   end
`endif

`endif // `CONFIG_DEBUG

   /*************** statistics *************/

   integer nbBranch = 0;
   integer nbBranchHit = 0;
   integer nbJAL  = 0;
   integer nbJALR = 0;
   integer nbJALRhit = 0;
   integer nbLoad = 0;
   integer nbStore = 0;
   integer nbLoadHazard = 0;
   integer nbRV32M = 0;
   integer nbMUL = 0;
   integer nbDIV = 0;

   always @(posedge clk) begin
      if(resetn & !D_stall) begin
	 if(riscv_disasm_isBranch(DE_instr)) begin
	    nbBranch <= nbBranch + 1;
`ifdef CONFIG_PC_PREDICT
	    if(E_takeBranch == DE_predictBranch) begin
	       nbBranchHit <= nbBranchHit + 1;
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
	 $display("Simulated processor's report");
	 $display("----------------------------");
	 $display("Branch hit = %3.3f\%%",
		   nbBranchHit*100.0/nbBranch	 );
	 $display("JALR   hit = %3.3f\%%",
		   nbJALRhit*100.0/nbJALR	 );
	 $display("Load hzrds = %3.3f\%%", nbLoadHazard*100.0/nbLoad);
	 $display("CPI        = %3.3f",(cycle*1.0)/(instret*1.0));
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
	 $finish();
      end
   end
   /* verilator lint_on WIDTH */

`endif // `BENCH

/******************************************************************************/

endmodule
