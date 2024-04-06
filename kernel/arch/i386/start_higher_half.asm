extern bootstrap_page_directory
extern bootstrap_stack_top
extern _init
extern _fini
extern kmain

global higher_half
global higher_half.halt

section .text
higher_half:
    ; Unmap the identity mapping as it is now unnecessary.
    mov [bootstrap_page_directory + 0], DWORD 0

    ; Reload cr3 to force a TLB flush so the changes to take effect.
    mov ecx, cr3
    mov cr3, ecx

    ; The stack grows down.
    ; The stack must be setting up before calling C functions.
    mov esp, bootstrap_stack_top

    ; Save EAX and EBX
    push eax
    push ebx
    call _init
    ; Restore EAX and EBX
    pop ebx
    pop eax
    
    ; Push EBX(virtual address of the MBI copy) and EAX(Magic Number)
    push ebx
    push eax
    call kmain
    pop eax
    pop ebx
    call _fini
    
    cli
.halt:
    hlt
    jmp .halt
.end:
