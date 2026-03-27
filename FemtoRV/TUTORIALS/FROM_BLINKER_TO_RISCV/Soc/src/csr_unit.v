module csr_unit (
    input  wire        clk,
    input  wire        resetn,
    
    // Increment triggers
    input  wire        valid_inst_retired,

    // Read interface
    input  wire [3:0]  csrId_is,
    output wire [31:0] csr_rdata,

    // For BENCH/simulation
    output wire [63:0] out_cycle,
    output wire [63:0] out_instret
);

    reg [63:0] cycle;
    reg [63:0] instret;

    always @(posedge clk) begin
        if (!resetn) begin
            cycle <= 64'b0;
            instret <= 64'b0;
        end else begin
            cycle <= cycle + 64'b1;
            if (valid_inst_retired) begin
                instret <= instret + 64'b1;
            end
        end
    end

    assign csr_rdata =
        (csrId_is[0] ? cycle[31:0]    : 32'b0) |
        (csrId_is[2] ? cycle[63:32]   : 32'b0) |
        (csrId_is[1] ? instret[31:0]  : 32'b0) |
        (csrId_is[3] ? instret[63:32] : 32'b0) ;

    assign out_cycle = cycle;
    assign out_instret = instret;

endmodule
