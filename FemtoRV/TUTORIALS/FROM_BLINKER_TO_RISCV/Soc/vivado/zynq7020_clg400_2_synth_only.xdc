create_clock -name soc_clk -period 10.000 [get_ports CLK]

set_input_delay 0.000 -clock soc_clk [get_ports RESET]
set_false_path -from [get_ports RESET]
