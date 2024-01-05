#include "include/stdio.h"
#include "../kernel/arch/i386/egatextmode/egatextmode.h"
#include "../kernel/arch/i386/serial/serial_ports.h"

int printf_ega(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    char ega_text_buffer[256];
    const int ret = vsnprintf_(ega_text_buffer, sizeof(ega_text_buffer) - 1, format, va);
    for (int i = 0; i < ret; ++i) {
        ega_textmode_putch((uint8_t)ega_text_buffer[i]);
    }
    va_end(va);
    return ret;
}

int printf_serial(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    char serial_text_buffer[256];
    const int ret = vsnprintf_(serial_text_buffer, sizeof(serial_text_buffer) - 1, format, va);
    for (int i = 0; i < ret; ++i) {
        serial_putch(serial_text_buffer[i]);
    }
    va_end(va);
    return ret;
}
