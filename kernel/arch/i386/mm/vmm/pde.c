#include "pde.h"
#include "bits.h"

inline void pde_add_attrib(pd_entry* entry, uint32_t attrib)
{
    *entry |= attrib;
}

inline void pde_del_attrib(pd_entry* entry, uint32_t attrib)
{
    *entry &= ~attrib;
}

inline void pde_set_frame_addr(pd_entry* entry, uintptr_t frame_addr)
{
    *entry = (*entry & ~(uint32_t)(bits_set_bit(PDE_FRAME, 31))) | frame_addr;
}

inline uintptr_t pde_get_frame_addr(pd_entry entry)
{
    return (uintptr_t)(entry & (uint32_t)(bits_set_bit(PDE_FRAME, 31)));
}
