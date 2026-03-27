module lsu_align (
    input  wire [31:0] addr,
    input  wire [2:0]  funct3,
    input  wire [31:0] store_rs2,
    input  wire [31:0] load_raw_word,
    output wire [31:0] store_data,
    output wire [3:0]  store_wmask,
    output wire [31:0] load_data
);

    // 中文说明：
    // 该模块集中处理 LSU 的字节对齐逻辑：
    // 1) Store：根据 addr[1:0] 生成重排后的 wdata 与 wmask
    // 2) Load ：根据 funct3 与 addr[1:0] 从 32-bit 原始读数据中抽取字节/半字并做符号扩展
    // 这样可以把 M 级最容易出错的位操作从主流水里抽离出来单测。

    wire is_b = (funct3[1:0] == 2'b00);
    wire is_h = (funct3[1:0] == 2'b01);
    wire sext = !funct3[2];

    assign store_data[7:0]   = store_rs2[7:0];
    assign store_data[15:8]  = addr[0] ? store_rs2[7:0]  : store_rs2[15:8];
    assign store_data[23:16] = addr[1] ? store_rs2[7:0]  : store_rs2[23:16];
    assign store_data[31:24] = addr[0] ? store_rs2[7:0]  :
                               addr[1] ? store_rs2[15:8] : store_rs2[31:24];

    assign store_wmask = is_b ?
                           (addr[1] ?
                              (addr[0] ? 4'b1000 : 4'b0100) :
                              (addr[0] ? 4'b0010 : 4'b0001)) :
                         is_h ? (addr[1] ? 4'b1100 : 4'b0011) :
                                4'b1111;

    wire [15:0] load_h = addr[1] ? load_raw_word[31:16] : load_raw_word[15:0];
    wire [7:0]  load_b = addr[0] ? load_h[15:8] : load_h[7:0];
    wire load_sign = sext & (is_b ? load_b[7] : load_h[15]);

    assign load_data = is_b ? {{24{load_sign}}, load_b} :
                       is_h ? {{16{load_sign}}, load_h} :
                              load_raw_word;

endmodule

