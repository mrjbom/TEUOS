#ifndef _USEFUL_THINGS_H_
#define _USEFUL_THINGS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*
    Useful things that can be useful everywhere.
*/

/*
    Used by the kernel if some critical error has occurred. Outputs an error message to the serial port, and disables interrupts and stops execution.
*/
extern void kpanic(const char* title, const char* text, const char* file, int line);

/*
    Checks if the address is aligned and aligns it if necessary.
    @addr_ptr: A pointer to the address to be checked and aligned
    @alignment: The address will be aligned to this value
    @return: The number of bytes by which the address was aligned

    Example
    uintptr_t addr = 0xC; // 12 not aligned
    size_t new_bytes = useful_align_to(&addr, 8);
    printf("0x%x %u", addr, new_bytes);
    0x10 4
*/
extern size_t useful_align_to(uintptr_t* addr_ptr, size_t alignment);

#endif
