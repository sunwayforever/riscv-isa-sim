    .global _start,tohost, fromhost

_start:
    li a0, 'G'
    la a1, helloworld
    li tp, 0xa
    sb a0, 0(a1)
    
    .data
helloworld:      .ascii "Hello World!\n"
    
tohost: .dword 1
fromhost: .dword 1
