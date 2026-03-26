`timescale 1ns/1ps
module tb_branch_predictor;
    reg clk;
    reg resetn;
    
    // IF/ID inputs
    reg [31:0] fd_pc;
    reg [31:0] fd_instr;
    reg d_is_branch, d_is_jal, d_is_jalr;
    reg [4:0] d_rd_id, d_rs1_id;
    reg [31:0] d_bimm, d_jimm;
    reg d_flush, fd_nop;
    
    // EX inputs
    reg de_is_branch, e_take_branch;
    reg [11:0] de_bht_index;
    reg e_stall;

    // Outputs
    wire d_predict_branch;
    wire d_predict_pc_valid;
    wire [31:0] d_pc_prediction;
    wire [31:0] d_predict_ra;
    wire [11:0] d_bht_index;

    branch_predictor #(
        .BHT_ADDR_BITS(12),
        .BHT_HISTO_BITS(9),
        .WITH_GSHARE(1),
        .WITH_RAS(1)
    ) dut (
        .clk(clk),
        .resetn(resetn),
        .fd_pc(fd_pc),
        .fd_instr(fd_instr),
        .d_is_branch(d_is_branch),
        .d_is_jal(d_is_jal),
        .d_is_jalr(d_is_jalr),
        .d_rd_id(d_rd_id),
        .d_rs1_id(d_rs1_id),
        .d_bimm(d_bimm),
        .d_jimm(d_jimm),
        .d_flush(d_flush),
        .fd_nop(fd_nop),
        .d_predict_branch(d_predict_branch),
        .d_predict_pc_valid(d_predict_pc_valid),
        .d_pc_prediction(d_pc_prediction),
        .d_predict_ra(d_predict_ra),
        .d_bht_index(d_bht_index),
        .de_is_branch(de_is_branch),
        .e_take_branch(e_take_branch),
        .de_bht_index(de_bht_index),
        .e_stall(e_stall)
    );

    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end

    initial begin
        $dumpfile("bp.vcd");
        $dumpvars(0, tb_branch_predictor);

        resetn = 0;
        fd_pc = 0; fd_instr = 0;
        d_is_branch = 0; d_is_jal = 0; d_is_jalr = 0;
        d_rd_id = 0; d_rs1_id = 0; d_bimm = 0; d_jimm = 0;
        d_flush = 0; fd_nop = 0;
        de_is_branch = 0; e_take_branch = 0; de_bht_index = 0; e_stall = 0;

        #15 resetn = 1;

        // Test JAL / RAS Call
        fd_pc = 32'h1000;
        d_is_jal = 1;
        d_jimm = 32'h100;
        d_rd_id = 1; // Return address saved to ra
        fd_nop = 0;
        d_flush = 0;
        fd_instr = {12'b0, 5'd0, 3'b000, 5'd1, 7'b1101111}; // fake instr
        #5;
        if (d_pc_prediction !== 32'h1100 || !d_predict_pc_valid) $display("FAIL: JAL predict");
        #5; // cycle to allow posedge
        
        // Let it clock so RAS pushes 1004
        #5;
        d_is_jal = 0;
        #5;
        
        // Test JALR / RAS Return
        d_is_jalr = 1;
        d_rd_id = 0;
        d_rs1_id = 1;
        fd_instr = {12'b0, 5'd1, 3'b000, 5'd0, 7'b1100111}; // JALR zero, ra, 0
        #5;
        if (d_pc_prediction !== 32'h1004) $display("FAIL: JALR RAS return, expected 1004 got %h", d_pc_prediction); else $display("PASS: JALR RAS return");
        #5;
        
        $display("ALL TESTS DONE.");
        $finish;
    end
endmodule
