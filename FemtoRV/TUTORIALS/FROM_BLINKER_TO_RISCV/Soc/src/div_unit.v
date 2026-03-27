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

    // 中文说明：
    // 该模块保持与 pipeline10 原始 DIV/REM 状态机等价：
    // - 支持 DIV/DIVU/REM/REMU（由 funct3 区分）
    // - 通过 busy/finished 与外部流水线握手
    // - 算法是移位减法迭代，每拍处理一位

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
                    quotient_msk_r <= 32'h8000_0000;
                    busy_r <= 1'b1;
                end
                dividend_r <= (~funct3[0] && rs1[31]) ? -rs1 : rs1;
                divisor_r  <= {((~funct3[0] && rs2[31]) ? -rs2 : rs2), 31'b0};
                quotient_r <= 32'b0;
                sign_r <= ~funct3[0] & (funct3[1] ? rs1[31] :
                          ((rs1[31] != rs2[31]) & |rs2));
            end else begin
                dividend_r <= divstep_do ? (dividend_r - divisor_r[31:0]) : dividend_r;
                divisor_r  <= divisor_r >> 1;
                quotient_r <= divstep_do ? (quotient_r | quotient_msk_r) : quotient_r;
                quotient_msk_r <= quotient_msk_r >> 1;
                busy_r <= busy_r & !quotient_msk_r[0];
            end
            finished_r <= quotient_msk_r[0];
        end
    end

    assign busy = busy_r;
    assign finished = finished_r;
    assign quotient_abs = quotient_r;
    assign remainder_abs = dividend_r;
    assign sign_flag = sign_r;

    wire [2:0] divsel = {is_div, funct3[1], sign_r};
    assign result =
        (divsel == 3'b100) ?  quotient_r :
        (divsel == 3'b101) ? -quotient_r :
        (divsel == 3'b110) ?  dividend_r :
                             -dividend_r;

endmodule
