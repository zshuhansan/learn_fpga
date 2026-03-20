/*
 * A simple assembler for RiscV written in VERILOG.
 * See table page 104 of RiscV instruction manual.
 * Bruno Levy, March 2022
 */

/*
 * 这份文件本质上是一个“用 Verilog 写的 RV32I 小汇编器”：
 * - 通过一组 task/function，把类似 ADD(x1,x0,x0) 这样的写法转换成 32 位机器码
 * - 生成的机器码直接写入当前模块里的 MEM[] 数组
 * - memPC 作为“汇编器程序计数器”（byte 地址），每写一条指令自动 +4
 *
 * 重要：这是预处理器 `include 的文本包含文件，不是独立模块。
 * 也就是说它会被“原样粘贴”到包含它的那个 module 内部，因此它可以直接访问：
 * - 由外部 module 声明的 MEM 数组（reg [31:0] MEM [0:255]）
 * - 外部 module 的仿真宏（如 `BENCH）等
 *
 * 参数约定（非常关键，避免你写汇编时踩坑）：
 * - 立即数 imm 输入统一用 32 位传入，但编码时只取 RV32I 规定的那几位（如 I-type 取 imm[11:0]）
 * - 分支/跳转的 imm 在 RISC-V 语义上是“相对 PC 的字节偏移”，并且最低位为 0（对齐要求）
 *   在本汇编器里通常通过 LabelRef(label) 来得到 offset（label - 当前 memPC）
 * - LUI/AUIPC 的 imm 这里按“已经左移过 12 位”的值传入（也就是你传入 0x12345000 这种）
 *   与 GNU 汇编里写 `lui x1,0x12345` 的立即数含义不同（GNU 会隐式 <<12）
 */

// Machine code will be generated in MEM,
// starting from address 0 (can be changed below,
// initial value of memPC).
//
// Example:
//
// module MyModule( my inputs, my outputs ...);
//    ...
//    reg [31:0] MEM [0:255]; 
//    `include "riscv_assembly.v" // yes, needs to be included from here.
//    integer L0_;
//    initial begin
//                  ADD(x1,x0,x0);
//                  ADDI(x2,x0,32);
//      Label(L0_); ADDI(x1,x1,1); 
//                  BNE(x1, x2, LabelRef(L0_));
//                  EBREAK();
//    end
//   1) simulate with icarus, it will complain about uninitialized labels,
//      and will display for each Label() statement the address to be used
//      (in the present case, it is 8)
//   2) replace the declaration of the label:
//      integer L0_ = 8;
//      re-simulate with icarus
//      If you made an error, it will be detected
//   3) synthesize with yosys
// (if you do not use labels, you can synthesize directly, of course...)
//
//
// You can change the address where code is generated
//   by assigning to memPC (needs to be a word boundary).
//
// NOTE: to be checked, LUI, AUIPC take as argument
//     pre-shifted constant, unlike in GNU assembly

integer memPC;
initial memPC = 0;

/***************************************************************************/

/*
 * 寄存器编号（x0..x31）。
 * 这看起来有点“多余”，但能让汇编代码可读性提升很多：
 * - ADD(x1,x0,x0) 一眼就知道 x1/x0 是寄存器编号
 * - ADDI(x1,x0,123) 一眼就知道 123 是立即数而不是寄存器
 */ 

localparam x0 = 0, x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6, x7 = 7, 
           x8 = 8, x9 = 9, x10=10, x11=11, x12=12, x13=13, x14=14, x15=15,
           x16=16, x17=17, x18=18, x19=19, x20=20, x21=21, x22=22, x23=23,
           x24=24, x25=25, x26=26, x27=27, x28=28, x29=29, x30=30, x31=31;


// add x0,x0,x0   
localparam [31:0] NOP_CODEOP = 32'b0000000_00000_00000_000_00000_0110011; 

/***************************************************************************/

/*
 * R-Type instructions.
 * 语义：rd <- rs1 OP rs2
 * 编码：{funct7, rs2, rs1, funct3, rd, opcode}
 * - opcode 对于 RV32I 的寄存器 ALU 类指令是 0110011
 * - funct3/funct7 选择具体运算（ADD/SUB/AND/OR/...）
 */

task RType;
   input [6:0] opcode;
   input [4:0] rd;   
   input [4:0] rs1;
   input [4:0] rs2;
   input [2:0] funct3;
   input [6:0] funct7;
   begin
      MEM[memPC[31:2]] = {funct7, rs2, rs1, funct3, rd, opcode};
      memPC = memPC + 4;
   end
endtask

task ADD;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // ADD rd, rs1, rs2
   // 语义：rd = rs1 + rs2
   // opcode=0110011 funct3=000 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b000, 7'b0000000);
endtask
   
task SUB;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // SUB rd, rs1, rs2
   // 语义：rd = rs1 - rs2
   // opcode=0110011 funct3=000 funct7=0100000
   RType(7'b0110011, rd, rs1, rs2, 3'b000, 7'b0100000);
endtask

task SLL;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // SLL rd, rs1, rs2
   // 语义：rd = rs1 << (rs2[4:0])
   // opcode=0110011 funct3=001 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b001, 7'b0000000);
endtask

task SLT;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // SLT rd, rs1, rs2
   // 语义：rd = (signed(rs1) < signed(rs2)) ? 1 : 0
   // opcode=0110011 funct3=010 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b010, 7'b0000000);
endtask
   
task SLTU;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // SLTU rd, rs1, rs2
   // 语义：rd = (unsigned(rs1) < unsigned(rs2)) ? 1 : 0
   // opcode=0110011 funct3=011 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b011, 7'b0000000);
endtask

task XOR;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // XOR rd, rs1, rs2
   // 语义：rd = rs1 ^ rs2
   // opcode=0110011 funct3=100 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b100, 7'b0000000);
endtask

task SRL;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // SRL rd, rs1, rs2
   // 语义：rd = rs1 >> (rs2[4:0])（逻辑右移，高位补 0）
   // opcode=0110011 funct3=101 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b101, 7'b0000000);
endtask

task SRA;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // SRA rd, rs1, rs2
   // 语义：rd = signed(rs1) >>> (rs2[4:0])（算术右移，高位补符号位）
   // opcode=0110011 funct3=101 funct7=0100000
   RType(7'b0110011, rd, rs1, rs2, 3'b101, 7'b0100000);
endtask

task OR;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // OR rd, rs1, rs2
   // 语义：rd = rs1 | rs2
   // opcode=0110011 funct3=110 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b110, 7'b0000000);
endtask

task AND;
   input [4:0] rd;
   input [4:0] rs1;
   input [4:0] rs2;
   // AND rd, rs1, rs2
   // 语义：rd = rs1 & rs2
   // opcode=0110011 funct3=111 funct7=0000000
   RType(7'b0110011, rd, rs1, rs2, 3'b111, 7'b0000000);
endtask

/***************************************************************************/

/*
 * I-Type instructions.
 * 语义：rd <- rs1 OP imm
 * 编码：{imm[11:0], rs1, funct3, rd, opcode}
 * - opcode 对于 RV32I 的寄存器-立即数 ALU 类指令是 0010011
 * - 这里 imm 传入 32 位，但编码只取低 12 位（并在 CPU 执行时按 Iimm 规则符号扩展）
 */
   
task IType;
   input [6:0]  opcode;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   input [2:0]  funct3;
   begin
      MEM[memPC[31:2]] = {imm[11:0], rs1, funct3, rd, opcode};
      memPC = memPC + 4;
   end
endtask

task ADDI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // ADDI rd, rs1, imm
      // 语义：rd = rs1 + signext(imm[11:0])
      IType(7'b0010011, rd, rs1, imm, 3'b000);
   end
endtask

task SLTI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // SLTI rd, rs1, imm
      // 语义：rd = (signed(rs1) < signext(imm[11:0])) ? 1 : 0
      IType(7'b0010011, rd, rs1, imm, 3'b010);
   end
endtask

task SLTIU;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // SLTIU rd, rs1, imm
      // 语义：rd = (unsigned(rs1) < unsigned(signext(imm[11:0]))) ? 1 : 0
      IType(7'b0010011, rd, rs1, imm, 3'b011);
   end
endtask

task XORI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // XORI rd, rs1, imm
      // 语义：rd = rs1 ^ signext(imm[11:0])
      IType(7'b0010011, rd, rs1, imm, 3'b100);
   end
endtask

task ORI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // ORI rd, rs1, imm
      // 语义：rd = rs1 | signext(imm[11:0])
      IType(7'b0010011, rd, rs1, imm, 3'b110);
   end
endtask

task ANDI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // ANDI rd, rs1, imm
      // 语义：rd = rs1 & signext(imm[11:0])
      IType(7'b0010011, rd, rs1, imm, 3'b111);
   end
endtask

// The three shifts, SLLI, SRLI, SRAI, encoded in RType format
// (rs2 is replaced with shift amount=imm[4:0])   
   
task SLLI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // SLLI rd, rs1, shamt
      // 语义：rd = rs1 << shamt，其中 shamt=imm[4:0]
      RType(7'b0010011, rd, rs1, imm[4:0], 3'b001, 7'b0000000);
   end
endtask

task SRLI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // SRLI rd, rs1, shamt
      // 语义：rd = rs1 >> shamt（逻辑右移），shamt=imm[4:0]
      RType(7'b0010011, rd, rs1, imm[4:0], 3'b101, 7'b0000000);
   end
endtask

task SRAI;
   input [4:0]  rd;   
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // SRAI rd, rs1, shamt
      // 语义：rd = signed(rs1) >>> shamt（算术右移），shamt=imm[4:0]
      RType(7'b0010011, rd, rs1, imm[4:0], 3'b101, 7'b0100000);
   end
endtask

/***************************************************************************/

/*
 * Jumps (JAL and JALR)
 */

task JType;
   input [6:0]  opcode;
   input [4:0]  rd;
   input [31:0] imm;
   begin
      // J-type 立即数编码（JAL）
      // RISC-V Jimm 的 bit0 恒为 0（2 字节对齐/4 字节对齐），这里也不存 bit0
      // 本汇编器使用传入的 imm（通常为相对 memPC 的 offset）拆分为：
      // {imm[20], imm[10:1], imm[11], imm[19:12]}
      MEM[memPC[31:2]] = {imm[20], imm[10:1], imm[11], imm[19:12], rd, opcode};
      memPC = memPC + 4;
   end
endtask
   
task JAL;
   input [4:0] rd;
   input [31:0] imm;
   begin
      // JAL rd, offset
      // 语义：rd = PC+4; PC = PC + offset
      // offset 通常用 LabelRef(label) 得到
      JType(7'b1101111, rd, imm);
   end
endtask 

// JALR is encoded in the IType format.
   
task JALR;
   input [4:0] rd;
   input [4:0] rs1;
   input [31:0] imm;
   begin
      // JALR rd, rs1, imm
      // 语义：rd = PC+4; PC = (rs1 + signext(imm)) & ~1
      // 编码属于 I-type：opcode=1100111 funct3=000
      IType(7'b1100111, rd, rs1, imm, 3'b000);
   end
endtask   

/***************************************************************************/   

/*
 * Branch instructions.
 */    
   
task BType;
   input [6:0]  opcode;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   input [2:0]  funct3;
   begin
      // B-type 立即数编码（分支）
      // {imm[12], imm[10:5], rs2, rs1, funct3, imm[4:1], imm[11], opcode}
      // Bimm 的 bit0 恒为 0，因此这里也不存 bit0
      MEM[memPC[31:2]] = {imm[12],imm[10:5], rs2, rs1, funct3, imm[4:1], imm[11], opcode};
      memPC = memPC + 4;
   end
endtask

task BEQ;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BEQ rs1, rs2, offset
      // 语义：if(rs1==rs2) PC = PC + offset
      BType(7'b1100011, rs1, rs2, imm, 3'b000);
   end
endtask

task BNE;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BNE rs1, rs2, offset
      // 语义：if(rs1!=rs2) PC = PC + offset
      BType(7'b1100011, rs1, rs2, imm, 3'b001);
   end
endtask

task BLT;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BLT rs1, rs2, offset（有符号比较）
      // 语义：if(signed(rs1)<signed(rs2)) PC = PC + offset
      BType(7'b1100011, rs1, rs2, imm, 3'b100);
   end
endtask

task BGE;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BGE rs1, rs2, offset（有符号比较）
      // 语义：if(signed(rs1)>=signed(rs2)) PC = PC + offset
      BType(7'b1100011, rs1, rs2, imm, 3'b101);
   end
endtask

task BLTU;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BLTU rs1, rs2, offset（无符号比较）
      // 语义：if(unsigned(rs1)<unsigned(rs2)) PC = PC + offset
      BType(7'b1100011, rs1, rs2, imm, 3'b110);
   end
endtask

task BGEU;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BGEU rs1, rs2, offset（无符号比较）
      // 语义：if(unsigned(rs1)>=unsigned(rs2)) PC = PC + offset
      BType(7'b1100011, rs1, rs2, imm, 3'b111);
   end
endtask
   
/***************************************************************************/   

/*
 * LUI and AUIPC
 */    

task UType;
   input [6:0]  opcode;
   input [4:0]  rd;
   input [31:0] imm;
   begin
      // U-type 立即数编码（LUI/AUIPC）
      // 编码：{imm[31:12], rd, opcode}
      // 注意：RISC-V 执行时相当于把 imm[31:12] 放到高 20 位，低 12 位为 0
      MEM[memPC[31:2]] = {imm[31:12], rd, opcode};
      memPC = memPC + 4;
   end
endtask

task LUI;
   input [4:0]  rd;
   input [31:0] imm;
   begin
      // LUI rd, imm
      // 语义：rd = imm[31:12] << 12
      // 这里要求传入的 imm 本身就是“已经 <<12 的值”（例如 32'h12345000）
      UType(7'b0110111, rd, imm);
   end
endtask

task AUIPC;
   input [4:0]  rd;
   input [31:0] imm;
   begin
      // AUIPC rd, imm
      // 语义：rd = PC + (imm[31:12] << 12)
      // 同样要求传入的 imm 是已经 <<12 的形式
      UType(7'b0010111, rd, imm);
   end
endtask
   
/***************************************************************************/   

/*
 * Load instructions
 */    

task LB;
   input [4:0]  rd;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // LB rd, imm(rs1)
      // 语义：rd = signext(mem8[rs1+imm])
      IType(7'b0000011, rd, rs1, imm, 3'b000);
   end
endtask      

task LH;
   input [4:0]  rd;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // LH rd, imm(rs1)
      // 语义：rd = signext(mem16[rs1+imm])
      IType(7'b0000011, rd, rs1, imm, 3'b001);
   end
endtask      
   
task LW;
   input [4:0]  rd;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // LW rd, imm(rs1)
      // 语义：rd = mem32[rs1+imm]
      IType(7'b0000011, rd, rs1, imm, 3'b010);
   end
endtask      

task LBU;
   input [4:0]  rd;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // LBU rd, imm(rs1)
      // 语义：rd = zeroext(mem8[rs1+imm])
      IType(7'b0000011, rd, rs1, imm, 3'b100);
   end
endtask      

task LHU;
   input [4:0]  rd;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // LHU rd, imm(rs1)
      // 语义：rd = zeroext(mem16[rs1+imm])
      IType(7'b0000011, rd, rs1, imm, 3'b101);
   end
endtask      
   
/***************************************************************************/   

/*
 * Store instructions
 */ 

task SType;
   input [6:0]  opcode;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   input [2:0]  funct3;
   begin
      // S-type 立即数编码（存储）
      // 编码：{imm[11:5], rs2, rs1, funct3, imm[4:0], opcode}
      MEM[memPC[31:2]] = {imm[11:5], rs2, rs1, funct3, imm[4:0], opcode};
      memPC = memPC + 4;
   end
endtask

// Note: in SB, SH, SW, rs1 and rs2 are swapped, to match assembly code:
// for instance:
//   
//     rs2   rs1   
//  sw ra, 0(sp)   
   
task SB;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // SB rs2, imm(rs1)
      // 语义：mem8[rs1+imm] = rs2[7:0]
      SType(7'b0100011, rs2, rs1, imm, 3'b000);
   end
endtask   

task SH;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // SH rs2, imm(rs1)
      // 语义：mem16[rs1+imm] = rs2[15:0]
      SType(7'b0100011, rs2, rs1, imm, 3'b001);
   end
endtask   

task SW;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // SW rs2, imm(rs1)
      // 语义：mem32[rs1+imm] = rs2
      SType(7'b0100011, rs2, rs1, imm, 3'b010);
   end
endtask   
   
/***************************************************************************/   
   
/*
 * SYSTEM instructions
 */

task FENCE;
   input [3:0] pred;
   input [3:0] succ;
   begin
      // FENCE pred,succ
      // 内存排序屏障（本教程内核大多不实现该语义，这里主要用于生成机器码）
      MEM[memPC[31:2]] = {4'b0000, pred, succ, 5'b00000, 3'b000, 5'b00000, 7'b1110011};
      memPC = memPC + 4;
   end
endtask   

task FENCE_I;
   begin
      // FENCE.I
      // 指令流与数据流同步（同样此处主要用于生成机器码）
      MEM[memPC[31:2]] = {4'b0000, 4'b0000, 4'b0000, 5'b00000, 3'b001, 5'b00000, 7'b1110011};
      memPC = memPC + 4;
   end
endtask   
   
task ECALL;
   begin
      // ECALL
      // 环境调用（系统调用入口）
      MEM[memPC[31:2]] = {12'b000000000000, 5'b00000, 3'b000, 5'b00000, 7'b1110011};
      memPC = memPC + 4;
   end
endtask   
   
task EBREAK;
   begin
      // EBREAK
      // 断点/停止。教程里常用它作为“程序结束”标记
      MEM[memPC[31:2]] = {12'b000000000001, 5'b00000, 3'b000, 5'b00000, 7'b1110011};
      memPC = memPC + 4;
   end
endtask   

task CSRRW;
   input [4:0] rd;
   input [11:0] csr;
   input [4:0] rs1;
   begin
      // CSRRW rd, csr, rs1
      // 读写 CSR：rd = CSR[csr]; CSR[csr] = rs1
      MEM[memPC[31:2]] = {csr, rs1, 3'b001, rd, 7'b1110011};
      memPC = memPC + 4;      
   end
endtask

task CSRRS;
   input [4:0] rd;
   input [11:0] csr;
   input [4:0] rs1;
   begin
      // CSRRS rd, csr, rs1
      // 读并置位 CSR：rd = CSR[csr]; CSR[csr] = CSR[csr] | rs1
      MEM[memPC[31:2]] = {csr, rs1, 3'b010, rd, 7'b1110011};
      memPC = memPC + 4;      
   end
endtask

task CSRRC;
   input [4:0] rd;
   input [11:0] csr;   
   input [4:0] rs1;
   begin
      // CSRRC rd, csr, rs1
      // 读并清位 CSR：rd = CSR[csr]; CSR[csr] = CSR[csr] & ~rs1
      MEM[memPC[31:2]] = {csr, rs1, 3'b011, rd, 7'b1110011};
      memPC = memPC + 4;      
   end
endtask

task CSRRWI;
   input [4:0] rd;
   input [11:0] csr;
   input [31:0] imm;
   begin
      // CSRRWI rd, csr, zimm
      // 立即数版本：zimm=imm[4:0]
      MEM[memPC[31:2]] = {csr, imm[4:0], 3'b101, rd, 7'b1110011};
      memPC = memPC + 4;      
   end
endtask

task CSRRSI;
   input [4:0] rd;
   input [11:0] csr;
   input [31:0] imm;
   begin
      // CSRRSI rd, csr, zimm
      MEM[memPC[31:2]] = {csr, imm[4:0], 3'b110, rd, 7'b1110011};
      memPC = memPC + 4;      
   end
endtask

task CSRRCI;
   input [4:0] rd;
   input [11:0] csr;
   input [31:0] imm;
   begin
      // CSRRCI rd, csr, zimm
      MEM[memPC[31:2]] = {csr, imm[4:0], 3'b111, rd, 7'b1110011};
      memPC = memPC + 4;      
   end
endtask
   
/***************************************************************************/   

/*
 * Labels.
 * Example of usage:
 *  
 *                   ADD(x1,x0,x0);
 *       Label(L0_); ADDI(x1,x1,1); 
 *                   JAL(x0, LabelRef(L0_));
 */

   integer ASMerror=0;
   
   task Label;
      inout integer L;
      begin
`ifdef BENCH
	if(L[0] === 1'bx) begin
	   $display("Missing label initialization");
	   ASMerror = 1;
	end else if(L != memPC) begin
	   $display("Incorrect label initialization");
	   $display("Expected: %0d    Got: %0d",memPC,L);
	   ASMerror = 1;
	end
	$display("Label:",memPC);
`endif	 
     end
   endtask

   function [31:0] LabelRef;
      input integer L;
      begin
`ifdef BENCH
	if(L[0] === 1'bx) begin
	   $display("Reference to uninitialized label");
	   ASMerror = 1;
	end
`endif	 
	 LabelRef = L - memPC;
      end
   endfunction

   task endASM;
      begin
`ifdef GET_ASM_LABELS
	 $finish();
`endif	 
`ifdef BENCH  
	 if(ASMerror) $finish();
`endif
      end
   endtask
   
   
/****************************************************************************/

/*
 * RISC-V ABI register names.
 */    

   localparam zero = x0;
   localparam ra   = x1;
   localparam sp   = x2;
   localparam gp   = x3;
   localparam tp   = x4;
   localparam t0   = x5;
   localparam t1   = x6;
   localparam t2   = x7;
   localparam fp   = x8;
   localparam s0   = x8;
   localparam s1   = x9;
   localparam a0   = x10;
   localparam a1   = x11;
   localparam a2   = x12;
   localparam a3   = x13;
   localparam a4   = x14;
   localparam a5   = x15;
   localparam a6   = x16;
   localparam a7   = x17;
   localparam s2   = x18;
   localparam s3   = x19;
   localparam s4   = x20;
   localparam s5   = x21;
   localparam s6   = x22;
   localparam s7   = x23;
   localparam s8   = x24;
   localparam s9   = x25;
   localparam s10  = x26;
   localparam s11  = x27;
   localparam t3   = x28;
   localparam t4   = x29;
   localparam t5   = x30;      
   localparam t6   = x31;   

/*
 * RISC-V pseudo-instructions
 */

task NOP;
   begin
      // NOP（伪指令）
      // 语义：什么也不做。RISC-V 常用 ADD x0,x0,x0 作为 NOP
      ADD(x0,x0,x0);
   end
endtask

// See https://stackoverflow.com/questions/50742420/
// risc-v-build-32-bit-constants-with-lui-and-addi
// Add imm[11] << 12 to the constant passed to LUI,
// so that it cancels sign expansion done by ADDI
// if imm[11] is 1.
task LI;
   input [4:0]  rd;
   input [31:0] imm;
   begin
      // LI（伪指令）：把 32 位常量装载到 rd
      // - 若常量为 0：用 ADD rd,zero,zero
      // - 若能用 12 位有符号立即数表示：用 ADDI rd,zero,imm
      // - 否则：用 LUI + ADDI 组合构造 32 位常量
      if(imm == 0) begin
	 ADD(rd,zero,zero);
      end else if($signed(imm) >= -2048 && $signed(imm) < 2048) begin
	 ADDI(rd,zero,imm);
      end else begin
	 LUI(rd,imm + (imm[11] << 12)); // cancel sign expansion
	 if(imm[11:0] != 0) begin
	    ADDI(rd,rd,imm[11:0]);
	 end
      end
   end
endtask

task CALL;
  input [31:0] offset;
  begin
     // CALL（伪指令）：调用远距离目标（相对 PC 的 32 位范围）
     // 展开为：AUIPC 临时寄存器, offset_hi ; JALR ra, 临时寄存器, offset_lo
     AUIPC(x6, offset);
     JALR(x1, x6, offset[11:0]);
  end
endtask 

task RET;
  begin
     // RET（伪指令）：返回（跳到 ra）
     JALR(x0,x1,0);
  end
endtask   

task MV;
   input [4:0]  rd;
   input [4:0] 	rs1;
   begin
      // MV（伪指令）：rd = rs1
      ADD(rd,rs1,zero);
   end
endtask

task J;
   input [31:0] imm;
   begin
      // TODO: far targets
      // J（伪指令）：无条件跳转（不写回 ra）
      JAL(zero,imm);
   end
endtask

task JR;
   input [4:0]  rs1;
   input [31:0] imm;   
   begin
      // JR（伪指令）：无条件跳转到寄存器 + 立即数（不写回 ra）
      JALR(zero,rs1,imm);
   end
endtask
   
task BEQZ;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // BEQZ（伪指令）：if(rs1==0) branch
      BEQ(rs1,x0,imm);
   end
endtask

task BNEZ;
   input [4:0]  rs1;
   input [31:0] imm;
   begin
      // BNEZ（伪指令）：if(rs1!=0) branch
      BNE(rs1,x0,imm);
   end
endtask

task BGT;
   input [4:0]  rs1;
   input [4:0]  rs2;   
   input [31:0] imm;
   begin
      // BGT（伪指令）：if(rs1>rs2) branch
      // RISC-V 没有 BGT，等价于 BLT(rs2,rs1,...)
      BLT(rs2,rs1,imm);
   end
endtask

task DATAW;
   input [31:0] w;
   begin
      // DATAW：把一个 32 位 word 原样写入 MEM（用于放常量/表）
      MEM[memPC[31:2]] = w;
      memPC = memPC+4;
   end
endtask

task DATAB;
   input [7:0] b1;
   input [7:0] b2;
   input [7:0] b3;
   input [7:0] b4;   
   begin
      // DATAB：把 4 个字节按 little-endian 顺序塞进当前 word
      // MEM[word][7:0]=b1, [15:8]=b2, [23:16]=b3, [31:24]=b4
      MEM[memPC[31:2]][ 7: 0] = b1;
      MEM[memPC[31:2]][15: 8] = b2;
      MEM[memPC[31:2]][23:16] = b3;
      MEM[memPC[31:2]][31:24] = b4;            
      memPC = memPC+4;
   end
endtask

      
   
/****************************************************************************/ 
   
