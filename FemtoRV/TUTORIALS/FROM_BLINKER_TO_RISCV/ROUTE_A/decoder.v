`default_nettype none

module decoder (
    input  wire [31:0] instr,
    output wire [4:0]  rd_id,
    output wire [4:0]  rs1_id,
    output wire [4:0]  rs2_id,
    output wire [2:0]  funct3,
    output wire        funct7_bit30,
    output wire [1:0]  csr_id,
    output wire [7:0]  funct3_onehot,
    output wire        is_alu_reg,
    output wire        is_alu_imm,
    output wire        is_branch,
    output wire        is_jalr,
    output wire        is_jal,
    output wire        is_auipc,
    output wire        is_lui,
    output wire        is_load,
    output wire        is_store,
    output wire        is_system,
    output wire        is_csrrs,
    output wire        is_ebreak,
    output wire        reads_rs1,
    output wire        reads_rs2,
    output wire        wb_enable_raw,
    output wire        is_rv32m,
    output wire        is_mul,
    output wire        is_div,
    output wire        is_jal_or_jalr,
    output wire        is_lui_or_auipc,
    output wire        is_jal_or_jalr_or_lui_or_auipc
);

    // 中文说明：
    // 这个模块只做“指令位字段 -> 控制信号”的纯组合译码，
    // 不带时序，不依赖流水线上下文，便于单独测试。

    assign rd_id   = instr[11:7];
    assign rs1_id  = instr[19:15];
    assign rs2_id  = instr[24:20];
    assign funct3  = instr[14:12];
    assign funct7_bit30 = instr[30];
    assign csr_id  = {instr[27], instr[21]};
    assign funct3_onehot = 8'b00000001 << instr[14:12];

    assign is_alu_reg = (instr[6:2] == 5'b01100);
    assign is_alu_imm = (instr[6:2] == 5'b00100);
    assign is_load    = (instr[6:2] == 5'b00000);
    assign is_store   = (instr[6:2] == 5'b01000);
    assign is_system  = (instr[6:2] == 5'b11100);

    // 与 pipeline10 一致的优化判定写法
    assign is_jal    = instr[3];
    assign is_jalr   = {instr[6], instr[3], instr[2]} == 3'b101;
    assign is_lui    = instr[6:4] == 3'b111;
    assign is_auipc  = instr[6:4] == 3'b101;
    assign is_branch = {instr[6], instr[4], instr[2]} == 3'b100;

    assign is_jal_or_jalr  = (instr[2] & instr[6]);
    assign is_lui_or_auipc = (instr[4] & instr[6]);
    assign is_jal_or_jalr_or_lui_or_auipc = instr[2];

    assign is_csrrs  = is_system && instr[13];
    assign is_ebreak = is_system && !instr[13];

    // 读取源寄存器判定沿用 pipeline10：
    // rs1 除 JAL/LUI/AUIPC 外都可能读
    assign reads_rs1 = !(is_jal || is_lui_or_auipc);
    // rs2 仅 ALUreg/Branch/Store/SYSTEM 需要
    assign reads_rs2 = (instr[5] && (instr[3:2] == 2'b00));

    // pipeline10 的写回原始判定（后续还会与 rd!=0 相与）
    assign wb_enable_raw = (instr[5:2] != 4'b1000);

    // RV32M 判定：R-type 且 funct7[0]=1（即 instr[25]）
    assign is_rv32m = is_alu_reg && instr[25];
    assign is_mul   = is_alu_reg && instr[25] && !instr[14];
    assign is_div   = is_alu_reg && instr[25] &&  instr[14];

endmodule

