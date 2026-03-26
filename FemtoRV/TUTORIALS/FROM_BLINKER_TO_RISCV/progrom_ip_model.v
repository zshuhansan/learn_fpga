module progrom_ip_model #(
    parameter ADDR_WIDTH = 14,
    parameter INIT_FILE = "PROGROM.hex"
) (
    input wire clk,
    input wire en,
    input wire [ADDR_WIDTH-1:0] addr,
    output reg [31:0] dout
);

    // Xilinx BRAM IP-like model for Instruction ROM
    // 1-cycle read latency
    reg [31:0] mem[0:(1<<ADDR_WIDTH)-1];

    initial begin
        if (INIT_FILE != "") begin
            $readmemh(INIT_FILE, mem);
        end
    end

    always @(posedge clk) begin
        if (en) begin
            dout <= mem[addr];
        end
    end

endmodule
