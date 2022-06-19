section .text

global tlb_flush
tlb_flush:
    mov eax, cr3
    mov cr3, eax
    ret
