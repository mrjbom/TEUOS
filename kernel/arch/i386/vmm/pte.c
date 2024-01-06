#include "pte.h"
#include "bits.h"

inline void pte_add_attrib(pt_entry* entry, uint32_t attrib)
{
    *entry |= attrib;
}

inline void pte_del_attrib(pt_entry* entry, uint32_t attrib)
{
    *entry &= ~attrib;
}

inline void pte_set_frame_addr(pt_entry* entry, uintptr_t frame_addr)
{
    *entry = (*entry & ~(uint32_t)(bits_set_bit(PTE_FRAME, 31))) | frame_addr;
}

inline uintptr_t pte_get_frame_addr(pt_entry entry)
{
    return (uintptr_t)(entry & (uint32_t)(bits_set_bit(PTE_FRAME, 31)));
}
