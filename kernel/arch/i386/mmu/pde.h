#ifndef _PDE_H_
#define _PDE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum PAGE_PDE_FLAGS {
    PDE_PRESENT     = 1,            //00000000000000000000000000000001
    PDE_WRITABLE    = 2,            //00000000000000000000000000000010
    PDE_USER        = 4,            //00000000000000000000000000000100
    PDE_PWT         = 8,            //00000000000000000000000000001000
    PDE_PCD         = 0x10,         //00000000000000000000000000010000
    PDE_ACCESSED    = 0x20,         //00000000000000000000000000100000
    PDE_DIRTY       = 0x40,         //00000000000000000000000001000000
    PDE_4MB         = 0x80,         //00000000000000000000000010000000
    PDE_CPU_GLOBAL  = 0x100,        //00000000000000000000000100000000
    PDE_AVL         = 0xE00,        //00000000000000000000111000000000
    PDE_FRAME       = 0x7FFFF000    //01111111111111111111000000000000
};

typedef uint32_t pd_entry;

// Sets a flag in the page table entry
extern void pde_add_attrib(pd_entry* entry, uint32_t attrib);

// Clear a flag in the page table entry
extern void pde_del_attrib(pd_entry* entry, uint32_t attrib);

// Get page table entry frame address
extern uint32_t* pde_get_frame_addr(pd_entry entry);

#endif
