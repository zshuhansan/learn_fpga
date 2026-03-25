`timescale 1ns/1ps
`default_nettype none
`include "ROUTE_A/div_unit.v"

module tb_div_unit;
    reg clk = 0;
    always #5 clk = ~clk;

    reg resetn = 0;
    reg start;
    reg is_div;
    reg [2:0] funct3;
    reg [31:0] rs1, rs2;
    wire busy, finished, sign_flag;
    wire [31:0] quotient_abs, remainder_abs, result;

    div_unit dut(
        .clk(clk),
        .resetn(resetn),
        .start(start),
        .is_div(is_div),
        .funct3(funct3),
        .rs1(rs1),
        .rs2(rs2),
        .busy(busy),
        .finished(finished),
        .quotient_abs(quotient_abs),
        .remainder_abs(remainder_abs),
        .sign_flag(sign_flag),
        .result(result)
    );

    task check;
        input cond;
        input [255:0] msg;
        begin
            if(!cond) begin
                $display("FAIL: %0s", msg);
                $fatal;
            end
        end
    endtask

    task run_div_case;
        input [2:0] f3;
        input [31:0] a;
        input [31:0] b;
        input [31:0] expect_val;
        begin
            @(negedge clk);
            funct3 = f3;
            rs1 = a;
            rs2 = b;
            is_div = 1'b1;
            start = 1'b1;
            @(posedge clk);
            @(negedge clk);
            start = 1'b0;
            wait(finished == 1'b1);
            #1;
            check(result == expect_val, "div_unit 结果错误");
            @(posedge clk);
        end
    endtask

    initial begin
        start = 0; is_div = 0; funct3 = 0; rs1 = 0; rs2 = 1;
        repeat(2) @(posedge clk);
        resetn = 1;

        // DIV 100 / 7 = 14
        run_div_case(3'b100, 32'd100, 32'd7, 32'd14);
        // REM 100 % 7 = 2
        run_div_case(3'b110, 32'd100, 32'd7, 32'd2);
        // DIV -100 / 7 = -14
        run_div_case(3'b100, -32'sd100, 32'd7, -32'sd14);

        $display("tb_div_unit PASS");
        $finish;
    end
endmodule
