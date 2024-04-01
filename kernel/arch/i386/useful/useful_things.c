#include "useful_things.h"
#include <stdio.h>

void kpanic(const char* title, const char* text, const char* file, int line)
{
    asm volatile ("cli");
    printf_serial("\n[KERNEL PANIC]\n" \
        "Title: %s\n" \
        "Text: %s\n" \
        "File: %s:%d\n", \
        title, text, file, line \
    );
    
    asm volatile ("l: jmp l");
}

size_t useful_align_up(uintptr_t* addr_ptr, size_t alignment)
{
    size_t extra = *addr_ptr % alignment;
    if (extra != 0) {
        *addr_ptr += alignment - extra;
        return alignment - extra;
    }
    return 0;
}

size_t useful_align_down(uintptr_t* addr_ptr, size_t alignment)
{
    size_t extra = *addr_ptr % alignment;
    if (extra != 0) {
        if (extra > *addr_ptr) {
            *addr_ptr = 0;
            return extra;
        }
        *addr_ptr -= extra;
        return extra;
    }
    return 0;
}
