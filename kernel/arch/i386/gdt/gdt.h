#ifndef _GDT_H_
#define _GDT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//Access
#define SEG_ACCESS_PRESENT(x)   ((x) << 0x07)           // Present
#define SEG_ACCESS_PRIV(x)      (((x) &  0x03) << 0x05) // Set privilege level (0 - 3)
#define SEG_ACCESS_TYPE(x)      ((x) << 0x04)           // Set descriptor type bit
#define SEG_ACCESS_EX(x)        ((x) << 0x03)           // Executable bit
#define SEG_ACCESS_DC(x)        ((x) << 0x02)           // Direction bit/Conforming bit
#define SEG_ACCESS_RW(x)        ((x) << 0x01)           // Readable bit/Writable bit
#define SEG_ACCESS_ACCESSED(x)  ((x) << 0x00)           // Accessed bit

//Flags
#define SEG_FLAGS_GRAN(x)       ((x) << 0x03)           // Granularity flag
#define SEG_FLAGS_DB(x)         ((x) << 0x02)           // Size flag
#define SEG_FLAGS_L(x)          ((x) << 0x01)           // Long mode flag

typedef struct
{
    uint16_t    limit_low;      // Limit low word
    uint16_t    base_low;       // Base low word
    uint8_t     base_middle;    // Base middle byte
    uint8_t     access;         // Access byte
    uint8_t     limit_high : 4; // Limit high 4 bits
    uint8_t     flags      : 4; // Flags 4 bits
    uint8_t     base_high;      // Base high byte
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr_data;

/* Load GDTR */
extern void gdt_flush(uint32_t);

/* Create GDT and fill GDT and load GDT */
extern void gdt_init(void);

/* Create GDT descriptor */
extern gdt_entry_t gdt_create_descriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);

#endif
