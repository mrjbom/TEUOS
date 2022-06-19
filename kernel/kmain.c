#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stdio.h"
#include "arch/i386/egatextmode/egatextmode.h"
#include "arch/i386/other/other.h"
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupts/idt/idt.h"
#include "arch/i386/interrupts/pic.h"
#include "arch/i386/pit/pit.h"
#include "arch/i386/inlineassembly/inlineassembly.h"
#include "arch/i386/multiboot.h"

#ifdef __linux__
#error "This is not intended to be compiled for Linux"
#endif
#ifndef __i386__
#error "It is necessary to use the iX86 compiler"
#endif
#ifndef __i686__
#warning "It is supposed to be compiled using the i686 cross-compiler"
#endif

void kmain(uint32_t magic, multiboot_info_t* mbi_phys_addr)
{
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) { while(true); };

    multiboot_info_t* mbi = other_copy_multiboot_info_from_physical_addr(mbi_phys_addr);
    if(!mbi) { while(true); }

    ega_textmode_init();
    printf_("Hello from Kernel!\n\n\n");

    printf_("Setting up kernel sections permissions...\n");
    other_set_kernel_sections_permissions();

    printf_("Setting up GDT...\n");
    gdt_init();
    
    printf_("Setting up IDT...\n");
    idt_init();

    printf_("Setting up PIC...\n");
    pic_init();

    printf_("Setting up PIT...\n");
    pit_init();

    printf_("Enabling interrupts...\n");
    __asm__ volatile("sti");

    while(true);
}
