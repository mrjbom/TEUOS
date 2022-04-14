#include "mmu.h"
#include "bits.h"
#include "pde.h"
#include "pte.h"

// Start and end addresses of sections
// .multiboot.data
// start
extern uintptr_t _multiboot_data_start;
uint32_t* _multiboot_data_start_addr = &_multiboot_data_start;
// end
extern uintptr_t _multiboot_data_end;
uint32_t* _multiboot_data_end_addr = &_multiboot_data_end;

// .multiboot.text
// start
extern uintptr_t _multiboot_text_start;
uint32_t* _multiboot_text_start_addr = &_multiboot_text_start;
// end
extern uintptr_t _multiboot_text_end;
uint32_t* _multiboot_text_end_addr = &_multiboot_text_end;

// .text section
// start
extern uintptr_t _text_start;
uint32_t* _text_start_addr = &_text_start;
// end
extern uintptr_t _text_end;
uint32_t* _text_end_addr = &_text_end;

// .rodata section
// start
extern uintptr_t _rodata_start;
uint32_t* _rodata_start_addr = &_rodata_start;
// end
extern uintptr_t _rodata_end;
uint32_t* _rodata_end_addr = &_rodata_end;

// .data section
// start
extern uintptr_t _data_start;
uint32_t* _data_start_addr = &_data_start;
// end
extern uintptr_t _data_end;
uint32_t* _data_end_addr = &_data_end;

// .bss section
// start
extern uintptr_t _bss_start;
uint32_t* _bss_start_addr = &_bss_start;
// end
extern uintptr_t _bss_end;
uint32_t* _bss_end_addr = &_bss_end;

void mmu_init_sections_permissions(void)
{
    uint32_t* boot_page_directory = 0;
    __asm__ volatile ("mov %0, cr3" : "=r"(boot_page_directory));
    // Convert from physical to virtual addres
    boot_page_directory = (uint32_t*)((uint8_t*)boot_page_directory + 0xC0000000);
    uint32_t* boot_page_table1 = (uint32_t*)boot_page_directory[768];
    // Convert from physical to virtual addres
    boot_page_table1 = (uint32_t*)((uint8_t*)boot_page_table1 + 0xC0000000);
    // Get only the address of the page table
    boot_page_table1 = pde_get_frame_addr((pd_entry)boot_page_table1);
    
    // Setting permissions for .text
    for(uint32_t frame_addr = (uint32_t)_text_start_addr; frame_addr < (uint32_t)_text_end_addr; frame_addr += 4096) {
        uint32_t index = (frame_addr - 0xC0000000) / 4096;
        pt_entry* pte_value_ptr = &boot_page_table1[index];
        pte_del_attrib(pte_value_ptr, PTE_WRITABLE);
    }
    // Setting permissions for .rodata
    for(uint32_t frame_addr = (uint32_t)_rodata_start_addr; frame_addr < (uint32_t)_rodata_end_addr; frame_addr += 4096) {
        uint32_t index = (frame_addr - 0xC0000000) / 4096;
        pt_entry* pte_value_ptr = &boot_page_table1[index];
        pte_del_attrib(pte_value_ptr, PTE_WRITABLE);
    }
}
