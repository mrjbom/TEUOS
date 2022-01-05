BITS 32 ; generate code designed to run on a processor operating in 16-bit mode

; Multiboot header constants
MAGIC     equ 0x1BADB002         ; 'magic' field
; Flags
MBALIGN   equ 1 << 0             ; boot modules must be alligned on page boundaries (4KB)
MEMINFO   equ 1 << 1             ; provide memory map
GRAPHICSF equ 1 << 2             ; enable graphics fields
FLAGS     equ MBALIGN | MEMINFO \
            | GRAPHICSF          ; 'flags' field
CHECKSUM  equ -(MAGIC + FLAGS)   ; checksum, value "which, when added to the other magic fields (i.e. ‘magic’ and ‘flags’), must have a 32-bit unsigned sum of zero."

; The bootloader will search for this signature in the first 8 KiB of the kernel file, aligned at a 32-bit boundary.
; The signature is in its own section so the header can be forced to be within the first 8 KiB of the kernel file.
section .multiboot
; Magic fields
dd MAGIC
dd FLAGS
dd CHECKSUM
; Address fields
dd 0
dd 0
dd 0
dd 0
dd 0
; Graphics fields
dd 1
dd 80
dd 25
dd 0

; "The OS image must create its own stack as soon as it needs one."
; The stack grows downwards on x86.
; The stack on x86 must be 16-byte aligned according to the System V ABI standard and de-facto extensions.
; The compiler will assume the stack is properly aligned and failure to align the stack will result in undefined behavior.
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

; Declare _start as a function symbol with the given symbol size.
section .text
global _start:function (_start.end - _start)
extern kmain
_start:
    ; The stack grows down. The stack must be configured before calling C functions.
    mov esp, stack_top

    call kmain

    cli
.halt:
    hlt
    jmp .halt
.end:

section .rodata
global stack_bottom_addr
global stack_top_addr

stack_bottom_addr dd stack_bottom
stack_top_addr dd stack_top
