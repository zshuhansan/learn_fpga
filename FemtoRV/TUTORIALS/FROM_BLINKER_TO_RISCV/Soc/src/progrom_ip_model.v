module progrom_ip_model #(
    parameter ADDR_WIDTH = 14,
    parameter INIT_FILE = "PROGROM.hex"
) (
    input wire clk,
    input wire en,
    input wire [ADDR_WIDTH-1:0] addr,
    output wire [31:0] dout
);

`ifdef VIVADO_XPM_MEMORY

    wire [31:0] dout_xpm;

    xpm_memory_sprom #(
        .ADDR_WIDTH_A(ADDR_WIDTH),
        .AUTO_SLEEP_TIME(0),
        .CASCADE_HEIGHT(0),
        .ECC_MODE("no_ecc"),
        .MEMORY_INIT_FILE(INIT_FILE),
        .MEMORY_INIT_PARAM("0"),
        .MEMORY_OPTIMIZATION("true"),
        .MEMORY_PRIMITIVE("block"),
        .MEMORY_SIZE(32 * (1 << ADDR_WIDTH)),
        .MESSAGE_CONTROL(0),
        .READ_DATA_WIDTH_A(32),
        .READ_LATENCY_A(1),
        .READ_RESET_VALUE_A("0"),
        .RST_MODE_A("SYNC"),
        .SIM_ASSERT_CHK(0),
        .USE_MEM_INIT(1),
        .WAKEUP_TIME("disable_sleep")
    ) XPM_PROGROM (
        .dbiterra(),
        .douta(dout_xpm),
        .sbiterra(),
        .addra(addr),
        .clka(clk),
        .ena(en),
        .injectdbiterra(1'b0),
        .injectsbiterra(1'b0),
        .regcea(1'b1),
        .rsta(1'b0),
        .sleep(1'b0)
    );

    assign dout = dout_xpm;

`else

    // 程序 ROM 的行为模型。和很多 FPGA ROM IP 一样，这里使用同步读接口，
    // 这样取指阶段在仿真里能看到更接近真实硬件的一拍延迟。
    reg [31:0] mem[0:(1<<ADDR_WIDTH)-1];
    reg [31:0] dout_r;

    assign dout = dout_r;

    initial begin
        // 从外部 HEX 文件装载指令镜像，便于软件与硬件分离构建。
        if (INIT_FILE[7:0] != 8'h00) begin
            $readmemh(INIT_FILE, mem);
        end
    end

    always @(posedge clk) begin
        if (en) begin
            // 读使能有效时输出当前地址对应的 32 位指令字。
            dout_r <= mem[addr];
        end
    end

`endif

endmodule
