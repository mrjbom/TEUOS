#include "stack.h"
#include <string.h>

void stack_init(stack_t* stack, void* stack_memory_ptr, size_t stack_memory_size)
{
    if (stack == NULL || stack_memory_ptr == NULL || stack_memory_size == 0) {
        return;
    }
    stack->memory_start_ptr = stack_memory_ptr;
    stack->memory_size = stack_memory_size;
    stack->memory_end_addr = (uintptr_t)stack_memory_ptr + stack_memory_size;
    stack->current_top_addr = (uintptr_t)stack_memory_ptr;
}

void stack_push(stack_t* stack, void* data, size_t data_size)
{
    if (stack == NULL || data == NULL || data_size == 0) {
        return;
    }
    // Is there enough memory on the stack for the data?
    if (stack->current_top_addr + data_size > stack->memory_end_addr) {
        return;
    }

    // Copy data to stack
    memcpy((void*)stack->current_top_addr, data, data_size);
    stack->current_top_addr += data_size;
}

void stack_pop(stack_t* stack, void* data, size_t data_size)
{
    if (stack == NULL || data == NULL || data_size == 0) {
        return;
    }
    // Aren't we trying to get more data than there is on the stack?
    if (stack->current_top_addr - data_size < (uintptr_t)stack->memory_start_ptr) {
        return;
    }

    // Copy data from stack
    stack->current_top_addr -= data_size;
    memcpy(data, (void*)stack->current_top_addr, data_size);
}

void stack_increase_memory_size(stack_t* stack, size_t added_memory_size)
{
    if (stack == NULL || added_memory_size == 0) {
        return;
    }

    stack->memory_size += added_memory_size;
    stack->memory_end_addr += added_memory_size;
}

void stack_decrease_memory_size(stack_t* stack, size_t redused_memory_size)
{
    if (stack == NULL || redused_memory_size == 0) {
        return;
    }

    stack->memory_size -= redused_memory_size;
    stack->memory_end_addr -= redused_memory_size;
    if (stack->current_top_addr > stack->memory_end_addr) {
        stack->current_top_addr = stack->memory_end_addr;
    }
}
