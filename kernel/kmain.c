#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "arch/i386/egatextmode/egatextmode.h"
#include "arch/i386/mmu/mmu.h"
#include "arch/i386/gdt/gdt.h"

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
    ega_textmode_init();
    ega_textmode_write("Hello from Kernel!\n\n");

    ega_textmode_write("Setting up sections permissions...\n");
    mmu_init_sections_permissions();

    ega_textmode_write("Setting up GDT...\n");
    gdt_init();
}
