%macro ISR_NOERRCODE 1
global isr%1

isr%1:
    cli     ; usless if interrupt gate type
    push 0  ; fake errorcode
    push %1 ; ISR num
    jmp isr_common_stub

%endmacro

%macro ISR_ERRCODE 1
global isr%1

isr%1:
    cli     ; usless if interrupt gate type
    push %1 ; ISR num
    jmp isr_common_stub

%endmacro

section .text
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE 21
; 22 - 31  Intel reserved. Do not use.
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31
; Hardware PIC interrupts
ISR_NOERRCODE 32
ISR_NOERRCODE 33
ISR_NOERRCODE 34
ISR_NOERRCODE 35
ISR_NOERRCODE 36
ISR_NOERRCODE 37
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47

extern isr_handler
isr_common_stub:
    pusha               ; push EAX, ECX, EDX, EBX, "original" ESP, EBP, ESI, EDI

    mov     eax, 0
    mov     ax, ds      ; save ds
    push    eax

    mov     ax, 0x10    ; load kernel data segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    call    isr_handler ; call C handler

    pop     eax         ; restore ds
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    popa                ; pop EAX, ECX, EDX, EBX, "original" ESP, EBP, ESI, EDI
    add     esp, 8      ; delete error code and ISR num
    iret                ; pushed out of the stack: EIP, CS, EFLAGS, ESP, SS
