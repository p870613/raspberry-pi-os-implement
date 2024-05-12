#include "peripheral/uart.h"
#include <type.h>

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

void uart_nput(char* str, int n){
    for(int i = 0; i < n; i++){
        if(str[i] == '\n'){
            uart_send('\r');
        }
        uart_send(str[i]);
    }
}

void uart_int(int n){
    char output[100];
    int index = 0, flag = 0;

    if(n < 0){
        n = -n;
        flag = 1;
    }else if(n == 0){
        uart_put("0");
        return ;
    }

    while(n > 0){
        output[index++] = (n % 10) + '0';
        n = n / 10;
    }
    if(flag == 1)
        output[index ++] = '-';
    output[index] = '\0';

    for(int i = index - 1; i >= 0; i--)
        uart_send(output[i]);
}

void uart_get_cmd(char *ret){
    int index = 0;
    while(1){
        char input = uart_recv();
        
        if(input == '\n' || input == '\r'){
            ret[index] = '\0';
            uart_send('\r');
            uart_send('\n');
            break;
        }

        if((input == 127 || input == 8) && index <= 0)
            continue;

        //backspace or delete
        if((input == 127 || input == 8) && index > 0){
            uart_send('\b');
            uart_send(' ');
            uart_send('\b');
            ret[index--] = '\0';
        }else {
            uart_send(input);
    
            ret[index] = input;
            index ++;
        }
    }
}

void uart_get(char *ret){
    int index = 0;
    while(1){
        char input = uart_recv();
        if(input == '\n' || input == '\r'){
            ret[index] = '\0';
            break;
        }

        ret[index] = input;
        index ++;
    }
}

void uart_nget(char* buf, int n) {
    for(int i = 0; i < n; i++){
        buf[i] = uart_recv();
    }
}

void uart_hex(unsigned int d){
    unsigned int n;
    
    for(int i = 28; i >= 0; i -= 4){
        n = (d >> i) & 0xf;
        n += n > 9 ? 0x37 : 0x30;
        uart_send(n);
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

size_t get_time()
{
    size_t count, freq;
    
    asm volatile("mrs %[result], cntpct_el0": [result]"=r"(count));
    asm volatile("mrs %[result], cntfrq_el0": [result]"=r"(freq));

    return (1000 * count) / freq;
}

void delay(unsigned int n)
{
    int start_time = get_time();
    int cur_time = start_time;

    while(cur_time - start_time < n) {
        cur_time = get_time();
    }
}

void uart_init(void){
    unsigned int selector = *GPFSEL1;
    register unsigned int r;

    selector &= ~(7<<12);                   // clean gpio14
    selector |= 2<<12;                      // set alt5 for gpio14
    selector &= ~(7<<15);                   // clean gpio15
    selector |= 2<<15;                      // set alt5 for gpio 15
    *GPFSEL1 = selector;

    *GPPUD = 0;
    r = 150; while(r--) {asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r = 150; while(r--) {asm volatile("nop"); }
    *GPPUDCLK0 = 0;

    *AUX_ENABLES |= 1;                   //Enable mini uart (this also enables access to its registers)
    *AUX_MU_CNTL_REG = 0;               //Disable auto flow control and disable receiver and transmitter (for now)
    *AUX_MU_IER_REG = 0;                //Disable receive and transmit interrupts
    *AUX_MU_LCR_REG = 3;                //Enable 8 bit mode
    *AUX_MU_MCR_REG = 0;                //Set RTS line to be always high
    *AUX_MU_BAUD_REG = 270;             //Set baud rate to 115200
    *AUX_MU_IIR_REG = 6;                // NO FIFO
    *AUX_MU_CNTL_REG = 3;               //Finally, enable transmitter and receiver

}
