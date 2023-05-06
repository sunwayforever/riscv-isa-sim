    .global _start, tohost, fromhost

_start:
    # NOTE: set sstatus_fs to enable FPU
    li t0, 0x00006000
    csrs sstatus, t0

    la t0,data1
    la t3,float2
    la t4,float3
    la t5,float4
    la t6,float5
    la s11,float6
    flw  f2, 0(t3)
    flw  f3, 0(t4)
    flw  f4, 0(t5)
    flw  f5, 0(t6)
    flw  f6, 0(s11)

    # test csr
    addi  tp, zero, 0xc
    # first cal f2*f3+f4
    fmadd.s f5,f2,f3,f4,dyn
    # second cal f2*f3+f4
    fmadd.s f6,f2,f3,f4,dyn
    li t0, 0

    .data
float2: .float 1.0
float3: .float 1.0000003
float4: .float 0.0000002
float5: .float 1.0
float6: .float 1.0  

    .align 16
data1: .dword 0x12345678

tohost: .dword 1
fromhost: .dword 1
