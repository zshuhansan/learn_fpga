// ROUTE_A 自定义 riscv-tests 运行环境
// 目标：让 rv32ui/rv32um 测试能在本项目的 pipeline10_routeA 上直接运行
// 特点：
// 1) 不使用 ecall/trap/tohost（本核不支持完整特权环境）
// 2) 通过 MMIO UART 输出单字符 'P'/'F' 标记结果，然后 ebreak 结束仿真

#ifndef _ROUTEA_RISCV_TEST_H
#define _ROUTEA_RISCV_TEST_H

#define RVTEST_RV32U .macro init; .endm
#define RVTEST_RV32M .macro init; .endm
#define RVTEST_RV64U RVTEST_RV32U
#define RVTEST_RV64M RVTEST_RV32M

#define TESTNUM gp

#define ROUTEA_UART_ADDR 0x400008
#define ROUTEA_MBOX0_ADDR 0x400020
#define ROUTEA_MBOXD_ADDR 0x400100

#define RVTEST_CODE_BEGIN                       \
  .section .text.init;                          \
  .align 2;                                     \
  .globl _start;                                \
_start:                                         \
  li x1, 0; li x2, 0; li x3, 0; li x4, 0;       \
  li x5, 0; li x6, 0; li x7, 0; li x8, 0;       \
  li x9, 0; li x10,0; li x11,0; li x12,0;       \
  li x13,0; li x14,0; li x15,0; li x16,0;       \
  li x17,0; li x18,0; li x19,0; li x20,0;       \
  li x21,0; li x22,0; li x23,0; li x24,0;       \
  li x25,0; li x26,0; li x27,0; li x28,0;       \
  li x29,0; li x30,0; li x31,0;                 \
  init;

#define RVTEST_CODE_END                         \
  ebreak

#define RVTEST_PASS                             \
  li TESTNUM, 1;                                \
  li t0, ROUTEA_UART_ADDR;                      \
  li t1, 'P';                                   \
  sw t1, 0(t0);                                 \
  li t0, ROUTEA_MBOX0_ADDR;                     \
  sw t1, 0(t0);                                 \
  li t0, ROUTEA_MBOXD_ADDR;                     \
  sw zero, 0(t0);                               \
  ebreak

#define RVTEST_FAIL                             \
1: beqz TESTNUM, 1b;                            \
  sll TESTNUM, TESTNUM, 1;                      \
  or TESTNUM, TESTNUM, 1;                       \
  li t0, ROUTEA_UART_ADDR;                      \
  li t1, 'F';                                   \
  sw t1, 0(t0);                                 \
  li t0, ROUTEA_MBOX0_ADDR;                     \
  sw t1, 0(t0);                                 \
  li t0, ROUTEA_MBOXD_ADDR;                     \
  sw zero, 0(t0);                               \
  ebreak

#define RVTEST_DATA_BEGIN                       \
  .section .data;                               \
  .align 4;                                     \
  .global begin_signature;                      \
begin_signature:

#define RVTEST_DATA_END                         \
  .align 4;                                     \
  .global end_signature;                        \
end_signature:

#define EXTRA_DATA

#endif
