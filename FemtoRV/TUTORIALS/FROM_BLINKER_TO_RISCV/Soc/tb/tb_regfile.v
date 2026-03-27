`timescale 1ns/1ps
`default_nettype none
`include "ROUTE_A/regfile.v"

module tb_regfile;
    reg clk = 0;
    always #5 clk = ~clk;

    reg wen;
    reg [4:0] waddr;
    reg [31:0] wdata;
    reg [4:0] raddr1, raddr2;
    wire [31:0] rdata1, rdata2;

    regfile dut(
        .clk(clk),
        .wen(wen), .waddr(waddr), .wdata(wdata),
        .raddr1(raddr1), .raddr2(raddr2),
        .rdata1(rdata1), .rdata2(rdata2)
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
        wen = 0; waddr = 0; wdata = 0; raddr1 = 0; raddr2 = 0;

        // 写 x1
        @(negedge clk);
        wen = 1; waddr = 5'd1; wdata = 32'h1234_5678;
        @(posedge clk);
        #1;
        raddr1 = 5'd1;
        #1;
        check(rdata1 == 32'h1234_5678, "x1 写后读错误");

        // 写 x0 应被忽略
        @(negedge clk);
        wen = 1; waddr = 5'd0; wdata = 32'hFFFF_FFFF;
        @(posedge clk);
        #1;
        raddr1 = 5'd0;
        #1;
        check(rdata1 == 32'b0, "x0 必须恒为0");

        // 双读检查
        @(negedge clk);
        wen = 1; waddr = 5'd2; wdata = 32'hCAFE_BABE;
        @(posedge clk);
        #1;
        wen = 0;
        raddr1 = 5'd1;
        raddr2 = 5'd2;
        #1;
        check(rdata1 == 32'h1234_5678, "双读端口1错误");
        check(rdata2 == 32'hCAFE_BABE, "双读端口2错误");

        $display("tb_regfile PASS");
        $finish;
    end
endmodule

