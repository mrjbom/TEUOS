#include "memory_layout.h"
#include "mm.h"
#include "../useful/useful_things.h"

uintptr_t kernel_start_physical_addr = 0;
uintptr_t kernel_start_virtual_addr = 0;
uintptr_t kernel_end_physical_addr = 0;
uintptr_t kernel_end_virtual_addr = 0;
uintptr_t kernel_end_physical_addr_page_aligned = 0;
uintptr_t kernel_end_virtual_addr_page_aligned = 0;

uintptr_t kernel_text_start_physical_addr = 0;
uintptr_t kernel_text_start_virtual_addr = 0;
uintptr_t kernel_text_end_physical_addr = 0;
uintptr_t kernel_text_end_virtual_addr = 0;
uintptr_t kernel_text_end_physical_addr_page_aligned = 0;
uintptr_t kernel_text_end_virtual_addr_page_aligned = 0;

uintptr_t kernel_rodata_start_physical_addr = 0;
uintptr_t kernel_rodata_start_virtual_addr = 0;
uintptr_t kernel_rodata_end_physical_addr = 0;
uintptr_t kernel_rodata_end_virtual_addr = 0;
uintptr_t kernel_rodata_end_physical_addr_page_aligned = 0;
uintptr_t kernel_rodata_end_virtual_addr_page_aligned = 0;

uintptr_t kernel_data_start_physical_addr = 0;
uintptr_t kernel_data_start_virtual_addr = 0;
uintptr_t kernel_data_end_physical_addr = 0;
uintptr_t kernel_data_end_virtual_addr = 0;
uintptr_t kernel_data_end_physical_addr_page_aligned = 0;
uintptr_t kernel_data_end_virtual_addr_page_aligned = 0;

uintptr_t kernel_bss_start_physical_addr = 0;
uintptr_t kernel_bss_start_virtual_addr = 0;
uintptr_t kernel_bss_end_physical_addr = 0;
uintptr_t kernel_bss_end_virtual_addr = 0;
uintptr_t kernel_bss_end_physical_addr_page_aligned = 0;
uintptr_t kernel_bss_end_virtual_addr_page_aligned = 0;

void init_memory_layout_variables()
{
    kernel_start_physical_addr = (uintptr_t)&_kernel_start;
    kernel_start_virtual_addr = KERNEL_PHYSICAL_TO_VIRTUAL(kernel_start_physical_addr);
    kernel_end_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_kernel_end);
    kernel_end_virtual_addr = (uintptr_t)&_kernel_end;
    kernel_end_physical_addr_page_aligned = kernel_end_physical_addr;
    useful_align_to(&kernel_end_physical_addr_page_aligned, PAGE_SIZE);
    kernel_end_virtual_addr_page_aligned = kernel_end_virtual_addr;
    useful_align_to(&kernel_end_virtual_addr_page_aligned, PAGE_SIZE);

    kernel_text_start_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_text_start);
    kernel_text_start_virtual_addr = (uintptr_t)&_text_start;
    kernel_text_end_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_text_end);
    kernel_text_end_virtual_addr = (uintptr_t)&_text_end;
    kernel_text_end_physical_addr_page_aligned = kernel_text_end_physical_addr;
    useful_align_to(&kernel_text_end_physical_addr_page_aligned, PAGE_SIZE);
    kernel_text_end_virtual_addr_page_aligned = kernel_text_end_virtual_addr;
    useful_align_to(&kernel_text_end_virtual_addr_page_aligned, PAGE_SIZE);

    kernel_rodata_start_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_rodata_start);
    kernel_rodata_start_virtual_addr = (uintptr_t)&_rodata_start;
    kernel_rodata_end_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_rodata_end);
    kernel_rodata_end_virtual_addr = (uintptr_t)&_rodata_end;
    kernel_rodata_end_physical_addr_page_aligned = kernel_rodata_end_physical_addr;
    useful_align_to(&kernel_rodata_end_physical_addr_page_aligned, PAGE_SIZE);
    kernel_rodata_end_virtual_addr_page_aligned = kernel_rodata_end_virtual_addr;
    useful_align_to(&kernel_rodata_end_virtual_addr_page_aligned, PAGE_SIZE);

    kernel_data_start_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_data_start);
    kernel_data_start_virtual_addr = (uintptr_t)&_data_start;
    kernel_data_end_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_data_end);
    kernel_data_end_virtual_addr = (uintptr_t)&_data_end;
    kernel_data_end_physical_addr_page_aligned = kernel_data_end_physical_addr;
    useful_align_to(&kernel_data_end_physical_addr_page_aligned, PAGE_SIZE);
    kernel_data_end_virtual_addr_page_aligned = kernel_data_end_virtual_addr;
    useful_align_to(&kernel_data_end_virtual_addr_page_aligned, PAGE_SIZE);

    kernel_bss_start_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_bss_start);
    kernel_bss_start_virtual_addr = (uintptr_t)&_bss_start;
    kernel_bss_end_physical_addr = KERNEL_VIRTUAL_TO_PHYSICAL((uintptr_t)&_bss_end);
    kernel_bss_end_virtual_addr = (uintptr_t)&_bss_end;
    kernel_bss_end_physical_addr_page_aligned = kernel_bss_end_physical_addr;
    useful_align_to(&kernel_bss_end_physical_addr_page_aligned, PAGE_SIZE);
    kernel_bss_end_virtual_addr_page_aligned = kernel_bss_end_virtual_addr;
    useful_align_to(&kernel_bss_end_virtual_addr_page_aligned, PAGE_SIZE);
}
