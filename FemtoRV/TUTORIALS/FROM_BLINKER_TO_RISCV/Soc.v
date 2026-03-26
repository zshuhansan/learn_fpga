/** Soc.v: Top level System on Chip */
`define CONFIG_PC_PREDICT // enables D -> F path (needed by RAS and GSHARE)
`define CONFIG_RAS        // return address stack
`define CONFIG_GSHARE     // gshare branch prediction (or BTFNT if not set)
`define CONFIG_RV32M      // RV32M instruction set (MUL,DIV,REM)
`ifndef CPU_FREQ
`define CPU_FREQ 10
`endif
`default_nettype none
`include "clockworks.v"
`include "emitter_uart.v"
`include "processor.v"
`include "progrom_ip_model.v"
`include "dataram_ip_model.v"

module SOC (
    input            CLK, // system clock
    input            RESET,// reset button
    output reg [4:0] LEDS, // system LEDs
    input            RXD, // UART receive
    output           TXD  // UART transmit
);

   wire clk;
   wire resetn;

   // 处理器指令存储器接口
   wire [31:0] inst_addr;
   wire [31:0] inst_rdata;
   wire        inst_en;

   // 处理器数据存储器接口
   wire [31:0] data_raddr;
   wire [31:0] data_rdata;
   wire [31:0] data_waddr;
   wire [31:0] data_wdata;
   wire [3:0]  data_wmask;
   wire        data_wen;

   // IO memory 接口
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
      
      .data_raddr(data_raddr),
      .data_rdata(data_rdata),
      .data_waddr(data_waddr),
      .data_wdata(data_wdata),
      .data_wmask(data_wmask),
      .data_wen(data_wen),

      .IO_mem_addr(IO_mem_addr),
      .IO_mem_rdata(IO_mem_rdata),
      .IO_mem_wdata(IO_mem_wdata),
      .IO_mem_wr(IO_mem_wr)
   );

   // 实例化程序 ROM
   progrom_ip_model #(
      .ADDR_WIDTH(14),
      .INIT_FILE("PROGROM.hex")
   ) PROGROM (
      .clk(clk),
      .en(inst_en),
      .addr(inst_addr[15:2]),
      .dout(inst_rdata)
   );

   // 实例化数据 RAM
   dataram_ip_model #(
      .ADDR_WIDTH(14),
      .INIT_FILE("DATARAM.hex")
   ) DATARAM (
      .clk(clk),
      .ren(1'b1), // RAM一直可读
      .raddr(data_raddr[15:2]),
      .rdata(data_rdata),
      .wmask(data_wmask),
      .waddr(data_waddr[15:2]),
      .wdata(data_wdata)
   );

   // MMIO 映射逻辑
   wire [13:0] IO_wordaddr = IO_mem_addr[15:2];

   // Memory-mapped IO in IO page, 1-hot addressing in word address.
   localparam IO_LEDS_bit      = 0;  // W five leds
   localparam IO_UART_DAT_bit  = 1;  // W data to send (8 bits)
   localparam IO_UART_CNTL_bit = 2;  // R status. bit 9: busy sending

   always @(posedge clk) begin
      if(IO_mem_wr & IO_wordaddr[IO_LEDS_bit]) begin
         LEDS <= IO_mem_wdata[4:0];
      end
   end

   wire uart_valid = IO_mem_wr & IO_wordaddr[IO_UART_DAT_bit];
   wire uart_ready;

   corescore_emitter_uart #(
      .clk_freq_hz(`CPU_FREQ*1000000)
   ) UART(
      .i_clk(clk),
      .i_rst(!resetn),
      .i_data(IO_mem_wdata[7:0]),
      .i_valid(uart_valid),
      .o_ready(uart_ready),
      .o_uart_tx(TXD)
   );

   assign IO_mem_rdata =
                    IO_wordaddr[IO_UART_CNTL_bit] ? { 22'b0, !uart_ready, 9'b0}
                                                  : 32'b0;

`ifdef BENCH
   always @(posedge clk) begin
      if(uart_valid) begin
`ifdef CONFIG_DEBUG
         $display("UART: %c", IO_mem_wdata[7:0]);
`else
         $write("%c", IO_mem_wdata[7:0] );
         $fflush(32'h8000_0001);
`endif
      end
   end
`endif

   // Gearbox and reset circuitry.
   Clockworks CW(
     .CLK(CLK),
     .RESET(RESET),
     .clk(clk),
     .resetn(resetn)
   );

endmodule
