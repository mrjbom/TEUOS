#include "string.h"

void* memmove(void* destptr, const void* srcptr, size_t num)
{
    char *d = destptr;
    const char *s = srcptr;
    if (d < s) {
        while (num--) {
            *d++ = *s++;
        }
    }
    else {
        char *lasts = (char*)s + (num-1);
        char *lastd = (char*)d + (num-1);
        while (num--) {
            *lastd-- = *lasts--;
        }
    }
    return destptr;
}
