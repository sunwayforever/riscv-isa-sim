
    .global _start, tohost, fromhost

    #leif
    .section .text.init
    .align 2
    .type interrupt_vector_table, @object
interrupt_vector_table:
    .word   0                       # 0x00: reset
    .word   _start                  # 0x04: non-maskable interrupt (machine check)
    .word   _start                  # 0x08: non-maskable interrupt (external interrupt)
    .word   handle_soft_interrupt   # 0x0c: non-maskable interrupt (software interrupt)
    .word   _start                  # 0x10: non-maskable interrupt (timer interrupt)
    .word   _start                  # 0x14: non-maskable interrupt (external interrupt)
    .word   _start                  # 0x18: non-maskable interrupt (timer interrupt)
    .word   _start                  # 0x1c: non-maskable interrupt (external interrupt)
    .word   _start                  # 0x20: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x24: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x28: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x2c: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x30: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x34: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x38: non-maskable interrupt (local interrupt)
    .word   _start                  # 0x3c: non-maskable interrupt (local interrupt)

    .section .text
    .align 2
    .global _start, tohost, fromhost
    #leif

_start:
    # NOTE: set sstatus_fs to enable FPU
    li t0, 0x00006000
    csrs sstatus, t0

    #leif
    # Set mtvec to the address of the interrupt vector table
    la t0, interrupt_vector_table
    csrw mtvec, t0
    #leif

    la t0,data1
    la t1,data2
    la t2,float1
    la t3,float2
    la t4,float3
    flw  f2, 0(t3)

    # test xpr
    addi  tp, zero, 0xa
    ld  a0, 0(t0)
    lw  a1, 0(t1)
    # a1 changed to -1 outside
    addi a1, a1, 1

    # test fpr
    # test double
    addi  tp, zero, 0xb
    fld  f0, 0(t2)
    fadd.d f0,f0,f0
    # test nan and inf
    fadd.s f0,f0,f2
    # test flat
    flw  f1, 0(t3)
    fadd.s f2,f1,f2

    # test csr
    # NOTE: enable software interrupt
    li t0, 1<<3
    csrs mie, t0
    csrs mstatus,t0

    #leif
    la t0,handle_soft_interrupt
    #leif

    addi  tp, zero, 0xc
    li t0, 0

.align 4
handle_soft_interrupt:
    # handle software interrupt
    addi tp, zero, 0xd
    # clear software interrupt flag
    li t0, 1<<3
    csrs mstatus, t0
    ret

    .data
float1: .double 2.0
float2: .float 1.0
float3: .float 1.0    
    .align 16
data1: .dword 0x12345678
data2: .word 0x1234

tohost: .dword 1
fromhost: .dword 1
