section .text

global port_out
; di: port
; sil: value
port_out:
push rbp
mov rbp, rsp

mov al, sil
mov dx, di
out dx, al

mov rsp, rbp
pop rbp
ret
