#include "serial_ports.h"
#include "../inlineassembly/inlineassembly.h"

void serial_init()
{
    outb(SERIAL_PORT_COM1 + 1, 0x00);    // Disable all interrupts
    outb(SERIAL_PORT_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT_COM1 + 1, 0x00);    //                  (hi byte)
    outb(SERIAL_PORT_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(SERIAL_PORT_COM1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(SERIAL_PORT_COM1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(SERIAL_PORT_COM1 + 0) != 0xAE) {
        // Error
        while (true);
        return;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(SERIAL_PORT_COM1 + 4, 0x0F);
    return;
}

int serial_is_transmit_empty()
{
    return inb(SERIAL_PORT_COM1 + 5) & 0x20;
}

void serial_putch(char ch)
{
    serial_write_COM1((uint8_t)ch);
}

void serial_write_COM1(uint8_t b)
{
    while (serial_is_transmit_empty() == 0);
    outb(SERIAL_PORT_COM1, b);
}
