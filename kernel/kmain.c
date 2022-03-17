#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/egatextmode/egatextmode.h"

#ifdef __linux__
#error "This is not intended to be compiled for Linux"
#endif
#ifndef __i386__
#error "It is necessary to use the iX86 compiler"
#endif
#ifndef __i686__
#warning "It is supposed to be compiled using the i686 cross-compiler"
#endif

//extern uintptr_t stack_bottom_addr_symbol;
//extern uintptr_t stack_top_addr_symbol;

//uint32_t* stack_bottom_addr = &stack_bottom_addr_symbol;
//uint32_t* stack_top_addr = &stack_top_addr_symbol;

void kmain(void)
{
    gdt_init();

    ega_textbuffer_clear();
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("Hello from Kernel!\n");
}
