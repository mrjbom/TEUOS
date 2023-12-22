#include "other.h"
#include "bits.h"
#include "../vmm/pde.h"
#include "../vmm/pte.h"

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

// TODO: move this prototype to vmm.h 
extern void tlb_flush(void);

void other_set_kernel_sections_permissions(void)
{
    uint32_t* boot_page_directory = 0;
    __asm__ volatile ("mov %0, cr3" : "=r"(boot_page_directory));
    // Convert from physical to virtual address
    boot_page_directory = (uint32_t*)((uint8_t*)boot_page_directory + 0xC0000000);
    uint32_t* boot_page_table1 = (uint32_t*)boot_page_directory[768];
    // Convert from physical to virtual address
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

    tlb_flush();
}

multiboot_info_t mbi_copy;

#define MMAP_ENTRIES_MAX_COUNT 32
multiboot_memory_map_t mmap_entries[MMAP_ENTRIES_MAX_COUNT];

uint32_t* boot_page_directory = 0;

multiboot_info_t* other_copy_multiboot_info_from_physical_addr(multiboot_info_t* mbi_phys_addr)
{
    // TODO: USE MULTIBOOT2

    // In first 1 mb?
    /* TODO:
     * multiboot specification says:
     * The Multiboot information structure and its related substructures may be placed anywhere in memory by the boot loader
     * (with the exception of the memory reserved for the kernel and boot modules, of course)
     * Now I hope that the entire structure is contained in the first megabyte of RAM, but this is not guaranteed, which means that I should ensure safe access to this memory (since virtual memory is enabled).
     * Decisions:
     * 1.(good) Managing virtual memory to read and copy data.
     * 2.(bad and rude) copy the data at the start stage, before turning on the virtual memory.
     */
    if((uint32_t)mbi_phys_addr > 0x100000) {
        return 0;
    };

    other_map_first1MB_identity();

    // Copy multiboot info struct
    memcpy(&mbi_copy, mbi_phys_addr, sizeof(multiboot_info_t));

    /* Are mem_* valid? */
    if (!get_bit(mbi_phys_addr->flags, 0)) { return 0; }

    // Can we copy all mmap entries?
    if(mbi_phys_addr->mmap_length / sizeof(multiboot_memory_map_t) > MMAP_ENTRIES_MAX_COUNT) {
        return 0;
    }
    // Copy mmap entries
    for(uint32_t i = 0, j = 0; i < mbi_phys_addr->mmap_length; i += sizeof(multiboot_memory_map_t), ++j) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*)(mbi_phys_addr->mmap_addr + i);
        memcpy(&mmap_entries[j], entry, sizeof(multiboot_memory_map_t));
    }

    mbi_copy.mmap_addr = (uint32_t)mmap_entries;
    mbi_copy.mmap_length = mbi_phys_addr->mmap_length;

    other_unmap_first1MB_identity();
    return &mbi_copy;
}

void other_map_first1MB_identity()
{
    uint32_t page_table_first4mb[256] __attribute__((aligned(4096))); // 1 MB

    // Map first physical 1 MB to virtual 1 MB(identity)
    __asm__ volatile ("mov %0, cr3" : "=r"(boot_page_directory));
    // Convert from physical to virtual address
    boot_page_directory = (uint32_t*)((uint8_t*)boot_page_directory + 0xC0000000);
    
    // Fill page table for first 1MB(4 MB)
    for(uint32_t i = 0; i < 256; ++i) {
        pte_set_frame_addr(&page_table_first4mb[i], i * 0x1000);
        pte_add_attrib(&page_table_first4mb[i], PTE_PRESENT);
    }

    // Set physical address of page table to page directory[0] and map first 4 MB(1MB)
    pde_set_frame_addr(&boot_page_directory[0], (uint32_t)page_table_first4mb - 0xC0000000);
    pde_add_attrib(&boot_page_directory[0], PTE_PRESENT);
    tlb_flush();
}

void other_unmap_first1MB_identity()
{
    // Unmap first 4 MB
    boot_page_directory[0] = 0;
    tlb_flush();
}
