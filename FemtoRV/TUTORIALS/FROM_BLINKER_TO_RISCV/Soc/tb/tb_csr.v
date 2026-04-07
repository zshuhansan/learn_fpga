`timescale 1ns/1ps
module tb_csr;
    reg clk;
    reg resetn;
    reg valid_inst_retired;
    reg [3:0] csrId_is;

    wire [31:0] csr_rdata;
    wire [63:0] out_cycle;
    wire [63:0] out_instret;

    csr_unit dut(
        .clk(clk),
        .resetn(resetn),
        .valid_inst_retired(valid_inst_retired),
        .csrId_is(csrId_is),
        .csr_rdata(csr_rdata),
        .out_cycle(out_cycle),
        .out_instret(out_instret)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        // 通过复位、退休脉冲和 CSR 读出三步，检查计数器更新链路是否闭合。
        $dumpfile("csr.vcd");
        $dumpvars(0, tb_csr);

        resetn = 0;
        valid_inst_retired = 0;
        csrId_is = 0;

        #15;
        resetn = 1;
        
        #10;
        if (out_cycle !== 1 || out_instret !== 0) $display("FAIL: init cycle=%d instret=%d", out_cycle, out_instret);

        #10;
        valid_inst_retired = 1;
        #10;
        valid_inst_retired = 0;
        #10;

        if (out_cycle !== 4 || out_instret !== 1) $display("FAIL: inc cycle=%d instret=%d", out_cycle, out_instret);

        csrId_is = 4'b0001;
        #1;
        if (csr_rdata !== 4) $display("FAIL: read cycle[31:0] = %d", csr_rdata);

        csrId_is = 4'b0010;
        #1;
        if (csr_rdata !== 1) $display("FAIL: read instret[31:0] = %d", csr_rdata);

        $display("ALL TESTS DONE.");
        $finish;
    end
endmodule
