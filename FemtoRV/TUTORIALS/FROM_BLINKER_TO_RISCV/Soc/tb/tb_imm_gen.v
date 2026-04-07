`timescale 1ns/1ps
`default_nettype none
`include "ROUTE_A/imm_gen.v"

module tb_imm_gen;
    reg  [31:0] instr;
    reg         is_store;
    wire [31:0] imm_i, imm_s, imm_b, imm_u, imm_j, i_or_s_imm;

    imm_gen dut(
        .instr(instr),
        .is_store(is_store),
        .imm_i(imm_i), .imm_s(imm_s), .imm_b(imm_b), .imm_u(imm_u), .imm_j(imm_j),
        .i_or_s_imm(i_or_s_imm)
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

    initial begin
        // 这组用例覆盖 I/S/B/U/J 五种立即数格式，重点检查位拼接和符号扩展是否正确。
        instr = 32'hFFF00093;
        is_store = 1'b0;
        #1;
        check(imm_i == 32'hFFFF_FFFF, "Iimm 符号扩展错误");
        check(i_or_s_imm == 32'hFFFF_FFFF, "i_or_s_imm(I) 错误");

        instr = 32'h0021A423;
        is_store = 1'b1;
        #1;
        check(imm_s == 32'd8, "Simm 错误");
        check(i_or_s_imm == 32'd8, "i_or_s_imm(S) 错误");

        instr = 32'h00208463;
        #1;
        check(imm_b == 32'd8, "Bimm 错误");

        instr = 32'h00010517;
        #1;
        check(imm_u == 32'h0001_0000, "Uimm 错误");

        instr = 32'h008000EF;
        #1;
        check(imm_j == 32'd8, "Jimm 错误");

        $display("tb_imm_gen PASS");
        $finish;
    end
endmodule
