#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../isr_handler.h"

extern void exceptions_handler(registers_t regs);

#endif
