.equ IO_BASE, 0x400000
.equ IO_MBOX0, 32
.equ IO_MBOX1, 64
.equ IO_MBOX2, 128
.equ IO_MBOXD, 256

.global main

main:
    li t0, 0x00010000
    li t1, 0x12345678
    sw t1, 0(t0)
    lw t2, 0(t0)

    li t3, IO_BASE
    addi t4, zero, 0
    bne t1, t2, fail

    li t4, 111
    sw t4, IO_MBOX0(t3)
    li t4, 222
    sw t4, IO_MBOX1(t3)
    li t4, 333
    sw t4, IO_MBOX2(t3)
    li t4, 1
    sw t4, IO_MBOXD(t3)
1:  j 1b

fail:
    sw t2, IO_MBOX0(t3)
    sw t1, IO_MBOX1(t3)
    li t4, 999
    sw t4, IO_MBOX2(t3)
    li t4, 1
    sw t4, IO_MBOXD(t3)
1:  j 1b
