#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __linux__
#warning "This is not intended to be compiled for Linux"
#endif
#ifndef __i386__
#warning "It is necessary to use the iX86 compiler"
#endif
#ifndef __i686__
#warning "It is supposed to be compiled using the i686 cross-compiler"
#endif

int foo(int in)
{
    int a_in_foo = in * 2;
    return a_in_foo;
}

extern uint32_t stack_bottom_addr;
extern uint32_t stack_top_addr;

void kmain(void)
{
    // Debugger test
    uint32_t stack_size = (uint32_t)(stack_top_addr - stack_bottom_addr);
    stack_size = stack_size; //Not unused warning suppression
    int a = 100;
    a += foo(150); //a = 400
}