#ifndef _BITS_H_
#define _BITS_H_

/* Get bit from a number by position */
#define bits_get_bit(num, pos) ((num) & (1 << (pos)))

/* Set bit in a number by position */
#define bits_set_bit(num, pos) ((num) | (1 << (pos)))

/* Clear bit in a number by position */
#define bits_clear_bit(num, pos) ((num) & (~(1 << (pos))))

/* Clear a few lower bits in a number */
#define bits_clear_lower_bits(num, count) ((num >> count) << count)

#endif
