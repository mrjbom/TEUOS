#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

// Will be called second
__attribute__ ((constructor)) void foo(void)
{
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("constructor foo is running\n");
}

// Will be called first
__attribute__ ((constructor)) void foo2(void)
{
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("constructor foo2 is running\n");
}

// Will be called first
__attribute__ ((destructor)) void foo3(void)
{
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("destructor foo3 is running\n");
}

// Will be called second
__attribute__ ((destructor)) void foo4(void)
{
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("destructor foo4 is running\n");
}

void kmain(void)
{
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("Hello from Kernel!\n");
    for(uint8_t i = 1; i < 16; ++i) {
        ega_textbuffer_set_color(i, EGA_COLOR_BLACK);
        ega_textbuffer_write("Hello from Kernel!\n");
    }
}
