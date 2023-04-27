
    .global _start, tohost, fromhost,handle_soft_interrupt

    .section .text.init
    .align 2
    .type interrupt_vector_table, @object
interrupt_vector_table:
    .word   0                       # 0x00: reset
    .word   _start                  # 0x04: non-maskable interrupt (machine check)
    .word   _start                  # 0x08: non-maskable interrupt (external interrupt)
    jal   handle_soft_interrupt     # 0x0c: non-maskable interrupt (software interrupt)

_start:
    # set mtvec as vector mode interrupt
    la t0, interrupt_vector_table
    li t1, 1         # 将最低两位设置为1，表示Vectored模式
    or t0, t0, t1    # 将t1的值与t0相或，将中断向量表的地址和模式合并
    csrw mtvec, t0   # 将t0的值写入mtvec寄存器

    # NOTE: enable software interrupt
    li t0, 1<<3
    csrs mie, t0
    csrs mstatus,t0

    # temp value for test_interrupt.c
    la t0,handle_soft_interrupt
    la t1,interrupt_vector_table

    #test_interrupt.c will found here first
    addi  tp, zero, 0xc
    li t0, 0

.align 4
handle_soft_interrupt:
    # handle software interrupt
    addi tp, zero, 0xe
    # clear software interrupt flag
    li t0, 1<<3
    csrs mstatus, t0
    mret


    .data
tohost: .dword 1
fromhost: .dword 1
