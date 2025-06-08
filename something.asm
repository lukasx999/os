section .text

%define COM1 0x3F8

global something
something:
push rbp
mov rbp, rsp

mov al, 00000011b
out COM1+3, al


mov rsp, rbp
pop rbp
ret
