#include "gdt.h"

#define GDT_SIZE 5

uint64_t gdt[GDT_SIZE];

void gdt_init(void)
{
    //Fill GDT
    gdt[0] = gdt_create_descriptor(0, 0, 0);
    gdt[1] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    gdt[2] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    gdt[3] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    gdt[4] = gdt_create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

    //Fill GDTR
    gdtr_data gdtr_data_ptr;
    gdtr_data_ptr.limit = sizeof(gdt) - 1;
    gdtr_data_ptr.base = (uint32_t)&gdt;

    //Load GDT and reload segment registers
    gdt_flush((uint32_t)&gdtr_data_ptr);
}

uint64_t gdt_create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor = 0;
    // Create the high 32 bit segment
    descriptor  =  limit & 0x000F0000;        // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;  // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;  // set base bits 23:16
    descriptor |=  base & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;        // set limit bits 15:0

    return descriptor;
}
