#include "isa_interrupts.h"
#include "pit/pit.h"

void isa_interrupts_handler(registers_t regs)
{
    if (regs.isr_num == 32) {
        pit_handler();
    }
}
