#ifndef CHIP328LIB_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define CHIP328LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern uint8_t V[16];
extern uint16_t PC, SP, I;
extern uint8_t DT, ST;
extern uint8_t chip328Memory[4096];
extern uint8_t chip328Display[8][32];

void chip328Begin();
uint8_t chip328PutPixel(uint8_t x, uint8_t y, uint8_t pixel);
void chip328Emulate();

#endif
