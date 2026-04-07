`timescale 1ns/1ps
module tb_hazard_unit;
    reg d_is_load, d_is_store, d_is_alu_reg, d_is_branch, d_is_jalr;
    reg [4:0] d_rs1_id, d_rs2_id;
    reg fd_nop;
    reg [4:0] de_rd_id;
    reg de_is_load, de_is_csrrs, de_is_store;
    reg e_correct_pc;
    reg alu_busy;
    reg halt;

    wire f_stall, d_stall, e_stall, d_flush, e_flush, m_flush;
    wire data_hazard, rs1_hazard, rs2_hazard;

    hazard_unit dut (
        .d_is_load(d_is_load),
        .d_is_store(d_is_store),
        .d_is_alu_reg(d_is_alu_reg),
        .d_is_branch(d_is_branch),
        .d_is_jalr(d_is_jalr),
        .d_rs1_id(d_rs1_id),
        .d_rs2_id(d_rs2_id),
        .fd_nop(fd_nop),
        .de_rd_id(de_rd_id),
        .de_is_load(de_is_load),
        .de_is_csrrs(de_is_csrrs),
        .de_is_store(de_is_store),
        .e_correct_pc(e_correct_pc),
        .alu_busy(alu_busy),
        .halt(halt),
        .f_stall(f_stall),
        .d_stall(d_stall),
        .e_stall(e_stall),
        .d_flush(d_flush),
        .e_flush(e_flush),
        .m_flush(m_flush),
        .data_hazard(data_hazard),
        .rs1_hazard(rs1_hazard),
        .rs2_hazard(rs2_hazard)
    );

    initial begin
        // 保留波形导出，方便观察 stall/flush 是在哪一拍拉高。
        $dumpfile("hazard.vcd");
        $dumpvars(0, tb_hazard_unit);

        d_is_load = 0; d_is_store = 0; d_is_alu_reg = 0; d_is_branch = 0; d_is_jalr = 0;
        d_rs1_id = 0; d_rs2_id = 0; fd_nop = 0;
        de_rd_id = 0; de_is_load = 0; de_is_csrrs = 0; de_is_store = 0;
        e_correct_pc = 0; alu_busy = 0; halt = 0;

        #10;
        
        // 先构造最典型的 load-use 相关，确认 hazard_unit 会停前端并向 E 级插气泡。
        de_is_load = 1;
        de_rd_id = 5;
        d_is_alu_reg = 1;
        d_rs1_id = 5;
        d_rs2_id = 6;
        #10;
        if (!data_hazard || !f_stall || !d_stall || !e_flush) $display("FAIL: Load-use hazard not detected");

        // 清掉前一组刺激，避免影响后续场景。
        de_is_load = 0;
        d_is_alu_reg = 0;
        d_rs1_id = 0;
        d_rs2_id = 0;
        de_rd_id = 0;
        #10;

        // 再模拟一次分支纠正，检查 D/E 级 flush 是否同时生效。
        e_correct_pc = 1;
        #10;
        if (!d_flush || !e_flush || f_stall) $display("FAIL: Branch mispredict flush not working");
        
        e_correct_pc = 0;
        #10;

        // 最后验证多周期除法忙时，流水线会整体被正确冻结。
        alu_busy = 1;
        #10;
        if (!f_stall || !d_stall || !e_stall || !m_flush) $display("FAIL: ALU busy stalls not working");

        $display("ALL TESTS DONE.");
        $finish;
    end
endmodule
