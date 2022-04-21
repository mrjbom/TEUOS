#include "pic.h"
#include "../inlineassembly/inlineassembly.h"

void pic_init(void)
{
    pic_remap(0x20, 0x28);
}

void pic_remap(uint8_t offset1, uint8_t offset2)
{
    unsigned char a1, a2;
    a1 = inb(PIC1_DATA);                        // save masks
    a2 = inb(PIC2_DATA);
 
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();
 
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    // Note the presence of io_wait() calls, on older machines its necessary to give the PIC some time to react to commands as they might not be processed quickly
 
    outb(PIC1_DATA, a1);   // restore saved masks.
    outb(PIC2_DATA, a2);
}

void pic_send_EOI(uint8_t irq_num)
{
    if(irq_num >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    outb(PIC1_COMMAND, PIC_EOI);
}
