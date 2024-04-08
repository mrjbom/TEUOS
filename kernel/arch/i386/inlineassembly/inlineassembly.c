#include "inlineassembly.h"
#include <stdint.h>
#include <stdbool.h>

inline void outb(uint16_t port, uint8_t byte)
{
    asm volatile ("outb %0, %1" : : "d"(port), "a"(byte) : "memory");
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret = 0;
    asm volatile ("inb %0, %1" : "=a"(ret) : "d"(port) : "memory");
    return ret;
}

inline void io_wait(void)
{
    outb(0x80, 0);
}

inline bool are_interrupts_enabled(void)
{
    uint32_t flags = 0;
    asm volatile ("pushf\n\t"
                  "pop %0"
                  : "=rm"(flags) : : "memory" );
    return flags & (1 << 9);
}

inline uint32_t save_irqdisable(void)
{
    uint32_t flags = 0;
    asm volatile ("pushf\n\t"
                  "cli\n\t"
                  "pop %0"
                  : "=r"(flags) : : "memory");
    return flags;
}

inline void irqrestore(uint32_t flags)
{
    asm volatile ("push %0\n\t"
                  "popf"
                   : : "r"(flags) : "memory", "cc");
}

inline uint32_t read_cr3(void)
{
    uint32_t cr3 = 0;
    asm volatile ("mov %0, cr3" : "=rm"(cr3) : : "memory");
    return cr3;
}
