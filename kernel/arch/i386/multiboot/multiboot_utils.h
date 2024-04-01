#ifndef _MULTIBOOT_UTILS_H_
#define _MULTIBOOT_UTILS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "multiboot2.h"

/*
 * Checks the validity of multiboot information
 * If panic is true, then in case of invalidity, the kpanic() function will be called with a description of the problem.
 */
extern bool multiboot_info_is_valid(uintptr_t mbi_addr, bool panic);

// Searches for and returns a pointer to the tag
extern multiboot_tag_t* multiboot_get_tag_ptr(uintptr_t mbi_addr, uint32_t tag_type);

extern void multiboot_print_mbi_info(uintptr_t mbi_addr);

#endif
