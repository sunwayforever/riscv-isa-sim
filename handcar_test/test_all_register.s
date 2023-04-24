    .global _start, tohost, fromhost

_start:
    # NOTE: set sstatus_fs to enable FPU
    li t0, 0x00006000
    csrs sstatus, t0
    
    li zero, 0
    li ra, 1
    li sp, 2
    li gp, 3
    li tp, 4
    li t0, 5
    li t1, 6
    li t2, 7
    li s0, 8
    li s1, 9
    li a0, 10
    li a1, 11
    li a2, 12
    li a3, 13
    li a4, 14
    li a5, 15
    li a6, 16
    li a7, 17
    li s2, 18
    li s3, 19
    li s4, 20
    li s5, 21
    li s6, 22
    li s7, 23
    li s8, 24
    li s9, 25
    li s10, 26
    li s11, 27
    li t3, 28
    li t4, 29
    li t5, 30
    li t6, 31

    addi zero, zero,1
    addi ra, ra,1
    addi sp, sp,1
    addi gp, gp,1
    addi tp, tp,1
    addi t0, t0,1
    addi t1, t1,1
    addi t2, t2,1
    addi s0, s0,1
    addi s1, s1,1
    addi a0, a0,1
    addi a1, a1,1
    addi a2, a2,1
    addi a3, a3,1
    addi a4, a4,1
    addi a5, a5,1
    addi a6, a6,1
    addi a7, a7,1
    addi s2, s2,1
    addi s3, s3,1
    addi s4, s4,1
    addi s5, s5,1
    addi s6, s6,1
    addi s7, s7,1
    addi s8, s8,1
    addi s9, s9,1
    addi s10, s10,1
    addi s11, s11,1
    addi t3, t3,1
    addi t4, t4,1
    addi t5, t5,1
    addi t6, t6,1
    # fpr
    fcvt.s.w ft0,  zero 
    fcvt.s.w ft1,  ra   
    fcvt.s.w ft2,  sp   
    fcvt.s.w ft3,  gp   
    fcvt.s.w ft4,  tp   
    fcvt.s.w ft5,  t0   
    fcvt.s.w ft6,  t1   
    fcvt.s.w ft7,  t2   
    fcvt.s.w fs0,  s0   
    fcvt.s.w fs1,  s1   
    fcvt.s.w fa0,  a0   
    fcvt.s.w fa1,  a1   
    fcvt.s.w fa2,  a2   
    fcvt.s.w fa3,  a3   
    fcvt.s.w fa4,  a4   
    fcvt.s.w fa5,  a5   
    fcvt.s.w fa6,  a6   
    fcvt.s.w fa7,  a7   
    fcvt.s.w fs2,  s2   
    fcvt.s.w fs3,  s3   
    fcvt.s.w fs4,  s4   
    fcvt.s.w fs5,  s5   
    fcvt.s.w fs6,  s6   
    fcvt.s.w fs7,  s7   
    fcvt.s.w fs8,  s8   
    fcvt.s.w fs9,  s9   
    fcvt.s.w fs10, s10   
    fcvt.s.w fs11, s11   
    fcvt.s.w ft8,  t3   
    fcvt.s.w ft9,  t4   
    fcvt.s.w ft10, t5    
    fcvt.s.w ft11, t6
    li t6, 0xa
    fadd.s ft1,ft1,ft0 
    fadd.s ft2,ft2,ft0 
    fadd.s ft3,ft3,ft0 
    fadd.s ft4,ft4,ft0 
    fadd.s ft5,ft5,ft0 
    fadd.s ft6,ft6,ft0 
    fadd.s ft7,ft7,ft0 
    fadd.s fs0,fs0,ft0 
    fadd.s fs1,fs1,ft0 
    fadd.s fa0,fa0,ft0 
    fadd.s fa1,fa1,ft0 
    fadd.s fa2,fa2,ft0 
    fadd.s fa3,fa3,ft0 
    fadd.s fa4,fa4,ft0 
    fadd.s fa5,fa5,ft0 
    fadd.s fa6,fa6,ft0 
    fadd.s fa7,fa7,ft0 
    fadd.s fs2,fs2,ft0 
    fadd.s fs3,fs3,ft0 
    fadd.s fs4,fs4,ft0 
    fadd.s fs5,fs5,ft0 
    fadd.s fs6,fs6,ft0 
    fadd.s fs7,fs7,ft0 
    fadd.s fs8,fs8,ft0 
    fadd.s fs9,fs9,ft0 
    fadd.s fs10,fs10,ft0 
    fadd.s fs11,fs11,ft0 
    fadd.s ft8,ft8,ft0 
    fadd.s ft9,ft9,ft0 
    fadd.s ft10,ft10,ft0 
    fadd.s ft11,ft11,ft0 
    li t6, 0xb
    
    .data
tohost: .dword 1
fromhost: .dword 1
