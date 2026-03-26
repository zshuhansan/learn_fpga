module dataram_ip_model #(
    parameter ADDR_WIDTH = 14,
    parameter INIT_FILE = "DATARAM.hex"
) (
    input wire clk,
    // Port A: Read
    input wire ren,
    input wire [ADDR_WIDTH-1:0] raddr,
    output reg [31:0] rdata,
    // Port B: Write
    input wire [3:0] wmask,
    input wire [ADDR_WIDTH-1:0] waddr,
    input wire [31:0] wdata
);

    // Xilinx True Dual-Port BRAM IP-like model for Data RAM
    // Port A for read, Port B for write
    // 1-cycle latency
    reg [31:0] mem[0:(1<<ADDR_WIDTH)-1];

    initial begin
        if (INIT_FILE != "") begin
            $readmemh(INIT_FILE, mem);
        end
    end

    // Port A: Read
    always @(posedge clk) begin
        if (ren) begin
            rdata <= mem[raddr];
        end
    end

    // Port B: Write
    always @(posedge clk) begin
        if (wmask[0]) mem[waddr][ 7:0 ] <= wdata[ 7:0 ];
        if (wmask[1]) mem[waddr][15:8 ] <= wdata[15:8 ];
        if (wmask[2]) mem[waddr][23:16] <= wdata[23:16];
        if (wmask[3]) mem[waddr][31:24] <= wdata[31:24];
    end

endmodule
