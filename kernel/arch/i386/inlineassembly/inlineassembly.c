#include "inlineassembly.h"

inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %1, %0" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type
     */
}

inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %0, %1"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

inline void io_wait(void)
{
    outb(0x80, 0);
}

inline bool are_interrupts_enabled(void)
{
    unsigned long flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

inline uint32_t save_irqdisable(void)
{
    uint32_t flags;
    asm volatile ("pushf\n\tcli\n\tpop %0" : "=r"(flags) : : "memory");
    return flags;
}

inline void irqrestore(unsigned long flags)
{
    asm ("push %0\n\tpopf" : : "rm"(flags) : "memory","cc");
}
