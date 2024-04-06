#include "pmm.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../../multiboot/multiboot_utils.h"
#include "../../useful/useful_things.h"
#include "../../mm/memory_layout.h"
#include "../mm.h"

#define PMM_AREAS_MAX_NUMBER 64

size_t pmm_mbi_areas_count = 0;
pmm_memory_area_mbi_t pmm_mbi_areas[PMM_AREAS_MAX_NUMBER];

size_t pmm_free_areas_count = 0;
pmm_memory_free_area_t pmm_free_areas[PMM_AREAS_MAX_NUMBER];

/*
 * Copies areas info from MBI to pmm_mbi_areas
 */
static void copy_mbi_areas_info(uintptr_t mbi_addr)
{
    multiboot_tag_mmap_t* mmap_tag_ptr = (multiboot_tag_mmap_t*)multiboot_get_tag_ptr(mbi_addr, MULTIBOOT_TAG_TYPE_MMAP);
    if (mmap_tag_ptr == NULL) {
        kpanic("PMM/MULTIBOOT", "MMAP TAG was not found!", __FILE__, __LINE__);
    }
    size_t entries_num = (mmap_tag_ptr->size - 16) / mmap_tag_ptr->entry_size;
    if (entries_num > PMM_AREAS_MAX_NUMBER) {
        kpanic("PMM/MULTIBOOT", "There are too many memory map entries, can't save them all!", __FILE__, __LINE__);
    }
    
    // Copy entries
    for (uint32_t entry_index = 0; entry_index < entries_num; ++entry_index) {
        uintptr_t current_entry_addr = (uintptr_t)mmap_tag_ptr + 16 + entry_index * mmap_tag_ptr->entry_size;
        multiboot_mmap_entry_t* current_entry_ptr = (multiboot_mmap_entry_t*)current_entry_addr;
        // Copy current entry
        pmm_mbi_areas[pmm_mbi_areas_count].addr = current_entry_ptr->addr;
        pmm_mbi_areas[pmm_mbi_areas_count].size = current_entry_ptr->len;
        if (current_entry_ptr->type > MULTIBOOT_MEMORY_BADRAM) {
            kpanic("PMM/MULTIBOOT", "Memory map entry type is wrong!", __FILE__, __LINE__);
        }
        pmm_mbi_areas[pmm_mbi_areas_count].type = current_entry_ptr->type;
        pmm_mbi_areas_count++;
    }
}

/*
 * Sort free areas (bubble sort)
 */
static void sort_free_areas(pmm_memory_free_area_t* areas, size_t count)
{
    if (count < 2) {
        return;
    }
    for (size_t i = 0; i < count - 1; i++) {
        for (size_t j = 0; j < count - i - 1; j++) {
            if (areas[j].addr > areas[j + 1].addr) {
                // Swap
                pmm_memory_free_area_t tmp = areas[j];
                areas[j] = areas[j + 1];
                areas[j + 1] = tmp;
            }
        }
    }
}

/*
 * Fill pmm_free_areas using pmm_mbi_areas
 * The code combines adjacent areas, fixes overlaps and combine areas
 */
static void fill_free_areas_info(void)
{
    // Marks all areas as { 0xFFFFFFFF, 0xFFFFFFFF }, so all valid areas will be at the beginning of the list, because they will be sorted by addresses
    memset(pmm_free_areas, UINT8_MAX, sizeof(pmm_free_areas));

    // Iterating over pmm_mbi_areas array and copying available areas to pmm_free_areas
    for (size_t i = 0; i < pmm_mbi_areas_count; ++i) {
        // Skip unavailable areas
        if (pmm_mbi_areas[i].type != MULTIBOOT_MEMORY_AVAILABLE) {
            continue;
        }
        // Skip areas below 1 MB and above 4 GB
        if (pmm_mbi_areas[i].addr < FIRST_1MB_END || pmm_mbi_areas[i].addr >= FIRST_4GB_END) {
            continue;
        }
        // Skip areas with a size of less than two pages,
        // because with further alignment of their addresses and sizes, they may become smaller than the page
        if (pmm_mbi_areas[i].size < PAGE_SIZE * 2) {
            continue;
        }

        // If the entire area is within 4 GB, just copy its info
        if (pmm_mbi_areas[i].addr + pmm_mbi_areas[i].size <= FIRST_4GB_END) {
            pmm_free_areas[pmm_free_areas_count].addr = (uintptr_t)pmm_mbi_areas[i].addr;
            pmm_free_areas[pmm_free_areas_count].size = (uintptr_t)pmm_mbi_areas[i].size;
            pmm_free_areas_count++;
        }
        // The beginning of the area is below 4 GB, but it goes beyond its 4 GB limit. We need to reduce its size.
        else {
            size_t new_size = (size_t)(FIRST_4GB_END - pmm_free_areas[pmm_free_areas_count].addr);
            pmm_free_areas[pmm_free_areas_count].addr = (uintptr_t)pmm_mbi_areas[i].addr;
            pmm_free_areas[pmm_free_areas_count].size = new_size;
            pmm_free_areas_count++;
        }
    }

    if (pmm_free_areas_count == 0) {
        kpanic("PMM/FREE AREAS", "Free areas count is 0!", __FILE__, __LINE__);
    }

    // Sort areas by addreses
    sort_free_areas(pmm_free_areas, pmm_free_areas_count);

    // Remove memory used by the kernel
    // The area after the 0x100000 in which the kernel is located must be the first in the array
    if (pmm_free_areas[0].addr != FIRST_1MB_END) {
        kpanic("PMM/FREE AREAS", "The first area of pmm_areas is not 0x100000!", __FILE__, __LINE__);
    }
    // Set the area address after the kernel and reduce size
    size_t kernel_size = KERNEL_VA_TO_PA(kernel_end_va_page_aligned) - kernel_start_pa;
    pmm_free_areas[0].addr += kernel_size;
    pmm_free_areas[0].size -= kernel_size;

    // It's almost impossible, but you never know what
    if (pmm_free_areas[0].size < PAGE_SIZE * 2) {
        pmm_free_areas[0].addr = UINT32_MAX;
        pmm_free_areas[0].size = UINT32_MAX;
        pmm_free_areas_count--;
    }

    // Sort areas by addreses (deleted places to end)
    sort_free_areas(pmm_free_areas, pmm_free_areas_count);

    // Combine areas and fix overlapings
    // Some types of overlaps cannot be fixed at one time.
    // For example {0, 10} {2, 2} {6, 2}. During the first iteration, {2, 2} will be deleted, but another one will be needed to delete {6, 2}.
    bool fix_occurs = false;
    do {
        fix_occurs = false;
        // Combine areas and fix overlapings
        size_t old_pmm_free_areas_count = pmm_free_areas_count;
        for (size_t i = old_pmm_free_areas_count - 1; i > 0; --i) {
            // Are the areas adjacent but not overlapping?
            // [ ]   {0, 1}
            //   [ ] {1, 1}
            // {0, 2}
            if (pmm_free_areas[i - 1].addr + pmm_free_areas[i - 1].size == pmm_free_areas[i].addr) {
                // Let's combine them into one
                pmm_free_areas[i - 1].size += pmm_free_areas[i].size;
                // Mark current as deleted
                pmm_free_areas[i].addr = UINT32_MAX;
                pmm_free_areas[i].size = UINT32_MAX;
                pmm_free_areas_count--;
                fix_occurs = true;
            }
            // Are the areas overlap? (Does the previous overlap the current?)
            else if (pmm_free_areas[i - 1].addr + pmm_free_areas[i - 1].size > pmm_free_areas[i].addr) {
                // The previous area overlaps the current
                // Partially:
                // prev.addr + prev.size < current.addr + current.size
                // prev [    ]   {0, 4}
                // curr   [    ] {2, 4}
                // {0, 6}
                // Fully:
                // prev.addr + prev.size == current.addr + current.size
                // prev [      ] {0, 6}
                // curr   [    ] {2, 4}
                // {0, 6}
                // More than fully
                // prev.addr + prev.size > current.addr + current.size
                // prev [        ] {0, 8}
                // curr   [    ]   {2, 4}
                // {0, 8}
                if (pmm_free_areas[i - 1].addr + pmm_free_areas[i - 1].size < pmm_free_areas[i].addr + pmm_free_areas[i].size) {
                    // The previous area partially overlaps the current
                    pmm_free_areas[i - 1].size = pmm_free_areas[i].addr + pmm_free_areas[i].size;
                }
                // In the remaining two overlaping cases (fully and more), it is enough just to delete the current area
                // Mark current as deleted
                pmm_free_areas[i].addr = UINT32_MAX;
                pmm_free_areas[i].size = UINT32_MAX;
                pmm_free_areas_count--;
                fix_occurs = true;
            }
        }
        // Places deleted to end
        sort_free_areas(pmm_free_areas, old_pmm_free_areas_count);
    } while (fix_occurs);

    // Iterating over pmm_free_areas array and correct the addresses and sizes so that the addresses and sizes are aligned on the page size
    for (size_t i = 0; i < pmm_free_areas_count; ++i) {
        // Align address to next page bound
        size_t new_bytes = useful_align_up(&pmm_free_areas[i].addr, PAGE_SIZE);
        if (new_bytes != 0) {
            // Address has been aligned up, so we need to reduce the size of the area
            pmm_free_areas[i].size -= new_bytes;
        }
        // Align the size and divide it by a multiple of the page size
        new_bytes = useful_align_down(&pmm_free_areas[i].size, PAGE_SIZE);
        
        if (pmm_free_areas[i].size < PAGE_SIZE) {
            kpanic("PMM/FREE AREAS", "After the areas were aligned, the size became 0!", __FILE__, __LINE__);
        }
    }
}

void pmm_init_memory_info(uintptr_t mbi_addr)
{
    copy_mbi_areas_info(mbi_addr);
    //pmm_print_mbi_areas();
    fill_free_areas_info();
    pmm_print_free_areas();
}

void pmm_print_mbi_areas()
{
    printf_serial("mbi areas (%u):\n", pmm_mbi_areas_count);
    for (size_t i = 0; i < pmm_mbi_areas_count; ++i) {
        printf_serial("base_addr: 0x%llx\n", pmm_mbi_areas[i].addr);
        printf_serial("size: %llu\n", pmm_mbi_areas[i].size);
        switch (pmm_mbi_areas[i].type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            printf_serial("type: AVAILABLE\n");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            printf_serial("type: RESERVED\n");
            break;
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
            printf_serial("type: ACPI_RECLAIMABLE\n");
            break;
        case MULTIBOOT_MEMORY_NVS:
            printf_serial("type: NVS\n");
            break;
        case MULTIBOOT_MEMORY_BADRAM:
            printf_serial("type: BADRAM\n");
            break;
        default:
            printf_serial("type: WRONG TYPE!\n");
            break;
        }
    }
}

void pmm_print_free_areas(void)
{
    printf_serial("free areas (%u):\n", pmm_free_areas_count);
    for (size_t i = 0; i < pmm_free_areas_count; ++i) {
        printf_serial("base_addr: 0x%x\n", pmm_free_areas[i].addr);
        printf_serial("size: %u (0x%x)\n", pmm_free_areas[i].size, pmm_free_areas[i].size);
    }
}
