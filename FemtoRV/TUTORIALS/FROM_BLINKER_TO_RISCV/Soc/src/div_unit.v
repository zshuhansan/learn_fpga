module div_unit(
    input  wire        clk,
    input  wire        resetn,
    input  wire        start,
    input  wire        is_div,
    input  wire [2:0]  funct3,
    input  wire [31:0] rs1,
    input  wire [31:0] rs2,
    output wire        busy,
    output wire        finished,
    output wire [31:0] quotient_abs,
    output wire [31:0] remainder_abs,
    output wire        sign_flag,
    output wire [31:0] result
);
    wire div_unit_unused_ok = funct3[2];

    // 这个除法器采用逐位试商的串行算法。每次启动后从最高位开始尝试减法，
    // 每个时钟处理一位，最后得到绝对值商和绝对值余数，再根据指令类型恢复符号。
    reg [31:0] dividend_r;
    reg [62:0] divisor_r;
    reg [31:0] quotient_r;
    reg [31:0] quotient_msk_r;
    reg sign_r;
    reg busy_r;
    reg finished_r;

    wire divstep_do = (divisor_r <= {31'b0, dividend_r});

    always @(posedge clk) begin
        if (!resetn) begin
            dividend_r     <= 32'b0;
            divisor_r      <= 63'b0;
            quotient_r     <= 32'b0;
            quotient_msk_r <= 32'b0;
            sign_r         <= 1'b0;
            busy_r         <= 1'b0;
            finished_r     <= 1'b0;
        end else begin
            if (!busy_r) begin
                if (start && !finished_r) begin
                    // 初始化时把除数左移到最高对齐位置，后续再逐拍右移，
                    // quotient_msk_r 对应当前正在决定的商位。
                    quotient_msk_r <= 32'h8000_0000;
                    busy_r <= 1'b1;
                    dividend_r <= (~funct3[0] && rs1[31]) ? -rs1 : rs1;
                    divisor_r  <= {31'b0, (~funct3[0] && rs2[31]) ? -rs2 : rs2} << 31;
                    quotient_r <= 32'b0;
                    // funct3[1] 用来区分商类结果还是余数类结果：
                    // - 商的符号取决于被除数和除数异号
                    // - 余数的符号跟随被除数
                    sign_r <= funct3[1] ? (~funct3[0] && rs1[31]) :
                              (~funct3[0] && (rs1[31] != rs2[31]) && |rs2);
                end else begin
                    finished_r <= 1'b0;
                end
            end else begin
                if (divstep_do) begin
                    // 当前位可减时，把对应商位置 1，并更新剩余被除数。
                    dividend_r <= dividend_r - divisor_r[31:0];
                    quotient_r <= quotient_r | quotient_msk_r;
                end
                // 无论本位是否可减，都继续进入下一位判断。
                divisor_r  <= divisor_r >> 1;
                quotient_msk_r <= quotient_msk_r >> 1;
                busy_r <= busy_r & !quotient_msk_r[0];
            end
            // 当最低位商位处理完成时，finished 拉高一个周期，供上层锁存结果。
            finished_r <= quotient_msk_r[0];
        end
    end

    assign busy = busy_r;
    assign finished = finished_r;
    assign quotient_abs = quotient_r;
    assign remainder_abs = dividend_r;
    assign sign_flag = sign_r;

    // 结果选择统一放在这里：
    // DIV/DIVU 返回商，REM/REMU 返回余数，符号已经在 sign_r 中准备好。
    wire [2:0] divsel = {is_div, funct3[1], sign_r};
    assign result =
        (divsel == 3'b100) ?  quotient_r :
        (divsel == 3'b101) ? -quotient_r :
        (divsel == 3'b110) ?  dividend_r :
                             -dividend_r;

endmodule
