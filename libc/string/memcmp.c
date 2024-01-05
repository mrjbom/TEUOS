#include "string.h"

int memcmp(const void* memptr1, const void* memptr2, size_t num)
{
    const unsigned char *s1 = memptr1;
    const unsigned char *s2 = memptr2;

    while (num-- > 0) {
        if (*s1++ != *s2++) {
            return s1[-1] < s2[-1] ? -1 : 1;
        }
    }
    return 0;
}
