#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "isr_handler.h"

extern void interrupts_handler(registers_t regs);

#endif
