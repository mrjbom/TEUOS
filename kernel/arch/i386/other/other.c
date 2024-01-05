#include "other.h"

// Start and end addresses of sections
// .multiboot.data
// start
extern uintptr_t _multiboot_data_start;
uint32_t* _multiboot_data_start_addr = &_multiboot_data_start;
// end
extern uintptr_t _multiboot_data_end;
uint32_t* _multiboot_data_end_addr = &_multiboot_data_end;

// .multiboot.text
// start
extern uintptr_t _multiboot_text_start;
uint32_t* _multiboot_text_start_addr = &_multiboot_text_start;
// end
extern uintptr_t _multiboot_text_end;
uint32_t* _multiboot_text_end_addr = &_multiboot_text_end;

// .text section
// start
extern uintptr_t _text_start;
uint32_t* _text_start_addr = &_text_start;
// end
extern uintptr_t _text_end;
uint32_t* _text_end_addr = &_text_end;

// .rodata section
// start
extern uintptr_t _rodata_start;
uint32_t* _rodata_start_addr = &_rodata_start;
// end
extern uintptr_t _rodata_end;
uint32_t* _rodata_end_addr = &_rodata_end;

// .data section
// start
extern uintptr_t _data_start;
uint32_t* _data_start_addr = &_data_start;
// end
extern uintptr_t _data_end;
uint32_t* _data_end_addr = &_data_end;

// .bss section
// start
extern uintptr_t _bss_start;
uint32_t* _bss_start_addr = &_bss_start;
// end
extern uintptr_t _bss_end;
uint32_t* _bss_end_addr = &_bss_end;
