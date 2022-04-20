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

#endif
