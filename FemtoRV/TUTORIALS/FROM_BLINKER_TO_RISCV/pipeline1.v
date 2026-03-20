/**
 * pipeline1.v
 * Let us see how to morph our multi-cycle CPU into a pipelined CPU !
 * Step 1: separate program memory and data memory, both included in CPU
 *   (not a pipelined processor yet)
 */

/*
 * 中文导读（本文件做什么）
 *
 * pipeline1 是“流水线篇”的第一个过渡版本：它还不是流水线 CPU，
 * 但它把第一篇末尾的“统一内存（指令与数据共用一条总线）”拆成两块内部存储：
 *
 * - PROGROM：程序只读存储（指令），用 $readmemh("PROGROM.hex") 初始化
 * - DATARAM：数据读写存储（变量/栈等），用 $readmemh("DATARAM.hex") 初始化
 *
 * CPU 内部仍然是 3~4 周期多周期执行（状态机）：取指 -> 读寄存器 -> 执行 -> (可选)等待 load 数据
 *
 * 另外保留一个“外部 IO 总线”用于 UART/LED 等外设（与第一篇 step17 兼容）：
 * - 地址空间用 mem_addr[22] 区分 RAM 与 IO 页：
 *   mem_addr[22]=0 -> DATARAM（内部 RAM）
 *   mem_addr[22]=1 -> IO 页（外设寄存器，由 SOC 译码）
 *
 * 这一步的意义：
 * - 为后续真正流水线（取指/译码/执行/访存/写回并行）准备“指令存储/数据存储分离”的结构
 * - 让访存延迟变成固定 1 拍（内部 BRAM），为后续 hazard/forwarding 更好建模
 */

`default_nettype none
`include "clockworks.v"
`include "emitter_uart.v"

/******************************************************************************/

module Processor (
    input 	  clk,
    input 	  resetn,
    output [31:0] IO_mem_addr,  // IO memory address
    input [31:0]  IO_mem_rdata, // data read from IO memory
    output [31:0] IO_mem_wdata, // data written to IO memory
    output 	  IO_mem_wr     // IO write flag
);

   /*
    * 处理器内部存储：
    * - PROGROM：指令 ROM（只读），按 word 访问（PC[15:2]）
    * - DATARAM：数据 RAM（读写），按 word 访问（mem_addr[15:2]）
    *
    * 注意：这里把 ROM/RAM 都放在 Processor 内部，便于先做出“分离 I/D 存储”的最小改动。
    * 后续版本会把它们抽象成更通用的接口（甚至替换成 cache）。
    */

   reg [31:0] PROGROM [0:16383];
   reg [31:0] DATARAM [0:16383];

   initial begin
      $readmemh("PROGROM.hex",PROGROM);
      $readmemh("DATARAM.hex",DATARAM);
   end

   // Internal memory busses, used by Load and Store
   // (used for both DATARAM and IO).
   wire [31:0] mem_addr;
   wire [31:0] mem_rdata;
   wire [31:0] mem_wdata;
   wire [3:0]  mem_wmask;

   // bit 22 of memory address is set for IO page (and zero for RAM)
   wire isIO  = mem_addr[22];
   wire isRAM = !isIO;

   wire [13:0] mem_word_addr = mem_addr[15:2];

   // RAM access
   reg [31:0] dataram_rdata;
   wire [3:0] dataram_wmask = mem_wmask & {4{isRAM}};
   always @(posedge clk) begin
      dataram_rdata <= DATARAM[mem_word_addr];
      if(dataram_wmask[0]) DATARAM[mem_word_addr][ 7:0 ] <= mem_wdata[ 7:0 ];
      if(dataram_wmask[1]) DATARAM[mem_word_addr][15:8 ] <= mem_wdata[15:8 ];
      if(dataram_wmask[2]) DATARAM[mem_word_addr][23:16] <= mem_wdata[23:16];
      if(dataram_wmask[3]) DATARAM[mem_word_addr][31:24] <= mem_wdata[31:24];
   end

   assign mem_rdata = isRAM ? dataram_rdata : IO_mem_rdata;
   assign IO_mem_addr  = mem_addr;
   assign IO_mem_wdata = mem_wdata;
   assign IO_mem_wr    = isIO & mem_wmask[0];

   /*
    * PC / instr：
    * - PC 是字节地址（每条 32-bit 指令，PC 每次 +4）
    * - instr 保存当前指令字
    *
    * 这里把 opcode 判断写成 instr[6:2]，利用 RV32I 指令的低两位永远是 2'b11，
    * 从而省一点逻辑（把 7-bit opcode 压缩成 5-bit 判别）。
    */
   reg [31:0] PC=0;  // program counter
   reg [31:0] instr; // current instruction (ignore two LSBs, always 11)

   // See the table P. 105 in RISC-V manual

   // The 10 RISC-V instructions
   wire isALUreg  =  (instr[6:2] == 5'b01100); // rd <- rs1 OP rs2
   wire isALUimm  =  (instr[6:2] == 5'b00100); // rd <- rs1 OP Iimm
   wire isBranch  =  (instr[6:2] == 5'b11000); // if(rs1 OP rs2) PC<-PC+Bimm
   wire isJALR    =  (instr[6:2] == 5'b11001); // rd <- PC+4; PC<-rs1+Iimm
   wire isJAL     =  (instr[6:2] == 5'b11011); // rd <- PC+4; PC<-PC+Jimm
   wire isAUIPC   =  (instr[6:2] == 5'b00101); // rd <- PC + Uimm
   wire isLUI     =  (instr[6:2] == 5'b01101); // rd <- Uimm
   wire isLoad    =  (instr[6:2] == 5'b00000); // rd <- mem[rs1+Iimm]
   wire isStore   =  (instr[6:2] == 5'b01000); // mem[rs1+Simm] <- rs2
   wire isSYSTEM  =  (instr[6:2] == 5'b11100); // special

   // The 5 immediate formats
   wire [31:0] Uimm={    instr[31],   instr[30:12], {12{1'b0}}};
   wire [31:0] Iimm={{21{instr[31]}}, instr[30:20]};
   wire [31:0] Simm={{21{instr[31]}}, instr[30:25],instr[11:7]};
   wire [31:0] Bimm={{20{instr[31]}}, instr[7],instr[30:25],instr[11:8],1'b0};
   wire [31:0] Jimm={{12{instr[31]}}, instr[19:12],instr[20],instr[30:21],1'b0};

   // Destination registers
   wire [4:0] rdId  = instr[11:7];

   // function codes
   wire [2:0] funct3 = instr[14:12];
   wire [6:0] funct7 = instr[31:25];

   // SYSTEM: EBREAK
   wire isEBREAK     = isSYSTEM & (funct3 == 3'b000);

   /*
    * RegisterBank：
    * - 体系结构寄存器堆 x0..x31（注意 x0 写入应当被忽略）
    * - rs1/rs2 在 WAIT_INSTR 阶段从寄存器堆读出并锁存（建模“寄存器堆读延迟 1 拍”）
    * - writeBackData/writeBackEn 在 EXECUTE/WAIT_DATA 阶段控制写回
    *
    * cycle/instret：
    * - cycle：每拍+1，统计周期数
    * - instret：这里用“每取到一条指令”近似退休（retired）计数（用于粗略 CPI）
    */
   // The registers bank
   reg [31:0] RegisterBank [0:31];
   reg [31:0] rs1; // value of source
   reg [31:0] rs2; //  registers.
   wire [31:0] writeBackData; // data to be written to rd
   wire        writeBackEn;   // asserted if data should be written to rd

   reg [63:0] cycle;
   reg [63:0] instret;

   always @(posedge clk) begin
      cycle <= !resetn ? 0 : cycle + 1;
   end

`ifdef BENCH
   integer     i;
   initial begin
      cycle = 0;
      instret = 0;
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

   // Note: doing so with ADDR_WIDTH < 32, AUIPC may fail in
   // some RISC-V compliance tests because one can is supposed to use
   // it to generate arbitrary 32-bit values (and not only addresses).

   wire [31:0] PCplusImm = PC + ( instr[3] ? Jimm[31:0] :
					    instr[4] ? Uimm[31:0] :
				            Bimm[31:0] );
   wire [31:0] PCplus4 = PC+4;


   wire [31:0] nextPC =
               ((isBranch && takeBranch) || isJAL) ? PCplusImm            :
	                                    isJALR ? {aluPlus[31:1],1'b0} :
	                                             PCplus4;

   wire [31:0] loadstore_addr = rs1 + (isStore ? Simm : Iimm);

   assign writeBackData = (isJAL || isJALR) ? PCplus4   :
			      isLUI         ? Uimm      :
			      isAUIPC       ? PCplusImm :
			      isLoad        ? LOAD_data :
			                      aluOut;
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
   /*
    * 多周期状态机（仍非流水线）：
    * - FETCH_INSTR：从 PROGROM 读指令（同步读，下一拍可用）
    * - WAIT_INSTR：锁存 rs1/rs2，并让 instret++（近似每条指令一次）
    * - EXECUTE：执行 ALU/分支/跳转；若是 load 则进入 WAIT_DATA
    * - WAIT_DATA：load 数据已在 mem_rdata 上，写回后回到取指
    *
    * 说明：DATARAM 读延迟固定 1 拍，因此 load 只需额外等待 1 个状态。
    */
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam EXECUTE     = 2;
   localparam WAIT_DATA   = 3;
   reg [1:0] state = FETCH_INSTR;

   always @(posedge clk) begin
      if(!resetn) begin
	 PC      <= 32'h00000000;
	 state   <= WAIT_DATA;    // just wait for !mem_rbusy
	 instret <= 0;
      end else begin
	 if(writeBackEn && rdId != 0) begin
	    RegisterBank[rdId] <= writeBackData;
	 end
	 case(state)
	   FETCH_INSTR: begin
	      state <= WAIT_INSTR;
	      instr <= PROGROM[PC[15:2]];
`ifdef BENCH
	      if(PC >= 32'h10000) begin
		 $display("invalid PC, out of range: %h",PC);
		 $finish();
	      end
`endif
	   end
	   WAIT_INSTR: begin
	      rs1 <= RegisterBank[instr[19:15]];
	      rs2 <= RegisterBank[instr[24:20]];
	      state <= EXECUTE;
	      instret <= instret + 1;
	   end
	   EXECUTE: begin
	      if(!isEBREAK) begin
		 PC <= nextPC;
	      end
	      state <= isLoad  ? WAIT_DATA : FETCH_INSTR;
`ifdef BENCH
	      if(isLoad || isStore) begin
		 if(mem_addr <  32'h10000) begin
		    $display("invalid data addr: %h",mem_addr);
		    $finish();
		 end
	      end
	      if(isEBREAK) $finish();
`endif
	   end
	   WAIT_DATA: begin
	      state <= FETCH_INSTR;
	   end
	 endcase
      end
   end

   /*
    * 写回使能规则：
    * - EXECUTE：除 Branch/Store 外的大多数指令都在此阶段写回
    * - WAIT_DATA：Load 指令的数据在此阶段写回（LOAD_data 已从 mem_rdata 对齐/扩展）
    */
   assign writeBackEn = (state==EXECUTE && !isBranch && !isStore) ||
			(state==WAIT_DATA) ;
   /*
    * 对 DATARAM/IO 的访问：
    * - mem_addr：load/store 的字节地址（rs1 + imm）
    * - mem_wmask：只有在 EXECUTE 且 isStore 时为非 0（按字节 lane 写）
    * - mem_rdata：由 isRAM/isIO 在上面 mux 出来
    */
   assign mem_addr = loadstore_addr;
   assign mem_wmask = {4{(state == EXECUTE) & isStore}} & STORE_wmask;
endmodule


module SOC (
    input 	     CLK, // system clock
    input 	     RESET,// reset button
    output reg [4:0] LEDS, // system LEDs
    input 	     RXD, // UART receive
    output 	     TXD  // UART transmit
);

   /*
    * SOC 负责把“CPU 的 IO 总线”落到实际外设上。
    * 这里的 IO 地址译码采用 1-hot word 地址（与 step17 相同思想）：
    * - 只看 IO_mem_addr[15:2]，若某一位为 1 则命中对应外设寄存器
    *
    * 本文件只实现两个外设：
    * - LED：写 IO_LEDS_bit 对应地址，更新 LEDS[4:0]
    * - UART TX：写 IO_UART_DAT_bit 发送一个字节；读 IO_UART_CNTL_bit 返回 busy（bit9）
    */
   wire clk;
   wire resetn;

   wire [31:0] IO_mem_addr;
   wire [31:0] IO_mem_rdata;
   wire [31:0] IO_mem_wdata;
   wire        IO_mem_wr;

   Processor CPU(
      .clk(clk),
      .resetn(resetn),
      .IO_mem_addr(IO_mem_addr),
      .IO_mem_rdata(IO_mem_rdata),
      .IO_mem_wdata(IO_mem_wdata),
      .IO_mem_wr(IO_mem_wr)
   );

   wire [13:0] IO_wordaddr = IO_mem_addr[15:2];

   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   localparam IO_LEDS_bit      = 0;  // W five leds
   localparam IO_UART_DAT_bit  = 1;  // W data to send (8 bits)
   localparam IO_UART_CNTL_bit = 2;  // R status. bit 9: busy sending

   always @(posedge clk) begin
      if(IO_mem_wr & IO_wordaddr[IO_LEDS_bit]) begin
	 LEDS <= IO_mem_wdata[4:0];
      end
   end

   wire uart_valid = IO_mem_wr & IO_wordaddr[IO_UART_DAT_bit];
   wire uart_ready;

   corescore_emitter_uart #(
      .clk_freq_hz(`CPU_FREQ*1000000)
   ) UART(
      .i_clk(clk),
      .i_rst(!resetn),
      .i_data(IO_mem_wdata[7:0]),
      .i_valid(uart_valid),
      .o_ready(uart_ready),
      .o_uart_tx(TXD)
   );

   assign IO_mem_rdata =
		    IO_wordaddr[IO_UART_CNTL_bit] ? { 22'b0, !uart_ready, 9'b0}
	                                          : 32'b0;

`ifdef BENCH
   always @(posedge clk) begin
      if(uart_valid) begin
//	 $display("UART: %c", IO_mem_wdata[7:0]);
	 $write("%c", IO_mem_wdata[7:0] );
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
