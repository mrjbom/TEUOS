#ifndef _BOOTSTRAP_ALLOCATOR_H_
#define _BOOTSTRAP_ALLOCATOR_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Bootstrap Allocator
 * A simple and primitive allocator that uses a bitmap to pages allocation.
 * It is focused on working in the OS kernel to initialize the memory required to configure the main allocator and other things (page tables).
 * The bitmap memory is reserved in advance for 4 GB of memory.
 * It is focused on use in a 32-bit address space.
 * 
 * The allocator is designed for x86, so there is a division into DMA and NORMAL zones.
 * The DMA zone is usually located in the first 16 megabytes, and the NORMAL zone after it.
 */

#define BOOTSTRAP_ALLOCATOR_MAX_MEMORY_SIZE 4294967296ll

#define BOOTSTRAP_ALLOCATOR_PAGE_SIZE 4096

#define BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER BOOTSTRAP_ALLOCATOR_MAX_MEMORY_SIZE / BOOTSTRAP_ALLOCATOR_PAGE_SIZE

// States of the page
typedef enum state {
    BOOTSTRAP_ALLOCATOR_STATE_FREE = 0,
    BOOTSTRAP_ALLOCATOR_STATE_USED = 1
} state_t;

// Memory zones
typedef enum zone {
    BOOTSTRAP_ALLOCATOR_ZONE_DMA = 0,
    BOOTSTRAP_ALLOCATOR_ZONE_NORMAL = 1
} zone_t;

// Bitmap size in bytes
// 1 bit per page it's 1 byte per 8 pages
#define BOOTSTRAP_ALLOCATOR_BITMAP_SIZE BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER / 8

extern bool bootstrap_allocator_inited;
extern bool bootstrap_allocator_has_normal;
extern uint32_t bootstrap_allocator_dma_min_pfn;
extern uint32_t bootstrap_allocator_dma_max_pfn;
extern uint32_t bootstrap_allocator_normal_min_pfn;
extern uint32_t bootstrap_allocator_normal_max_pfn;
extern uint8_t bootstrap_allocator_bitmap[];

/*
 * Inits bootstrap allocator
 * Mark all pages all allocated by default!
 * Defines the DMA zone, from dma_min_pfn to dma_max_pfn (inclusive [dma_min_pfn; dma_max_pfn])
 * Defines the NORMAL zone, from normal_min_pfn to normal_max_pfn (inclusive [normal_min_pfn; normal_max_pfn])
 * The DMA zone must be lower and not intersect with the NORMAL zone
 * If the NORMAL zone is not required, then you can set normal_min_pfn and normal_max_pfn to 0
 * 
 * Use mark_range for setting up free for use pages
 */
extern void bootstrap_allocator_init(size_t dma_min_pfn, size_t dma_max_pfn, size_t normal_min_pfn, size_t normal_max_pfn);

/*
 * Marks pages starting with addr and ending with addr+size as free or used
 * 
 * Examples:
 * mark_range(0x2000, 16384, BOOTSTRAP_ALLOCATOR_STATE_FREE)
 * Marks 4 pages as free: 0x2000, 0x3000, 0x4000, 0x5000
 * mark_range(0x2000, 17000, BOOTSTRAP_ALLOCATOR_STATE_USED)
 * Marks 5 pages as used: 0x2000, 0x3000, 0x4000, 0x5000, 0x6000
 * mark_range(0x1FFF, 16384, BOOTSTRAP_ALLOCATOR_STATE_FREE)
 * Marks 4 pages as free: 0x1000, 0x2000, 0x3000, 0x4000
 */
extern void bootstrap_allocator_mark_range(void* addr, size_t size, state_t state);

/*
 * Allocates one or more pages depending on the size
 * zone - zone from which memory must be allocated
 * Returns NULL if failed
 * 
 * Memory freeing is unlikely to be necessary, but it can be done using mark_range().
 */
extern void* bootstrap_allocator_alloc_pages(size_t size, zone_t zone);

#endif

