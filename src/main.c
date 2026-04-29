#include "MK64F12.h"

#define UART0_BASE  0x4006A000u
#define SIM_SCGC4_UART0_MASK  (1 << 10)

typedef struct {
    uint32_t BDH;
    uint32_t BDL;
    uint32_t C1;
    uint32_t C2;
    uint32_t S1;
    uint32_t S2;
    uint32_t C3;
    uint32_t D;
    uint32_t C4;
    uint32_t C5;
    uint32_t ED;
    uint32_t MODEM;
    uint32_t IR;
    uint32_t RESERVED0[1];
    uint32_t PFIFO;
    uint32_t CFIFO;
    uint32_t SFIFO;
    uint32_t TWFIFO;
    uint32_t TCFIFO;
    uint32_t RWFIFO;
    uint32_t RCFIFO;
    uint32_t RESERVED1[2];
    uint32_t C7816[6];
} UART_Type;

#define UART0 ((UART_Type *)UART0_BASE)

#define UART_S1_TDRE_MASK  (1 << 7)
#define UART_C2_TE_MASK    (1 << 3)
#define UART_C2_RE_MASK    (1 << 2)
#define UART_S1_RDRF_MASK  (1 << 5)

void uart_putc(char c) {
    while (!(UART0->S1 & UART_S1_TDRE_MASK));
    UART0->D = c;
}

void uart_puts(const char *str) {
    while (*str) {
        if (*str == '\n') {
            uart_putc('\r');
        }
        uart_putc(*str++);
    }
}

char uart_getc(void) {
    while (!(UART0->S1 & UART_S1_RDRF_MASK));
    return (char)UART0->D;
}

void uart_init(void) {
    uint32_t bus_clock = 120000000u;
    uint32_t baud_rate = 115200u;
    uint16_t sbr;
    uint8_t temp;
    
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    
    sbr = (uint16_t)((bus_clock / 16) / baud_rate);
    
    temp = UART0->BDH & 0x1F;
    UART0->BDH = temp | (uint8_t)((sbr >> 8) & 0x1F);
    UART0->BDL = (uint8_t)(sbr & 0xFF);
    
    UART0->C1 = 0x00;
    UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

int main(void) {
    uart_init();
    
    uart_puts("\r\n");
    uart_puts("=========================================\r\n");
    uart_puts("  K64 UART Test\r\n");
    uart_puts("=========================================\r\n");
    uart_puts("System booted successfully!\r\n");
    uart_puts("UART0 initialized at 115200 baud\r\n");
    uart_puts("\r\n");
    
    uint32_t counter = 0;
    
    while (1) {
        uart_puts("Hello from K64FX512! Counter: ");
        
        char buf[16];
        uint32_t val = counter;
        int i = 0;
        
        if (val == 0) {
            buf[i++] = '0';
        } else {
            while (val > 0) {
                buf[i++] = '0' + (val % 10);
                val /= 10;
            }
        }
        
        while (i > 0) {
            uart_putc(buf[--i]);
        }
        
        uart_puts("\r\n");
        counter++;
        
        for (volatile uint32_t d = 0; d < 5000000; d++) {
            __asm("nop");
        }
    }
    
    return 0;
}
