#include "bootstrap_allocator.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define GET_BIT_MSB(num, bit) ((bool)(((num) >> ((sizeof(num) * 8) - 1 - (bit))) & 1))
#define SET_BIT_MSB(num, bit) ((num) |= (1 << ((sizeof(num) * 8) - 1 - (bit))))
#define CLEAR_BIT_MSB(num, bit) ((num) &= (typeof((num)))(~(1 << ((sizeof(num) * 8) - 1 - (bit)))))

bool bootstrap_allocator_inited = false;
bool bootstrap_allocator_has_normal = false;
size_t bootstrap_allocator_dma_min_pfn = 0;
size_t bootstrap_allocator_dma_max_pfn = 0;
size_t bootstrap_allocator_normal_min_pfn = 0;
size_t bootstrap_allocator_normal_max_pfn = 0;
uint8_t bootstrap_allocator_bitmap[BOOTSTRAP_ALLOCATOR_BITMAP_SIZE];

void bootstrap_allocator_init(size_t dma_min_pfn, size_t dma_max_pfn, size_t normal_min_pfn, size_t normal_max_pfn)
{
    if (dma_min_pfn == 0 || dma_max_pfn > BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER) {
        return;
    }
    if (normal_max_pfn > BOOTSTRAP_ALLOCATOR_PAGES_MAX_NUMBER) {
        return;
    }
    // If normal_min_pfn == 0, then normal zone not exist
    bootstrap_allocator_has_normal = (normal_min_pfn != 0);
    // dma_min_pfn <= dma_max_pfn
    // dma_max_pfn < normal_min_pfn (if NORMAL zone exist)
    // normal_min_pfn <= normal_max_pfn (if NORMAL zone exist)
    if (!(dma_min_pfn <= dma_max_pfn)) {
        return;
    }
    if (bootstrap_allocator_has_normal && !(dma_max_pfn < normal_min_pfn)) {
        return;
    }
    if (bootstrap_allocator_has_normal && !(normal_min_pfn <= normal_max_pfn)) {
        return;
    }

    bootstrap_allocator_dma_min_pfn = dma_min_pfn;
    bootstrap_allocator_dma_max_pfn = dma_max_pfn;
    bootstrap_allocator_normal_min_pfn = normal_min_pfn;
    bootstrap_allocator_normal_max_pfn = normal_max_pfn;
    // Mark all pages as used
    memset(bootstrap_allocator_bitmap, UINT8_MAX, BOOTSTRAP_ALLOCATOR_BITMAP_SIZE);
    bootstrap_allocator_inited = true;
}

void bootstrap_allocator_mark_range(void* addr, size_t size, state_t state)
{
    if (bootstrap_allocator_inited == false) {
        return;
    }

    size_t current_pfn = (uintptr_t)addr / BOOTSTRAP_ALLOCATOR_PAGE_SIZE;
    if (current_pfn < bootstrap_allocator_dma_min_pfn) {
        return;
    }
    size_t number_of_pages = size / BOOTSTRAP_ALLOCATOR_PAGE_SIZE;
    if (size % BOOTSTRAP_ALLOCATOR_PAGE_SIZE != 0) {
        number_of_pages++;
    }

    size_t max_pfn = bootstrap_allocator_has_normal ? bootstrap_allocator_normal_max_pfn : bootstrap_allocator_dma_max_pfn;
    if (current_pfn + number_of_pages - 1 > max_pfn) {
        return;
    }

    // Iterate over the pages and mark them accordingly
    for (size_t i = 0; i < number_of_pages; i++) {
        // Calculate the byte and bit offset within the bitmap
        size_t byte_offset = current_pfn / 8;
        size_t bit_offset = current_pfn % 8;

        // Clear or set the bit based on the state
        if (state == BOOTSTRAP_ALLOCATOR_STATE_FREE) {
            CLEAR_BIT_MSB(bootstrap_allocator_bitmap[byte_offset], bit_offset);
        }
        else {
            SET_BIT_MSB(bootstrap_allocator_bitmap[byte_offset], bit_offset);
        }
        current_pfn++;
    }
}

void* bootstrap_allocator_alloc_pages(size_t size, zone_t zone)
{
    if (bootstrap_allocator_inited == false) {
        return NULL;
    }
    if (size == 0) {
        return NULL;
    }
    // If the NORMAL zone does not exist, then memory can't be allocated from the NORMAL zone
    if (bootstrap_allocator_has_normal == false && zone == BOOTSTRAP_ALLOCATOR_ZONE_NORMAL) {
        return NULL;
    }

    size_t page_needed = size / BOOTSTRAP_ALLOCATOR_PAGE_SIZE;
    if (size % BOOTSTRAP_ALLOCATOR_PAGE_SIZE != 0) {
        page_needed++;
    }

    size_t min_pfn = (zone == BOOTSTRAP_ALLOCATOR_ZONE_DMA ? bootstrap_allocator_dma_min_pfn : bootstrap_allocator_normal_min_pfn);
    size_t max_pfn = (zone == BOOTSTRAP_ALLOCATOR_ZONE_DMA ? bootstrap_allocator_dma_max_pfn : bootstrap_allocator_normal_max_pfn);

    if (page_needed > max_pfn - min_pfn + 1) {
        return NULL;
    }

    size_t start_pfn = 0;
    size_t free_contiguous_blocks_count = 0;

    // Iterate through the bitmap from min_pfn to max_pfn
    for (size_t current_pfn = min_pfn; current_pfn <= max_pfn; ++current_pfn) {
        // Calculate the byte and bit offset within the bitmap
        size_t byte_offset = current_pfn / 8;
        size_t bit_offset = current_pfn % 8;
        
        // Check bit
        bool state = GET_BIT_MSB(bootstrap_allocator_bitmap[byte_offset], bit_offset);
        // Page is free
        if (state == BOOTSTRAP_ALLOCATOR_STATE_FREE) {
            if (start_pfn == 0) {
                start_pfn = current_pfn;
            }
            free_contiguous_blocks_count++;
        }
        else {
            start_pfn = 0;
            free_contiguous_blocks_count = 0;
            continue;
        }

        if (free_contiguous_blocks_count == page_needed) {
            void* ptr = (void*)(start_pfn * BOOTSTRAP_ALLOCATOR_PAGE_SIZE);
            // Mark allocated pages as used
            bootstrap_allocator_mark_range(ptr, free_contiguous_blocks_count * BOOTSTRAP_ALLOCATOR_PAGE_SIZE, BOOTSTRAP_ALLOCATOR_STATE_USED);
            return ptr;
        }
    }
    return NULL;
}
