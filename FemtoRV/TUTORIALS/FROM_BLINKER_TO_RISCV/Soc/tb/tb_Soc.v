`timescale 1ns/1ps
module tb_Soc();
   reg clk;
   reg resetn;
   wire [4:0] LEDS;
   reg  RXD = 1'b1; // Idle high for UART
   wire TXD;

   SOC uut(
     .clk(clk),
     .resetn(resetn),
     .LEDS(LEDS),
     .RXD(RXD),
     .TXD(TXD)
   );

   reg[4:0] prev_LEDS = 0;
   
   initial begin
      clk = 0;
      forever begin
         #5 clk = ~clk;
      end
   end

   initial begin
      $dumpfile("soc.vcd");
      $dumpvars(0, tb_Soc);
      
      resetn = 0;
      #20;
      resetn = 1;
      
      // Stop after some time to prevent infinite loop if EBREAK fails
      #1000000;
      $display("Simulation Timeout");
      $finish;
   end

   always @(posedge clk) begin
      if(LEDS != prev_LEDS) begin
         $display("Time: %0t | LEDS = %b", $time, LEDS);
         prev_LEDS <= LEDS;
      end
   end
endmodule
