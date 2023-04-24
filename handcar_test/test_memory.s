    .global _start,tohost, fromhost

_start:
    addi  a0, x0, 1
    la    a1, helloworld 
    
    .data
helloworld:      .ascii "Hello World!\n"
    
tohost: .dword 1
fromhost: .dword 1
