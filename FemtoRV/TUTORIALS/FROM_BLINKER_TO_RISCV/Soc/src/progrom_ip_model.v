module progrom_ip_model #(
    parameter ADDR_WIDTH = 14,
    parameter INIT_FILE = "PROGROM.hex"
) (
    input wire clk,
    input wire en,
    input wire [ADDR_WIDTH-1:0] addr,
    output reg [31:0] dout
);

    // 程序 ROM 的行为模型。和很多 FPGA ROM IP 一样，这里使用同步读接口，
    // 这样取指阶段在仿真里能看到更接近真实硬件的一拍延迟。
    reg [31:0] mem[0:(1<<ADDR_WIDTH)-1];

    initial begin
        // 从外部 HEX 文件装载指令镜像，便于软件与硬件分离构建。
        if (INIT_FILE[7:0] != 8'h00) begin
            $readmemh(INIT_FILE, mem);
        end
    end

    always @(posedge clk) begin
        if (en) begin
            // 读使能有效时输出当前地址对应的 32 位指令字。
            dout <= mem[addr];
        end
    end

endmodule
