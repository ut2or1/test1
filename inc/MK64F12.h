#ifndef MK64F12_H
#define MK64F12_H

#include <stdint.h>

// Базовые адреса
#define SIM_BASE       0x40047000u
#define PORTC_BASE     0x400FF080u

// System Integration Module
typedef struct {
    uint32_t SOPT1;
    uint32_t SOPT1CFG;
    uint32_t RESERVED0[1022];
    uint32_t SCGC5;
} SIM_Type;

#define SIM ((SIM_Type *)SIM_BASE)
#define SIM_SCGC5_PORTC_MASK    (1 << 11)

// Port Control
typedef struct {
    uint32_t PCR[32];
    uint32_t GPCLR;
    uint32_t GPCHR;
    uint32_t RESERVED0[6];
    uint32_t ISFR;
    uint32_t RESERVED1[7];
    uint32_t PDOR;
    uint32_t PSOR;
    uint32_t PCOR;
    uint32_t PTOR;
    uint32_t PDIR;
    uint32_t PDDR;
} PORT_Type;

#define PORTC ((PORT_Type *)PORTC_BASE)

#endif // MK64F12_H
