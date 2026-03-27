module branch_predictor #(
    parameter BHT_ADDR_BITS = 12,
    parameter BHT_HISTO_BITS = 9,
    parameter WITH_GSHARE = 1,
    parameter WITH_RAS = 1
) (
    input  wire        clk,
    input  wire        resetn,
    
    // IF/ID Stage (Prediction Request)
    input  wire [31:0] fd_pc,
    input  wire [31:0] fd_instr,
    input  wire        d_is_branch,
    input  wire        d_is_jal,
    input  wire        d_is_jalr,
    input  wire [4:0]  d_rd_id,
    input  wire [4:0]  d_rs1_id,
    input  wire [31:0] d_bimm,
    input  wire [31:0] d_jimm,
    input  wire        d_flush,
    input  wire        fd_nop,

    output wire        d_predict_branch,
    output wire        d_predict_pc_valid,
    output wire [31:0] d_pc_prediction,
    output wire [31:0] d_predict_ra,
    output wire [BHT_ADDR_BITS-1:0] d_bht_index,

    // EX Stage (Prediction Update)
    input  wire        de_is_branch,
    input  wire        e_take_branch,
    input  wire [BHT_ADDR_BITS-1:0] de_bht_index,
    input  wire        e_stall
);

    localparam BHT_SIZE = 1 << BHT_ADDR_BITS;
    
    reg [BHT_HISTO_BITS-1:0] branch_history;
    reg [1:0] BHT [BHT_SIZE-1:0];

    // RAS registers
    reg [31:0] ras_0, ras_1, ras_2, ras_3;

    // GSHARE BHT Index function
    /* verilator lint_off WIDTH */
    wire [BHT_ADDR_BITS-1:0] bht_index = fd_pc[BHT_ADDR_BITS+1:2] ^ (branch_history << (BHT_ADDR_BITS - BHT_HISTO_BITS));
    /* verilator lint_on WIDTH */
    assign d_bht_index = bht_index;

    // Branch Prediction
    generate
        if (WITH_GSHARE) begin
            assign d_predict_branch = BHT[bht_index][1];
        end else begin
            // BTFNT
            assign d_predict_branch = fd_instr[31];
        end
    endgenerate

    // PC Prediction and Valid flag
    generate
        if (WITH_RAS) begin
            assign d_predict_ra = ras_0;
            assign d_predict_pc_valid = !fd_nop && (d_is_jal || (fd_instr[6:4] == 3'b110 && (fd_instr[2] | d_predict_branch)));
            assign d_pc_prediction = (fd_instr[3:2] == 2'b01) ? ras_0 : (fd_pc + (d_is_jal ? d_jimm : d_bimm));
        end else begin
            assign d_predict_ra = 32'b0;
            assign d_predict_pc_valid = !fd_nop && (d_is_jal || (d_is_branch && d_predict_branch));
            assign d_pc_prediction = (fd_pc + (d_is_jal ? d_jimm : d_bimm));
        end
    endgenerate

    // State Updates
    function [1:0] incdec_sat;
        input [1:0] prev;
        input dir;
        incdec_sat = 
            {dir, prev} == 3'b000 ? 2'b00 :
            {dir, prev} == 3'b001 ? 2'b00 :
            {dir, prev} == 3'b010 ? 2'b01 :
            {dir, prev} == 3'b011 ? 2'b10 :
            {dir, prev} == 3'b100 ? 2'b01 :
            {dir, prev} == 3'b101 ? 2'b10 :
            {dir, prev} == 3'b110 ? 2'b11 :
                                    2'b11 ;
    endfunction

    integer i;
    always @(posedge clk) begin
        if (!resetn) begin
            branch_history <= 0;
            ras_0 <= 0; ras_1 <= 0; ras_2 <= 0; ras_3 <= 0;
            // Note: synthesis tools generally ignore this or handle it correctly if initialized properly.
`ifdef CONFIG_INITIALIZE
            for (i = 0; i < BHT_SIZE; i = i + 1) begin
                BHT[i] <= 2'b01; // Weakly taken
            end
`endif
        end else begin
            if (WITH_GSHARE && de_is_branch && !e_stall) begin
                branch_history <= {e_take_branch, branch_history[BHT_HISTO_BITS-1:1]};
                BHT[de_bht_index] <= incdec_sat(BHT[de_bht_index], e_take_branch);
            end

            if (WITH_RAS && !fd_nop && !d_flush) begin
                if (d_is_jal && d_rd_id == 1) begin
                    ras_3 <= ras_2;
                    ras_2 <= ras_1;
                    ras_1 <= ras_0;
                    ras_0 <= fd_pc + 4;
                end
                if (d_is_jalr && d_rd_id == 0 && (d_rs1_id == 1 || d_rs1_id == 5)) begin
                    ras_0 <= ras_1;
                    ras_1 <= ras_2;
                    ras_2 <= ras_3;
                end
            end
        end
    end

endmodule
