#ifndef _BITS_H_
#define _BITS_H_

#define GET_BIT(num, bit) (((num) >> (bit)) & 1)
#define SET_BIT(num, bit) ((num) |= (1 << (bit)))
#define CLEAR_BIT(num, bit) ((num) &= (typeof(num))~(1 << (bit)))

#define GET_BIT_MSB(num, bit) (((num) >> ((sizeof(num) * 8) - 1 - (bit))) & 1)
#define SET_BIT_MSB(num, bit) ((num) |= (1 << ((sizeof(num) * 8) - 1 - (bit))))
#define CLEAR_BIT_MSB(num, bit) ((num) &= (typeof(num))(~(1 << ((sizeof(num) * 8) - 1 - (bit)))))

#endif
