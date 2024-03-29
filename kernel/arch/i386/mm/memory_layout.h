#ifndef _MEMORY_LAYOUT_H_
#define _MEMORY_LAYOUT_H_

#include <stdint.h>

#define KERNEL_PA_TO_VA_OFFSET 0xC0000000
#define KERNEL_PA_TO_VA(pa) ((pa) + KERNEL_PA_TO_VA_OFFSET)
#define KERNEL_VA_TO_PA(va) ((va) - KERNEL_PA_TO_VA_OFFSET)

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
 * 
 *
 *     Physical Address Space Layout
 * DMA ZONE: 0x100000 - 0x8000000 (15 MB)
 * NORMAL ZONE: 0x8000000 - 0x1C0000000 (880 MB)
 * HIGH ZONE: 0x1C0000000 - 100000000 (3200 MB)
 * 
 * The DMA zone is required to work with some devices that cannot address memory above 16 megabytes.
 * Memory from this zone is allocated last, usually for the needs of the kernel when working with devices.
 * 
 * The NORMAL zone is used for basic memory allocations by the kernel for its needs.
 * Also, memory for user applications is taken from here if it is not in the HIGH zone.
 * 
 * The HIGH zone is mainly used to allocate memory for user applications, the kernel does not allocate free pages for itself from this area.
 * If any data from this zone is needed by the kernel, then it will flood it into its VMA area.
 * 
 * ---------------------------- 0x00000000
 * |         RESERVED         |
 * | DMA   KERNEL BINARY      | 0x100000 DMA ZONE START and kernel binary address
 * | DMA   KERNEL BINARY      | (kernel sections) (size: < 4 MB)
 * | DMA                      |
 * | NORMAL                   | DMA ZONE END and NORMAL ZONE START (0x1000000)
 * | NORMAL                   |
 * | NORMAL                   |
 * | HIGH                     | NORMAL ZONE END and HIGH ZONE START (0x38000000)
 * | HIGH                     |
 * | HIGH                     |
 * | HIGH                     | HIGH ZONE END (0x100000000)
 * ----------------------------
 * 
 *     Virtual Address Space Layout
 * ---------------------------- 0xC0000000
 * |         RESERVED         |
 * | DMA   KERNEL BINARY      | 0xC0100000 DMA ZONE START and kernel binary address
 * | DMA   KERNEL BINARY      | (kernel sections) (size: < 4 MB)
 * | DMA                      |
 * | NORMAL                   | DMA ZONE END and NORMAL ZONE START (0xC1000000)
 * | NORMAL                   |
 * | NORMAL                   |
 * | VMA                      | NORMAL ZONE END and VMA START (0xF8000000)
 * | VMA                      |
 * | VMA                      | VMA VMA END 0x100000000
 * ----------------------------
 * 
 * The VMA area is used to allocate large amounts of memory, since it is not always possible to allocate a large non-contiguous piece of physical memory.
 * Also, all stuff (used by drivers, like ACPI tables) from HIGH memory are mapped to this area.
 * PD is also mapped here.
 */

#define DMA_ZONE_START_PA 0x100000
#define DMA_ZONE_END_PA 0x1000000
#define NORMAL_ZONE_START_PA 0x1000000
#define NORMAL_ZONE_END_PA 0x38000000
#define HIGH_ZONE_START_PA 0x38000000
#define HIGH_ZONE_END_PA 0x100000000

#define DMA_ZONE_START_VA 0xC0100000
#define DMA_ZONE_END_VA 0xC1000000
#define NORMAL_ZONE_START_VA 0xC1000000
#define NORMAL_ZONE_END_VA 0xF8000000
#define VMA_START_VA 0xF8000000
#define VMA_END_VA 0x100000000

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
