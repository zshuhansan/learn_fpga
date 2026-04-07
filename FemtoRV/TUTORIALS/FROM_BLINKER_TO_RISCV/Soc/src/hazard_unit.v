module hazard_unit (
    // D 级当前指令的属性与源寄存器使用情况。
    input  wire        d_is_load,
    input  wire        d_is_store,
    input  wire        d_reads_rs1,
    input  wire        d_reads_rs2,
    input  wire [4:0]  d_rs1_id,
    input  wire [4:0]  d_rs2_id,
    input  wire        fd_nop,

    // E 级上一条指令的目的寄存器和执行状态。
    input  wire [4:0]  de_rd_id,
    input  wire        de_is_load,
    input  wire        de_is_csrrs,
    input  wire        de_is_store,
    input  wire        e_correct_pc,
    input  wire        alu_busy,

    // 处理器主动停止时也要冻结前端。
    input  wire        halt,

    // 输出给各级的停顿和冲刷水位。
    output wire        f_stall,
    output wire        d_stall,
    output wire        e_stall,
    output wire        d_flush,
    output wire        e_flush,
    output wire        m_flush,

    // 这些信号既用于调试，也能帮助观测数据相关是否命中。
    output wire        data_hazard,
    output wire        rs1_hazard,
    output wire        rs2_hazard,

    // 指令缓存 miss 会阻塞取指，但不需要把 D 级旧指令也一起冻住。
    input  wire        icache_miss
);

    wire hazard_unit_unused_ok = &{1'b0, d_is_load, d_is_store, de_is_store, icache_miss};

    assign rs1_hazard = d_reads_rs1 && (d_rs1_id == de_rd_id) && (d_rs1_id != 0);
    assign rs2_hazard = d_reads_rs2 && (d_rs2_id == de_rd_id) && (d_rs2_id != 0);

    // 当前实现只把“结果本拍还拿不到”的相关当作真正 hazard：
    // 主要是 load-use，以及把 CSR 读结果当作访存后可用的数据路径。
    assign data_hazard = !fd_nop &&
        ((de_is_load || de_is_csrrs) && (rs1_hazard || rs2_hazard));

    // F 级需要额外感知 ICache miss；D 级则继续持有已译码指令等待后续恢复。
    assign f_stall = alu_busy | data_hazard | halt | icache_miss;
    assign d_stall = alu_busy | data_hazard | halt;
    assign e_stall = alu_busy;

    // 分支修正会冲掉 F/D 之间的错误路径，load-use 则通过给 E 级注入气泡解决。
    assign d_flush = e_correct_pc;
    assign e_flush = e_correct_pc | data_hazard;
    assign m_flush = alu_busy;

endmodule
