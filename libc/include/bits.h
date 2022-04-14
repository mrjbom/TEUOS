#ifndef _BITS_H_
#define _BITS_H_

//Get bit from a number by position
#define get_bit(num, pos) ((num) & (1 << (pos)))

//Set bit in a number by position
#define set_bit(num, pos) ((num) | (1 << (pos)))

//Clear bit in a number by position
#define clear_bit(num, pos) ((num) & (~(1 << (pos))))

//Clear a few lower bits in a number
#define clear_lower_bits(num, count) ((num >> count) << count)

#endif
