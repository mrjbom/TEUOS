#include "interrupts.h"
#include "exceptions/exceptions.h"
#include "isa_interrupts/isa_interrupts.h"

void interrupts_handler(registers_t regs)
{
    // Exception?
    if (regs.isr_num < 32) {
        exceptions_handler(regs);
    }
    else {
        isa_interrupts_handler(regs);
    }
}
