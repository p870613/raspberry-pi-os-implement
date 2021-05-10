#include <peripheral/mailbox.h>

unsigned int get_board_revision(){
    mailbox[0] = 7 * 4;
    mailbox[1] = MAILBOX_REQUEST_CODE;
    mailbox[2] = MAILBOX_GET_BOARD_REVISION;
    mailbox[3] = 4;     //response buffer size
    mailbox[4] = MAILBOX_TAG_REQUEST_CODE;
    mailbox[5] = 0;     //value buffer
    mailbox[6] = MAILBOX_END_TAG;
    
    mailbox_call(MAILBOX_CH_PROP);
    
    return mailbox[5];
} 

void  mailbox_call(unsigned char channel) {
    unsigned int addr = (((size_t)&mailbox) & (unsigned int)(~0xf)) | (channel & 0xf);

    while(*MAILBOX_STATUS & MAILBOX_FULL);

    *MAILBOX_WRITE = addr;

    while(*MAILBOX_STATUS & MAILBOX_EMPTY);

    while(*MAILBOX_READ != addr);
}
