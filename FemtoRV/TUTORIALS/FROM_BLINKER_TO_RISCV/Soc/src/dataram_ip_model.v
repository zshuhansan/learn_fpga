module dataram_ip_model #(
    parameter ADDR_WIDTH = 14,
    parameter INIT_FILE = "DATARAM.hex"
) (
    input wire clk,
    // A 口负责读，行为上等价于同步读 BRAM。
    input wire ren,
    input wire [ADDR_WIDTH-1:0] raddr,
    output reg [31:0] rdata,
    // B 口负责按字节写，方便支持 SB/SH/SW。
    input wire [3:0] wmask,
    input wire [ADDR_WIDTH-1:0] waddr,
    input wire [31:0] wdata
);

    // 这是一个接近 FPGA 双口 BRAM 使用方式的行为模型：
    // 读写分口，读有 1 拍延迟，便于仿真时保持和综合后结构一致的时序感受。
    reg [31:0] mem[0:(1<<ADDR_WIDTH)-1];

    initial begin
        // 如果给定了初始化镜像，就在仿真启动时把数据 RAM 预装好。
        if (INIT_FILE[7:0] != 8'h00) begin
            $readmemh(INIT_FILE, mem);
        end
    end

    always @(posedge clk) begin
        if (ren) begin
            // 同步读语义：本拍给地址，下拍得到数据。
            rdata <= mem[raddr];
        end
    end

    always @(posedge clk) begin
        // 写掩码逐字节生效，这样不需要在上层先读再改写整字。
        if (wmask[0]) mem[waddr][ 7:0 ] <= wdata[ 7:0 ];
        if (wmask[1]) mem[waddr][15:8 ] <= wdata[15:8 ];
        if (wmask[2]) mem[waddr][23:16] <= wdata[23:16];
        if (wmask[3]) mem[waddr][31:24] <= wdata[31:24];
    end

endmodule
