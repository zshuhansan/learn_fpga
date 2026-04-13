module dataram_ip_model #(
    parameter ADDR_WIDTH = 14,
    parameter INIT_FILE = "DATARAM.hex"
) (
    input wire clk,
    // A 口负责读，行为上等价于同步读 BRAM。
    input wire ren,
    input wire [ADDR_WIDTH-1:0] raddr,
    output wire [31:0] rdata,
    // B 口负责按字节写，方便支持 SB/SH/SW。
    input wire [3:0] wmask,
    input wire [ADDR_WIDTH-1:0] waddr,
    input wire [31:0] wdata
);

`ifdef VIVADO_XPM_MEMORY

    wire [31:0] rdata_xpm;

    xpm_memory_sdpram #(
        .ADDR_WIDTH_A(ADDR_WIDTH),
        .ADDR_WIDTH_B(ADDR_WIDTH),
        .AUTO_SLEEP_TIME(0),
        .BYTE_WRITE_WIDTH_A(8),
        .CASCADE_HEIGHT(0),
        .CLOCKING_MODE("common_clock"),
        .ECC_MODE("no_ecc"),
        .MEMORY_INIT_FILE(INIT_FILE),
        .MEMORY_INIT_PARAM("0"),
        .MEMORY_OPTIMIZATION("true"),
        .MEMORY_PRIMITIVE("block"),
        .MEMORY_SIZE(32 * (1 << ADDR_WIDTH)),
        .MESSAGE_CONTROL(0),
        .READ_DATA_WIDTH_B(32),
        .READ_LATENCY_B(1),
        .READ_RESET_VALUE_B("0"),
        .RST_MODE_A("SYNC"),
        .RST_MODE_B("SYNC"),
        .SIM_ASSERT_CHK(0),
        .USE_MEM_INIT(1),
        .WAKEUP_TIME("disable_sleep"),
        .WRITE_DATA_WIDTH_A(32),
        .WRITE_MODE_B("read_first")
    ) XPM_DATARAM (
        .dbiterrb(),
        .doutb(rdata_xpm),
        .sbiterrb(),
        .addra(waddr),
        .addrb(raddr),
        .clka(clk),
        .clkb(clk),
        .dina(wdata),
        .ena(|wmask),
        .enb(ren),
        .injectdbiterra(1'b0),
        .injectsbiterra(1'b0),
        .regceb(1'b1),
        .rstb(1'b0),
        .sleep(1'b0),
        .wea(wmask)
    );

    assign rdata = rdata_xpm;

`else

    // 这是一个接近 FPGA 双口 BRAM 使用方式的行为模型：
    // 读写分口，读有 1 拍延迟，便于仿真时保持和综合后结构一致的时序感受。
    reg [31:0] mem[0:(1<<ADDR_WIDTH)-1];
    reg [31:0] rdata_r;

    assign rdata = rdata_r;

    initial begin
        // 如果给定了初始化镜像，就在仿真启动时把数据 RAM 预装好。
        if (INIT_FILE[7:0] != 8'h00) begin
            $readmemh(INIT_FILE, mem);
        end
    end

    always @(posedge clk) begin
        if (ren) begin
            // 同步读语义：本拍给地址，下拍得到数据。
            rdata_r <= mem[raddr];
        end
    end

    always @(posedge clk) begin
        // 写掩码逐字节生效，这样不需要在上层先读再改写整字。
        if (wmask[0]) mem[waddr][ 7:0 ] <= wdata[ 7:0 ];
        if (wmask[1]) mem[waddr][15:8 ] <= wdata[15:8 ];
        if (wmask[2]) mem[waddr][23:16] <= wdata[23:16];
        if (wmask[3]) mem[waddr][31:24] <= wdata[31:24];
    end

`endif

endmodule
