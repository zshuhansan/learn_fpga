module hazard_unit (
    // Inputs from Decode stage (current instruction)
    input  wire        d_is_load,
    input  wire        d_is_store,
    input  wire        d_reads_rs1,
    input  wire        d_reads_rs2,
    input  wire [4:0]  d_rs1_id,
    input  wire [4:0]  d_rs2_id,
    input  wire        fd_nop,

    // Inputs from Execute stage (previous instruction)
    input  wire [4:0]  de_rd_id,
    input  wire        de_is_load,
    input  wire        de_is_csrrs,
    input  wire        de_is_store,
    input  wire        e_correct_pc,
    input  wire        alu_busy,

    // Inputs for Halt
    input  wire        halt,

    // Outputs for Stalls and Flushes
    output wire        f_stall,
    output wire        d_stall,
    output wire        e_stall,
    output wire        d_flush,
    output wire        e_flush,
    output wire        m_flush,

    // Outputs for Data Hazard indicator
    output wire        data_hazard,
    output wire        rs1_hazard,
    output wire        rs2_hazard
);

    assign rs1_hazard = d_reads_rs1 && (d_rs1_id == de_rd_id) && (d_rs1_id != 0);
    assign rs2_hazard = d_reads_rs2 && (d_rs2_id == de_rd_id) && (d_rs2_id != 0);

    // Hazard condition: load-use or csr-use or special case (D_isLoad && DE_isStore)
    assign data_hazard = !fd_nop && (
        ((de_is_load || de_is_csrrs) && (rs1_hazard || rs2_hazard)) ||
        (d_is_load && de_is_store)
    );

    assign f_stall = alu_busy | data_hazard | halt;
    assign d_stall = alu_busy | data_hazard | halt;
    assign e_stall = alu_busy;

    assign d_flush = e_correct_pc;
    assign e_flush = e_correct_pc | data_hazard;
    assign m_flush = alu_busy;

endmodule
