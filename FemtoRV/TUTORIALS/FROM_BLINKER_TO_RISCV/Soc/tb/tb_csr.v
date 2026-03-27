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
        $dumpfile("csr.vcd");
        $dumpvars(0, tb_csr);

        resetn = 0;
        valid_inst_retired = 0;
        csrId_is = 0;

        #15;
        resetn = 1; // Unreset at t=15
        
        #10;
        if (out_cycle !== 1 || out_instret !== 0) $display("FAIL: init cycle=%d instret=%d", out_cycle, out_instret);

        #10; // cycle 2
        valid_inst_retired = 1;
        #10; // cycle 3, instret 1
        valid_inst_retired = 0;
        #10; // cycle 4, instret 1

        if (out_cycle !== 4 || out_instret !== 1) $display("FAIL: inc cycle=%d instret=%d", out_cycle, out_instret);

        // test reads
        csrId_is = 4'b0001; // cycle[31:0]
        #1;
        if (csr_rdata !== 4) $display("FAIL: read cycle[31:0] = %d", csr_rdata);

        csrId_is = 4'b0010; // instret[31:0]
        #1;
        if (csr_rdata !== 1) $display("FAIL: read instret[31:0] = %d", csr_rdata);

        $display("ALL TESTS DONE.");
        $finish;
    end
endmodule
