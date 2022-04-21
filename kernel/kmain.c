#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "arch/i386/egatextmode/egatextmode.h"
#include "arch/i386/mmu/mmu.h"
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupts/idt/idt.h"
#include "arch/i386/interrupts/pic.h"
#include "arch/i386/inlineassembly/inlineassembly.h"

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
    printf_("Hello from Kernel!\n\n");

    printf_("Setting up sections permissions...\n");
    mmu_init_sections_permissions();

    printf_("Setting up GDT...\n");
    gdt_init();
    
    printf_("Setting up IDT...\n");
    idt_init();
    pic_init();
    printf_("Enabling interrupts...");
    __asm__ volatile("sti");
    while(true);
}
