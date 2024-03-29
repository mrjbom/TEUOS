#include "memory_layout.h"
#include <stdint.h>
#include "mm.h"
#include "../useful/useful_things.h"

uintptr_t kernel_start_pa;
uintptr_t kernel_end_va;
uintptr_t kernel_end_va_page_aligned;

uintptr_t kernel_text_start_va;
uintptr_t kernel_text_end_va;
uintptr_t kernel_text_end_va_page_aligned;

uintptr_t kernel_rodata_start_va;
uintptr_t kernel_rodata_end_va;
uintptr_t kernel_rodata_end_va_page_aligned;

uintptr_t kernel_data_start_va;
uintptr_t kernel_data_end_va;
uintptr_t kernel_data_end_va_page_aligned;

uintptr_t kernel_bss_start_va;
uintptr_t kernel_bss_end_va;
uintptr_t kernel_bss_end_va_page_aligned;

void init_memory_layout_variables()
{
    kernel_start_pa = (uintptr_t)&_kernel_start;
    kernel_end_va = (uintptr_t)&_kernel_end;
    kernel_end_va_page_aligned = kernel_end_va;
    useful_align_to(&kernel_end_va_page_aligned, PAGE_SIZE);

    kernel_text_start_va = (uintptr_t)&_text_start;
    kernel_text_end_va = (uintptr_t)&_text_end;
    kernel_text_end_va_page_aligned = kernel_text_end_va;
    useful_align_to(&kernel_text_end_va_page_aligned, PAGE_SIZE);

    kernel_rodata_start_va = (uintptr_t)&_rodata_start;
    kernel_rodata_end_va = (uintptr_t)&_rodata_end;
    kernel_rodata_end_va_page_aligned = kernel_rodata_end_va;
    useful_align_to(&kernel_rodata_end_va_page_aligned, PAGE_SIZE);

    kernel_data_start_va = (uintptr_t)&_data_start;
    kernel_data_end_va = (uintptr_t)&_data_end;
    kernel_data_end_va_page_aligned = kernel_data_end_va;
    useful_align_to(&kernel_data_end_va_page_aligned, PAGE_SIZE);

    kernel_bss_start_va = (uintptr_t)&_bss_start;
    kernel_bss_end_va = (uintptr_t)&_bss_end;
    kernel_bss_end_va_page_aligned = kernel_bss_end_va;
    useful_align_to(&kernel_bss_end_va_page_aligned, PAGE_SIZE);
}
