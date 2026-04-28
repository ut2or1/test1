#include <stdint.h>

extern int main(void);
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

// cppcheck-suppress constParameterPointer
void Reset_Handler(void) {
    const uint32_t *src = &_etext;
    // cppcheck-suppress comparePointers
    uint32_t *dst = (uint32_t *)&_sdata;
    
    // Копируем data section из Flash в RAM
    // cppcheck-suppress comparePointers
    while ((uint32_t)dst < (uint32_t)&_edata) {
        *dst++ = *src++;
    }
    
    // Обнуляем bss
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

// Таблица векторов прерываний
__attribute__((section(".vectors")))
void (* const vector_table[])(void) = {
    (void (*)(void))0x20000000,  // Начальное значение SP
    Reset_Handler,
    Default_Handler,  // NMI
    Default_Handler,  // HardFault
    Default_Handler,  // MemManage
    Default_Handler,  // BusFault
    Default_Handler,  // UsageFault
    0, 0, 0, 0,       // Reserved
    Default_Handler,  // SVC
    Default_Handler,  // DebugMon
    0,                 // Reserved
    Default_Handler,  // PendSV
    Default_Handler,  // SysTick
};
