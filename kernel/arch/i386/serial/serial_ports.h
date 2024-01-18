#ifndef _SERIAL_PORTS_H_
#define _SERIAL_PORTS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Disables initialization and work with COM1
//#define SERIAL_DISABLE

// Port addresses
#define SERIAL_PORT_COM1 0x3F8

// Initializes the COM1 serial port for debugging using qemu
extern void serial_init(void);

extern int serial_is_transmit_empty(void);

extern void serial_putch(char ch);

extern void serial_write_COM1(uint8_t b);

#endif
