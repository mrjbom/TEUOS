#include "pmm.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../../multiboot/multiboot2.h"
#include "../../useful/useful_things.h"

#define PMM_PAGE_SIZE 4096

typedef struct
{
    void* next; // for doubly linked list node
    void* prev; // for doubly linked list node
    uint32_t addr; // free area start addr
    uint32_t size; // free area size in pages
} pmm_free_area_t;

/* Before start allocating memory for PMM initialization, we need to find the free areas available */
#define INITIAL_FREE_AREAS_MAX_SIZE 16
size_t initial_free_areas_count = 0;
pmm_free_area_t initial_free_areas[INITIAL_FREE_AREAS_MAX_SIZE];

/* Main variables for memory management */
/* Number of free physical pages */
size_t free_pages_count = 0;

/*
 * Searches for free memory areas above 1 MB (under 4 GB) available for use (using multiboot info)
 */
static void pmm_init_find_free_areas(uintptr_t mbi_addr)
{
    // Parse multiboot info
    uintptr_t current_mbi_tag_addr = mbi_addr;
    // Skip total_size and reserved fields
    current_mbi_tag_addr += 8;
    // Stopping multiboot info parsing after getting information about memory areas
    bool mbi_parse_ended = false;
    while (!mbi_parse_ended)
    {
        // Every tag start at 8-bytes aligned address
        useful_align_to(&current_mbi_tag_addr, MULTIBOOT_TAG_ALIGN);
        multiboot_tag_t* multiboot_tag_ptr = (multiboot_tag_t*)current_mbi_tag_addr;
        
        if (multiboot_tag_ptr->type == MULTIBOOT_TAG_TYPE_MMAP) {
            multiboot_tag_mmap_t* mmap_tag_ptr = (multiboot_tag_mmap_t*)current_mbi_tag_addr;
            size_t entries_count = (mmap_tag_ptr->size - sizeof(multiboot_tag_mmap_t)) / mmap_tag_ptr->entry_size;
            
            for (size_t i = 0; i < entries_count; ++i) {
                multiboot_mmap_entry_t entry = mmap_tag_ptr->entries[i];
                if(entry.type != MULTIBOOT_MEMORY_AVAILABLE) {
                    continue;
                }
                // Free memory area found
                // Is the free area start above 1 MB and below 4 GB?
                if (entry.addr >= FIRST_1MB_END && entry.addr < FIRST_4GB_END) {
                    // Free area length in physical pages 
                    size_t len_in_pages = 0;
                    if (((entry.addr + entry.len) <= FIRST_4GB_END)) {
                        // Is the free area all below 4 GB
                        len_in_pages = (uint32_t)(entry.len / PMM_PAGE_SIZE);
                    }
                    else {
                        // The free area exceeds the 4 GB, need to reduce its size to the 4 GB space
                        size_t len_reduced = (uint32_t)(FIRST_4GB_END - entry.addr);
                        len_in_pages = len_reduced / PMM_PAGE_SIZE;
                    }

                    // We found a free area and calculated its size in pages
                    if (len_in_pages > 0 && initial_free_areas_count < INITIAL_FREE_AREAS_MAX_SIZE) {
                        initial_free_areas[initial_free_areas_count].addr = (uint32_t)entry.addr;
                        initial_free_areas[initial_free_areas_count].size = len_in_pages;
                        free_pages_count += len_in_pages;
                        ++initial_free_areas_count;
                    }
                }
            }

            mbi_parse_ended = true;
        }
        else if (multiboot_tag_ptr->type == MULTIBOOT_TAG_TYPE_END) {
            mbi_parse_ended = true;
            if (multiboot_tag_ptr->size != sizeof(multiboot_tag_t)) {
                kpanic("PMM init", "Wrong end tag, error in multiboot info parsing", __FILE__, __LINE__);
            }
            // The end tag has been reached, but no memory areas information has been found!
            kpanic("PMM init", "Memory map not found in multiboot info", __FILE__, __LINE__);
        }
        
        // Go to next tag
        current_mbi_tag_addr += multiboot_tag_ptr->size;
    }
}

void pmm_init(uintptr_t mbi_addr)
{
    memset(initial_free_areas, 0, sizeof(initial_free_areas));
    pmm_init_find_free_areas(mbi_addr);
}