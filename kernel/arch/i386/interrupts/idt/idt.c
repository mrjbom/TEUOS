#include "idt.h"
#include "string.h"

#define IDT_SIZE 255

idt_entry_t idt[IDT_SIZE];

idtr_data_t idtr_data;

void idt_init(void)
{
    //Clear IDT
    memset(&idt, 0, sizeof(idt_entry_t) * IDT_SIZE);

    idt[0] = idt_create_descriptor((uint32_t)isr0, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[1] = idt_create_descriptor((uint32_t)isr1, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[2] = idt_create_descriptor((uint32_t)isr2, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[3] = idt_create_descriptor((uint32_t)isr3, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[4] = idt_create_descriptor((uint32_t)isr4, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[5] = idt_create_descriptor((uint32_t)isr5, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[6] = idt_create_descriptor((uint32_t)isr6, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[7] = idt_create_descriptor((uint32_t)isr7, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[8] = idt_create_descriptor((uint32_t)isr8, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[9] = idt_create_descriptor((uint32_t)isr9, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[10] = idt_create_descriptor((uint32_t)isr10, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[11] = idt_create_descriptor((uint32_t)isr11, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[12] = idt_create_descriptor((uint32_t)isr12, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[13] = idt_create_descriptor((uint32_t)isr13, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[14] = idt_create_descriptor((uint32_t)isr14, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[15] = idt_create_descriptor((uint32_t)isr15, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[16] = idt_create_descriptor((uint32_t)isr16, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[17] = idt_create_descriptor((uint32_t)isr17, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[18] = idt_create_descriptor((uint32_t)isr18, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[19] = idt_create_descriptor((uint32_t)isr19, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[20] = idt_create_descriptor((uint32_t)isr20, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[21] = idt_create_descriptor((uint32_t)isr21, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[22] = idt_create_descriptor((uint32_t)isr22, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[23] = idt_create_descriptor((uint32_t)isr23, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[24] = idt_create_descriptor((uint32_t)isr24, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[25] = idt_create_descriptor((uint32_t)isr25, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[26] = idt_create_descriptor((uint32_t)isr26, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[27] = idt_create_descriptor((uint32_t)isr27, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[28] = idt_create_descriptor((uint32_t)isr28, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[29] = idt_create_descriptor((uint32_t)isr29, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[30] = idt_create_descriptor((uint32_t)isr30, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[31] = idt_create_descriptor((uint32_t)isr31, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));

    idt[32] = idt_create_descriptor((uint32_t)isr32, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[33] = idt_create_descriptor((uint32_t)isr33, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[34] = idt_create_descriptor((uint32_t)isr34, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[35] = idt_create_descriptor((uint32_t)isr35, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[36] = idt_create_descriptor((uint32_t)isr36, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[37] = idt_create_descriptor((uint32_t)isr37, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[38] = idt_create_descriptor((uint32_t)isr38, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[39] = idt_create_descriptor((uint32_t)isr39, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[40] = idt_create_descriptor((uint32_t)isr40, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[41] = idt_create_descriptor((uint32_t)isr41, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[42] = idt_create_descriptor((uint32_t)isr42, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[43] = idt_create_descriptor((uint32_t)isr43, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[44] = idt_create_descriptor((uint32_t)isr44, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[45] = idt_create_descriptor((uint32_t)isr45, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[46] = idt_create_descriptor((uint32_t)isr46, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));
    idt[47] = idt_create_descriptor((uint32_t)isr47, 0x08, GATE_TYPE(0xE) | GATE_DPL(0) | GATE_P(1));

    //Fill IDTR
    idtr_data.limit = sizeof(idt) - 1;
    idtr_data.base = (uint32_t)&idt;

    //Load IDTR
    idt_flush((uintptr_t)&idtr_data);
}

idt_entry_t idt_create_descriptor(uint32_t offset, uint16_t selector, uint8_t flags)
{
    idt_entry_t descriptor;

    descriptor.offset_low       = (uint16_t)(offset & 0x0000FFFF);
    descriptor.offset_high      = (uint16_t)((offset & 0xFFFF0000) >> 16);
    descriptor.segment_selector = selector;
    descriptor.allways0         = 0;
    descriptor.flags            = flags;

    return descriptor;
}
