#ifndef _MMU_H_
#define _MMU_H_

/*
 * Here are the auxiliary things that I don't want to separate or group with the "main" things
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../multiboot.h"

/* Forbid writing to .text and .rodata kernel sections */
extern void other_set_kernel_sections_permissions(void);

/* Maps the first megabyte of physical memory so that data can be read from it */
extern void other_map_first1MB_identity(void);

/* Unmap the first megabyte of physical memory making it not accessible */
extern void other_unmap_first1MB_identity(void);
/* Copies multiboot_info_t using its address in physical memory if it is located below 1 MB, for this it temporarily maps the first megobyte of memory using the two previous functions.
 * Copies memory map info
 * Returns the address of the structure in the virtual address space safe for normal use.
 */
extern multiboot_info_t* other_copy_multiboot_info_from_physical_addr(multiboot_info_t* mbi_phys_addr);

#endif
