

module axi_ddr_model #(
    parameter MEM_SIZE = 1024 * 1024
) (
    input  wire        clk,
    input  wire        resetn,

    // 这里只实现了 ICache 回填所需的 AXI 只读从接口。
    input  wire [31:0] axi_araddr,
    input  wire [7:0]  axi_arlen,
    input  wire [2:0]  axi_arsize,
    input  wire [1:0]  axi_arburst,
    input  wire        axi_arvalid,
    output wire        axi_arready,
    
    output wire [31:0] axi_rdata,
    output wire [1:0]  axi_rresp,
    output wire        axi_rlast,
    output wire        axi_rvalid,
    input  wire        axi_rready
);

    reg [31:0] memory [0:(MEM_SIZE/4)-1];

    // 启动时先清零，再把程序镜像装进内存，模拟 DDR 中已经存在可取指内容。
    integer i;
    initial begin
        for (i = 0; i < MEM_SIZE/4; i = i + 1) begin
            memory[i] = 0;
        end
        $readmemh("PROGROM.hex", memory);
    end

    // 状态机故意加了一段固定等待，用来近似模拟外部存储的访问延迟。
    localparam IDLE = 2'd0;
    localparam WAIT_LATENCY = 2'd1;
    localparam BURST_READ = 2'd2;

    reg [1:0] state;
    reg [31:0] current_addr;
    reg [7:0]  burst_left;
    reg [3:0]  latency_cnt;

    always @(posedge clk) begin
        if (!resetn) begin
            state <= IDLE;
            current_addr <= 0;
            burst_left <= 0;
            latency_cnt <= 0;
        end else begin
            case (state)
                IDLE: begin
                    if (axi_arvalid && axi_arready) begin
                        state <= WAIT_LATENCY;
                        current_addr <= axi_araddr;
                        burst_left <= axi_arlen;
                        latency_cnt <= 4'd10;
                    end
                end
                
                WAIT_LATENCY: begin
                    if (latency_cnt == 0) begin
                        state <= BURST_READ;
                    end else begin
                        latency_cnt <= latency_cnt - 1;
                    end
                end
                
                BURST_READ: begin
                    if (axi_rvalid && axi_rready) begin
                        if (burst_left == 0) begin
                            state <= IDLE;
                        end else begin
                            burst_left <= burst_left - 1;
                            current_addr <= current_addr + 4;
                        end
                    end
                end
            endcase
        end
    end

    assign axi_arready = (state == IDLE);
    assign axi_rvalid  = (state == BURST_READ);
    // 地址按字对齐访问，每拍返回一个 32 位数据 beat。
    assign axi_rdata   = memory[current_addr[19:2]];
    assign axi_rresp   = 2'b00;
    assign axi_rlast   = (state == BURST_READ) && (burst_left == 0);

endmodule
