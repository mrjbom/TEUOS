#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "arch/i386/useful/useful_things.h"
#include "arch/i386/serial/serial_ports.h"
#include "arch/i386/multiboot/multiboot_utils.h"
#include "arch/i386/egatextmode/egatextmode.h"
#include "arch/i386/gdt/gdt.h"
#include "arch/i386/interrupts/idt/idt.h"
#include "arch/i386/interrupts/pic.h"
#include "arch/i386/interrupts/isa_interrupts/pit/pit.h"
#include "arch/i386/mm/pmm/pmm.h"

#ifdef __linux__
#error "This is not intended to be compiled for Linux"
#endif
#ifndef __i686__
#warning "It is necessary to use the i686 compiler"
#endif

// magic: multiboot2 magic number must be equal to MULTIBOOT2_BOOTLOADER_MAGIC
// mbi_addr: virtual address of the multiboot info structure, that was copied in the start code
void kmain(uint32_t magic, uintptr_t mbi_addr)
{
    serial_init();

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        kpanic("MULTIBOOT", "magic number invalid!", __FILE__, __LINE__);
    };

    ega_textmode_init();
    printf_ega("Hello from Kernel!\n\n\n");

    //printf_("Setting up kernel sections permissions...\n");
    //other_setup_kernel_sections_permissions();

    printf_ega("Setting up GDT...\n");
    gdt_init();
    
    printf_ega("Setting up IDT...\n");
    idt_init();

    printf_ega("Setting up PIC...\n");
    pic_init();

    printf_ega("Setting up PIT...\n");
    pit_init();

    printf_ega("Enabling interrupts...\n");
    asm volatile("sti");

    multiboot_print_mbi_info(mbi_addr);

    printf_ega("Setting up PMM...\n");
    pmm_init(mbi_addr);

    printf_ega("\n\nKernel finish\n");
    return;
}
