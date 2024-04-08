#include "pde.h"
#include "bits.h"

void pde_add_attrib(pd_entry_t* entry_ptr, pde_flags_t attrib)
{
    if (entry_ptr == NULL) {
        return;
    }
    SET_BITS(*entry_ptr, attrib);
}

bool pde_check_attrib(pd_entry_t entry, pde_flags_t attrib)
{
    return CHECK_BITS(entry, attrib);
}

void pde_del_attrib(pd_entry_t* entry_ptr, pde_flags_t attrib)
{
    if (entry_ptr == NULL) {
        return;
    }
    CLEAR_BITS(*entry_ptr, attrib);
}

void pde_set_pt_addr(pd_entry_t *entry_ptr, uintptr_t pt_addr)
{
    if (entry_ptr == NULL) {
        return;
    }
    //*entry_ptr = (pt_addr & 0xFFFFF000) | (*entry_ptr & 0xFFF);
    *entry_ptr = (pt_addr & (uint32_t)PDE_PTADDR) | (*entry_ptr & (uint32_t)~PDE_PTADDR);
}

uintptr_t pde_get_pt_addr(pd_entry_t entry)
{
    return entry & (uint32_t)PDE_PTADDR;
}
