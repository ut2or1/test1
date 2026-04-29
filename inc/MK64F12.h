#ifndef MK64F12_H
#define MK64F12_H

#include <stdint.h>

#define SIM_BASE       0x40047000u

typedef struct {
    uint32_t SOPT1;
    uint32_t SOPT1CFG;
    uint32_t RESERVED0[2];
    uint32_t SOPT2;
    uint32_t RESERVED1[3];
    uint32_t SCGC4;
    uint32_t SCGC5;
    uint32_t SCGC6;
    uint32_t SCGC7;
    uint32_t CLKDIV1;
    uint32_t CLKDIV2;
    uint32_t FCFG1;
    uint32_t FCFG2;
    uint32_t UIDH;
    uint32_t UIDMH;
    uint32_t UIDML;
    uint32_t UIDL;
} SIM_Type;

#define SIM ((SIM_Type *)SIM_BASE)

#endif
