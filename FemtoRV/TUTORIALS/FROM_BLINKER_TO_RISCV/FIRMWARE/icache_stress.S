    .section .text
    .globl main
    .align  2

main:
    li      s0, 16
    la      t0, funcA
    jalr    x0, 0(t0)

    .balign 4096
funcA:
    addi    t1, x0, 32
1:
    addi    t1, t1, -1
    bnez    t1, 1b
    # Hop to funcB at next 4KB-aligned region
    la      t0, funcB
    jalr    x0, 0(t0)

    .balign 4096
funcB:
    addi    t2, x0, 32
2:
    addi    t2, t2, -1
    bnez    t2, 2b
    # Hop to funcC at next 4KB-aligned region
    la      t0, funcC
    jalr    x0, 0(t0)

    .balign 4096
funcC:
    addi    t3, x0, 32
3:
    addi    t3, t3, -1
    bnez    t3, 3b
    # One outer iteration completed
    addi    s0, s0, -1
    bnez    s0, funcA

    ebreak
