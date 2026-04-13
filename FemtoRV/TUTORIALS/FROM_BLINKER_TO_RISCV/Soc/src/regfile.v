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

    // 标准 RV32I 通用寄存器堆，提供两个组合读端口和一个时序写端口。
    // x0 是硬连线零寄存器，因此任何写入都会被忽略，读取时也始终返回 0。

    reg [31:0] regs[0:31];

    (* max_fanout = 32 *) wire [31:0] rdata1_int;
    (* max_fanout = 32 *) wire [31:0] rdata2_int;

    always @(posedge clk) begin
        if (wen && (waddr != 5'd0)) begin
            // 只有写使能有效且目标寄存器不是 x0 时才真正更新寄存器内容。
            regs[waddr] <= wdata;
        end
        // 即便仿真中曾被未知值污染，也在每拍强制把 x0 拉回 0。
        regs[0] <= 32'b0;
    end

    // 读口是异步的，地址变化后数据会直接从数组反映出来。
    assign rdata1_int = (raddr1 == 5'd0) ? 32'b0 : regs[raddr1];
    assign rdata2_int = (raddr2 == 5'd0) ? 32'b0 : regs[raddr2];

    assign rdata1 = rdata1_int;
    assign rdata2 = rdata2_int;

endmodule
