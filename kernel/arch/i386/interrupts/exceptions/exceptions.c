#include "exceptions.h"
#include <stdio.h>

void exceptions_handler(registers_t regs)
{
    printf_serial("EXCEPTION %u\n", regs.isr_num);
}
