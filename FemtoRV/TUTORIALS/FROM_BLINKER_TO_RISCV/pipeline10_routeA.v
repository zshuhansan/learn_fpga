/** routeA top wrapper */
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
`include "ROUTE_A/processor_routeA.v"

module SOC (
    input 	     CLK, // system clock
    input 	     RESET,// reset button
    output reg [4:0] LEDS, // system LEDs
    input 	     RXD, // UART receive
    output 	     TXD  // UART transmit
);

   wire clk;
   wire resetn;

   wire [31:0] IO_mem_addr;
   wire [31:0] IO_mem_rdata;
   wire [31:0] IO_mem_wdata;
   wire        IO_mem_wr;

   Processor_routeA CPU(
      .clk(clk),
      .resetn(resetn),
      .IO_mem_addr(IO_mem_addr),
      .IO_mem_rdata(IO_mem_rdata),
      .IO_mem_wdata(IO_mem_wdata),
      .IO_mem_wr(IO_mem_wr)
   );

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
