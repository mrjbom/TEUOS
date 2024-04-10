#ifndef _VMM_H_
#define _VMM_H_

#define VA_PDX_SHIFT 22
#define VA_PDX_MASK 0xFFC00000
#define VA_PTX_SHIFT 12
#define VA_PTX_MASK 0x3FF000
#define VA_PO_SHIFT 0
#define VA_PO_MASK 0xFFF
#define VA_PDX(va) ((va) & VA_PDX_MASK) >> VA_PDX_SHIFT
#define VA_PTX(va) ((va) & VA_PTX_MASK) >> VA_PTX_SHIFT
#define VA_PO(va) (va) & VA_PO_MASK

#define MAKE_VA(pdx, ptx, po) (((uint32_t)(pdx) << VA_PDX_SHIFT) & VA_PDX_MASK) | (((ptx) << VA_PTX_SHIFT) & VA_PTX_MASK) | ((po) & VA_PO_MASK)

extern void tlb_flush(void);

/*
 * Maps the page directory and its page tables to the kernel virtual space
 * (Page Directory Self Mapping(https://lord-turmoil.github.io/posts/Page-Directory-Self-Mapping/) and osdev wiki (https://wiki.osdev.org/User:Neon/Recursive_Paging))
 * To do this, the physical address of the page directory itself is written to one of the page directory entries
 * This leads to the fact that a 4 MB area will appear in the virtual space of the kernel, with access to which you can edit the current page directory and the page tables located in it.
 * 
 * How it works:
 * Let's write the physical address of PD in PD[1023]
 * Then, for example, to edit the page table recorded in PD[3], you can use address 0xFFC03000(pdx = 1023, ptx = 3, page offset = 0).
 * The processor will take the physical address of the PD itself as PT from the current PD,
 * and then PD[3] (which is the physical address of the page table) will be used as a physical frame,
 * so page offset will address the page table of interest to us (PD[3]).
 * And to access the contents of the PD itself, you can use the address 0xFFFFF000(ptx = 1023).
 * 
 * At the moment, this is necessary so that new page tables can be allocated and used.
 * The problem is that when we find memory for a new page table, we cannot address its contents because it does not have a virtual address.
 * And with this mapping, we can add the found PT to the PD and then address its contents.
 * 
 * I will use the last 4 MB of virtual space (0xFFC00000 - 0xFFFFFFFF) for PD self mapping.
 * [0xFFC00000 - 0xFFFFE000] Page Tables from PD[0] - PD[1022]
 * [0xFFFFF000 - 0xFFFFFFFF] Page Directory entries
 * 
 * ATTENTION:
 * Only those page tables that have the PRESENT bit will be mapped in virtual memory!
 */
extern void map_pd_into_itself(void);

#endif
