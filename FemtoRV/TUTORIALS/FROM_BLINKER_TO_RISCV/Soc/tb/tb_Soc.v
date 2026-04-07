`timescale 1ns/1ps
module tb_Soc();
   reg clk;
   reg resetn;
   wire [4:0] LEDS;
   reg  RXD = 1'b1;
   wire TXD;

   SOC uut(
     .CLK(clk),
     .RESET(resetn),
     .LEDS(LEDS),
     .RXD(RXD),
     .TXD(TXD)
   );

   reg[4:0] prev_LEDS = 0;
   
   initial begin
      // 100MHz 等效时钟，方便和大多数其他 testbench 保持一致。
      clk = 0;
      forever begin
         #5 clk = ~clk;
      end
   end

   initial begin
      // 上电后先拉低复位，再释放处理器开始执行程序。
      resetn = 0;
      #20;
      resetn = 1;
      
      // 如果程序没有按预期执行到 EBREAK，就用超时机制避免仿真无限挂住。
      #1000000000;
      $display("Simulation Timeout");
      $finish;
   end

   always @(posedge clk) begin
      // LED 变化通常代表软件跑到了某个关键状态，因此直接在终端打印。
      if(LEDS != prev_LEDS) begin
         $display("Time: %0t | LEDS = %b", $time, LEDS);
         prev_LEDS <= LEDS;
      end
   end
endmodule
