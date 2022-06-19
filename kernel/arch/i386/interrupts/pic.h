#ifndef _PIC_H_
#define _PIC_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define PIC_PIC1            0x20        // IO base address for master PIC
#define PIC_PIC2            0xA0        // IO base address for slave PIC
#define PIC_PIC1_COMMAND    PIC_PIC1
#define PIC_PIC1_DATA       (PIC_PIC1 + 1)
#define PIC_PIC2_COMMAND    PIC_PIC2
#define PIC_PIC2_DATA       (PIC_PIC2 + 1)

#define PIC_ICW1_ICW4       0x01        // ICW4 (not) needed */
#define PIC_ICW1_SINGLE     0x02        // Single (cascade) mode */
#define PIC_ICW1_INTERVAL4  0x04        // Call address interval 4 (8) */
#define PIC_ICW1_LEVEL      0x08        // Level triggered (edge) mode */
#define PIC_ICW1_INIT       0x10        // Initialization - required! */

#define PIC_ICW4_8086       0x01        // 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO       0x02        // Auto (normal) EOI */
#define PIC_ICW4_BUF_SLAVE  0x08        // Buffered mode/slave */
#define PIC_ICW4_BUF_MASTER 0x0C        // Buffered mode/master */
#define PIC_ICW4_SFNM       0x10        // Special fully nested (not) */

#define PIC_EOI         0x20        // End-of-interrupt command code

/* Init PIC (remap) */
extern void pic_init(void);

/*
 * Remap PIC
 *
 * offset1 - vector offset for master PIC
 *           vectors on the master become offset1..offset1+7
 * offset2 - same for slave PIC: offset2..offset2+7
 */
extern void pic_remap(uint8_t offset1, uint8_t offfset2);

/* 
 * Sends the EOI signal to the PIC depending on the IRQ number
 * irq_num - IRQ number(0-15)
 */
extern void pic_send_EOI(uint8_t irq_num);

#endif
