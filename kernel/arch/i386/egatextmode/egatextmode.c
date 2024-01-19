#include "egatextmode.h"
#include "../inlineassembly/inlineassembly.h"

uint16_t* ega_textmode_buffer_addr = (uint16_t*)0xC03FF000;

const uint8_t EGA_TEXTMODE_BUFFER_WIDTH = 80;
const uint8_t EGA_TEXTMODE_BUFFER_HEIGHT = 25;

uint8_t ega_textmode_x_pos = 0;
uint8_t ega_textmode_y_pos = 0;

uint8_t ega_textmode_color = 0;

void ega_textmode_init(void)
{
    ega_textmode_clear();
    ega_textmode_set_color(EGA_COLOR_LIGHT_GREY, EGA_COLOR_BLACK);
}

static uint16_t ega_textmode_entry(uint8_t uch, uint8_t color)
{
    return (uint16_t)((uint16_t)uch | (uint16_t)color << 8);
}

void ega_textmode_set_color(uint8_t fg, uint8_t bg)
{
    /*
     * The attribute byte carries the foreground colour in its lowest 4 bits and the background color in its highest 3 bits. The interpretation of bit #7 depends on how you (or the BIOS) configured the hardware
     * TODO: When loading with GRUB, it draws a background of all 16 colors, which means #7 bit works without features, deal with it.
     */
    if (bg > (uint8_t)7/*0111b bg color*/) {
        bg = 7;
    }
    ega_textmode_color = (uint8_t)(fg | bg << 4);
}

void ega_textmode_set_position(uint8_t x, uint8_t y)
{
    if (x >= EGA_TEXTMODE_BUFFER_WIDTH || y >= EGA_TEXTMODE_BUFFER_HEIGHT) {
        return;
    }
    ega_textmode_x_pos = x;
    ega_textmode_y_pos = y;
}

uint8_t ega_textmode_get_x_position(void)
{
    return ega_textmode_x_pos;
}

uint8_t ega_textmode_get_y_position(void)
{
    return ega_textmode_y_pos;
}

void ega_textmode_clear(void)
{
    for (uint16_t i = 0; i < EGA_TEXTMODE_BUFFER_WIDTH * EGA_TEXTMODE_BUFFER_HEIGHT; ++i) {
        ega_textmode_buffer_addr[i] = (uint16_t)0;
    }
}

void ega_textmode_putch(uint8_t ch)
{
    if (ega_textmode_x_pos >= EGA_TEXTMODE_BUFFER_WIDTH || ega_textmode_y_pos >= EGA_TEXTMODE_BUFFER_HEIGHT) {
        return;
    }
    if (ch == '\n') {
        if (ega_textmode_y_pos + 1 == EGA_TEXTMODE_BUFFER_HEIGHT) {
            ega_textmode_scroll();
            ega_textmode_set_position(0, ega_textmode_y_pos);
        }
        else {
            ega_textmode_set_position(0, ega_textmode_y_pos + 1);
        }
        return;
    }
    uint16_t offset = (uint16_t)(ega_textmode_y_pos * EGA_TEXTMODE_BUFFER_WIDTH + ega_textmode_x_pos);
    // Check textmode buffer overflow
    if (offset < EGA_TEXTMODE_BUFFER_WIDTH * EGA_TEXTMODE_BUFFER_HEIGHT) {
        ega_textmode_buffer_addr[offset] = ega_textmode_entry(ch, ega_textmode_color);
        ega_textmode_set_position(ega_textmode_x_pos + 1, ega_textmode_y_pos);
    }
}

void ega_textmode_scroll()
{
    uint32_t flags = save_irqdisable();
    /*
     * Copy line by line
     * 
     * 1 -> 0
     * 2 -> 1
     * 3 -> 2
     * 4 -> 3
     * ...
     */
    for (uint8_t y = 1; y < EGA_TEXTMODE_BUFFER_HEIGHT; ++y) {
        memcpy(
            (ega_textmode_buffer_addr + ((y - 1) * EGA_TEXTMODE_BUFFER_WIDTH)),
            (ega_textmode_buffer_addr + ((y) * EGA_TEXTMODE_BUFFER_WIDTH)),
            EGA_TEXTMODE_BUFFER_WIDTH * sizeof(uint16_t)
        );
    }
    // Clear bottom line
    for (uint8_t x = 0; x < EGA_TEXTMODE_BUFFER_WIDTH; ++x) {
        ega_textmode_buffer_addr[(EGA_TEXTMODE_BUFFER_HEIGHT - 1) * EGA_TEXTMODE_BUFFER_WIDTH + x] = ega_textmode_entry(' ', ega_textmode_color);
    }

    irqrestore(flags);
}
