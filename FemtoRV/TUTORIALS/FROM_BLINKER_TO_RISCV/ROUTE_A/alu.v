module alu #(
    parameter WITH_RV32M = 1
) (
    input  wire [31:0] in1,
    input  wire [31:0] in2,
    input  wire [2:0]  funct3,
    input  wire        is_minus,
    input  wire        is_arith_shift,
    input  wire        is_rv32m,
    input  wire        is_div,
    input  wire [31:0] div_result,

    output wire [31:0] alu_out,
    output wire [31:0] alu_plus,
    output wire        eq,
    output wire        lt,
    output wire        ltu
);

   wire [7:0] funct3_is = 8'b1 << funct3;

   // The adder is used by both arithmetic instructions and JALR.
   assign alu_plus = in1 + in2;

   // Use a single 33 bits subtract to do subtraction and all comparisons
   wire [32:0] alu_minus = {1'b1, ~in2} + {1'b0, in1} + 33'b1;
   assign lt  = (in1[31] ^ in2[31]) ? in1[31] : alu_minus[32];
   assign ltu = alu_minus[32];
   assign eq  = (alu_minus[31:0] == 0);

   // Flip a 32 bit word. Used by the shifter
   function [31:0] flip32;
      input [31:0] x;
      flip32 = {x[ 0], x[ 1], x[ 2], x[ 3], x[ 4], x[ 5], x[ 6], x[ 7],
            x[ 8], x[ 9], x[10], x[11], x[12], x[13], x[14], x[15],
            x[16], x[17], x[18], x[19], x[20], x[21], x[22], x[23],
            x[24], x[25], x[26], x[27], x[28], x[29], x[30], x[31]};
   endfunction

   wire [31:0] shifter_in = (funct3 == 3'b001) ? flip32(in1) : in1;

   /* verilator lint_off WIDTH */
   wire [31:0] shifter =
       $signed({is_arith_shift & in1[31], shifter_in}) >>> in2[4:0];
   /* verilator lint_on WIDTH */

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

   generate
      if (WITH_RV32M) begin
         wire is_mulh   = funct3_is[1];
         wire is_mulhsu = funct3_is[2];

         wire mul_sign1 = in1[31] &  is_mulh;
         wire mul_sign2 = in2[31] & (is_mulh | is_mulhsu);

         wire signed [32:0] mul_signed1 = {mul_sign1, in1};
         wire signed [32:0] mul_signed2 = {mul_sign2, in2};
         wire signed [63:0] multiply = mul_signed1 * mul_signed2;

         wire [31:0] alu_out_muldiv =
           (  funct3_is[0]    ? multiply[31: 0] : 32'b0) | // 0:MUL
           ( |funct3_is[3:1]  ? multiply[63:32] : 32'b0) | // 1:MH, 2:MHSU, 3:MHU
           (  is_div          ? div_result      : 32'b0) ;

         assign alu_out = is_rv32m ? alu_out_muldiv : alu_out_base;
      end else begin
         assign alu_out = alu_out_base;
      end
   endgenerate

endmodule
