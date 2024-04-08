#ifndef _PDE_H_
#define _PDE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Only for 4KB pages
typedef enum PDE_FLAGS {
    PDE_PRESENT     = 1,    // 00000000000000000000000000000001
    PDE_WRITABLE    = 2,    // 00000000000000000000000000000010
    PDE_USER        = 4,    // 00000000000000000000000000000100
    PDE_PWT         = 8,    // 00000000000000000000000000001000
    PDE_PCD         = 16,   // 00000000000000000000000000010000
    PDE_ACCESSED    = 32,   // 00000000000000000000000000100000
    PDE_AVL1        = 64,   // 00000000000000000000000001000000
    PDE_PS          = 128,  // 00000000000000000000000010000000
    PDE_AVL4        = 3840, // 00000000000000000000111100000000
    PDE_PTADDR      = -4096 // 11111111111111111111000000000000
} pde_flags_t;

typedef uint32_t pd_entry_t;

/*
 * Adds the attribute to the page directory entry
 * Sets the bits that are responsible for the attribute
 */
extern void pde_add_attrib(pd_entry_t* entry_ptr, pde_flags_t attrib);

/*
 * Checks attribute in the page directory entry
 * Checks the bits that are responsible for the attribute
 * 
 * Returns true if the attribute is set, otherwise false
 */
extern bool pde_check_attrib(pd_entry_t entry, pde_flags_t attrib);

/*
 * Removes attribute from the page directory entry
 * Clears the bits that are responsible for the attribute
 */
extern void pde_del_attrib(pd_entry_t* entry_ptr, pde_flags_t attrib);

/*
 * Sets the page table address in the page directory entry
 */
extern void pde_set_pt_addr(pd_entry_t* entry_ptr, uintptr_t frame_addr);

/*
 * Gets the page table address from the page directory entry
 */
extern uintptr_t pde_get_pt_addr(pd_entry_t entry);

#endif
