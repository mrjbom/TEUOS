#include "vmm.h"
#include "pde.h"
#include "pte.h"
#include "../memory_layout.h"
#include "../../inlineassembly/inlineassembly.h"

uint32_t* pd_self_mapping = (uint32_t*)PD_SELF_MAPPING_START_VA;

extern uint32_t bootstrap_page_directory[];

void map_pd_into_itself()
{
    // Write physical address of page directory to page directory entry
    pde_set_pt_addr(&bootstrap_page_directory[VA_PDX(PD_SELF_MAPPING_START_VA)], (uintptr_t)KERNEL_VA_TO_PA(bootstrap_page_directory));
    pde_add_attrib(&bootstrap_page_directory[VA_PDX(PD_SELF_MAPPING_START_VA)], PDE_PRESENT | PDE_WRITABLE);
    tlb_flush();
}
