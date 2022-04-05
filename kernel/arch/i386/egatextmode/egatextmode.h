#ifndef _EGATEXTMODE_H_
#define _EGATEXTMODE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

enum ega_textmode_color {
    EGA_COLOR_BLACK = 0,
    EGA_COLOR_BLUE = 1,
    EGA_COLOR_GREEN = 2,
    EGA_COLOR_CYAN = 3,
    EGA_COLOR_RED = 4,
    EGA_COLOR_MAGENTA = 5,
    EGA_COLOR_BROWN = 6,
    EGA_COLOR_LIGHT_GREY = 7,
    EGA_COLOR_DARK_GREY = 8,
    EGA_COLOR_LIGHT_BLUE = 9,
    EGA_COLOR_LIGHT_GREEN = 10,
    EGA_COLOR_LIGHT_CYAN = 11,
    EGA_COLOR_LIGHT_RED = 12,
    EGA_COLOR_LIGHT_MAGENTA = 13,
    EGA_COLOR_LIGHT_BROWN = 14,
    EGA_COLOR_WHITE = 15,
};

extern uint16_t* ega_textmode_buffer_addr;

extern const uint8_t EGA_TEXTMODE_BUFFER_WIDTH;
extern const uint8_t EGA_TEXTMODE_BUFFER_HEIGHT;

// Text mode state
// Next symbol position
extern uint8_t ega_textmode_xpos;
extern uint8_t ega_textmode_ypos;

// Current symbol color(fg and bg)
extern uint8_t ega_textmode_color;

extern void ega_textmode_init(void);

extern void ega_textmode_set_color(uint8_t fg, uint8_t bg);

extern void ega_textmode_set_position(uint8_t x, uint8_t y);

extern void ega_textmode_clear(void);

extern void ega_textmode_putch(uint8_t ch);

extern void ega_textmode_write(const char* str);

#endif
