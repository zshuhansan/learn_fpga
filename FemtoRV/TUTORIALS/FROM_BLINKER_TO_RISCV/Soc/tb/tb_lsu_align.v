`timescale 1ns/1ps
`default_nettype none
`include "ROUTE_A/lsu_align.v"

module tb_lsu_align;
    reg  [31:0] addr;
    reg  [2:0]  funct3;
    reg  [31:0] store_rs2;
    reg  [31:0] load_raw_word;
    wire [31:0] store_data;
    wire [3:0]  store_wmask;
    wire [31:0] load_data;

    lsu_align dut(
        .addr(addr), .funct3(funct3), .store_rs2(store_rs2), .load_raw_word(load_raw_word),
        .store_data(store_data), .store_wmask(store_wmask), .load_data(load_data)
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
        // 写路径主要检查字节掩码，读路径主要检查切片位置与符号扩展是否一致。
        store_rs2 = 32'hA1B2_C3D4;
        load_raw_word = 32'h1122_3344;

        addr = 32'h0000_0002; funct3 = 3'b000; #1;
        check(store_wmask == 4'b0100, "SB wmask 错误");

        addr = 32'h0000_0002; funct3 = 3'b001; #1;
        check(store_wmask == 4'b1100, "SH wmask 错误");

        addr = 32'h0000_0000; funct3 = 3'b010; #1;
        check(store_wmask == 4'b1111, "SW wmask 错误");

        addr = 32'h0000_0001; funct3 = 3'b000; #1;
        check(load_data == 32'h0000_0033, "LB 提取错误");

        addr = 32'h0000_0002; funct3 = 3'b101; #1;
        check(load_data == 32'h0000_1122, "LHU 错误");

        $display("tb_lsu_align PASS");
        $finish;
    end
endmodule
