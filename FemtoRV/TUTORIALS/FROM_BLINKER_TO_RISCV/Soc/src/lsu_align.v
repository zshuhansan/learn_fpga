module lsu_align (
    input  wire [31:0] addr,
    input  wire [2:0]  funct3,
    input  wire [31:0] store_rs2,
    input  wire [31:0] load_raw_word,
    output wire [31:0] store_data,
    output wire [3:0]  store_wmask,
    output wire [31:0] load_data
);
    wire lsu_align_unused_ok = &{1'b0, addr[31:2]};

    // LSU 在内部总是按 32 位字访问 RAM，本模块负责把 LB/LH/LW、SB/SH/SW
    // 的地址低位含义翻译成字节位置、写掩码以及读回后的对齐与符号扩展。
    wire is_b = (funct3[1:0] == 2'b00);
    wire is_h = (funct3[1:0] == 2'b01);
    wire sext = !funct3[2];

    // 写入数据总线按目标字节位置重排。对字节写和半字写而言，只需要把有效部分
    // 复制到对应槽位，真正哪几个字节会落地由 store_wmask 决定。
    assign store_data[7:0]   = store_rs2[7:0];
    assign store_data[15:8]  = addr[0] ? store_rs2[7:0]  : store_rs2[15:8];
    assign store_data[23:16] = addr[1] ? store_rs2[7:0]  : store_rs2[23:16];
    assign store_data[31:24] = addr[0] ? store_rs2[7:0]  :
                               addr[1] ? store_rs2[15:8] : store_rs2[31:24];

    // 写掩码直接由地址低两位和访问宽度决定，方便底层 RAM 做按字节写。
    assign store_wmask = is_b ?
                           (addr[1] ?
                              (addr[0] ? 4'b1000 : 4'b0100) :
                              (addr[0] ? 4'b0010 : 4'b0001)) :
                         is_h ? (addr[1] ? 4'b1100 : 4'b0011) :
                                4'b1111;

    // 读路径先从 32 位原始字中切出半字，再按最低位切出字节，最后根据无符号/
    // 有符号访问补高位。
    wire [15:0] load_h = addr[1] ? load_raw_word[31:16] : load_raw_word[15:0];
    wire [7:0]  load_b = addr[0] ? load_h[15:8] : load_h[7:0];
    wire load_sign = sext & (is_b ? load_b[7] : load_h[15]);

    assign load_data = is_b ? {{24{load_sign}}, load_b} :
                       is_h ? {{16{load_sign}}, load_h} :
                              load_raw_word;

endmodule
