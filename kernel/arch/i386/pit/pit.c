#include "pit.h"
#include "../inlineassembly/inlineassembly.h"
#include "stdio.h"
#include "../egatextmode/egatextmode.h"

/*
 * Channel 0 is connected directly to IRQ0, so it is best to use it only for purposes that should generate interrupts.
 * Channel 1 is unusable, and may not even exist.
 * Channel 2 is connected to the PC speaker, but can be used for other purposes without producing audible speaker tones.
 * 
 * Each 8 bit data port is the same,
 * and is used to set the counter's 16 bit reload value or read the channel's 16 bit current count
 */

#define PIT_DATA_PORT_COUNTER0  0x40
#define PIT_DATA_PORT_COUNTER1  0x41
#define PIT_DATA_PORT_COUNTER2  0x42
#define PIT_COMMAND_PORT        0x43

#define PIT_BASE_FREQ           1193181
// The divisor is a 16 bit number
#define PIT_DIVISOR_MIN         1
#define PIT_DIVISOR_MAX         65535
 
// Global tick count
uint32_t pit_ticks = 0;

void pit_init()
{
    //100 Hz is 10 ms
    pit_start_counter(100, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
}

void pit_start_counter(uint32_t required_freq, uint8_t counter, uint8_t mode)
{
    // Save eflags and disable interrupts
    uint32_t flags = save_irqdisable();

    // The divisor is a 16 bit number
    if(required_freq < PIT_BASE_FREQ / PIT_DIVISOR_MAX || required_freq > PIT_BASE_FREQ / PIT_DIVISOR_MIN) {
        return;
    }
    uint16_t divisor = (uint16_t)(PIT_BASE_FREQ / required_freq);

    // Send OCW
    uint8_t ocw = 0;
    ocw = (uint8_t)((ocw & ~PIT_OCW_MASK_MODE) | mode);
    ocw = (uint8_t)((ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA);
    ocw = (uint8_t)((ocw & ~PIT_OCW_MASK_COUNTER) | counter);
    pit_send_command(ocw);
 
    // Set divisor
    pit_send_data(0, (uint8_t)(divisor & 0xFF)); // Low byte(Least Significant Byte)
    pit_send_data(0, (uint8_t)((divisor >> 8) & 0xFF)); // High byte(Most Significant Byte)
 
    // Reset tick count
    pit_ticks = 0;

    // Restore eflags (does not turn on interrupts forcibly if they were turned off before save_irqdisable())
    irqrestore(flags);
}

void pit_send_command(uint8_t cmd)
{
    outb(PIT_COMMAND_PORT, cmd);
}

void pit_send_data(uint8_t counter, uint8_t data)
{
    uint16_t port = PIT_DATA_PORT_COUNTER0 + counter;
    outb(port, data);
}

uint8_t pit_read_data(uint16_t counter)
{
    uint16_t port = PIT_DATA_PORT_COUNTER0 + counter;
    return inb(port);
}

uint8_t hour = 0;
uint8_t min = 0;
uint8_t sec = 0;

void clock_test()
{
    pit_ticks++;
    if (pit_ticks == 100)
    {
        pit_ticks = 0;
        sec++;
    }
 
    if (sec > 59)
    {
        sec = 0;
        min++;
    }
 
    if (min > 59)
    {
        min = 0;
        hour++;
    }
    
    //ega_textmode_set_position(0, 1);
    //printf_("Time: %hhu hours %hhu minuts %hhu seconds", hour, min, sec);
}
