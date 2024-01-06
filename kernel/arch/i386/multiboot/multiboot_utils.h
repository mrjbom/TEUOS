#ifndef _MULTIBOOT_UTILS_H_
#define _MULTIBOOT_UTILS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "multiboot2.h"

void multiboot_print_mbi_info(uintptr_t mbi_addr);

#endif
