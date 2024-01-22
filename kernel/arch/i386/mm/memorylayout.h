#ifndef _MEMORYLAYOUT_H_
#define _MEMORYLAYOUT_H_

#include <stdint.h>

#define KERNEL_PHYSICAL_TO_VIRTUAL(phys_addr) ((phys_addr) + 0xC0000000)
#define KERNEL_VIRTUAL_TO_PHYSICAL(virt_addr) ((virt_addr) - 0xC0000000)

/*
 * Variables and constants required for memory management
 */

/*
  Virtual address space layout
  ---------------------------- 0x00000000
  |                          |
  |                          |
  |                          |
  |                          |
  |                          |
  |        USER SPACE        |
  |                          |
  |                          |
  |                          |
  |                          |
  |                          |
  |                          |
  ---------------------------- 0xC0000000
  |                          |
  |       KERNEL SPACE       |
  |                          |
  |                          |
  ---------------------------- 0xFFFFFFFF

  User space is separate for each process and changes when the process is switched over.
  Kernel space is one and is always mapped to virtual space.

              KERNEL SPACE
  -----------------------------------
  |       loaded kernel start       |
  |               ...               |
  |        loaded kernel end        |
  |---------------------------------|
  |    Memory areas used by PMM     |
  |---------------------------------|
  |    Memory areas used by VMM     |
  |---------------------------------|
  |           kernel heap           |
  |---------------------------------|

  The memory areas used by PMM and VMM are reserved in advance and are necessary for their work.
*/

/* Declared in the linker script */
extern uintptr_t _kernel_start;
extern uintptr_t _kernel_end;

extern uintptr_t _text_start;
extern uintptr_t _text_end;

extern uintptr_t _rodata_start;
extern uintptr_t _rodata_end;

extern uintptr_t _data_start;
extern uintptr_t _data_end;

extern uintptr_t _bss_start;
extern uintptr_t _bss_end;

/* Declared by this code */
extern uintptr_t kernel_start_physical_addr;
extern uintptr_t kernel_start_virtual_addr;
extern uintptr_t kernel_end_physical_addr;
extern uintptr_t kernel_end_virtual_addr;
extern uintptr_t kernel_end_physical_addr_page_aligned;
extern uintptr_t kernel_end_virtual_addr_page_aligned;

extern uintptr_t kernel_text_start_physical_addr;
extern uintptr_t kernel_text_start_virtual_addr;
extern uintptr_t kernel_text_end_physical_addr;
extern uintptr_t kernel_text_end_virtual_addr;
extern uintptr_t kernel_text_end_physical_addr_page_aligned;
extern uintptr_t kernel_text_end_virtual_addr_page_aligned;

extern uintptr_t kernel_rodata_start_physical_addr;
extern uintptr_t kernel_rodata_start_virtual_addr;
extern uintptr_t kernel_rodata_end_physical_addr;
extern uintptr_t kernel_rodata_end_virtual_addr;
extern uintptr_t kernel_rodata_end_physical_addr_page_aligned;
extern uintptr_t kernel_rodata_end_virtual_addr_page_aligned;

extern uintptr_t kernel_data_start_physical_addr;
extern uintptr_t kernel_data_start_virtual_addr;
extern uintptr_t kernel_data_end_physical_addr;
extern uintptr_t kernel_data_end_virtual_addr;
extern uintptr_t kernel_data_end_physical_addr_page_aligned;
extern uintptr_t kernel_data_end_virtual_addr_page_aligned;

extern uintptr_t kernel_bss_start_physical_addr;
extern uintptr_t kernel_bss_start_virtual_addr;
extern uintptr_t kernel_bss_end_physical_addr;
extern uintptr_t kernel_bss_end_virtual_addr;
extern uintptr_t kernel_bss_end_physical_addr_page_aligned;
extern uintptr_t kernel_bss_end_virtual_addr_page_aligned;

extern void init_memorylayout_variables(void);

#endif
