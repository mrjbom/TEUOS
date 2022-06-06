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
/*
 * Buffer referring to the virtual address of the text mode buffer
 * Virtual addres 0xC03FF000 mapped to physical 0xB8000
 */
extern uint16_t* ega_textmode_buffer_addr;

/* Initial buffer width (in symbols)*/
extern const uint8_t EGA_TEXTMODE_BUFFER_WIDTH;
/* Initial buffer height (in symbols) */
extern const uint8_t EGA_TEXTMODE_BUFFER_HEIGHT;

/* Current text mode states */

/*  Next symbol position */
extern uint8_t ega_textmode_xpos;
extern uint8_t ega_textmode_ypos;

/* Current symbol color (foreground and background) */
extern uint8_t ega_textmode_color;

/* 
 * Basic initialization
 * Clears the screen
 * Sets the default text color 
 */
extern void ega_textmode_init(void);

/* Change foreground and background text color */
extern void ega_textmode_set_color(uint8_t fg, uint8_t bg);

/* Change next symbol position */
extern void ega_textmode_set_position(uint8_t x, uint8_t y);

/* Clears the text mode buffer by filling it with zeros */
extern void ega_textmode_clear(void);

/* Put character in the text mode buffer using the current position and color */
extern void ega_textmode_putch(uint8_t ch);

extern void ega_textmode_scroll(void);

#endif
