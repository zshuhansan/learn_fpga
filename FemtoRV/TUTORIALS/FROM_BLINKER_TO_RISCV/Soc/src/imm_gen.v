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

    // 中文说明：
    // 本模块只负责立即数拼接与符号扩展，避免这些位操作散落在顶层代码中。
    // i_or_s_imm 与 pipeline10 中 DE_IorSimm 语义一致：
    // - store 指令使用 Simm
    // - 其它用 Iimm（ALUimm/load/jalr 等）

    assign imm_i = {{21{instr[31]}}, instr[30:20]};
    assign imm_s = {{21{instr[31]}}, instr[30:25], instr[11:7]};
    assign imm_b = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
    assign imm_u = {instr[31], instr[30:12], {12{1'b0}}};
    assign imm_j = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};

    assign i_or_s_imm = is_store ? imm_s : imm_i;

endmodule

