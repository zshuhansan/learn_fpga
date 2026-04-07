module csr_unit (
    input  wire        clk,
    input  wire        resetn,
    
    // 当一条指令真正退休时拉高，用来累加 instret。
    input  wire        valid_inst_retired,

    // 这里用 onehot 形式选择最小 CSR 子集中的哪一个 32 位片段。
    input  wire [3:0]  csrId_is,
    output wire [31:0] csr_rdata,

    // 对外导出完整 64 位计数器，便于仿真和统计。
    output wire [63:0] out_cycle,
    output wire [63:0] out_instret
);

    // 当前实现只维护 cycle 和 instret 两组只读 CSR，已经足够支撑软件统计与简单系统调用。
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

    // 通过 onehot 选择把 64 位 CSR 拆成上下两个 32 位窗口返回。
    assign csr_rdata =
        (csrId_is[0] ? cycle[31:0]    : 32'b0) |
        (csrId_is[2] ? cycle[63:32]   : 32'b0) |
        (csrId_is[1] ? instret[31:0]  : 32'b0) |
        (csrId_is[3] ? instret[63:32] : 32'b0) ;

    assign out_cycle = cycle;
    assign out_instret = instret;

endmodule
