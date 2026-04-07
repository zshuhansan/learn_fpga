`timescale 1ns/1ps
module tb_alu;
    reg [31:0] in1;
    reg [31:0] in2;
    reg [2:0]  funct3;
    reg        is_minus;
    reg        is_arith_shift;
    reg        is_rv32m;
    reg        is_div;
    reg [31:0] div_result;

    wire [31:0] alu_out;
    wire [31:0] alu_plus;
    wire        eq;
    wire        lt;
    wire        ltu;

    alu #(
        .WITH_RV32M(1)
    ) dut (
        .in1(in1),
        .in2(in2),
        .funct3(funct3),
        .is_minus(is_minus),
        .is_arith_shift(is_arith_shift),
        .is_rv32m(is_rv32m),
        .is_div(is_div),
        .div_result(div_result),
        .alu_out(alu_out),
        .alu_plus(alu_plus),
        .eq(eq),
        .lt(lt),
        .ltu(ltu)
    );

    initial begin
        // 导出波形后，按“基本算术 → 比较 → RV32M 乘法”的顺序做烟雾测试。
        $dumpfile("alu.vcd");
        $dumpvars(0, tb_alu);
        
        in1 = 10; in2 = 20; funct3 = 3'b000; is_minus = 0; is_arith_shift = 0; is_rv32m = 0; is_div = 0; div_result = 0;
        #10;
        if (alu_out !== 30 || alu_plus !== 30) $display("FAIL: ADD"); else $display("PASS: ADD");

        in1 = 50; in2 = 20; funct3 = 3'b000; is_minus = 1;
        #10;
        if (alu_out !== 30) $display("FAIL: SUB"); else $display("PASS: SUB");

        in1 = 32'hF0F0F0F0; in2 = 32'h0F0F0F0F; funct3 = 3'b100; is_minus = 0;
        #10;
        if (alu_out !== 32'hFFFFFFFF) $display("FAIL: XOR"); else $display("PASS: XOR");

        in1 = -10; in2 = 20; funct3 = 3'b010;
        #10;
        if (alu_out !== 1 || lt !== 1) $display("FAIL: SLT"); else $display("PASS: SLT");

        in1 = 5; in2 = 6; funct3 = 3'b000; is_rv32m = 1; is_div = 0;
        #10;
        if (alu_out !== 30) $display("FAIL: MUL"); else $display("PASS: MUL");

        $display("ALL TESTS DONE.");
        $finish;
    end
endmodule
