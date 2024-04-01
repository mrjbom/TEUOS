#ifndef _MM_H_
#define _MM_H_

#include "pmm/pmm.h"
#include "vmm/vmm.h"

#define PAGE_SIZE 4096

extern void mm_init(uintptr_t mbi_addr);

#endif