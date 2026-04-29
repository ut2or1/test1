#include <stdint.h>

extern int main(void);

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

// cppcheck-suppress constParameterPointer
void Reset_Handler(void) {
    // cppcheck-suppress constVariablePointer
    const uint32_t *src = &_etext;
    // cppcheck-suppress comparePointers
    uint32_t *dst = (uint32_t *)&_sdata;
    
    // cppcheck-suppress comparePointers
    while ((uint32_t)dst < (uint32_t)&_edata) {
        *dst++ = *src++;
    }
    
    // cppcheck-suppress comparePointers
    dst = (uint32_t *)&_sbss;
    // cppcheck-suppress comparePointers
    while ((uint32_t)dst < (uint32_t)&_ebss) {
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
