#include "../../multiboot/multiboot2.h"
#include "../../useful/useful_things.h"
#include "dllist.h"
#include "pmm.h"
#include "../memorylayout.h"

/* Before start allocating memory for PMM initialization, we need to find the free areas available */
#define INITIAL_FREE_AREAS_MAX_NUMBER 16
size_t initial_free_areas_count = 0;
/* Nodes must be reserved to initialize and configure the initial free areas */
#define INITIAL_FREE_NODES_NUMBER 32
/* 
 * The first array stores the nodes used by the list.
 * The second array stores pointers to nodes free for use.
 * (We need to store addresses to free nodes because nodes can be used in any order.)
 */
pmm_free_area_t initial_free_areas_nodes[INITIAL_FREE_NODES_NUMBER];
/* 
 * Every time we need a new node, we use this counter to avoid overusing previous nodes.
 * Nodes that are not needed in the list are only removed from the list, but not reused.
 */
size_t initial_free_areas_nodes_used_number = 0;
doubly_linked_list_t initial_free_areas_list;

/*
 * Searches for free memory areas above 1 MB (under 4 GB) available for use (using multiboot info)
 * and puts them on the list
 */
static void pmm_init_stage1_find_free_areas(uintptr_t mbi_addr)
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
                    if (len_in_pages > 0 && initial_free_areas_count < INITIAL_FREE_AREAS_MAX_NUMBER) {
                        dll_insert_node_to_tail(&initial_free_areas_list, (dll_node_t*)&initial_free_areas_nodes[initial_free_areas_nodes_used_number]);
                        initial_free_areas_nodes[initial_free_areas_count].addr = (uint32_t)entry.addr;
                        initial_free_areas_nodes[initial_free_areas_count].size = len_in_pages;
                        ++initial_free_areas_nodes_used_number;
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

/* We need to delete the memory area used by the kernel so that it will not be allocated by PMM */
static void pmm_init_stage1_remove_kernel_from_free_areas(void)
{
    // The kernel is loaded at physical address 0x100000 (1 MB) into a free area with the same starting address.
    // Therefore, the space occupied by the kernel must be removed from the free area starting at 0x100000.
    // This is assumed to be the first area found (since areas below 1 MB are ignored).
    // If it is the first area found, it is at the top of the list.
    pmm_free_area_t* first_free_area = (pmm_free_area_t*)initial_free_areas_list.head;
    if (first_free_area->addr != FIRST_1MB_END) {
        kpanic("PMM init", "The first free memory area does not start at 0x100000", __FILE__, __LINE__);
    }
    // We just need to move the start address of the free area beyond the area occupied by the kernel and reduce the size of the free area.
    size_t kernel_size_in_pages = (kernel_end_physical_addr_page_aligned - kernel_start_physical_addr) / PMM_PAGE_SIZE;
    first_free_area->addr = kernel_end_physical_addr_page_aligned;
    first_free_area->size -= kernel_size_in_pages;
}

void pmm_init_stage1(uintptr_t mbi_addr)
{
    memset(initial_free_areas_nodes, 0, sizeof(initial_free_areas_nodes));
    memset(&initial_free_areas_list, 0, sizeof(initial_free_areas_list));
    pmm_init_stage1_find_free_areas(mbi_addr);
    pmm_init_stage1_remove_kernel_from_free_areas();
}
