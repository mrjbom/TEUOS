#ifndef _PIT_H_
#define _PIT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Bits         Usage
 * 6 and 7      Select channel :
 *                0 0 = Channel 0
 *                0 1 = Channel 1
 *                1 0 = Channel 2
 *                1 1 = Read-back command (8254 only)
 * 4 and 5      Access mode :
 *                0 0 = Latch count value command
 *                0 1 = Access mode: lobyte only
 *                1 0 = Access mode: hibyte only
 *                1 1 = Access mode: lobyte/hibyte
 * 1 to 3       Operating mode :
 *                0 0 0 = Mode 0 (interrupt on terminal count)
 *                0 0 1 = Mode 1 (hardware re-triggerable one-shot)
 *                0 1 0 = Mode 2 (rate generator)
 *                0 1 1 = Mode 3 (square wave generator)
 *                1 0 0 = Mode 4 (software triggered strobe)
 *                1 0 1 = Mode 5 (hardware triggered strobe)
 *                1 1 0 = Mode 2 (rate generator, same as 010b)
 *                1 1 1 = Mode 3 (square wave generator, same as 011b)
 * 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
 */

#define PIT_OCW_MASK_BINCOUNT       1       //00000001
#define PIT_OCW_MASK_MODE           0xE     //00001110
#define PIT_OCW_MASK_RL             0x30    //00110000
#define PIT_OCW_MASK_COUNTER        0xC0    //11000000

#define PIT_OCW_BINCOUNT_BINARY     0       //0         // Use when setting PIT_OCW_MASK_BINCOUNT
#define PIT_OCW_BINCOUNT_BCD        1       //1
#define PIT_OCW_MODE_TERMINALCOUNT  0       //0000      // Use when setting PIT_OCW_MASK_MODE
#define PIT_OCW_MODE_ONESHOT        0x2     //0010
#define PIT_OCW_MODE_RATEGEN        0x4     //0100
#define PIT_OCW_MODE_SQUAREWAVEGEN  0x6     //0110
#define PIT_OCW_MODE_SOFTWARETRIG   0x8     //1000
#define PIT_OCW_MODE_HARDWARETRIG   0xA     //1010
#define PIT_OCW_RL_LATCH            0       //000000    // Use when setting PIT_OCW_MASK_RL
#define PIT_OCW_RL_LSBONLY          0x10    //010000
#define PIT_OCW_RL_MSBONLY          0x20    //100000
#define PIT_OCW_RL_DATA             0x30    //110000
#define PIT_OCW_COUNTER_0           0       //00000000  // Use when setting PIT_OCW_MASK_COUNTER
#define PIT_OCW_COUNTER_1           0x40    //01000000
#define PIT_OCW_COUNTER_2           0x80    //10000000

extern void pit_init(void);

// required_freq - the frequency at which PIT will try to work (18 - 1193181)
// counter - PIT_OCW_COUNTER_x
void pit_start_counter(uint32_t required_freq, uint8_t counter, uint8_t mode);

// send OCW(Operation Command Word)
extern void pit_send_command(uint8_t cmd);

// counter - (0-2)
extern void pit_send_data(uint8_t counter, uint8_t data);

// counter - (0-2)
extern uint8_t pit_read_data(uint16_t counter);

extern uint32_t pit_get_ticks(void);

extern void clock_test(void);

#endif
