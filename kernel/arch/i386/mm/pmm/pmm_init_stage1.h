#ifndef _PMM_INITIAL_H_
#define _PMM_INITIAL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* 
 * Finds and sets up available free areas.
 * These will later be used for full PMM initialization.
 */
extern void pmm_init_stage1(uintptr_t mbi_addr);

#endif
