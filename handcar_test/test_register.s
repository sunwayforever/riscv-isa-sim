    .global _start, tohost, fromhost

_start:
    la t0,data1
    la t1,data2   
    addi  tp, zero, 0xa
    ld  a0, 0(t0)
    lw  a1, 0(t1)    
    .data
    
data1: .dword 0x12345678
data2: .word 0x1234
float1: .float 1.0

tohost: .dword 1
fromhost: .dword 1
