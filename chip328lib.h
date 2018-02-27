#ifndef CHIP328LIB_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define CHIP328LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern uint8_t V[16];
extern uint16_t PC, SP_, I;
extern uint8_t DT, ST;
extern uint8_t chip328Memory[800];
extern uint8_t chip328Display[8][32];

#ifdef __cplusplus
extern "C" {
#endif
void chip328Begin();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
uint8_t chip328PutPixel(uint8_t x, uint8_t y, uint8_t pixel);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
void chip328Emulate();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
uint8_t chip328MemoryRead(uint16_t address);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif
void chip328MemoryWrite(uint16_t address, uint8_t data);
#ifdef __cplusplus
}
#endif

#endif
