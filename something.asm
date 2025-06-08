section .text

%define COM1 0x3F8

global serial_init
serial_init:
push rbp
mov rbp, rsp

; ; Interrupt enable register
; ; disable all interrupts
; mov al, 00000000b
; mov dx, COM1+1
; out dx, al
;
; ; Line control register
; ; enable DLAB + 8 bit char length
; mov al, 10000011b
; mov dx, COM1+3
; out dx, al
;
; ; divisor low byte
; ; set baud rate divisor to 3
; mov al, 3
; mov dx, COM1+0
; out dx, al
;
; ; divisor high byte
; mov al, 0
; mov dx, COM1+1
; out dx, al
;
; ; FIFO control registers
; ; enable + clear
; mov al, 00000111b
; mov dx, COM1+2
; out dx, al
;
; ; Modem control register
; mov al, 1001b
; mov dx, COM1+4
; out dx, al
;
; ; Modem control register
; mov al, 1111b
; mov dx, COM1+4
; out dx, al

mov rsp, rbp
pop rbp
ret


global serial_send
serial_send:
push rbp
mov rbp, rsp

; ; check if transmitter is doing something
; .try:
; mov dx, COM1+5
; in al, dx
; test al, 1 << 5
; jz .try

; Send some stuff
mov al, 65
mov dx, COM1
out dx, al


mov rsp, rbp
pop rbp
ret



global port_out
port_out:
push rbp
mov rbp, rsp

; rdi(byte): di: port
; rsi(word): sil: value

; out only accepts al and dx as register operands, therefore we cannot use si and dil directly
mov al, sil
mov dx, di
out dx, al

mov rsp, rbp
pop rbp
ret
