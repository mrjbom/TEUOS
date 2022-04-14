#ifndef _MMU_H_
#define _MMU_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Prohibits writing to .text and .rodata sections
extern void mmu_init_sections_permissions(void);

#endif
