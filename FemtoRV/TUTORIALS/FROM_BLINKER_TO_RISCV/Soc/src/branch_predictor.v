module branch_predictor #(
    parameter BHT_ADDR_BITS = 12,
    parameter BHT_HISTO_BITS = 9,
    parameter WITH_GSHARE = 1,
    parameter WITH_RAS = 1
) (
    input  wire        clk,
    input  wire        resetn,
    
    // D 级发起预测请求，输入包括当前 PC、指令类型以及已经拼好的跳转立即数。
    input  wire [31:0] fd_pc,
    input  wire [31:0] fd_instr,
    input  wire        d_is_branch,
    input  wire        d_is_jal,
    input  wire        d_is_jalr,
    input  wire [4:0]  d_rd_id,
    input  wire [4:0]  d_rs1_id,
    input  wire [31:0] d_bimm,
    input  wire [31:0] d_jimm,
    input  wire        d_stall,
    input  wire        d_flush,
    input  wire        fd_nop,

    output wire        d_predict_branch,
    output wire        d_predict_pc_valid,
    output wire [31:0] d_pc_prediction,
    output wire [31:0] d_predict_ra,
    output wire [BHT_ADDR_BITS-1:0] d_bht_index,

    // E 级在真正执行后回写预测器状态。
    input  wire        de_is_jal,
    input  wire        de_is_jalr,
    input  wire [4:0]  de_rd_id,
    input  wire [4:0]  de_rs1_id,
    input  wire [31:0] de_pcplus4,
    input  wire        de_is_branch,
    input  wire        e_take_branch,
    input  wire        e_correct_pc,
    input  wire [BHT_ADDR_BITS-1:0] de_bht_index,
    input  wire        e_stall
);

    localparam BHT_SIZE = 1 << BHT_ADDR_BITS;
    
    // branch_history 保存全局历史，BHT 用 2 位饱和计数器记录某个索引近期偏向。
    reg [BHT_HISTO_BITS-1:0] branch_history;
    reg [1:0] BHT [BHT_SIZE-1:0];

    // RAS 维护两份栈：
    // - ras_spec_* 是前端投机版本，服务即时预测
    // - ras_arch_* 是执行提交版本，遇到修正时可快速回滚
    reg [31:0] ras_spec_0, ras_spec_1, ras_spec_2, ras_spec_3;
    reg [31:0] ras_arch_0, ras_arch_1, ras_arch_2, ras_arch_3;

    // GShare 用 PC 与全局历史异或形成索引，能让同一段代码在不同历史上下文下分开学习。
    /* verilator lint_off WIDTH */
    wire [BHT_ADDR_BITS-1:0] bht_index = fd_pc[BHT_ADDR_BITS+1:2] ^ (branch_history << (BHT_ADDR_BITS - BHT_HISTO_BITS));
    /* verilator lint_on WIDTH */
    assign d_bht_index = bht_index;

    generate
        if (WITH_GSHARE) begin
            assign d_predict_branch = BHT[bht_index][1];
        end else begin
            // 退化模式下使用 BTFNT：向后跳默认 taken，向前跳默认 not taken。
            assign d_predict_branch = fd_instr[31];
        end
    endgenerate

    generate
        if (WITH_RAS) begin
            assign d_predict_ra = ras_spec_0;
            // JAL 一定可预测；分支和 JALR 则要满足当前指令类别和预测方向要求。
            assign d_predict_pc_valid = !fd_nop && (d_is_jal || (fd_instr[6:4] == 3'b110 && (fd_instr[2] | d_predict_branch)));
            // JALR 且命中返回模式时直接取 RAS 栈顶，否则按 PC+imm 形成预测目标。
            assign d_pc_prediction = (fd_instr[3:2] == 2'b01) ? ras_spec_0 : (fd_pc + (d_is_jal ? d_jimm : d_bimm));
        end else begin
            assign d_predict_ra = 32'b0;
            assign d_predict_pc_valid = !fd_nop && (d_is_jal || (d_is_branch && d_predict_branch));
            assign d_pc_prediction = (fd_pc + (d_is_jal ? d_jimm : d_bimm));
        end
    endgenerate

    // State Updates
    function [1:0] incdec_sat;
        input [1:0] prev;
        input dir;
        incdec_sat = 
            {dir, prev} == 3'b000 ? 2'b00 :
            {dir, prev} == 3'b001 ? 2'b00 :
            {dir, prev} == 3'b010 ? 2'b01 :
            {dir, prev} == 3'b011 ? 2'b10 :
            {dir, prev} == 3'b100 ? 2'b01 :
            {dir, prev} == 3'b101 ? 2'b10 :
            {dir, prev} == 3'b110 ? 2'b11 :
                                    2'b11 ;
    endfunction

    integer i;
    wire branch_predictor_unused_ok = &{1'b0, fd_instr[31:7], fd_instr[1:0], d_is_branch};
`ifdef CONFIG_INITIALIZE
    initial begin
        // 上电初始化为弱不跳转，能减少仿真里 X 状态扩散造成的预测污染。
        for (i = 0; i < BHT_SIZE; i = i + 1) begin
            BHT[i] = 2'b01;
        end
    end
`endif
    always @(posedge clk) begin
        if (!resetn) begin
            branch_history <= 0;
            ras_spec_0 <= 0; ras_spec_1 <= 0; ras_spec_2 <= 0; ras_spec_3 <= 0;
            ras_arch_0 <= 0; ras_arch_1 <= 0; ras_arch_2 <= 0; ras_arch_3 <= 0;
        end else begin
            if (WITH_GSHARE && de_is_branch && !e_stall) begin
                // 真正完成分支后再更新历史和计数器，保证学习的是提交结果而不是投机路径。
                branch_history <= {e_take_branch, branch_history[BHT_HISTO_BITS-1:1]};
                BHT[de_bht_index] <= incdec_sat(BHT[de_bht_index], e_take_branch);
            end

            if (WITH_RAS) begin
                if (e_correct_pc) begin
                    // 一旦发现前端走错路径，投机栈直接回滚到架构栈。
                    ras_spec_0 <= ras_arch_0;
                    ras_spec_1 <= ras_arch_1;
                    ras_spec_2 <= ras_arch_2;
                    ras_spec_3 <= ras_arch_3;
                end else if (!d_stall && !d_flush && !fd_nop) begin
                    // jal x1, ... 视为 call，把返回地址压入投机栈顶。
                    if (d_is_jal && d_rd_id == 1) begin
                        ras_spec_3 <= ras_spec_2;
                        ras_spec_2 <= ras_spec_1;
                        ras_spec_1 <= ras_spec_0;
                        ras_spec_0 <= fd_pc + 4;
                    end
                    // jalr x0, x1/x5, 0 视为 return，从投机栈中弹出一层。
                    if (d_is_jalr && d_rd_id == 0 && (d_rs1_id == 1 || d_rs1_id == 5)) begin
                        ras_spec_0 <= ras_spec_1;
                        ras_spec_1 <= ras_spec_2;
                        ras_spec_2 <= ras_spec_3;
                    end
                end

                if (!e_stall) begin
                    // 同样的 call/return 语义在执行级提交到架构栈，作为回滚基准。
                    if (de_is_jal && de_rd_id == 1) begin
                        ras_arch_3 <= ras_arch_2;
                        ras_arch_2 <= ras_arch_1;
                        ras_arch_1 <= ras_arch_0;
                        ras_arch_0 <= de_pcplus4;
                    end
                    if (de_is_jalr && de_rd_id == 0 && (de_rs1_id == 1 || de_rs1_id == 5)) begin
                        ras_arch_0 <= ras_arch_1;
                        ras_arch_1 <= ras_arch_2;
                        ras_arch_2 <= ras_arch_3;
                    end
                end
            end
        end
    end

endmodule
