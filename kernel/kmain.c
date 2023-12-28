#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "arch/i386/multiboot/multiboot_utils.h"
#include "arch/i386/other/other.h"
#include "arch/i386/egatextmode/egatextmode.h"
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupts/idt/idt.h"
#include "arch/i386/interrupts/pic.h"
#include "arch/i386/interrupts/isa_interrupts/pit/pit.h"

#ifdef __linux__
#error "This is not intended to be compiled for Linux"
#endif
#ifndef __i686__
#warning "It is necessary to use the i686 compiler"
#endif

// magic: multiboot2 magic number must be equal to MULTIBOOT2_BOOTLOADER_MAGIC
// mbi_addr: virtual address of the multiboot info structure, that was copied in the start code
void kmain(uint32_t magic, uint32_t mbi_addr)
{
    (void)magic;
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) { while (true); };

    ega_textmode_init();
    printf_("Hello from Kernel!\n\n\n");

    //printf_("Setting up kernel sections permissions...\n");
    //other_setup_kernel_sections_permissions();

    printf_("Setting up GDT...\n");
    gdt_init();
    
    printf_("Setting up IDT...\n");
    idt_init();

    printf_("Setting up PIC...\n");
    pic_init();

    printf_("Setting up PIT...\n");
    pit_init();

    printf_("Enabling interrupts...");
    __asm__ volatile("sti");

    ega_textmode_clear();
    ega_textmode_set_position(0, 0);

    multiboot_print_mbi_info(mbi_addr);

    printf_("\n\nKernel finish (loop)\n");
    while (true);
}
