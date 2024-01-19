BITS 32 ; generate code designed to run on a processor operating in 32-bit mode

; Multiboot header constants
MAGIC         equ 0xE85250D6                                      ; 'magic' field
ARCH          equ 0                                               ; 32-bit (protected) mode of i386
HEADER_LENGTH equ .multiboot_header_end - .multiboot_header_start ; length of Multiboot2 header in bytes including magic fields.
CHECK_SUM     equ -(MAGIC + ARCH + HEADER_LENGTH)                 ; check sum

EGA_WIDTH     equ 80                                              ; number of the columns
EGA_HEIGHT    equ 25                                              ; number of the lines

MULTIBOOT_INFO_MAX_SIZE equ 4096 ; size of the memory to which the multiboot info will be copied

; Multiboot2 header
; The bootloader will search for this signature in the first 32 KiB of the kernel file, aligned at a 64-bit boundary.
; The signature is in its own section so the header can be forced to be within the first 32 KiB of the kernel file.
section .multiboot.data align=8
; Magic fields
.multiboot_header_start:
dd MAGIC
dd ARCH
dd HEADER_LENGTH
dd CHECK_SUM
; Tags
; 3.1.9 Flags tag
align 8 ; every tag start at 8-bytes aligned address
.flags_tag_start:
dw 4                                                 ; type
dw 0                                                 ; flags
dd .flags_tag_end - .flags_tag_start                 ; size
dd 10b                                               ; console_flags
.flags_tag_end:
; 3.1.10 The framebuffer tag of Multiboot2 header
align 8
.framebuffer_tag_start:
dw 5                                                 ; type
dw 0                                                 ; flags: it is not optional tag
dd .framebuffer_tag_end - .framebuffer_tag_start     ; size
dd EGA_WIDTH                                         ; width
dd EGA_HEIGHT                                        ; height
dd 0                                                 ; depth: text mode
.framebuffer_tag_end:
; Terminator tag
align 8
dw 0                                                 ; type
dw 0                                                 ; flags
dd 8                                                 ; size
.multiboot_header_end:

; "The OS image must create its own stack as soon as it needs one."
; The stack grows downwards on x86.
; The stack on x86 must be 16-byte aligned according to the System V ABI standard and de-facto extensions.
; The compiler will assume the stack is properly aligned and failure to align the stack will result in undefined behavior.
section .bootstrap_stack nobits alloc noexec write align=16 ; The information about the section is the same as that of a default bss
bootstrap_stack_bottom:
resb 16384 ; 16 KiB
bootstrap_stack_top:

global bootstrap_stack_top
global boot_page_directory

; Preallocate pages used for paging.
section .bss
; Page directory and page table addresses must be page aligned
align 4096
boot_page_directory:
resb 4096
boot_page_table1:
resb 4096
; Here we will copy the multiboot info
align 8
multiboot_info:
resb MULTIBOOT_INFO_MAX_SIZE

extern _init
extern _fini
extern kmain
extern _kernel_start
extern _kernel_end

; From start_higher_half.asm
extern higher_half
extern higher_half.halt

; Declare _start as a function symbol with the given symbol size.
section .multiboot.text progbits alloc exec nowrite align=16 ; The information about the section is the same as that of a default text
global _start;:function (_start.end - _start)
_start:
    ; Calculate kernel size
    mov edi, (_kernel_end - 0xC0000000)
    sub edi, _kernel_start

    ; Check whether the whole kernel will fit into one page table (3 MiB) (First 1 MB reserved by )
    ; 768 pages it's 3 MiB (1 page for VGA video mem)
    cmp edi, 4096 * (768 - 1)
    ; halt if the kernel size is more than 3 MiB
    jge higher_half.halt

    ; I want to copy the multiboot info to the bss
    ; Get total_size(first 4 bytes at the beginning of the mbi) of multiboot info
    mov ecx, [ebx]
    cmp ecx, MULTIBOOT_INFO_MAX_SIZE
    ; halt if multiboot information is more than MULTIBOOT_INFO_MAX_SIZE
    jg higher_half.halt
    ; Copy multiboot info
    mov esi, ebx
    mov edi, (multiboot_info - 0xC0000000)
.copy_multiboot_info:
    movsb
    loop .copy_multiboot_info
    ; Now we have a copy of the multibutton info, we will replace its old address with a new address(virtual)
    mov ebx, multiboot_info

    ; Physical address of boot_page_table1
    mov edi, (boot_page_table1 - 0xC0000000)
    ; The physical address that the current page table entry points to (at the page table entry address in edi)
    mov esi, 0
    ; Map 1023 pages. The 1024th will be the VGA text buffer.
    mov ecx, 1023

.check_and_make_pt:
    ; Only map the kernel
    cmp esi, _kernel_start
    jl .increase
    cmp esi, (_kernel_end - 0xC0000000)
    jge .make_pt_end

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
    mov [boot_page_table1 - 0xC0000000 + 1023 * 4], DWORD (0x000B8000 | 3)

    ; The page table is used at both page directory entry 0 (virtually from 0x0
    ; to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
    ; 768 (virtually from 0xC0000000 to 0xC03FFFFF) (thus mapping it in the
    ; higher half). The kernel is identity mapped because enabling paging does
    ; not change the next instruction, which continues to be physical. The CPU
    ; would instead page fault if there was no identity mapping.

    ; Map the page table to both virtual addresses 0x00000000 and 0xC0000000 as "present, writable".
    mov [boot_page_directory - 0xC0000000 + 0], DWORD (boot_page_table1 - 0xC0000000) + 3
    mov [boot_page_directory - 0xC0000000 + (768 * 4)], DWORD (boot_page_table1 - 0xC0000000) + 3

    ; Set cr3 to the address of the boot_page_directory.
    mov ecx, (boot_page_directory - 0xC0000000)
    mov cr3, ecx

    ; Enable paging and the write-protect bit.
    mov ecx, cr0
    or ecx, 0x80010000
    mov cr0, ecx

    ; Jump to higher half with an absolute jump.
    lea ecx, higher_half
    jmp ecx

;section .rodata

;section .data
