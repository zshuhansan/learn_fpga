module emitter_uart
  #(
    parameter clk_freq_hz = 0,
    parameter baud_rate = 115200)
  (
   input wire 	    i_clk,
   input wire 	    i_rst,
   input wire [7:0] i_data,
   input wire 	    i_valid,
   output reg 	    o_ready,
   output wire 	    o_uart_tx
);

   // 发送器把 1 字节数据打包成起始位 + 8 位数据 + 停止位，总共 10 位串行移出。
   // o_ready 为 1 时表示当前空闲，可以接受新的发送请求。
   localparam START_VALUE = clk_freq_hz/baud_rate;
   
   localparam WIDTH = $clog2(START_VALUE);
   
   reg [WIDTH:0]  cnt = 0;
   
   reg [9:0]        data = 0;
   
   initial begin
      // 仿真里显式初始化，避免 ready 信号落入 X 状态后把整个 SoC 拖死。
      o_ready = 1'b1;
      cnt = {1'b0,START_VALUE[WIDTH-1:0]};
      data = 0;
   end

   // 空闲时线路保持高电平；发送期间输出移位寄存器最低位。
   assign o_uart_tx = data[0] | !(|data);

   always @(posedge i_clk) begin
      if(i_rst) begin
	 o_ready <= 1'b1;
	 cnt <= {1'b0,START_VALUE[WIDTH-1:0]};
	 data <= 10'b0;
      end else begin
	 // 计数器溢出且 data 已经清空，说明上一帧完全发送结束。
	 if (cnt[WIDTH] & !(|data)) begin
	    o_ready <= 1'b1;
	 end else if (i_valid & o_ready) begin
	    // 只有在 ready 时才接受新字节，避免覆盖正在发送的数据。
	    o_ready <= 1'b0;
	 end

	 // ready 时重新装载分频计数，发送中则逐拍递减，走完一个 bit 周期后溢出。
	 if (o_ready | cnt[WIDTH])
	   cnt <= {1'b0,START_VALUE[WIDTH-1:0]};
	 else
	   cnt <= cnt-1;
	 
	 // 每到一个 bit 边界右移一位；若此时收到新请求，则装载完整 UART 帧。
	 if (cnt[WIDTH])
	   data <= {1'b0, data[9:1]};
	 else if (i_valid & o_ready)
	   data <= {1'b1, i_data, 1'b0};
      end
   end

endmodule
