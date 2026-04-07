module imm_gen (
    input  wire [31:0] instr,
    input  wire        is_store,
    output wire [31:0] imm_i,
    output wire [31:0] imm_s,
    output wire [31:0] imm_b,
    output wire [31:0] imm_u,
    output wire [31:0] imm_j,
    output wire [31:0] i_or_s_imm
);
    wire imm_gen_unused_ok = &{1'b0, instr[6:0]};

    // RISC-V 不同指令格式的立即数字段分散在不同 bit 位置，本模块把它们统一
    // 拼成 32 位结果。上层译码后可以直接取用，不必在主流水线里重复切片。
    assign imm_i = {{21{instr[31]}}, instr[30:20]};
    assign imm_s = {{21{instr[31]}}, instr[30:25], instr[11:7]};
    assign imm_b = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
    assign imm_u = {instr[31], instr[30:12], {12{1'b0}}};
    assign imm_j = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};

    // Load 使用 I 型偏移，Store 使用 S 型偏移，上层常把这两类地址生成合并处理。
    assign i_or_s_imm = is_store ? imm_s : imm_i;

endmodule
