#ifndef _MEMORY_LAYOUT_H_
#define _MEMORY_LAYOUT_H_

#include <stdint.h>
#include "mm.h"

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
 * 0x0
 * REAL MODE    1 MB
 * DMA ZONE     15 MB
 * NORMAL ZONE  880 MB
 * HIGH ZONE    3200 MB
 * 0x100000000
 * 
 * The DMA zone is required to work with some devices that cannot address memory above 16 megabytes.
 * Memory from this zone is allocated last, usually for the needs of the kernel when working with devices.
 * 
 * The NORMAL zone is used for basic memory allocations by the kernel for its needs.
 * Also, memory for user applications is taken from here if it is not in the HIGH zone.
 * 
 * The HIGH zone is mainly used to allocate memory for user applications, the kernel does not allocate free pages for itself from this area.
 * If any data from this zone is needed by the kernel, then it will map it to VMA area.
 * 
 * The VMA area is used to allocate large amounts of memory for kernel, since it is not always possible to allocate a large non-contiguous piece of physical memory.
 * Also, all stuff (used by drivers, like ACPI tables) from HIGH physical memory are mapped to this area.
 * PD is also mapped here.
 * 
 *     Virtual Address Space Layout
 * 0x0
 * USERSPACE | V86 MODE:            1 MB
 * USERSPACE | USERSPACE:           3071 MB
 * 0xC0000000
 * KERNEL    | DMA zone mapping:    16 MB
 * KERNEL    | NORMAL zone mapping: 880 MB
 * KERNEL    | VMA:                 124 MB
 * KERNEL    | PD self mapping:     4 MB
 * 0x100000000
 */

// Physical Address Space
#define REAL_MODE_START_PA 0 * 1024 * 1024 // 0x0
#define REAL_MODE_END_PA 1 * 1024 * 1024 // 0x100000
#define DMA_ZONE_START_PA 1 * 1024 * 1024 // 0x100000
#define DMA_ZONE_END_PA 16 * 1024 * 1024 // 0x1000000
#define NORMAL_ZONE_START_PA 16 * 1024 * 1024 // 0x1000000
#define NORMAL_ZONE_END_PA 896 * 1024 * 1024 // 0x38000000
#define HIGH_ZONE_START_PA 896 * 1024 * 1024 // 0x38000000
#define HIGH_ZONE_END_PA 4096ll * 1024 * 1024 // 0x100000000

// Virtual Address Space
#define DMA_ZONE_START_VA KERNEL_PA_TO_VA(DMA_ZONE_START_PA) // 0xC0100000
#define DMA_ZONE_END_VA KERNEL_PA_TO_VA(DMA_ZONE_END_PA)// 0xC1000000
#define NORMAL_ZONE_START_VA KERNEL_PA_TO_VA(NORMAL_ZONE_START_PA) // 0xC1000000
#define NORMAL_ZONE_END_VA KERNEL_PA_TO_VA(NORMAL_ZONE_END_PA) // 0xF8000000
#define VMA_START_VA 896 * 1024 * 1024 + KERNEL_PA_TO_VA_OFFSET // 0xF8000000
#define VMA_END_VA 1020 * 1024 * 1024 + KERNEL_PA_TO_VA_OFFSET // 0xFFC00000
#define PD_SELF_MAPPING_START_VA 1020 * 1024 * 1024 + KERNEL_PA_TO_VA_OFFSET // 0xFFC00000
#define PD_SELF_MAPPING_END_VA 1024 * 1024 * 1024 + KERNEL_PA_TO_VA_OFFSET // 0x100000000ll

/*
 * The numbers of the physical frames defining the zones. Useful for initializing bootstrap bitmap allocator.
 * Attention: max pfn is also included in the zone and means the number of the last page in it.
 */
#define DMA_ZONE_MIN_PFN DMA_ZONE_START_PA / PAGE_SIZE // 256
#define DMA_ZONE_MAX_PFN DMA_ZONE_END_PA / PAGE_SIZE - 1 // 4095
#define NORMAL_ZONE_MIN_PFN NORMAL_ZONE_START_PA / PAGE_SIZE // 4096
#define NORMAL_ZONE_MAX_PFN NORMAL_ZONE_END_PA / PAGE_SIZE - 1 // 229375
#define HIGH_ZONE_MIN_PFN HIGH_ZONE_START_PA / PAGE_SIZE // 229376
#define HIGH_ZONE_MAX_PFN (uint32_t)(HIGH_ZONE_END_PA / PAGE_SIZE - 1) // 1048575

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
