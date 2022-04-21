#include "isr_handler.h"
#include "exceptions/exceptions.h"
#include "interrupts.h"
#include "stdio.h"
#include "pic.h"

void isr_handler(registers_t regs)
{
    //printf_("unhandled interrupt 0x%x(%d)\n", regs.isr_num, regs.isr_num);
    //printf_("EAX=0x%x EBX=0x%x ECX=0x%x EDX=0x%x\n", regs.eax, regs.ebx, regs.ecx, regs.edx);
    //printf_("ESI=0x%x EDI=0x%x EBP=0x%x ESP=0x%x\n", regs.esi, regs.edi, regs.ebp, regs.esp + 20);
    //printf_("EIP=0x%x EFL=0x%x\n", regs.eip, regs.eflags);
    //printf_("userSS=0x%x userESP=0x%x", regs.userss, regs.useresp);

    if(regs.isr_num >= 32 && regs.isr_num <= 47) {
        pic_send_EOI((uint8_t)regs.isr_num - 32);
    }
}
