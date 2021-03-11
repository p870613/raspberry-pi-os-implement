#include "uart.h"
#include "gpio.h"

void uart_send(char c) {
    while(1) {
        if((*AUX_MU_LSR_REG) & 0X20)
            break;
    }
    *AUX_MU_IO_REG = c;
}

void uart_put(char* str) {
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == '\n'){
            uart_send('\r');
        }
        uart_send(str[i]);
    }
}

void uart_get(char *ret){
    int index = 0;
    while(1){
        char input = uart_recv();
        uart_send(input);
        if(input == '\n' || input == '\r'){
            ret[index] = '\0';
            break;
        }

        ret[index] = input;
        index ++;
    }
}

char uart_recv(void) {
    while(1) {
		if((*AUX_MU_LSR_REG) & 0x01)
			break;
	}
	char ret = ((*AUX_MU_IO_REG) & 0xFF);
    return ret == '\r' ? '\n' : ret;
}

void delay(unsigned int n){
    while(n --)
        asm volatile("nop");
}

void uart_init(void){
    unsigned int selector = *GPFSEL1;

    selector &= ~(7<<12);                   // clean gpio14
    selector |= 2<<12;                      // set alt5 for gpio14
    selector &= ~(7<<15);                   // clean gpio15
    selector |= 2<<15;                      // set alt5 for gpio 15
    *GPFSEL1 = selector;

    *GPPUD = 0;
    delay(150);
    *GPPUDCLK0 = (1<<14)|(1<<15);
    delay(150);
    *GPPUDCLK0 = 0;

    *AUX_ENABLES |= 1;                   //Enable mini uart (this also enables access to its registers)
    *AUX_MU_CNTL_REG = 0;               //Disable auto flow control and disable receiver and transmitter (for now)
    *AUX_MU_IER_REG = 0;                //Disable receive and transmit interrupts
    *AUX_MU_LCR_REG = 3;                //Enable 8 bit mode
    *AUX_MU_MCR_REG = 0;                //Set RTS line to be always high
    *AUX_MU_BAUD_REG = 270;             //Set baud rate to 115200
    *AUX_MU_CNTL_REG = 3;               //Finally, enable transmitter and receiver
}
