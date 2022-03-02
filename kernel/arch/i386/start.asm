BITS 32 ; generate code designed to run on a processor operating in 32-bit mode

; Multiboot header constants
MAGIC     equ 0x1BADB002         ; 'magic' field
; Flags
MBALIGN   equ 1 << 0             ; boot modules must be alligned on page boundaries (4KB)
MEMINFO   equ 1 << 1             ; provide memory map
GRAPHICSF equ 1 << 2             ; enable graphics fields
FLAGS     equ MBALIGN | MEMINFO \
            | GRAPHICSF          ; 'flags' field
CHECKSUM  equ -(MAGIC + FLAGS)   ; checksum, value "which, when added to the other magic fields (i.e. ‘magic’ and ‘flags’), must have a 32-bit unsigned sum of zero."

EGAWIDTH  equ 80                 ; number of the columns
EGAHEIGHT equ 25                 ; number of the lines

; The bootloader will search for this signature in the first 8 KiB of the kernel file, aligned at a 32-bit boundary.
; The signature is in its own section so the header can be forced to be within the first 8 KiB of the kernel file.
section .multiboot.data
align 4
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
dd EGAWIDTH
dd EGAHEIGHT
dd 0

; "The OS image must create its own stack as soon as it needs one."
; The stack grows downwards on x86.
; The stack on x86 must be 16-byte aligned according to the System V ABI standard and de-facto extensions.
; The compiler will assume the stack is properly aligned and failure to align the stack will result in undefined behavior.
section .bootstrap_stack nobits alloc noexec write align=4 ; The information about the section is the same as that of a default bss
align 16
bootstrap_stack_bottom:
resb 16384 ; 16 KiB
bootstrap_stack_top:

; Preallocate pages used for paging.
section .bss
; Page directory and page table addresses must be page aligned
align 4096
boot_page_directory:
resb 4096
boot_page_table1:
resb 4096


extern _init
extern _fini
extern kmain
extern _kernel_start
extern _kernel_end
; Declare _start as a function symbol with the given symbol size.
section .multiboot.text progbits alloc exec nowrite align=16 ; The information about the section is the same as that of a default text
global _start;:function (_start.end - _start)
_start:
    ; Physical address of boot_page_table1
    mov edi, (boot_page_table1 - 0xC0000000)
    ; Current page address
    mov esi, 0
    ; Page counter for loop
    mov ecx, 1023

.check_and_make_pt:
    ; Only map the kernel
    cmp esi, _kernel_start
    jl .increase
    cmp esi, (_kernel_end - 0xC0000000)
    jge .make_pt_end

    ; TODO
    ; Map physical address as "present, writable". Note that this maps
    ; .text and .rodata as writable. Mind security and map them as non-writable.
    mov edx, esi
    or edx, 3
    mov [edi], edx

.increase:
    ; Size of entries in boot_page_table1 is 4 bytes.
    add edi, 4
    ; Size of page is 4096 bytes.
    add esi, 4096
    ; Loop to the next entry if we haven't finished.
    loop .check_and_make_pt

.make_pt_end:
    ; Map VGA video memory to 0xC03FF000 as "present, writable".
    mov [boot_page_table1 - 0xC0000000 + 1023 * 4], DWORD (0x000B8000 | 0x003)

    ; The page table is used at both page directory entry 0 (virtually from 0x0
    ; to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
    ; 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
    ; higher half). The kernel is identity mapped because enabling paging does
    ; not change the next instruction, which continues to be physical. The CPU
    ; would instead page fault if there was no identity mapping.

    ; Map the page table to both virtual addresses 0x00000000 and 0xC0000000.
    mov [boot_page_directory - 0xC0000000 + 0], DWORD boot_page_table1 - 0xC0000000 + 3
    mov [boot_page_directory - 0xC0000000 + 768 * 4], DWORD boot_page_table1 - 0xC0000000 + 3

    ; Set cr3 to the address of the boot_page_directory.
    mov ecx, boot_page_directory - 0xC0000000
    mov cr3, ecx

    ; Enable paging and the write-protect bit.
    mov ecx, cr0
    or ecx, 0x80010000
    mov cr0, ecx

    ; Jump to higher half with an absolute jump.
    lea ecx, .higher_half
    jmp ecx

section .text
.higher_half:
    ; Unmap the identity mapping as it is now unnecessary.
    mov [boot_page_directory + 0], DWORD 0

    ; Reload crc3 to force a TLB flush so the changes to take effect.
    mov ecx, cr3
    mov cr3, ecx

    ; The stack grows down.
    ; The stack must be setting up before calling C functions.
    mov esp, bootstrap_stack_top

    call _init
    call kmain
    call _fini

    cli
.halt:
    hlt
    jmp .halt
.end:

;section .rodata

;section .data
