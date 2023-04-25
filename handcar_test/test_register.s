    .global _start, tohost, fromhost

_start:
    # NOTE: set sstatus_fs to enable FPU
    li t0, 0x00006000
    csrs sstatus, t0

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
    addi  tp, zero, 0xb
    fld  f0, 0(t2)
    flw  f1, 0(t3)
    fadd.s f2,f1,f2

    # test csr
    # NOTE: enable software interrupt
    li t0, 1<<3
    csrs mie, t0
    csrs mstatus,t0

    addi  tp, zero, 0xc
    li t0, 0

    .data
float1: .double 2.0
float2: .float 1.0
float3: .float 1.0    
    .align 16
data1: .dword 0x12345678
data2: .word 0x1234

tohost: .dword 1
fromhost: .dword 1
