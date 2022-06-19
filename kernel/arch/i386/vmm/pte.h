#ifndef _PTE_H_
#define _PTE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum PAGE_PTE_FLAGS {
    PTE_PRESENT         = 1,            //00000000000000000000000000000001
    PTE_WRITABLE        = 2,            //00000000000000000000000000000010
    PTE_USER            = 4,            //00000000000000000000000000000100
    PTE_WRITETHOUGH     = 8,            //00000000000000000000000000001000
    PTE_NOT_CACHEABLE   = 0x10,         //00000000000000000000000000010000
    PTE_ACCESSED        = 0x20,         //00000000000000000000000000100000
    PTE_DIRTY           = 0x40,         //00000000000000000000000001000000
    PTE_PAT             = 0x80,         //00000000000000000000000010000000
    PTE_CPU_GLOBAL      = 0x100,        //00000000000000000000000100000000
    PTE_AVL             = 0x200,        //00000000000000000000111000000000
    PTE_FRAME           = 0x7FFFF000    //01111111111111111111000000000000
};

typedef uint32_t pt_entry;

/* Sets a flag in the page table entry */
extern void pte_add_attrib(pt_entry* entry, uint32_t attrib);

/* Clear a flag in the page table entry */
extern void pte_del_attrib(pt_entry* entry, uint32_t attrib);

/* Set page table entry frame address */
extern void pte_set_frame_addr(pt_entry* entry, uint32_t frame_addr);

/* Get page table entry frame address */
extern uint32_t* pte_get_frame_addr(pt_entry entry);

#endif
