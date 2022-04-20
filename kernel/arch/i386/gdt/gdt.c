#include "gdt.h"
#include "string.h"

#define GDT_SIZE 5

gdt_entry_t gdt[GDT_SIZE];

gdtr_data gdtr_data_ptr;

void gdt_init(void)
{
    //Clear GDT
    memset(&gdt, 0, sizeof(gdt_entry_t) * GDT_SIZE);

    //Fill GDT
    gdt[0] = gdt_create_descriptor(0, 0, 0, 0);
    // Kernel code segment
    gdt[1] = gdt_create_descriptor(
        0,
        0x000FFFFF,
        SEG_ACCESS_PRESENT(1) | SEG_ACCESS_PRIV(0) | SEG_ACCESS_TYPE(1) |
        SEG_ACCESS_EX(1)      | SEG_ACCESS_DC(0)   | SEG_ACCESS_RW(1) |
        SEG_ACCESS_ACCESSED(0),
        SEG_FLAGS_GRAN(1) | SEG_FLAGS_DB(1) | SEG_FLAGS_L(0)
    );
    // Kernel data segment
    gdt[2] = gdt_create_descriptor(
        0,
        0x000FFFFF,
        SEG_ACCESS_PRESENT(1) | SEG_ACCESS_PRIV(0) | SEG_ACCESS_TYPE(1) |
        SEG_ACCESS_EX(0)      | SEG_ACCESS_DC(0)   | SEG_ACCESS_RW(1) |
        SEG_ACCESS_ACCESSED(0),
        SEG_FLAGS_GRAN(1) | SEG_FLAGS_DB(1) | SEG_FLAGS_L(0)
    );
    // User code segment
    gdt[3] = gdt_create_descriptor(
        0,
        0x000FFFFF,
        SEG_ACCESS_PRESENT(1) | SEG_ACCESS_PRIV(3) | SEG_ACCESS_TYPE(1) |
        SEG_ACCESS_EX(1)      | SEG_ACCESS_DC(0)   | SEG_ACCESS_RW(1) |
        SEG_ACCESS_ACCESSED(0),
        SEG_FLAGS_GRAN(1) | SEG_FLAGS_DB(1) | SEG_FLAGS_L(0)
    );
    // User data segment
    gdt[4] = gdt_create_descriptor(
        0,
        0x000FFFFF,
        SEG_ACCESS_PRESENT(1) | SEG_ACCESS_PRIV(3) | SEG_ACCESS_TYPE(1) |
        SEG_ACCESS_EX(0)      | SEG_ACCESS_DC(0)   | SEG_ACCESS_RW(1) |
        SEG_ACCESS_ACCESSED(0),
        SEG_FLAGS_GRAN(1) | SEG_FLAGS_DB(1) | SEG_FLAGS_L(0)
    );

    //Fill GDTR
    gdtr_data_ptr.limit = sizeof(gdt) - 1;
    gdtr_data_ptr.base = (uint32_t)&gdt;

    //Load GDT and reload segment registers
    gdt_flush((uint32_t)&gdtr_data_ptr);
}

gdt_entry_t gdt_create_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
    gdt_entry_t descriptor;
    
    descriptor.base_low     =   (uint16_t)(base & 0x0000FFFF);
    descriptor.base_middle  =   (uint8_t)((base & 0x00FF0000) >> 16);
    descriptor.base_high    =   (uint8_t)((base & 0xFF000000) >> 24);

    descriptor.limit_low    =   (uint16_t)(limit & 0x0000FFFF);
    descriptor.limit_high   =   (uint8_t)((limit & 0x000F0000) >> 16);

    descriptor.access       =   access;

    descriptor.flags        =   (uint8_t)(flags & 0x0000000F);

    return descriptor;
}
