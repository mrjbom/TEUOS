#ifndef _ISR_HANDLER_H_
#define _ISR_HANDLER_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct 
{
    uint32_t    ds;
    /*
     * ESP is saved here after the processor has placed SS, ESP, EFLAGS, CS, EIP, ERRORCODE and my isr_num in the stack
     * It turns out that in order to calculate the values of the original(at the time of interruption) esp, you need to add 20/28(if privilege change) bytes
     */
    uint32_t    edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t    isr_num, err_code;
    uint32_t    eip, cs, eflags, useresp, userss;
}__attribute__((packed)) registers_t;

#endif
