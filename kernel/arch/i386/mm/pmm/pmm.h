#ifndef _PMM_H_
#define _PMM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define PMM_PAGE_SIZE 4096

typedef struct
{
    void* next; // for doubly linked list node
    void* prev; // for doubly linked list node
    uint32_t addr; // free area start addr
    uint32_t size; // free area size in pages
} pmm_free_area_t;

/* Main variables for memory management */
/* Number of free physical pages */
extern size_t free_pages_count;

extern void pmm_init(uintptr_t mbi_addr);

#endif
