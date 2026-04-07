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

    wire decoder_unused_ok = &{1'b0, instr[31], instr[29:28], instr[26], instr[1:0]};

    // 译码模块只负责把 32 位指令拆成字段并生成组合控制信号，不保存任何时序状态。
    // 这样做的好处是结构清晰，主流水线里只需要消费这些语义化信号即可。

    assign rd_id   = instr[11:7];
    assign rs1_id  = instr[19:15];
    assign rs2_id  = instr[24:20];
    assign funct3  = instr[14:12];
    assign funct7_bit30 = instr[30];
    assign csr_id  = {instr[27], instr[21]};
    assign funct3_onehot = 8'b00000001 << instr[14:12];

    // instr[6:2] 基本上决定主操作类型，因此这里先做最粗粒度分类。
    assign is_alu_reg = (instr[6:2] == 5'b01100);
    assign is_alu_imm = (instr[6:2] == 5'b00100);
    assign is_load    = (instr[6:2] == 5'b00000);
    assign is_store   = (instr[6:2] == 5'b01000);
    assign is_system  = (instr[6:2] == 5'b11100);

    // 下面这些写法是利用 opcode 某些关键位直接做快速判断，逻辑更短一些。
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

    // 源寄存器是否真的会被读取，会直接影响 hazard 检测和旁路判断。
    // rs1 除 JAL/LUI/AUIPC 外基本都参与运算或地址生成。
    assign reads_rs1 = !(is_jal || is_lui_or_auipc);
    // rs2 只在双源操作数场景中被使用，例如 R 型、Branch、Store、SYSTEM。
    assign reads_rs2 = (instr[5] && (instr[3:2] == 2'b00));

    // 写回原始判定先只看指令类别，后面流水线还会再和 rd!=0 相与。
    assign wb_enable_raw = (instr[5:2] != 4'b1000);

    // RV32M 扩展基于 R 型指令再细分：instr[25] 表示 M 扩展，instr[14] 再把乘法和除法类分开。
    assign is_rv32m = is_alu_reg && instr[25];
    assign is_mul   = is_alu_reg && instr[25] && !instr[14];
    assign is_div   = is_alu_reg && instr[25] &&  instr[14];

endmodule
