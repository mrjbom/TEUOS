#ifndef _STACK_H_
#define _STACK_H_

#include <stdint.h>
#include <stddef.h>

/*
 * Stack implementation.
 * Grow up.
 */

typedef struct
{
    void* memory_start_ptr;
    size_t memory_size;
    uintptr_t memory_end_addr;
    uintptr_t current_top_addr;
} stack_t;

/* 
 * Initializes the stack.
 * The address of the memory area allocated for the stack and its size must be specified.
 */
extern void stack_init(stack_t* stack, void* stack_memory_ptr, size_t stack_memory_size);

/*
 * Copies the data to the stack.
 */
extern void stack_push(stack_t* stack, void* data, size_t data_size);

/*
 * Copy the data from the stack. 
 * Copies data from the stack at the data pointer.
 */
extern void stack_pop(stack_t* stack, void* data, size_t data_size);

/*
 * Increases the size of the stack memory area.
 * The number of bytes by which to increase the stack size must be specified.
 */
extern void stack_increase_memory_size(stack_t* stack, size_t added_memory_size);

/*
 * Decreases the size of the stack memory area.
 * The number of bytes by which to decrease the stack size must be specified.
 * Discards top data stored outside the new memory area.
 */
extern void stack_decrease_memory_size(stack_t* stack, size_t reduced_memory_size);

#endif
