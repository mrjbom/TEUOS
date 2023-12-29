#include "useful_things.h"

size_t useful_align_to(uint32_t* addr_ptr, size_t alignment)
{
    size_t extra = *addr_ptr % alignment;
    if (extra != 0) {
        *addr_ptr += alignment - extra;
        return alignment - extra;
    }
    return 0;
}
