#include "MK64F12.h"

#define LED_PIN      (1 << 21)   // PTC21 - красный светодиод на FRDM-K64F
#define LED_PORT     PTC
#define DELAY_COUNT  1000000

void delay(volatile uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}

int main(void) {
    // Включаем тактирование порта C
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    
    // Настраиваем пин как GPIO output
    LED_PORT->PDDR |= LED_PIN;
    
    while (1) {
        LED_PORT->PSOR = LED_PIN;   // Включить светодиод
        delay(DELAY_COUNT);
        LED_PORT->PCOR = LED_PIN;   // Выключить светодиод
        delay(DELAY_COUNT);
    }
    
    return 0;
}
