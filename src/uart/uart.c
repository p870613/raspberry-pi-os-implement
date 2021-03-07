#include "uart.h"

void uart_send(char c) {
    while(1) {
        if(get32(AUX_MU_LSR_REG) & 0X20)
            break;
    }
    put32(AUX_MU_IO_REG,c);
}

void uart_write(char* str) {
    for(int i = 0; str[i] != '\0'; i++)
        uart_send()
}

char uart_recv(void) {
    while(1) {
		if(get32(AUX_MU_LSR_REG)&0x01)
			break;
	}
	return(get32(AUX_MU_IO_REG)&0xFF);
}
void uart_init(){
}
