#include <stdint.h>

extern int main(void);

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) {
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
    
    main();
    
    while (1);
}

void Default_Handler(void) {
    while (1);
}

__attribute__((section(".vectors")))
void (* const vector_table[])(void) = {
    (void (*)(void))0x20000000,
    Reset_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    Default_Handler,
    0, 0, 0, 0,
    Default_Handler,
    Default_Handler,
    0,
    Default_Handler,
    Default_Handler,
};
