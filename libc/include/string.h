#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

size_t strlen(const char* str);

void* memset(void* dest, int val, size_t num);

void* memcpy(void* dest, const void* src, size_t num);

int memcmp(const void* memptr1, const void* memptr2, size_t num);

void* memmove(void* destptr, const void* srcptr, size_t num);

#endif
