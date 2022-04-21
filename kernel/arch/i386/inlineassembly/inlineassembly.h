#ifndef _INLINEASSEMBLY_H_
#define _INLINEASSEMBLY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Sends a 8/16/32-bit value on a I/O location. Traditional names are outb, outw and outl respectively.
extern void outb(uint16_t port, uint8_t val);

// Receives a 8/16/32-bit value from an I/O location. Traditional names are inb, inw and inl respectively.
extern uint8_t inb(uint16_t port);

/*
 * Wait a very small amount of time (1 to 4 microseconds, generally).
 * 
 * You can do an IO operation on any unused port:
 * the Linux kernel by default uses port 0x80, which is often used during POST to log information on the motherboard's hex display but almost always unused after boot.
 */
extern void io_wait(void);

// Returns a true boolean value if irq are enabled for the CPU.
extern bool are_interrupts_enabled(void);

/*
 * Sometimes it is helpful to disable interrupts and then only re-enable them if they were disabled before.
 * While the above function can be used for that, the below functions do it the same way regardless of the setting of the state of the IF:
 *
 * unsigned long f = save_irqdisable();
 * do_whatever_without_irqs();
 * irqrestore(f);
 */
extern uint32_t save_irqdisable(void);

extern void irqrestore(unsigned long flags);

#endif
