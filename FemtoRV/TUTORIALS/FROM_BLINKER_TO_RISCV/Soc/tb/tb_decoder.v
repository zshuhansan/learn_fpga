`timescale 1ns/1ps
`default_nettype none
`include "ROUTE_A/decoder.v"

module tb_decoder;
    reg  [31:0] instr;
    wire [4:0]  rd_id, rs1_id, rs2_id;
    wire [2:0]  funct3;
    wire        funct7_bit30;
    wire [1:0]  csr_id;
    wire [7:0]  funct3_onehot;
    wire is_alu_reg, is_alu_imm, is_branch, is_jalr, is_jal, is_auipc, is_lui, is_load, is_store, is_system;
    wire is_csrrs, is_ebreak, reads_rs1, reads_rs2, wb_enable_raw, is_rv32m, is_mul, is_div;
    wire is_jal_or_jalr, is_lui_or_auipc, is_jal_or_jalr_or_lui_or_auipc;

    decoder dut(
        .instr(instr),
        .rd_id(rd_id), .rs1_id(rs1_id), .rs2_id(rs2_id),
        .funct3(funct3), .funct7_bit30(funct7_bit30), .csr_id(csr_id), .funct3_onehot(funct3_onehot),
        .is_alu_reg(is_alu_reg), .is_alu_imm(is_alu_imm), .is_branch(is_branch), .is_jalr(is_jalr),
        .is_jal(is_jal), .is_auipc(is_auipc), .is_lui(is_lui), .is_load(is_load), .is_store(is_store),
        .is_system(is_system), .is_csrrs(is_csrrs), .is_ebreak(is_ebreak),
        .reads_rs1(reads_rs1), .reads_rs2(reads_rs2), .wb_enable_raw(wb_enable_raw),
        .is_rv32m(is_rv32m), .is_mul(is_mul), .is_div(is_div),
        .is_jal_or_jalr(is_jal_or_jalr), .is_lui_or_auipc(is_lui_or_auipc),
        .is_jal_or_jalr_or_lui_or_auipc(is_jal_or_jalr_or_lui_or_auipc)
    );

    task check;
        input cond;
        input [255:0] msg;
        begin
            if(!cond) begin
                $display("FAIL: %0s", msg);
                $fatal;
            end
        end
    endtask

    initial begin
        // 依次覆盖立即数 ALU、Store、Branch、SYSTEM 和 RV32M 几类关键指令，
        // 只要其中一类识别错误，就说明组合译码表存在缺口。
        instr = 32'h00100093; #1;
        check(is_alu_imm, "addi 应识别为 ALUimm");
        check(rd_id==5'd1, "addi rd 应为 x1");
        check(rs1_id==5'd0, "addi rs1 应为 x0");
        check(reads_rs1 && !reads_rs2, "addi 只读 rs1");

        instr = 32'h0021A423; #1;
        check(is_store, "sw 应识别为 store");
        check(reads_rs1 && reads_rs2, "store 应读 rs1/rs2");

        instr = 32'h00208463; #1;
        check(is_branch, "beq 应识别为 branch");

        instr = 32'hC00022F3; #1;
        check(is_system && is_csrrs, "csrrs 应识别");

        instr = 32'h00100073; #1;
        check(is_system && is_ebreak, "ebreak 应识别");

        instr = 32'h025201B3; #1;
        check(is_rv32m && is_mul && !is_div, "mul 应识别为 RV32M/MUL");

        instr = 32'h025241B3; #1;
        check(is_rv32m && !is_mul && is_div, "div 应识别为 RV32M/DIV");

        $display("tb_decoder PASS");
        $finish;
    end
endmodule
