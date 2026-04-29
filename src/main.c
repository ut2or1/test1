#include "MK64F12.h"

#define UART0_BASE  0x4006A000u
#define SIM_SCGC4_UART0_MASK  (1 << 10)

typedef struct {
    volatile uint8_t BDH;
    volatile uint8_t BDL;
    volatile uint8_t C1;
    volatile uint8_t C2;
    volatile uint8_t S1;
    volatile uint8_t S2;
    volatile uint8_t C3;
    volatile uint8_t D;
    volatile uint8_t C4;
    volatile uint8_t C5;
    volatile uint8_t ED;
    volatile uint8_t MODEM;
    volatile uint8_t IR;
    uint8_t RESERVED0;
    volatile uint8_t PFIFO;
    volatile uint8_t CFIFO;
    volatile uint8_t SFIFO;
    volatile uint8_t TWFIFO;
    volatile uint8_t TCFIFO;
    volatile uint8_t RWFIFO;
    volatile uint8_t RCFIFO;
    uint8_t RESERVED1[2];
    uint8_t C7816[6];
} UART_Type;

#define UART0 ((UART_Type *)UART0_BASE)

#define UART_S1_TDRE_MASK  0x80
#define UART_C2_TE_MASK     0x08
#define UART_C2_RE_MASK     0x04
#define UART_S1_RDRF_MASK   0x20

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

void uart_init(void) {
    uint32_t bus_clock = 120000000u;
    uint32_t baud_rate = 115200u;
    uint16_t sbr;
    
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    UART0->C2 = 0;
    
    sbr = (uint16_t)((bus_clock / 16) / baud_rate);
    
    UART0->BDH = (uint8_t)((sbr >> 8) & 0x1F);
    UART0->BDL = (uint8_t)(sbr & 0xFF);
    
    UART0->C1 = 0;
    UART0->C4 = 0;
    UART0->C3 = 0;
    
    UART0->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
}

int main(void) {
    uart_init();
    
    uart_puts("\r\n");
    uart_puts("Hello from K64FX512!\r\n");
    uart_puts("UART0 test passed\r\n");
    
    uint32_t counter = 0;
    
    while (1) {
        uart_puts("Count: ");
        uart_putc('0' + (counter % 10));
        uart_puts("\r\n");
        counter++;
        
        for (volatile uint32_t d = 0; d < 500000; d++) {
            __asm("nop");
        }
    }
    
    return 0;
}
