#include "mm.h"
#include "memory_layout.h"

void mm_init(uintptr_t mbi_addr)
{
    init_memory_layout_variables();
    pmm_init_memory_info(mbi_addr);
}
