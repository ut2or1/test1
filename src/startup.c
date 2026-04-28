#include <stdint.h>

extern int main(void);
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) {
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    
    // Копируем data section из Flash в RAM
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Обнуляем bss
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
