/**
 * Step 24: Creating a RISC-V processor
 * Running programs from Mapped SPI flash (reset addr)
 */

/*
 * ============================
 * Step24（中文超详细导读）
 * ============================
 *
 * 这份 SoC 由三部分组成：
 *   1) Memory   ：片上 BRAM（6kB），用于 .data/.bss/stack，以及可选的数据读写。
 *   2) Processor：极简 RV32I（无压缩指令、无系统指令、无中断、无流水线、无 cache）。
 *                注意：它不是“一拍一条指令”的单周期 CPU，而是一个多周期状态机。
 *                每条指令至少经历“发起取指 -> 等待指令到达 -> 执行”，遇到 SPI Flash 还会额外等待。
 *   3) SOC      ：地址译码与外设互连：
 *                - 低地址（0x0000_0000 起）映射到片上 RAM（Memory）
 *                - 0x0040_0000 左右映射到 IO（LED/UART）
 *                - 0x0080_0000～ 映射到 SPI Flash（MappedSPIFlash），用于 XIP（从 flash 直接取指/读常量）
 *
 * 本 SoC 的“总线接口”是非常简化的同步接口：
 *   - mem_addr  ：地址（读写都用它）
 *   - mem_rstrb ：读请求脉冲（高电平表示“我要读”）
 *   - mem_rdata ：读回的数据（对 RAM 是同步读：下一拍有效；对 SPI Flash 是若干拍后有效）
 *   - mem_rbusy ：读忙信号（=1 表示外部还在取数据；=0 表示数据可用/已经稳定）
 *   - mem_wdata/mem_wmask：写数据 + 字节写使能（按 byte mask 支持 SB/SH/SW）
 *
 * 关键理解点：
 *   - RAM 访问很快：mem_rbusy 恒为 0（SOC 里只有 SPIFlash 会拉 busy）。
 *   - SPI Flash 访问很慢：一次 32-bit 读取要通过 SPI 串行移位完成，通常需要几十个时钟周期。
 *     CPU 会停在 WAIT_INSTR/WAIT_DATA 等待 mem_rbusy 变为 0，因此从 flash 取指会显著降低速度。
 */

`default_nettype none
`include "clockworks.v"
`include "emitter_uart.v"
`include "spi_flash.v"

module Memory (
   input             clk,
   input      [31:0] mem_addr,  // 要访问的地址（字节地址）
   output reg [31:0] mem_rdata, // 读回数据（同步读：在 mem_rstrb 拉高后的下一个上升沿更新）
   input   	     mem_rstrb, // 读请求：CPU 发起一次读
   input      [31:0] mem_wdata, // 写入数据（SW/SH/SB 会把数据放到对应 byte lane）
   input      [3:0]  mem_wmask	// 写掩码：每一位对应 1 个 byte（1=写该 byte）
);

   // 片上 RAM：这里用 reg 数组推断成 FPGA BRAM（或综合成寄存器阵列）
   // 6kB = 1536 * 4 bytes
   reg [31:0] MEM [0:1535]; // 1536 4-bytes words = 6 Kb of RAM in total

   // RAM 是按 32-bit word 组织的，因此地址要丢掉最低 2bit（字节偏移）得到 word 索引
   // 这里只取到 [12:2]，等价于 RAM 覆盖 0x0000..0x1FFF（8kB 地址空间），实际只实现了其中 6kB。
   wire [10:0] word_addr = mem_addr[12:2];

   always @(posedge clk) begin
      if(mem_rstrb) begin
         mem_rdata <= MEM[word_addr];
      end
      // 写端口：按字节写入（支持 SB/SH/SW），由 mem_wmask 指定哪个 byte lane 生效
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
    input         mem_rbusy,
    output 	  mem_rstrb,
    output [31:0] mem_wdata,
    output [3:0]  mem_wmask
);

   // 内部地址宽度：
   // - 为了省逻辑，PC 只保留低 ADDR_WIDTH 位（这里是 24 位，覆盖 16MB 空间）。
   // - 注意：这会让某些严格的 RISC-V 合规测试（尤其 AUIPC 生成任意 32-bit 常数）不完全通过，
   //   但对本教程“用地址跑起来程序”足够。
   localparam ADDR_WIDTH=24;

   // 程序计数器 PC：
   // - 复位时在 always 块里会被设置为 0x0082_0000（SPI Flash 映射地址 + 128k 偏移）
   // - 这里的位宽为 [ADDR_WIDTH:0]（25bit），高位会在赋值到 mem_addr 时自动补零/截断
   reg [ADDR_WIDTH:0] PC=0; // program counter（字节地址）

   // 当前指令寄存器：
   // - RV32I 的 32-bit 指令最低 2bit 恒为 2'b11（非压缩指令），因此这里把 instr 声明为 [31:2]
   // - 这样 decode 时用 instr[6:2] 即可得到 opcode[6:2]
   reg [31:2] instr;        // current instruction（去掉最低两位）

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
   // 立即数解码（按 RISC-V 标准把不同格式的 bit 拼出来，并做符号扩展）：
   // - Uimm：LUI/AUIPC
   // - Iimm：ALU 立即数/JALR/LOAD
   // - Simm：STORE
   // - Bimm：BRANCH（注意最低 bit 固定为 0，因为分支目标按 2 字节对齐；此实现只支持 32-bit 指令，因此实际为 4 对齐也成立）
   // - Jimm：JAL
   wire [31:0] Uimm={    instr[31],   instr[30:12], {12{1'b0}}};
   wire [31:0] Iimm={{21{instr[31]}}, instr[30:20]};
   /* verilator lint_off UNUSED */ // MSBs of SBJimms are not used by addr adder.
   wire [31:0] Simm={{21{instr[31]}}, instr[30:25],instr[11:7]};
   wire [31:0] Bimm={{20{instr[31]}}, instr[7],instr[30:25],instr[11:8],1'b0};
   wire [31:0] Jimm={{12{instr[31]}}, instr[19:12],instr[20],instr[30:21],1'b0};
   /* verilator lint_on UNUSED */

   // Destination registers
   // 目标寄存器 rd 编号（x0..x31），写回时会禁止写 x0
   wire [4:0] rdId  = instr[11:7];

   // function codes
   // funct3/funct7 用于区分同一 opcode 下不同操作（如 ADD/SUB、SRL/SRA 等）
   wire [2:0] funct3 = instr[14:12];
   wire [6:0] funct7 = instr[31:25];

   // The registers bank
   // 寄存器堆：32 个 32-bit 通用寄存器
   // - 读取是“组合逻辑读”：在 WAIT_INSTR 阶段把寄存器值采样到 rs1/rs2（见后面的状态机）
   // - 写回在时钟上升沿进行（writeBackEn 为真时写 rd）
   reg [31:0] RegisterBank [0:31];
   reg [31:0] rs1; // value of source
   reg [31:0] rs2; //  registers.
   wire [31:0] writeBackData; // data to be written to rd
   wire        writeBackEn;   // asserted if data should be written to rd

`ifdef BENCH
   integer     i;
   initial begin
      // 仿真基准模式下，把寄存器初始化为 0，避免 X 传播干扰观察
      for(i=0; i<32; ++i) begin
	 RegisterBank[i] = 0;
      end
   end
`endif

   // The ALU
   // ALU 输入选择：
   // - R 型算术/分支：第二操作数来自 rs2
   // - I 型算术：第二操作数来自 Iimm
   wire [31:0] aluIn1 = rs1;
   wire [31:0] aluIn2 = isALUreg | isBranch ? rs2 : Iimm;

   wire [4:0] shamt = isALUreg ? rs2[4:0] : instr[24:20]; // shift amount

   // The adder is used by both arithmetic instructions and JALR.
   wire [31:0] aluPlus = aluIn1 + aluIn2;

   // Use a single 33 bits subtract to do subtraction and all comparisons
   // (trick borrowed from swapforth/J1)
   // 33-bit 的“加法形式减法”：
   // aluMinus = aluIn1 - aluIn2
   // 同时借用最高位 aluMinus[32] 来得到 unsigned 比较结果（LTU）以及 EQ。
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
   // 通过“符号扩展 + 算术右移”实现 SRL/SRA，并用 flip32 复用实现 SLL
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
   // 分支条件判断（只依赖 rs1/rs2 的比较结果）：
   // - BEQ/BNE/BLT/BGE/BLTU/BGEU
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
   // 地址计算（nextPC / loadstore_addr）：
   // - PCplusImm 用于 B/J/U 这三类“PC 相对”地址
   // - loadstore_addr 用于 Load/Store 的有效地址（rs1 + imm）


   /* verilator lint_off WIDTH */

   // An adder used to compute branch address, JAL address and AUIPC.
   // branch->PC+Bimm    AUIPC->PC+Uimm    JAL->PC+Jimm
   // Equivalent to PCplusImm = PC + (isJAL ? Jimm : isAUIPC ? Uimm : Bimm)

   // Note: doing so with ADDR_WIDTH < 32, AUIPC may fail in
   // some RISC-V compliance tests because one can is supposed to use
   // it to generate arbitrary 32-bit values (and not only addresses).

   wire [ADDR_WIDTH-1:0] PCplusImm = PC + ( instr[3] ? Jimm[31:0] :
					    instr[4] ? Uimm[31:0] :
				            Bimm[31:0] );
   wire [ADDR_WIDTH-1:0] PCplus4 = PC+4;


   wire [ADDR_WIDTH-1:0] nextPC = ((isBranch && takeBranch) || isJAL) ? PCplusImm   :
	                                  isJALR   ? {aluPlus[31:1],1'b0} :
	                                             PCplus4;

   wire [ADDR_WIDTH-1:0] loadstore_addr = rs1 + (isStore ? Simm : Iimm);


   // register write back
   // 写回数据选择：
   // - JAL/JALR：写回 PC+4 到 rd（链接寄存器）
   // - LUI：写回 Uimm
   // - AUIPC：写回 PC+Uimm
   // - Load：写回内存读数据（已做符号/零扩展）
   // - 其他 ALU：写回 aluOut
   assign writeBackData = (isJAL || isJALR) ? PCplus4   :
			      isLUI         ? Uimm      :
			      isAUIPC       ? PCplusImm :
			      isLoad        ? LOAD_data :
			                      aluOut;
   /* verilator lint_on WIDTH */

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
   // Store 写数据重排：
   // 所有实际写入都以 32-bit 对齐地址进行（mem_addr 低 2bit 仍然保留在 loadstore_addr 中）。
   // 因此需要根据 loadstore_addr[1:0] 选择把 rs2 的低 8/16/32 位放到正确的 byte lane 上，
   // 再配合 STORE_wmask 只写对应字节。

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
   // ============================
   // 取指/执行状态机（核心！）
   // ============================
   //
   // 这颗“处理器”不是流水线，也不是单周期。它用一个 2-bit 状态机控制节拍：
   //
   //   FETCH_INSTR：发起取指（mem_rstrb=1，mem_addr=PC）
   //   WAIT_INSTR ：等待指令返回（对 RAM：下一拍就有；对 SPI Flash：需要等很多拍）
   //               当 mem_rbusy==0，认为 mem_rdata 有效，于是：
   //                 - instr <= mem_rdata[31:2]   （把指令锁存下来）
   //                 - rs1/rs2 <= RegisterBank[...]（把源寄存器值锁存下来）
   //                 - state -> EXECUTE
   //   EXECUTE    ：用锁存的 instr/rs1/rs2 做组合逻辑计算（ALU/分支/地址/写掩码等）
   //               - 更新 PC（除 SYSTEM/ebreak 之外）
   //               - 若是 Load：转 WAIT_DATA 等数据；否则回到 FETCH_INSTR 开始下一条
   //   WAIT_DATA  ：等待 load 数据返回（同样依赖 mem_rbusy）
   //               - mem_rbusy==0 时，允许把 LOAD_data 写回 rd，然后回到 FETCH_INSTR
   //
   // 这样做的好处是：无需 cache/流水/握手复杂度，也能“暂停等待”慢速 SPI Flash。
   localparam FETCH_INSTR = 0;
   localparam WAIT_INSTR  = 1;
   localparam EXECUTE     = 2;
   localparam WAIT_DATA   = 3;
   reg [1:0] state = FETCH_INSTR;

   always @(posedge clk) begin
      if(!resetn) begin
	 // 复位入口：
	 // 这里把 PC 直接设到 SPI Flash 映射地址 0x0082_0000（= 0x0080_0000 + 128k）
	 // 这样就能从 flash 中取第一条指令（相当于“从 flash 启动”）。
	 PC    <= 32'h00820000;
	 // reset 后 SPIFlash 可能仍在 busy/CS_N 状态尚未回到空闲，因此先进入 WAIT_DATA
	 // 等待 mem_rbusy 拉低，再开始正常取指（进入 FETCH_INSTR）。
	 state <= WAIT_DATA;
      end else begin
	 // 同步写回：只要 writeBackEn 为真、且 rd != x0，就在此拍把 writeBackData 写入寄存器堆
	 if(writeBackEn && rdId != 0) begin
	    RegisterBank[rdId] <= writeBackData;
	 end
	 case(state)
	   FETCH_INSTR: begin
	      // 这一拍对外发起读请求（mem_rstrb 会被组合逻辑拉高），下一拍进入 WAIT_INSTR 等返回
	      state <= WAIT_INSTR;
	   end
	   WAIT_INSTR: begin
	      // 说明：这里每拍都把 mem_rdata 锁存到 instr，并从 mem_rdata 里取出 rs1/rs2 编号去读寄存器。
	      // 真正“指令有效”的判定由 mem_rbusy 控制：只有 mem_rbusy==0 时，才进入 EXECUTE 执行。
	      instr <= mem_rdata[31:2];
	      rs1 <= RegisterBank[mem_rdata[19:15]];
	      rs2 <= RegisterBank[mem_rdata[24:20]];
	      if(!mem_rbusy) begin
		 state <= EXECUTE;
	      end
	   end
	   EXECUTE: begin
	      // 执行阶段：组合逻辑已经基于 instr/rs1/rs2 算好 nextPC / writeBackData / STORE_wmask 等
	      // 除了 SYSTEM（本教程用来做 ebreak/结束仿真）外，正常指令都会推进 PC
	      if(!isSYSTEM) begin
		 /* verilator lint_off WIDTH */
		 PC <= nextPC;
		 /* verilator lint_on WIDTH */
	      end
	      // 若是 Load：下一步需要等待内存返回数据，因此进入 WAIT_DATA
	      // 否则：下一条指令，回到 FETCH_INSTR
	      state <= isLoad  ? WAIT_DATA : FETCH_INSTR;
`ifdef BENCH
	      if(isSYSTEM) $finish();
`endif
	   end
	   WAIT_DATA: begin
	      // 等待 Load 数据返回（mem_rbusy 变为 0）
	      // 注意：writeBackEn 在 WAIT_DATA 恒为真（见后面 assign），因此只要 busy 拉低，就会在这一拍写回 LOAD_data
	      if(!mem_rbusy) begin
		 state <= FETCH_INSTR;
	      end
	   end
	 endcase
      end
   end

   // 写回使能：
   // - 在 EXECUTE 阶段：除了 Branch/Store（它们不写 rd）外，其他指令都写回
   // - 在 WAIT_DATA 阶段：Load 的数据返回在此阶段写回
   assign writeBackEn = (state==EXECUTE && !isBranch && !isStore) ||
			(state==WAIT_DATA) ;

   /* verilator lint_off WIDTH */
   // 对外地址选择：
   // - 取指阶段（FETCH/WAIT_INSTR）：mem_addr = PC
   // - 执行 Load/Store 时：mem_addr = loadstore_addr
   // 这里用 lint_off 是因为 PC/loadstore_addr 位宽小于 32，赋值到 mem_addr 时会自动补零/截断
   assign mem_addr = (state == WAIT_INSTR || state == FETCH_INSTR) ?
		     PC : loadstore_addr ;
   /* verilator lint_on WIDTH */

   // 读请求发起时机：
   // - FETCH_INSTR：发起“取指读”
   // - EXECUTE 且 isLoad：发起“数据读”
   assign mem_rstrb = (state == FETCH_INSTR || (state == EXECUTE & isLoad));

   // 写掩码发起时机：
   // - 只有在 EXECUTE 且 isStore 时才对外写（其余时候写掩码为 0）
   assign mem_wmask = {4{(state == EXECUTE) & isStore}} & STORE_wmask;

endmodule


module SOC (
    input 	     CLK, // system clock
    input 	     RESET,// reset button
    output reg [4:0] LEDS, // system LEDs
    input 	     RXD, // UART receive
    output 	     TXD, // UART transmit
    output 	     SPIFLASH_CLK,  // SPI flash clock
    output 	     SPIFLASH_CS_N, // SPI flash chip select (active low)
    inout [1:0]      SPIFLASH_IO    // SPI flash IO pins
);

   // ============================
   // SoC 顶层互连（总线 + 外设）
   // ============================
   //
   // CPU 只暴露一个极简“内存总线”接口（mem_*），SOC 做三件事：
   //   1) 地址译码：判断当前访问是 RAM / IO / SPIFlash
   //   2) 读数据复用：把对应从设备的 rdata 送回 CPU
   //   3) busy 复用：本设计只有 SPIFlash 会拉 busy（RAM/IO 都视为 0 wait）
   //
   // 地址映射（靠高位 bit 做非常粗粒度的 page 划分）：
   //   - RAM      ：mem_addr[23]==0 且 mem_addr[22]==0  -> 0x0000_0000 ～ 0x003F_FFFF（实际只实现了很小一段）
   //   - IO       ：mem_addr[23:22]==2'b01             -> 0x0040_0000 ～ 0x007F_FFFF
   //   - SPIFlash ：mem_addr[23]==1                     -> 0x0080_0000 ～ 0x00FF_FFFF（取其中低 20bit 做 word 地址）
   //
   // 你会发现这与固件里常用的 IO_BASE=0x400000（0x0040_0000）是一致的。

   wire clk;
   wire resetn;

   wire [31:0] mem_addr;
   wire [31:0] mem_rdata;
   wire        mem_rbusy;
   wire mem_rstrb;
   wire [31:0] mem_wdata;
   wire [3:0]  mem_wmask;

   Processor CPU(
      .clk(clk),
      .resetn(resetn),
      .mem_addr(mem_addr),
      .mem_rdata(mem_rdata),
      .mem_rstrb(mem_rstrb),
      .mem_rbusy(mem_rbusy),
      .mem_wdata(mem_wdata),
      .mem_wmask(mem_wmask)
   );

   wire [31:0] RAM_rdata;

   // 把字节地址换成 word 地址（右移 2 位）
   // 这里保留了更宽的 wordaddr 便于做 page/外设译码。
   wire [29:0] mem_wordaddr = mem_addr[31:2];

   // 译码信号：三选一（在本设计中是互斥的）
   wire isSPIFlash  = mem_addr[23];
   wire isIO        = mem_addr[23:22] == 2'b01;
   wire isRAM = !(mem_addr[23] | mem_addr[22]);

   // 写 strobe：只要任意 byte mask 置位，就认为这次是写事务
   wire mem_wstrb = |mem_wmask;

   // 片上 RAM：
   // - 只有在 isRAM 时才允许对 RAM 发起读写
   // - 对非 RAM 区域访问时，RAM 的写掩码被强制为 0，避免误写
   Memory RAM(
      .clk(clk),
      .mem_addr(mem_addr),
      .mem_rdata(RAM_rdata),
      .mem_rstrb(isRAM & mem_rstrb),
      .mem_wdata(mem_wdata),
      .mem_wmask({4{isRAM}}&mem_wmask)
   );

   wire [31:0] SPIFlash_rdata;
   wire SPIFlash_rbusy;

   // SPI Flash（内存映射只读）：
   // - CPU 访问 0x0080_0000 这一页时，会转成 SPIFlash 的 word_address
   // - rbusy 表示 SPI 串行读事务尚未完成
   MappedSPIFlash SPIFlash(
      .clk(clk),
      .word_address(mem_wordaddr[19:0]),
      .rdata(SPIFlash_rdata),
      .rstrb(isSPIFlash & mem_rstrb),
      .rbusy(SPIFlash_rbusy),
      .CLK(SPIFLASH_CLK),
      .CS_N(SPIFLASH_CS_N),
      .IO(SPIFLASH_IO)
   );

   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   // IO 页的寄存器采用非常简化的“1-hot word address”：
   // - mem_wordaddr[0] 这一 word 表示 LED 寄存器
   // - mem_wordaddr[1] 这一 word 表示 UART 数据寄存器
   // - mem_wordaddr[2] 这一 word 表示 UART 状态寄存器
   //
   // 这意味着软件侧通常会用类似（伪代码）：
   //   *(volatile uint32_t*)(0x400000 + 4*IO_LEDS_bit) = ...
   //   *(volatile uint32_t*)(0x400000 + 4*IO_UART_DAT_bit) = ...
   localparam IO_LEDS_bit      = 0;  // W five leds
   localparam IO_UART_DAT_bit  = 1;  // W data to send (8 bits)
   localparam IO_UART_CNTL_bit = 2;  // R status. bit 9: busy sending

   always @(posedge clk) begin
      if(isIO & mem_wstrb & mem_wordaddr[IO_LEDS_bit]) begin
	 LEDS <= mem_wdata[4:0];
//	 $display("Value sent to LEDS: %b %d %d",mem_wdata,mem_wdata,$signed(mem_wdata));
      end
   end

   // UART 发送：
   // - uart_valid：CPU 向 UART 数据寄存器写入时产生
   // - uart_ready：UART 是否能接受下一个字节（ready=1 表示空闲）
   wire uart_valid = isIO & mem_wstrb & mem_wordaddr[IO_UART_DAT_bit];
   wire uart_ready;

   // 发射器 UART（只发不收）：
   // - 波特率由 clk_freq_hz 推导，因此这里依赖 `CPU_FREQ 宏与实际 clk 频率一致
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

   // IO 读返回：
   // 目前只实现了 UART 状态寄存器：
   // - bit9 表示“busy sending”（这里用 !uart_ready 表示忙）
   wire [31:0] IO_rdata =
	       mem_wordaddr[IO_UART_CNTL_bit] ? { 22'b0, !uart_ready, 9'b0}
	                                      : 32'b0;

   // 读数据复用：根据 isRAM/isSPIFlash/isIO 选择数据源
   assign mem_rdata = isRAM      ? RAM_rdata :
                      isSPIFlash ? SPIFlash_rdata :
	                           IO_rdata ;

   // busy 复用：只有 SPIFlash 会产生 wait（RAM/IO 视为零等待）
   assign mem_rbusy = SPIFlash_rbusy;

`ifdef BENCH
   always @(posedge clk) begin
      if(uart_valid) begin
	 // 仿真模式下：把 UART 发送的字节直接打印到仿真控制台，便于看 printf 输出
	 $write("%c", mem_wdata[7:0] );
	 $fflush(32'h8000_0001);
      end
   end
`endif

   // Gearbox and reset circuitry.
   // 时钟与复位：
   // - Clockworks 把板上的 CLK/RESET 整形成内部 clk/resetn
   // - clk 是 SoC 真正运行的时钟；resetn 为低有效复位（注意 CPU 里用的是 !resetn 判断复位）
   Clockworks CW(
     .CLK(CLK),
     .RESET(RESET),
     .clk(clk),
     .resetn(resetn)
   );

endmodule
