#ifndef _INLINEASSEMBLY_H_
#define _INLINEASSEMBLY_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

extern void outb(uint16_t port, uint8_t byte);

extern uint8_t inb(uint16_t port);

extern void io_wait(void);

extern bool are_interrupts_enabled(void);

extern uint32_t save_irqdisable(void);

extern void irqrestore(uint32_t flags);

extern uint32_t read_cr3(void);

#endif
