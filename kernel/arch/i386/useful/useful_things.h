#ifndef _USEFUL_THINGS_H_
#define _USEFUL_THINGS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*
    I plan to put various useful functions and macros here that can be useful everywhere.
*/

/*
    Checks if the address is aligned and aligns it if necessary.
    addr_ptr: A pointer to the address to be checked and aligned
    alignment: The address will be aligned to this value
    Return: The number of bytes by which the address was aligned

    Example
    uint32_t addr = 0xC; // 12 not aligned
    uint32_t new_bytes = useful_align_to(&addr, 8);
    printf("0x%x %u", addr, new_bytes);
    0x10 4
*/
extern size_t useful_align_to(uint32_t* addr_ptr, size_t alignment);

#endif
