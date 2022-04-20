#include "string.h"

void* memset(void *dest, int val, size_t num)
{
    unsigned char *ptr = dest;
    while (num-- > 0) {
        *ptr++ = (unsigned char)val;
    }
    return dest;
}
