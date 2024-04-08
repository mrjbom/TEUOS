#ifndef _PTE_H_
#define _PTE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Only for 4KB pages
typedef enum PTE_FLAGS {
    PTE_PRESENT     = 1,    // 00000000000000000000000000000001
    PTE_WRITABLE    = 2,    // 00000000000000000000000000000010
    PTE_USER        = 4,    // 00000000000000000000000000000100
    PTE_PWT         = 8,    // 00000000000000000000000000001000
    PTE_PCD         = 16,   // 00000000000000000000000000010000
    PTE_ACCESSED    = 32,   // 00000000000000000000000000100000
    PTE_DIRTY       = 64,   // 00000000000000000000000001000000
    PTE_PAT         = 128,  // 00000000000000000000000010000000
    PTE_GLOBAL      = 256,  // 00000000000000000000000100000000
    PTE_AVL3        = 3584, // 00000000000000000000111000000000
    PTE_FRAMEADDR   = -4096 // 11111111111111111111000000000000
} pte_flags_t;

typedef uint32_t pt_entry_t;

/*
 * Adds the attribute to the page table entry
 * Sets the bits that are responsible for the attribute
 */
extern void pte_add_attrib(pt_entry_t* entry_ptr, pte_flags_t attrib);

/*
 * Checks attribute in the page table entry
 * Checks the bits that are responsible for the attribute
 * 
 * Returns true if the attribute is set, otherwise false
 */
extern bool pte_check_attrib(pt_entry_t entry, pte_flags_t attrib);

/*
 * Removes attribute from the page table entry
 * Clears the bits that are responsible for the attribute
 */
extern void pte_del_attrib(pt_entry_t* entry_ptr, pte_flags_t attrib);

/*
 * Sets the frame address in the page table entry
 */
extern void pte_set_frame_addr(pt_entry_t* entry_ptr, uintptr_t frame_addr);

/*
 * Gets the frame address from the page table entry
 */
extern uintptr_t pte_get_frame_addr(pt_entry_t entry);

#endif
