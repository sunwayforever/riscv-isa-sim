
    .global _start, tohost, fromhost,handle_soft_interrupt2

_start:
    # Set mtvec to be direct mode and the address as interrupt handler
    la t0, handle_soft_interrupt2
    csrw mtvec, t0

    # NOTE: enable software interrupt
    li t0, 1<<3
    csrs mie, t0
    csrs mstatus,t0

    # temp value for test_interrupt_2.c
    la t0,handle_soft_interrupt2

    addi  tp, zero, 0xd
    li t0, 0

.align 4
handle_soft_interrupt2:
    # handle software interrupt
    addi tp, zero, 0xe
    # clear software interrupt flag
    li t0, 1<<3
    csrs mstatus, t0
    mret

    .data
tohost: .dword 1
fromhost: .dword 1
