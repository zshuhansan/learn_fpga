/**
 * Step 17: Creating a RISC-V processor
 * Memory-mapped IO
 */

/*
 * 本步的目标：把“LED/UART 这种外设”从“直接连到寄存器”升级为“内存映射 IO（MMIO）”。
 *
 * 你将看到一个最小可用的 SoC 结构：
 * - Processor：RV32I（到 step16 为止的那套指令），通过一组 mem_* 信号访问“统一的地址空间”
 * - Memory：片上 BRAM（这里建模为 MEM[] 数组），同时支持读与带掩码写（按 byte 写）
 * - SOC：地址译码与外设（LED、UART TX），把地址空间分成 RAM 区和 IO 区
 *
 * 内存映射规则（本文件采用最省 LUT 的版本之一）：
 * - 当 mem_addr[22] == 0：访问 RAM
 * - 当 mem_addr[22] == 1：访问 IO（外设）
 *
 * IO 区内部的“地址选择”采用 1-hot（按 word 地址）：
 * - 只看 mem_wordaddr = mem_addr[31:2]（忽略低 2 位，因为按 word 对齐）
 * - 若 mem_wordaddr[n] == 1，则命中第 n 个 IO 设备/寄存器
 *   这种方式避免了大位宽比较器（在小 FPGA 上更省 LUT），代价是每个 IO 寄存器占用一个 bit 位。
 *
 * 运行效果：
 * - 先在 LED 上从 0 数到 15，每次加延时
 * - 再通过 UART 发送 'a'..'z'，并发出 CRLF
 * - 之后回到开头无限循环
 */

`default_nettype none
`include "clockworks.v"
`include "emitter_uart.v"

module Memory (
   input             clk,
   input      [31:0] mem_addr,  // address to be read
   output reg [31:0] mem_rdata, // data read from memory
   input   	     mem_rstrb, // goes high when processor wants to read
   input      [31:0] mem_wdata, // data to be written
   input      [3:0]  mem_wmask	// masks for writing the 4 bytes (1=write byte)
);

   /*
    * 这里的 Memory 是“RAM + 固件 ROM”的合体：同一个 MEM[] 既存放指令也存放数据。
    *
    * - mem_addr：byte 地址（CPU 视角）
    * - word_addr：word 地址（MEM[] 下标），因此使用 mem_addr[31:2]
    * - mem_rstrb：读使能，拉高时在下一个 clk 上输出 mem_rdata
    * - mem_wmask：按 byte 写掩码
    *   mem_wmask[0] -> 写低 8 位（byte0）
    *   mem_wmask[1] -> 写 [15:8]（byte1）
    *   mem_wmask[2] -> 写 [23:16]（byte2）
    *   mem_wmask[3] -> 写高 8 位（byte3）
    *
    * 这类“按 byte 写”的写法对综合工具非常友好：
    * - 在 FPGA 上通常会被映射到支持 byte-enable 的 BRAM primitive
    * - 在仿真上则直接表现为对数组某些 bit slice 的赋值
    */
   reg [31:0] MEM [0:1535]; // 1536 4-bytes words = 6 Kb of RAM in total

`ifdef BENCH
   localparam slow_bit=12;
`else
   localparam slow_bit=17;
`endif

   /*
    * IO 设备编号（用于 IO 页的 1-hot 选择，见 SOC 里的地址译码）：
    * - IO_LEDS_bit：写这个设备会更新 LEDS
    * - IO_UART_DAT_bit：写这个设备会向 UART 发送一个字节（仅 TX）
    * - IO_UART_CNTL_bit：读这个设备得到 UART 状态（bit9=busy）
    */
   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   localparam IO_LEDS_bit      = 0;  // W five leds
   localparam IO_UART_DAT_bit  = 1;  // W data to send (8 bits)
   localparam IO_UART_CNTL_bit = 2;  // R status. bit 9: busy sending

   /*
    * IO_BIT_TO_OFFSET(bitid)：
    * 把“第 bitid 个 IO 设备”转换成“IO 页内偏移（byte offset）”。
    *
    * 因为 IO 的 1-hot 译码是按 word 地址做的（mem_wordaddr = mem_addr[31:2]），
    * 所以如果我们希望 mem_wordaddr[bitid] == 1，则需要 mem_addr[31:2] 的第 bitid 位为 1，
    * 等价于 mem_addr 的第 (bitid + 2) 位为 1（因为整体左移 2 位从 word->byte）。
    *
    * 因此 offset = 1 << (bitid + 2)。
    *
    * 例子：
    * - IO_LEDS_bit=0 -> offset=1<<(0+2)=4（写 gp+4 会命中 LED 寄存器）
    * - IO_UART_DAT_bit=1 -> offset=8（写 gp+8 会发送字节）
    * - IO_UART_CNTL_bit=2 -> offset=16（读 gp+16 得到 busy 状态）
    */
   // Converts an IO_xxx_bit constant into an offset in IO page.
   function [31:0] IO_BIT_TO_OFFSET;
      input [31:0] bitid;
      begin
	 IO_BIT_TO_OFFSET = 1 << (bitid + 2);
      end
   endfunction

`include "riscv_assembly.v"
   integer    L0_      = 12;
   integer    L1_      = 20;
   integer    L2_      = 52;
   integer    wait_    = 104;
   integer    wait_L0_ = 112;
   integer    putc_    = 124;
   integer    putc_L0_ = 132;

   initial begin
      /*
       * 这段 initial 是“固件”，由 riscv_assembly.v 生成机器码并写入 MEM[]。
       *
       * 约定：
       * - sp（x2）初始化到 RAM 末尾 0x1800（6kB），为将来的栈使用做准备
       * - gp（x3）初始化到 IO 页基址 0x400000（即 1<<22），访问外设时用 gp+offset
       *
       * 程序结构：
       * L0_：外层无限循环
       *   1) LED 计数 0..15（每次等待一段时间）
       *   2) UART 发送 a..z，然后 CRLF
       * wait_：延时函数（忙等）
       * putc_：发一个字符到 UART（轮询 busy 位直到可发）
       *
       * 注意：这里的“函数调用/返回”依赖 ABI 的基本规则：
       * - CALL 会把返回地址写入 ra（x1）
       * - RET 展开为 JALR(x0,x1,0)，跳回 ra
       * 但此阶段没有用栈保存 ra，因此这些函数不能再调用其它函数（否则会覆盖 ra）。
       */
      LI(sp,32'h1800);   // End of RAM, 6kB
      LI(gp,32'h400000); // IO page

   Label(L0_);

      // Count from 0 to 15 on the LEDs
      LI(s0,16); // upper bound of loop
      LI(a0,0);
   Label(L1_);
      /*
       * SW(a0, gp, IO_BIT_TO_OFFSET(IO_LEDS_bit))
       * 含义：把寄存器 a0 的值写到 IO 页里的 LED 寄存器。
       * 这里的“写”并不是写入 RAM，而是 SOC 根据地址把写请求路由给 LED 外设。
       */
      SW(a0,gp,IO_BIT_TO_OFFSET(IO_LEDS_bit));
      CALL(LabelRef(wait_));
      ADDI(a0,a0,1);
      BNE(a0,s0,LabelRef(L1_));

      // Send abcdef...xyz to the UART
      LI(s0,26); // upper bound of loop
      LI(a0,"a");
      LI(s1,0);
   Label(L2_);
      /*
       * putc_ 会把 a0 的低 8 位发送到 UART。
       * 这里循环 26 次，从 'a' 到 'z'。
       */
      CALL(LabelRef(putc_));
      ADDI(a0,a0,1);
      ADDI(s1,s1,1);
      BNE(s1,s0,LabelRef(L2_));

      // CR;LF
      LI(a0,13);
      CALL(LabelRef(putc_));
      LI(a0,10);
      CALL(LabelRef(putc_));

      J(LabelRef(L0_));

      EBREAK(); // I systematically keep it before functions
                // in case I decide to remove the loop...

   Label(wait_);
      LI(t0,1);
      SLLI(t0,t0,slow_bit);
   Label(wait_L0_);
      ADDI(t0,t0,-1);
      BNEZ(t0,LabelRef(wait_L0_));
      RET();

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

      endASM();
   end

   wire [29:0] word_addr = mem_addr[31:2];

   always @(posedge clk) begin
      /*
       * 同步读：mem_rstrb 拉高时，在该时钟沿把 MEM[word_addr] 放到 mem_rdata
       *（相当于建模“BRAM 读延迟 1 个周期”）
       */
      if(mem_rstrb) begin
         mem_rdata <= MEM[word_addr];
      end
      /*
       * 同步写：mem_wmask 的每一位控制对应 byte lane 是否写入
       * 这让 SB/SH/SW 都能映射到同一个 32 位 word 写口。
       */
      if(mem_wmask[0]) MEM[word_addr][ 7:0 ] <= mem_wdata[ 7:0 ];
      if(mem_wmask[1]) MEM[word_addr][15:8 ] <= mem_wdata[15:8 ];
      if(mem_wmask[2]) MEM[word_addr][23:16] <= mem_wdata[23:16];
      if(mem_wmask[3]) MEM[word_addr][31:24] <= mem_wdata[31:24];
   end
endmodule


module Processor (
    input 	  clk,
    input 	  resetn,
    output [31:0] mem_addr,
    input [31:0]  mem_rdata,
    output 	  mem_rstrb,
    output [31:0] mem_wdata,
    output [3:0]  mem_wmask
);

   /*
    * Processor 对外暴露的是一个非常常见的“简化版总线接口”：
    * - mem_addr/mem_rdata/mem_rstrb：读端口（地址、读数据、读请求）
    * - mem_wdata/mem_wmask：写端口（写数据、按 byte 写掩码）
    *
    * SOC 会把这些请求路由到 RAM 或 IO（本步的重点）。
    */
   reg [31:0] PC=0;        // program counter
   reg [31:0] instr;       // current instruction

   // See the table P. 105 in RISC-V manual

   // The 10 RISC-V instructions
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

   // The 5 immediate formats
   wire [31:0] Uimm={    instr[31],   instr[30:12], {12{1'b0}}};
   wire [31:0] Iimm={{21{instr[31]}}, instr[30:20]};
   wire [31:0] Simm={{21{instr[31]}}, instr[30:25],instr[11:7]};
   wire [31:0] Bimm={{20{instr[31]}}, instr[7],instr[30:25],instr[11:8],1'b0};
   wire [31:0] Jimm={{12{instr[31]}}, instr[19:12],instr[20],instr[30:21],1'b0};

   // Source and destination registers
   wire [4:0] rs1Id = instr[19:15];
   wire [4:0] rs2Id = instr[24:20];
   wire [4:0] rdId  = instr[11:7];

   // function codes
   wire [2:0] funct3 = instr[14:12];
   wire [6:0] funct7 = instr[31:25];

   // The registers bank
   reg [31:0] RegisterBank [0:31];
   reg [31:0] rs1; // value of source
   reg [31:0] rs2; //  registers.
   wire [31:0] writeBackData; // data to be written to rd
   wire        writeBackEn;   // asserted if data should be written to rd

`ifdef BENCH
   integer     i;
   initial begin
      for(i=0; i<32; ++i) begin
	 RegisterBank[i] = 0;
      end
   end
`endif

   // The ALU
   wire [31:0] aluIn1 = rs1;
   wire [31:0] aluIn2 = isALUreg | isBranch ? rs2 : Iimm;

   wire [4:0] shamt = isALUreg ? rs2[4:0] : instr[24:20]; // shift amount

   // The adder is used by both arithmetic instructions and JALR.
   wire [31:0] aluPlus = aluIn1 + aluIn2;

   // Use a single 33 bits subtract to do subtraction and all comparisons
   // (trick borrowed from swapforth/J1)
   wire [32:0] aluMinus = {1'b1, ~aluIn2} + {1'b0,aluIn1} + 33'b1;
   wire        LT  = (aluIn1[31] ^ aluIn2[31]) ? aluIn1[31] : aluMinus[32];
   wire        LTU = aluMinus[32];
   wire        EQ  = (aluMinus[31:0] == 0);

   // Flip a 32 bit word. Used by the shifter (a single shifter for
   // left and right shifts, saves silicium !)
   function [31:0] flip32;
      input [31:0] x;
      flip32 = {x[ 0], x[ 1], x[ 2], x[ 3], x[ 4], x[ 5], x[ 6], x[ 7],
		x[ 8], x[ 9], x[10], x[11], x[12], x[13], x[14], x[15],
		x[16], x[17], x[18], x[19], x[20], x[21], x[22], x[23],
		x[24], x[25], x[26], x[27], x[28], x[29], x[30], x[31]};
   endfunction

   wire [31:0] shifter_in = (funct3 == 3'b001) ? flip32(aluIn1) : aluIn1;

   /* verilator lint_off WIDTH */
   wire [31:0] shifter =
               $signed({instr[30] & aluIn1[31], shifter_in}) >>> aluIn2[4:0];
   /* verilator lint_on WIDTH */

   wire [31:0] leftshift = flip32(shifter);



   // ADD/SUB/ADDI:
   // funct7[5] is 1 for SUB and 0 for ADD. We need also to test instr[5]
   // to make the difference with ADDI
   //
   // SRLI/SRAI/SRL/SRA:
   // funct7[5] is 1 for arithmetic shift (SRA/SRAI) and
   // 0 for logical shift (SRL/SRLI)
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

   // The predicate for branch instructions
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


   // Address computation
   // An adder used to compute branch address, JAL address and AUIPC.
   // branch->PC+Bimm    AUIPC->PC+Uimm    JAL->PC+Jimm
   // Equivalent to PCplusImm = PC + (isJAL ? Jimm : isAUIPC ? Uimm : Bimm)
   wire [31:0] PCplusImm = PC + ( instr[3] ? Jimm[31:0] :
				  instr[4] ? Uimm[31:0] :
				             Bimm[31:0] );
   wire [31:0] PCplus4 = PC+4;

   // register write back
   assign writeBackData = (isJAL || isJALR) ? PCplus4   :
			      isLUI         ? Uimm      :
			      isAUIPC       ? PCplusImm :
			      isLoad        ? LOAD_data :
			                      aluOut;

   /*
    * nextPC 选择规则：
    * - 分支且满足条件：PC + Bimm
    * - JAL：PC + Jimm
    * - JALR：rs1 + Iimm，并把 bit0 清 0（RISC-V 规范要求，保证至少 2 字节对齐）
    * - 默认顺序执行：PC + 4
    */
   wire [31:0] nextPC = ((isBranch && takeBranch) || isJAL) ? PCplusImm   :
	                                  isJALR   ? {aluPlus[31:1],1'b0} :
	                                             PCplus4;

   wire [31:0] loadstore_addr = rs1 + (isStore ? Simm : Iimm);

   // Load
   // All memory accesses are aligned on 32 bits boundary. For this
   // reason, we need some circuitry that does unaligned halfword
   // and byte load/store, based on:
   // - funct3[1:0]:  00->byte 01->halfword 10->word
   // - mem_addr[1:0]: indicates which byte/halfword is accessed

   wire mem_byteAccess     = funct3[1:0] == 2'b00;
   wire mem_halfwordAccess = funct3[1:0] == 2'b01;


   wire [15:0] LOAD_halfword =
	       loadstore_addr[1] ? mem_rdata[31:16] : mem_rdata[15:0];

   wire  [7:0] LOAD_byte =
	       loadstore_addr[0] ? LOAD_halfword[15:8] : LOAD_halfword[7:0];

   // LOAD, in addition to funct3[1:0], LOAD depends on:
   // - funct3[2] (instr[14]): 0->do sign expansion   1->no sign expansion
   wire LOAD_sign =
	!funct3[2] & (mem_byteAccess ? LOAD_byte[7] : LOAD_halfword[15]);

   wire [31:0] LOAD_data =
         mem_byteAccess ? {{24{LOAD_sign}},     LOAD_byte} :
     mem_halfwordAccess ? {{16{LOAD_sign}}, LOAD_halfword} :
                          mem_rdata ;

   // Store
   // ------------------------------------------------------------------------

   assign mem_wdata[ 7: 0] = rs2[7:0];
   assign mem_wdata[15: 8] = loadstore_addr[0] ? rs2[7:0]  : rs2[15: 8];
   assign mem_wdata[23:16] = loadstore_addr[1] ? rs2[7:0]  : rs2[23:16];
   assign mem_wdata[31:24] = loadstore_addr[0] ? rs2[7:0]  :
			     loadstore_addr[1] ? rs2[15:8] : rs2[31:24];

   // The memory write mask:
   //    1111                     if writing a word
   //    0011 or 1100             if writing a halfword
   //                                (depending on loadstore_addr[1])
   //    0001, 0010, 0100 or 1000 if writing a byte
   //                                (depending on loadstore_addr[1:0])

   wire [3:0] STORE_wmask =
	      mem_byteAccess      ?
	            (loadstore_addr[1] ?
		          (loadstore_addr[0] ? 4'b1000 : 4'b0100) :
		          (loadstore_addr[0] ? 4'b0010 : 4'b0001)
                    ) :
	      mem_halfwordAccess ?
	            (loadstore_addr[1] ? 4'b1100 : 4'b0011) :
              4'b1111;

   // The state machine
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam FETCH_REGS  = 2;
   localparam EXECUTE     = 3;
   localparam LOAD        = 4;
   localparam WAIT_DATA   = 5;
   localparam STORE       = 6;
   reg [2:0] state = FETCH_INSTR;

   always @(posedge clk) begin
      if(!resetn) begin
	 PC    <= 0;
	 state <= FETCH_INSTR;
      end else begin
	 if(writeBackEn && rdId != 0) begin
	    RegisterBank[rdId] <= writeBackData;
	    // $display("r%0d <= %b (%d) (%d)",rdId,writeBackData,writeBackData,$signed(writeBackData));
	    // For displaying what happens.
	 end
	 case(state)
	   FETCH_INSTR: begin
	      /*
	       * 发起“取指读请求”。注意：本处理器的 Memory 是同步读，
	       * 因此真正的 instr 会在下一拍（WAIT_INSTR）才能拿到 mem_rdata。
	       */
	      state <= WAIT_INSTR;
	   end
	   WAIT_INSTR: begin
	      /*
	       * 取到指令 word（mem_rdata），锁存到 instr。
	       * 下一拍去读寄存器堆（BRAM 读延迟 1 拍）。
	       */
	      instr <= mem_rdata;
	      state <= FETCH_REGS;
	   end
	   FETCH_REGS: begin
	      /*
	       * 根据 instr 中的 rs1Id/rs2Id 读寄存器堆。
	       * 下一拍进入 EXECUTE 执行/计算/可能发起 load/store。
	       */
	      rs1 <= RegisterBank[rs1Id];
	      rs2 <= RegisterBank[rs2Id];
	      state <= EXECUTE;
	   end
	   EXECUTE: begin
	      /*
	       * 执行阶段：
	       * - 计算 ALU、分支条件、nextPC
	       * - 若不是 SYSTEM 指令，则更新 PC
	       * - 若是 Load：进入 LOAD/WAIT_DATA 等待内存读回
	       * - 若是 Store：进入 STORE 发起写（本设计里 store 写在 STORE 状态生效）
	       */
	      if(!isSYSTEM) begin
		 PC <= nextPC;
	      end
	      state <= isLoad  ? LOAD  :
		       isStore ? STORE :
		       FETCH_INSTR;
`ifdef BENCH
	      if(isSYSTEM) $finish();
`endif
	   end
	   LOAD: begin
	      /*
	       * 发起一次数据读请求（mem_rstrb 在该状态为 1），
	       * 下一拍（WAIT_DATA）取回 mem_rdata 并写回寄存器。
	       */
	      state <= WAIT_DATA;
	   end
	   WAIT_DATA: begin
	      /*
	       * LOAD 的数据已经在 mem_rdata 上，writeBackEn 会在本状态生效，
	       * 把 LOAD_data 写回 rd。写回完成后回到取指。
	       */
	      state <= FETCH_INSTR;
	   end
	   STORE: begin
	      /*
	       * STORE：本状态 mem_wmask 变为非 0，从而让 Memory 在该拍写入数据。
	       * 写完后回到取指。
	       */
	      state <= FETCH_INSTR;
	   end
	 endcase
      end
   end

   /*
    * 写回使能：
    * - 在 EXECUTE：大多数“产生 rd”的指令都在这里写回
    *   但分支（isBranch）不写回；store 不写回；load 的写回推迟到 WAIT_DATA
    * - 在 WAIT_DATA：load 把 LOAD_data 写回
    */
   assign writeBackEn = (state==EXECUTE && !isBranch && !isStore) ||
			(state==WAIT_DATA) ;

   /*
    * 与 Memory/SOC 的接口时序：
    * - mem_addr：
    *   * 取指时用 PC
    *   * 执行 load/store 时用 loadstore_addr
    * - mem_rstrb：
    *   * 取指阶段拉高一次
    *   * load 阶段再拉高一次读取数据
    * - mem_wmask：
    *   * 只有在 STORE 状态为非零（并且按 STORE_wmask 选择 byte lane）
    */
   assign mem_addr = (state == WAIT_INSTR || state == FETCH_INSTR) ?
		     PC : loadstore_addr ;
   assign mem_rstrb = (state == FETCH_INSTR || state == LOAD);
   assign mem_wmask = {4{(state == STORE)}} & STORE_wmask;

endmodule


module SOC (
    input 	     CLK, // system clock
    input 	     RESET, // reset button
    output reg [4:0] LEDS, // system LEDs
    input 	     RXD, // UART receive
    output 	     TXD         // UART transmit
);

   /*
    * SOC 负责：
    * - 实例化 CPU 与 RAM
    * - 把 CPU 的统一地址空间请求译码为：
    *   * isRAM：访问片上 BRAM（Memory 模块）
    *   * isIO ：访问外设寄存器（LED、UART）
    * - 外设的访问方式：IO 页 + 1-hot word 地址
    */
   wire clk;
   wire resetn;

   wire [31:0] mem_addr;
   wire [31:0] mem_rdata;
   wire mem_rstrb;
   wire [31:0] mem_wdata;
   wire [3:0]  mem_wmask;

   Processor CPU(
      .clk(clk),
      .resetn(resetn),
      .mem_addr(mem_addr),
      .mem_rdata(mem_rdata),
      .mem_rstrb(mem_rstrb),
      .mem_wdata(mem_wdata),
      .mem_wmask(mem_wmask)
   );

   wire [31:0] RAM_rdata;
   wire [29:0] mem_wordaddr = mem_addr[31:2];
   wire isIO  = mem_addr[22];
   wire isRAM = !isIO;
   wire mem_wstrb = |mem_wmask;

   Memory RAM(
      .clk(clk),
      .mem_addr(mem_addr),
      .mem_rdata(RAM_rdata),
      .mem_rstrb(isRAM & mem_rstrb),
      .mem_wdata(mem_wdata),
      .mem_wmask({4{isRAM}}&mem_wmask)
   );


   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   localparam IO_LEDS_bit      = 0;  // W five leds
   localparam IO_UART_DAT_bit  = 1;  // W data to send (8 bits)
   localparam IO_UART_CNTL_bit = 2;  // R status. bit 9: busy sending

   always @(posedge clk) begin
      /*
       * LED 外设：当命中 IO 页且发生写（mem_wstrb=1）且 1-hot 命中 LED bit 时，
       * 把写入数据低 5 位显示到 LEDS。
       */
      if(isIO & mem_wstrb & mem_wordaddr[IO_LEDS_bit]) begin
	 LEDS <= mem_wdata;
      end
   end

   /*
    * UART 外设（只实现发送 TX）：
    * - 写 IO_UART_DAT_bit：触发发送（uart_valid）
    * - 读 IO_UART_CNTL_bit：返回 busy 状态（bit9）
    *
    * CPU 侧的软件通过轮询 busy 位来保证不会在 UART 忙时重复写入。
    */
   wire uart_valid = isIO & mem_wstrb & mem_wordaddr[IO_UART_DAT_bit];
   wire uart_ready;

   corescore_emitter_uart #(
      .clk_freq_hz(`CPU_FREQ*1000000)
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

   /*
    * mem_rdata 复用：
    * - 访问 RAM：返回 RAM_rdata
    * - 访问 IO ：返回 IO_rdata（否则 0）
    */
   assign mem_rdata = isRAM ? RAM_rdata :
	                      IO_rdata ;


`ifdef BENCH
   always @(posedge clk) begin
      /*
       * 仿真便利功能：
       * 当软件写 UART 数据寄存器时，不走真实串口时序，而是直接把字符打印到仿真控制台。
       * 这样可以在不搭建 UART RX/TX 波形解码器的情况下，看到“程序输出了什么”。
       */
      if(uart_valid) begin
	 $write("%c", mem_wdata[7:0] );
	 $fflush(32'h8000_0001);
      end
   end
`endif

   // Gearbox and reset circuitry.
   Clockworks CW(
     .CLK(CLK),
     .RESET(RESET),
     .clk(clk),
     .resetn(resetn)
   );

endmodule
