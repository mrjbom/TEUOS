#include "string.h"

void* memmove(void* dest, const void* src, size_t num)
{
    char* d = dest;
    const char* s = src;
    if (d < s) {
        while (num--) {
            *d++ = *s++;
        }
    }
    else {
        char* lasts = (char*)s + (num-1);
        char* lastd = (char*)d + (num-1);
        while (num--) {
            *lastd-- = *lasts--;
        }
    }
    return dest;
}
