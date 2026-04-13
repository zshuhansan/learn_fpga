module alu #(
    parameter WITH_RV32M = 1
) (
    input  wire [31:0] in1,
    input  wire [31:0] in2,
    input  wire [2:0]  funct3,
    input  wire        is_minus,
    input  wire        is_arith_shift,
    input  wire        is_rv32m,
   input  wire        is_mul,
   input  wire [31:0] mul_result,
    input  wire        is_div,
    input  wire [31:0] div_result,

    output wire [31:0] alu_out,
    output wire [31:0] alu_plus,
    output wire        eq,
    output wire        lt,
    output wire        ltu
);   wire [7:0] funct3_is = 8'b1 << funct3;
   wire [32:0] alu_minus = {1'b1, ~in2} + {1'b0, in1} + 33'b1;
   function [31:0] flip32;
      input [31:0] x;
      flip32 = {x[ 0], x[ 1], x[ 2], x[ 3], x[ 4], x[ 5], x[ 6], x[ 7],
            x[ 8], x[ 9], x[10], x[11], x[12], x[13], x[14], x[15],
            x[16], x[17], x[18], x[19], x[20], x[21], x[22], x[23],
            x[24], x[25], x[26], x[27], x[28], x[29], x[30], x[31]};
   endfunction
   wire [31:0] shifter_in = (funct3 == 3'b001) ? flip32(in1) : in1;
   wire [31:0] shifter =
       $signed({is_arith_shift & in1[31], shifter_in}) >>> in2[4:0];
   wire [31:0] leftshift = flip32(shifter);
   wire [31:0] alu_out_base =
    (funct3_is[0] ? (is_minus ? alu_minus[31:0] : alu_plus) : 32'b0) |
    (funct3_is[1] ? leftshift                               : 32'b0) |
    (funct3_is[2] ? {31'b0, lt }                            : 32'b0) |
    (funct3_is[3] ? {31'b0, ltu}                            : 32'b0) |
    (funct3_is[4] ? in1 ^ in2                               : 32'b0) |
    (funct3_is[5] ? shifter                                 : 32'b0) |
    (funct3_is[6] ? in1 | in2                               : 32'b0) |
    (funct3_is[7] ? in1 & in2                               : 32'b0) ;
         wire [31:0] alu_out_muldiv =
                (  is_mul          ? mul_result      : 32'b0) |
           (  is_div          ? div_result      : 32'b0) ;



   // 这个模块把整数 ALU 的常规算术、比较、移位，以及可选的 RV32M 乘除结果选择
   // 放在一起实现。上层已经把指令类型拆解成控制信号，这里只根据 funct3 和若干
   // 辅助标志生成最终结果。

   // 加法器不仅服务 ADD/ADDI，也复用于 JALR 的目标地址计算，因此单独导出。
   assign alu_plus = in1 + in2;

   // 用一套 33 位减法同时得到减法结果、相等比较和大小比较，能减少重复硬件。
   assign lt  = (in1[31] ^ in2[31]) ? in1[31] : alu_minus[32];
   assign ltu = alu_minus[32];
   assign eq  = (alu_minus[31:0] == 0);

   // 左移通过“位翻转 + 算术右移 + 再翻转”的写法统一到同一条移位路径上，
   // 这样可以让左右移共用一套实现逻辑。


   /* verilator lint_off WIDTH */
   /* verilator lint_on WIDTH */


   // 基础 ALU 结果完全由 funct3 选择，对应 ADD/SUB、SLT、逻辑运算和移位。

   generate
      if (WITH_RV32M) begin
         // 乘法和除法结果都来自独立多周期单元，ALU 这里只负责最终选择。

         assign alu_out = is_rv32m ? alu_out_muldiv : alu_out_base;
      end else begin
         assign alu_out = alu_out_base;
      end
   endgenerate

endmodule
