`default_nettype none

module regfile (
    input  wire        clk,
    input  wire        wen,
    input  wire [4:0]  waddr,
    input  wire [31:0] wdata,
    input  wire [4:0]  raddr1,
    input  wire [4:0]  raddr2,
    output wire [31:0] rdata1,
    output wire [31:0] rdata2
);

    // 中文说明：
    // - 32x32 通用寄存器堆
    // - 异步双读，同步单写
    // - x0 恒为 0（写入 x0 被忽略，读 x0 永远返回 0）

    reg [31:0] regs[0:31];

    always @(posedge clk) begin
        if (wen && (waddr != 5'd0)) begin
            regs[waddr] <= wdata;
        end
        regs[0] <= 32'b0;
    end

    assign rdata1 = (raddr1 == 5'd0) ? 32'b0 : regs[raddr1];
    assign rdata2 = (raddr2 == 5'd0) ? 32'b0 : regs[raddr2];

endmodule

