#ifndef _PMM_H_
#define _PMM_H_

#include <stddef.h>
#include <stdint.h>
#include "../../multiboot/multiboot2.h"

// Area in physical memory (MBI) (Taking into account the memory above 4 GB)
typedef struct {
    uint64_t addr;
    uint64_t size; // Size in bytes
    uint32_t type; // Memory MBI type
} pmm_memory_area_mbi_t;

// Free area in physical memory (Below 4 GB)
typedef struct {
    uintptr_t addr; // Page aligned
    size_t size; // Size in bytes (Page aligned)
} pmm_memory_free_area_t;

// List of all memory areas detected using multiboot2
// It may contain unpleasant problems. For example, areas may be unsorted, overlap, or occupied by the!
// This is a copy of the list provided by multiboot2, so it is not quite convenient to use.
extern size_t pmm_mbi_areas_count;
extern pmm_memory_area_mbi_t pmm_mbi_areas[];
// A list of free for use (multiboot type is available) areas, the list can be safely used to initialize allocators.
// There are no areas below 1 MB, the areas are sorted, the areas are not occupied by the kernel, start addresses and sizes are aligned along the page boundaries.
// The beginning of the first available zone does not start at 0x100000, because the kernel is loaded there, so this area starts immediately after the kernel.
// This list should be used to initialize allocators and other things.
extern size_t pmm_free_areas_count;
extern pmm_memory_free_area_t pmm_free_areas[];

// The total size of all physical memory (Considering the memory is more than above 4 GB)
extern uint64_t pmm_total_memory_size;
// The total size of all free for use physical memory (Considering the memory is more than above 4 GB)
extern uint64_t pmm_total_free_memory_size;
// The size of available physical memory in 4 GB. (Considering the memory is ONLY WITHIN 4 GB. So this number is always less than 4 GB.)
extern uint32_t pmm_free_memory_size;

// Init memory info data
extern void pmm_init_memory_info(uintptr_t mbi_addr);

// Print memory areas info copied from MBI
extern void pmm_print_mbi_areas(void);

// Print free memory areas
extern void pmm_print_free_areas(void);

#endif
