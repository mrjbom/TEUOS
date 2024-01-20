#ifndef _MEMORYLAYOUT_H_
#define _MEMORYLAYOUT_H_

/*
 * Variables and constants required for memory management
 */

/*
  Virtual address space layout
  ---------------------------- 0x00000000
  |                          |
  |                          |
  |                          |
  |                          |
  |                          |
  |        USER SPACE        |
  |                          |
  |                          |
  |                          |
  |                          |
  |                          |
  |                          |
  ---------------------------- 0xC0000000
  |                          |
  |       KERNEL SPACE       |
  |                          |
  |                          |
  ---------------------------- 0xFFFFFFFF

  User space is separate for each process and changes when the process is switched over.
  Kernel space is one and is always mapped to virtual space.

              KERNEL SPACE
  -----------------------------------
  |       loaded kernel start       |
  |               ...               |
  |        loaded kernel end        |
  |---------------------------------|
  |    Memory areas used by PMM     |
  |---------------------------------|
  |    Memory areas used by VMM     |
  |---------------------------------|
  |           kernel heap           |
  |---------------------------------|

  The memory areas used by PMM and VMM are reserved in advance and are necessary for their work.
*/

#endif
