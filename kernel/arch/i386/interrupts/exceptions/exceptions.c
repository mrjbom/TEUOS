#include "exceptions.h"
#include "../../egatextmode/egatextmode.h"

void exceptions_handler(registers_t regs)
{
    ega_textmode_set_position(0, 1);
    printf_("EXCEPTION %u\n", regs.isr_num);
}
