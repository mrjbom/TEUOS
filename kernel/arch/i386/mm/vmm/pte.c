#include "pte.h"
#include "bits.h"

void pte_add_attrib(pt_entry_t* entry_ptr, pte_flags_t attrib)
{
    if (entry_ptr == NULL) {
        return;
    }
    SET_BITS(*(uint32_t*)entry_ptr, attrib);
}

bool pte_check_attrib(pt_entry_t entry, pte_flags_t attrib)
{
    return CHECK_BITS(*(uint32_t*)&entry, attrib);
}

void pte_del_attrib(pt_entry_t* entry_ptr, pte_flags_t attrib)
{
    if (entry_ptr == NULL) {
        return;
    }
    CLEAR_BITS(*(uint32_t*)entry_ptr, attrib);
}

void pte_set_frame_addr(pt_entry_t *entry_ptr, uintptr_t frame_addr)
{
    if (entry_ptr == NULL) {
        return;
    }
    //*entry_ptr = (frame_addr & 0xFFFFF000) | (*entry_ptr & 0xFFF);
    *entry_ptr = (frame_addr & (uint32_t)PTE_FRAMEADDR) | (*entry_ptr & (uint32_t)~PTE_FRAMEADDR);
}

uintptr_t pte_get_frame_addr(pt_entry_t entry)
{
    return entry & (uint32_t)PTE_FRAMEADDR;
}
