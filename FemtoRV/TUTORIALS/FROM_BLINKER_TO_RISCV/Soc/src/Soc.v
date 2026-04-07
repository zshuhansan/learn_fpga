`ifndef DISABLE_BP
`define CONFIG_PC_PREDICT
`define CONFIG_RAS
`define CONFIG_GSHARE
`endif
`define CONFIG_RV32M
`ifndef CPU_FREQ
`define CPU_FREQ 10
`endif
`include "emitter_uart.v"
`include "processor.v"
`include "progrom_ip_model.v"
`include "dataram_ip_model.v"
`include "icache.v"
`include "axi_ddr_model.v"

module Soc (
    input  wire          CLK,
    input  wire          RESET,
    output reg  [4:0]    LEDS,
    input  wire          RXD,
    output wire          TXD,
    output wire          HALT
);

   wire clk = CLK;
   wire resetn = RESET;

   // SoC 顶层把处理器、指令存储、数据 RAM 和若干 MMIO 外设接在一起。
   // 这里约定 RESET 已经是低有效复位后的信号命名风格，即 resetn。

   // 处理器取指接口。
   wire [31:0] inst_addr;
   wire [31:0] inst_rdata;
   wire        inst_en;
   wire        inst_valid;
   wire        inst_ready;

   // 处理器数据访问接口，读写地址分别独立导出。
   wire [31:0] data_raddr;
   wire [31:0] data_rdata;
   wire [31:0] data_waddr;
   wire [31:0] data_wdata;
   wire [3:0]  data_wmask;
   wire        data_wen;

   // 访存地址落在 IO 空间时，经由这组信号访问外设寄存器。
   wire [31:0] IO_mem_addr;
   wire [31:0] IO_mem_rdata;
   wire [31:0] IO_mem_wdata;
   wire        IO_mem_wr;

   processor CPU(
      .clk(clk),
      .resetn(resetn),
      
      .inst_addr(inst_addr),
      .inst_rdata(inst_rdata),
      .inst_en(inst_en),
      .inst_valid(inst_valid),
      .inst_ready(inst_ready),
      
      .data_raddr(data_raddr),
      .data_rdata(data_rdata),
      .data_waddr(data_waddr),
      .data_wdata(data_wdata),
      .data_wmask(data_wmask),
      .data_wen(data_wen),

      .IO_mem_addr(IO_mem_addr),
      .IO_mem_rdata(IO_mem_rdata),
      .IO_mem_wdata(IO_mem_wdata),
      .IO_mem_wr(IO_mem_wr),
      .halt(HALT)
   );

   // 当启用 ICache 时，取指 miss 会通过 AXI 只读通道去 DDR 模型回填。
   wire [31:0] axi_araddr;
   wire [7:0]  axi_arlen;
   wire [2:0]  axi_arsize;
   wire [1:0]  axi_arburst;
   wire        axi_arvalid;
   wire        axi_arready;
   wire [31:0] axi_rdata;
   wire [1:0]  axi_rresp;
   wire        axi_rlast;
   wire        axi_rvalid;
   wire        axi_rready;

`ifdef BYPASS_ICACHE
   reg inst_valid_bypass = 1'b0;

   always @(posedge clk) begin
      if(!resetn) begin
         inst_valid_bypass <= 1'b0;
      end else begin
         inst_valid_bypass <= inst_en;
      end
   end

   progrom_ip_model #(
      .ADDR_WIDTH(14),
      .INIT_FILE("PROGROM.hex")
   ) PROGROM (
      .clk(clk),
      .en(inst_en),
      .addr(inst_addr[15:2]),
      .dout(inst_rdata)
   );

   assign inst_valid   = inst_valid_bypass;
   assign inst_ready   = 1'b1;
   assign axi_araddr   = 32'b0;
   assign axi_arlen    = 8'b0;
   assign axi_arsize   = 3'b0;
   assign axi_arburst  = 2'b0;
   assign axi_arvalid  = 1'b0;
   assign axi_arready  = 1'b0;
   assign axi_rdata    = 32'b0;
   assign axi_rresp    = 2'b0;
   assign axi_rlast    = 1'b0;
   assign axi_rvalid   = 1'b0;
   assign axi_rready   = 1'b0;
`else
   // 正常路径下由 ICache 接管取指请求，对处理器表现为 ready/valid 风格接口。
   icache ICACHE (
      .clk(clk),
      .resetn(resetn),
      .cpu_req_valid(inst_en),
      .cpu_req_en(1'b1),
      .cpu_req_addr(inst_addr),
      .cpu_req_ready(inst_ready),
      .cpu_resp_data(inst_rdata),
      .cpu_resp_valid(inst_valid),
      
      .axi_araddr(axi_araddr),
      .axi_arlen(axi_arlen),
      .axi_arsize(axi_arsize),
      .axi_arburst(axi_arburst),
      .axi_arvalid(axi_arvalid),
      .axi_arready(axi_arready),
      .axi_rdata(axi_rdata),
      .axi_rresp(axi_rresp),
      .axi_rlast(axi_rlast),
      .axi_rvalid(axi_rvalid),
      .axi_rready(axi_rready)
   );
`endif

`ifndef BYPASS_ICACHE
   `ifdef BENCH
   // 仿真模式下，ICache miss 访问的是一个简单的 AXI DDR 行为模型。
   axi_ddr_model #(
      .MEM_SIZE(1024 * 1024)
   ) AXI_DDR (
      .clk(clk),
      .resetn(resetn),
      .axi_araddr(axi_araddr),
      .axi_arlen(axi_arlen),
      .axi_arsize(axi_arsize),
      .axi_arburst(axi_arburst),
      .axi_arvalid(axi_arvalid),
      .axi_arready(axi_arready),
      .axi_rdata(axi_rdata),
      .axi_rresp(axi_rresp),
      .axi_rlast(axi_rlast),
      .axi_rvalid(axi_rvalid),
      .axi_rready(axi_rready)
   );
   `endif
`endif

   // 数据 RAM 映射在 0x0001_0000 一带，所以先把字地址换算成局部索引。
   wire [14:0] DATARAM_raddr_word = data_raddr[16:2] - 15'h4000;
   wire [14:0] DATARAM_waddr_word = data_waddr[16:2] - 15'h4000;
   wire [13:0] DATARAM_raddr = DATARAM_raddr_word[13:0];
   wire [13:0] DATARAM_waddr = DATARAM_waddr_word[13:0];

   dataram_ip_model #(
      .ADDR_WIDTH(14),
      .INIT_FILE("DATARAM.hex")
   ) DATARAM (
      .clk(clk),
      .ren(1'b1),
      .raddr(DATARAM_raddr),
      .rdata(data_rdata),
      .wmask(data_wmask),
      .waddr(DATARAM_waddr),
      .wdata(data_wdata)
   );

   // MMIO 使用字地址 one-hot 解码，每个 bit 对应一个寄存器槽位。
   wire [13:0] IO_wordaddr = IO_mem_addr[15:2];

   localparam IO_LEDS_bit      = 0;
   localparam IO_UART_DAT_bit  = 1;
   localparam IO_UART_CNTL_bit = 2;
   localparam IO_MBOX0_bit     = 3;
   localparam IO_MBOX1_bit     = 4;
   localparam IO_MBOX2_bit     = 5;
   localparam IO_MBOXD_bit     = 6;
   always @(posedge clk) begin
      // LED 是最简单的内存映射输出，写入低 5 位即可更新板载灯状态。
      if(IO_mem_wr & IO_wordaddr[IO_LEDS_bit]) begin
         LEDS <= IO_mem_wdata[4:0];
      end
   end

   // UART 数据寄存器采用“写即发送”的语义。
   wire uart_valid = IO_mem_wr & IO_wordaddr[IO_UART_DAT_bit];
   wire uart_ready;

   emitter_uart #(
      .clk_freq_hz(`CPU_FREQ*1000000)
   ) UART(
      .i_clk(clk),
      .i_rst(!resetn),
      .i_data(IO_mem_wdata[7:0]),
      .i_valid(uart_valid),
      .o_ready(uart_ready),
      .o_uart_tx(TXD)
   );

`ifdef FAST_UART_STATUS
   assign IO_mem_rdata =
                    IO_wordaddr[IO_UART_CNTL_bit] ? 32'b0 : 32'b0;
`else
   // 状态寄存器当前只返回 UART busy 位，软件可据此轮询发送是否完成。
   assign IO_mem_rdata =
                    IO_wordaddr[IO_UART_CNTL_bit] ? { 22'b0, !uart_ready, 9'b0}
                                                  : 32'b0;
`endif

`ifdef BENCH
   reg [31:0] MBOX0;
   reg [31:0] MBOX1;
   reg [31:0] MBOX2;
   always @(posedge clk) begin
      // 仿真时把 UART 输出直接打印到终端，便于观察软件运行过程。
      if(uart_valid) begin
`ifdef QUIET_UART_OUTPUT
`elsif CONFIG_DEBUG
         $display("UART: %c", IO_mem_wdata[7:0]);
`else
         $write("%c", IO_mem_wdata[7:0] );
         $fflush(32'h8000_0001);
`endif
      end
   end

   always @(posedge clk) begin
      if(resetn) begin
         // Mailbox 用于软件把 benchmark 统计量回写给仿真环境。
         if(IO_mem_wr & IO_wordaddr[IO_MBOX0_bit]) MBOX0 <= IO_mem_wdata;
         if(IO_mem_wr & IO_wordaddr[IO_MBOX1_bit]) MBOX1 <= IO_mem_wdata;
         if(IO_mem_wr & IO_wordaddr[IO_MBOX2_bit]) MBOX2 <= IO_mem_wdata;
         if(IO_mem_wr & IO_wordaddr[IO_MBOXD_bit]) begin
            $display("COREMARK_TICKS=%0d", MBOX0);
            $display("COREMARK_MHZ_X1000=%0d", MBOX1);
            $display("COREMARK_CPI_X1000=%0d", MBOX2);
            $finish;
         end
      end
   end
`endif

   wire soc_unused_ok = &{
      1'b0,
      RXD,
      inst_addr[31:16], inst_addr[1:0],
      data_raddr[31:17], data_raddr[1:0],
      data_waddr[31:17], data_waddr[1:0],
      data_wen,
      IO_mem_addr[31:16], IO_mem_addr[1:0],
      axi_araddr,
      axi_arlen,
      axi_arsize,
      axi_arburst,
      axi_arvalid,
      axi_arready,
      axi_rdata,
      axi_rresp,
      axi_rlast,
      axi_rvalid,
      axi_rready,
      DATARAM_raddr_word[14],
      DATARAM_waddr_word[14],
      uart_ready
   };

   // 当前教程版本不再经过独立的时钟/复位封装，而是直接使用顶层输入。
endmodule
