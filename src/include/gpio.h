#ifndef _GPIO_H
#define _GPIO_H

#define PBASE 0x3F000000
#define GPFSEL1         (volatile unsigned int*)(PBASE+0x00200004)
#define GPSET0          (volatile unsigned int*)(PBASE+0x0020001C)
#define GPCLR0          (volatile unsigned int*)(PBASE+0x00200028)
#define GPPUD           (volatile unsigned int*)(PBASE+0x00200094)
#define GPPUDCLK0       (volatile unsigned int*)(PBASE+0x00200098)

#endif
