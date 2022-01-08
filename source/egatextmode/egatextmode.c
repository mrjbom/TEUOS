#include "egatextmode.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

uint16_t* ega_textbuffer = (uint16_t*)0xB8000;

uint8_t ega_textbuffer_xpos = 0;
uint8_t ega_textbuffer_ypos = 0;

uint8_t ega_textbuffer_color = 0;

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static uint16_t ega_textbuffer_entry(uint8_t uch, uint8_t color)
{
    return (uint16_t)((uint16_t)uch | (uint16_t)color << 8);
}

void ega_textbuffer_set_color(uint8_t fg, uint8_t bg)
{
    ega_textbuffer_color = (uint8_t)(fg | bg << 4);
}

void ega_textbuffer_set_position(uint8_t x, uint8_t y)
{
    if (x >= EGA_TEXTBUFFER_WIDTH || y >= EGA_TEXTBUFFER_HEIGHT) {
        return;
    }
    ega_textbuffer_xpos = x;
    ega_textbuffer_ypos = y;
}

void ega_textbuffer_clear(void)
{
    for(uint16_t i = 0; i < EGA_TEXTBUFFER_WIDTH * EGA_TEXTBUFFER_HEIGHT; ++i) {
        ega_textbuffer[i] = (uint16_t)0;
    }
}

void ega_textbuffer_putch(uint8_t ch)
{
    uint16_t offset = (uint16_t)(ega_textbuffer_ypos * EGA_TEXTBUFFER_WIDTH + ega_textbuffer_xpos);
    ega_textbuffer[offset] = ega_textbuffer_entry(ch, ega_textbuffer_color);
}

void ega_textbuffer_write(const char* str)
{
    for (uint32_t i = 0; i < strlen(str); ++i) {
        ega_textbuffer_putch((uint8_t)str[i]);
        ega_textbuffer_xpos++;
    }
}
