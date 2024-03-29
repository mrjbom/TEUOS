#ifndef _MEMORY_LAYOUT_H_
#define _MEMORY_LAYOUT_H_

#include <stdint.h>

#define KERNEL_PA_TO_VA(pa) ((pa) + 0xC0000000)
#define KERNEL_VA_TO_PA(va) ((va) - 0xC0000000)

/*
 * Variables and constants required for memory management
 */

/*
 * Virtual address space layout
 * ---------------------------- 0x00000000
 * |                          |
 * |                          |
 * |                          |
 * |                          |
 * |                          |
 * |        USER SPACE        |
 * |                          |
 * |                          |
 * |                          |
 * |                          |
 * |                          |
 * |                          |
 * ---------------------------- 0xC0000000
 * |                          |
 * |       KERNEL SPACE       |
 * |                          |
 * |                          |
 * ---------------------------- 0xFFFFFFFF
 *
 * User space is separate for each process and changes when the process is switched over.
 * Kernel space is one and is always mapped to virtual space.
 */

/* 
 * Declared in the linker script
 * These symbols are themselves values in the symbol table, and they cannot be accessed as standart variables.
 */

extern uintptr_t _kernel_start; // Physical address (All others are virtual)
extern uintptr_t _kernel_end;

extern uintptr_t _text_start;
extern uintptr_t _text_end;

extern uintptr_t _rodata_start;
extern uintptr_t _rodata_end;

extern uintptr_t _data_start;
extern uintptr_t _data_end;

extern uintptr_t _bss_start;
extern uintptr_t _bss_end;

/*
 * You should use these variables.
 */

extern uintptr_t kernel_start_pa;
extern uintptr_t kernel_end_va;
extern uintptr_t kernel_end_va_page_aligned;

extern uintptr_t kernel_text_start_va;
extern uintptr_t kernel_text_end_va;
extern uintptr_t kernel_text_end_va_page_aligned;

extern uintptr_t kernel_rodata_start_va;
extern uintptr_t kernel_rodata_end_va;
extern uintptr_t kernel_rodata_end_va_page_aligned;

extern uintptr_t kernel_data_start_va;
extern uintptr_t kernel_data_end_va;
extern uintptr_t kernel_data_end_va_page_aligned;

extern uintptr_t kernel_bss_start_va;
extern uintptr_t kernel_bss_end_va;
extern uintptr_t kernel_bss_end_va_page_aligned;

extern void init_memory_layout_variables(void);

#endif
