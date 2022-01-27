#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "egatextmode/egatextmode.h"

#ifdef __linux__
#error "This is not intended to be compiled for Linux"
#endif
#ifndef __i386__
#error "It is necessary to use the iX86 compiler"
#endif
#ifndef __i686__
#warning "It is supposed to be compiled using the i686 cross-compiler"
#endif

void kmain(void)
{
    ega_textbuffer_clear();
    ega_textbuffer_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
    ega_textbuffer_write("Hello from Kernel!");
    for(uint8_t i = 1; i < 16; ++i) {
        ega_textbuffer_set_color(i, EGA_COLOR_BLACK);
        ega_textbuffer_set_position(i, i);
        ega_textbuffer_write("Hello from Kernel!");
    }
}
