`include "../../RTL/PLL/femtopll.v"

`ifdef ECP5_EVN
`define NEGATIVE_RESET
`endif

`ifdef ARTY
`define NEGATIVE_RESET
`endif

`ifdef TANGNANO9K
`define NEGATIVE_RESET
`endif


module Clockworks
(
   input  CLK,
   input  RESET,
   output clk,
   output resetn
);
   parameter SLOW=0;

   // 这个模块原本用于板级时钟与复位管理，包含两个职责：
   // 1. 在调试模式下把外部时钟分频，方便肉眼观察流水线行为
   // 2. 在高速模式下接 PLL，并在上电初期延长复位，避开 BRAM 尚未稳定的窗口
   generate

      if(SLOW != 0) begin
	 // 慢速模式直接用计数器分频，仿真时因为整体运行比真实硬件慢很多，
	 // 所以把分频位数再减 4，避免波形过于拖沓。
`ifdef BENCH
   localparam slow_bit=SLOW-4;
`else
   localparam slow_bit=SLOW;
`endif
	 reg [slow_bit:0] slow_CLK = 0;
	 always @(posedge CLK) begin
	    slow_CLK <= slow_CLK + 1;
	 end
	 assign clk = slow_CLK[slow_bit];

`ifdef NEGATIVE_RESET
	 assign resetn = RESET;
`else
	 assign resetn = !RESET;
`endif

      end else begin

`ifdef CPU_FREQ
        // 高速模式优先走 PLL，把板载时钟提升到期望 CPU 主频。
        femtoPLL #(
          .freq(`CPU_FREQ)
        ) pll(
           .pclk(CLK),
           .clk(clk)
	);
`else
        assign clk=CLK;
`endif


// 上电后先维持一段时间复位，让内部 RAM 和时钟网络进入稳定状态。
// IceStick 资源更紧张，所以单独使用较小计数器。
`ifdef ICE_STICK
	 reg [11:0] 	    reset_cnt = 0;
`else
	 reg [15:0] 	    reset_cnt = 0;
`endif
	 assign resetn = &reset_cnt;

`ifdef NEGATIVE_RESET
	 always @(posedge clk,negedge RESET) begin
	    if(!RESET) begin
	       reset_cnt <= 0;
	    end else begin
	       reset_cnt <= reset_cnt + !resetn;
	    end
	 end
`else
	 always @(posedge clk,posedge RESET) begin
	    if(RESET) begin
	       reset_cnt <= 0;
	    end else begin
	       // 宽度由参数和板卡选择共同决定，这里保留 lint 抑制避免工具误报。
	       /* verilator lint_off WIDTH */
	       reset_cnt <= reset_cnt + !resetn;
	       /* verilator lint_on WIDTH */
	    end
	 end
`endif
      end
   endgenerate

endmodule
