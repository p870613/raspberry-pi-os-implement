//source:https://github.com/raspberrypi/firmware/wiki/Mailboxes

#ifndef _H_MAILBOX
#define _H_MAILBOX

#include <gpio.h>
#include <type.h>

// register
#define MAILBOX_BASE (PBASE + 0xb880)
#define MAILBOX_READ ((volatile unsigned int*)(MAILBOX_BASE))
#define MAILBOX_STATUS ((volatile unsigned int*)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE ((volatile unsigned int*)(MAILBOX_BASE + 0X20))
#define MAILBOX_FULL 0X80000000
#define MAILBOX_EMPTY 0x40000000

//channel
#define MAILBOX_CH_POWER   0
#define MAILBOX_CH_FB      1
#define MAILBOX_CH_VUART   2
#define MAILBOX_CH_VCHIQ   3
#define MAILBOX_CH_LEDS    4
#define MAILBOX_CH_BTNS    5
#define MAILBOX_CH_TOUCH   6
#define MAILBOX_CH_COUNT   7
#define MAILBOX_CH_PROP    8

//tags 
#define MAILBOX_GET_BOARD_REVISION 0x00010002

//buffer content
#define MAILBOX_REQUEST_CODE 0x00000000
#define MAILBOX_REQUEST_SUCCEED 0x80000000
#define MAILBOX_REQUEST_FAILED 0x80000001

#define MAILBOX_TAG_REQUEST_CODE 0x00000000
#define MAILBOX_END_TAG 0x00000000
#define MAILBOX_RESPONSE 0x80000000

//buffer
volatile uint32_t __attribute__ ((aligned (16))) mailbox[36];

unsigned int get_board_revision();
void mailbox_call(unsigned char);

#endif
