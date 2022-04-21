#ifndef _IDT_H_
#define _IDT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define GATE_TYPE(x)    (((x) &  0xF) << 0x00)  // Gate type
#define GATE_DPL(x)     (((x) &  0x3) << 0x05)  // DPL
#define GATE_P(x)       ((x) << 0x07)           // Present

typedef struct
{
    uint16_t    offset_low;         // Offset low word
    uint16_t    segment_selector;   // Selector
    uint8_t     allways0;           // Reserved
    uint8_t     flags;              // Gate type(0-3), 0(4), DPL(5-6), Present(7)
    uint16_t    offset_high;        // Offset high word
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_data;

extern void idt_flush(uint32_t);

extern void idt_init(void);

extern idt_entry_t idt_create_descriptor(uint32_t offset, uint16_t selector, uint8_t flags);

// isr's entry points
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);

#endif
