#include "chip328lib.h"

uint8_t V[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PC, SP, I;
uint8_t DT, ST;
uint8_t chip328Memory[4096];
uint8_t chip328Display[8][32];

void chip328Begin(){
//Dxyn
  PC = 0x0200;
  I = 0x0000;
  SP = 0X0000;
  DT = 0;
  ST = 0;
  //0
  chip328Memory[0] = 0xF0;
  chip328Memory[1] = 0x90;
  chip328Memory[2] = 0x90;
  chip328Memory[3] = 0x90;
  chip328Memory[4] = 0xF0;
  //1
  chip328Memory[5] = 0x20;
  chip328Memory[6] = 0x60;
  chip328Memory[7] = 0x20;
  chip328Memory[8] = 0x20;
  chip328Memory[9] = 0x70;
  //2
  chip328Memory[10] = 0xF0;
  chip328Memory[11] = 0x10;
  chip328Memory[12] = 0xF0;
  chip328Memory[13] = 0x80;
  chip328Memory[14] = 0xF0;
  //3
  chip328Memory[15] = 0xF0;
  chip328Memory[16] = 0x10;
  chip328Memory[17] = 0xF0;
  chip328Memory[18] = 0x10;
  chip328Memory[19] = 0xF0;
  //4
  chip328Memory[20] = 0x90;
  chip328Memory[21] = 0x90;
  chip328Memory[22] = 0xF0;
  chip328Memory[23] = 0x10;
  chip328Memory[24] = 0x10;
  //5
  chip328Memory[25] = 0xF0;
  chip328Memory[26] = 0x80;
  chip328Memory[27] = 0xF0;
  chip328Memory[28] = 0x10;
  chip328Memory[29] = 0xF0;
  //6
  chip328Memory[30] = 0xF0;
  chip328Memory[31] = 0x80;
  chip328Memory[32] = 0xF0;
  chip328Memory[33] = 0x90;
  chip328Memory[34] = 0xF0;
  //7
  chip328Memory[35] = 0xF0;
  chip328Memory[36] = 0x10;
  chip328Memory[37] = 0x20;
  chip328Memory[38] = 0x40;
  chip328Memory[39] = 0x40;
  //8
  chip328Memory[40] = 0xF0;
  chip328Memory[41] = 0x90;
  chip328Memory[42] = 0xF0;
  chip328Memory[43] = 0x90;
  chip328Memory[44] = 0xF0;
  //9
  chip328Memory[45] = 0xF0;
  chip328Memory[46] = 0x90;
  chip328Memory[47] = 0xF0;
  chip328Memory[48] = 0x10;
  chip328Memory[49] = 0xF0;
  //A
  chip328Memory[50] = 0xF0;
  chip328Memory[51] = 0x90;
  chip328Memory[52] = 0xF0;
  chip328Memory[53] = 0x90;
  chip328Memory[54] = 0x90;
  //B
  chip328Memory[55] = 0xE0;
  chip328Memory[56] = 0x90;
  chip328Memory[57] = 0xE0;
  chip328Memory[58] = 0x90;
  chip328Memory[59] = 0xE0;
  //C
  chip328Memory[60] = 0xF0;
  chip328Memory[61] = 0x80;
  chip328Memory[62] = 0x80;
  chip328Memory[63] = 0x80;
  chip328Memory[64] = 0xF0;
  //D
  chip328Memory[65] = 0xE0;
  chip328Memory[66] = 0x90;
  chip328Memory[67] = 0x90;
  chip328Memory[68] = 0x90;
  chip328Memory[69] = 0xE0;
  //E
  chip328Memory[70] = 0xF0;
  chip328Memory[71] = 0x80;
  chip328Memory[72] = 0xF0;
  chip328Memory[73] = 0x80;
  chip328Memory[74] = 0xF0;
  //F
  chip328Memory[75] = 0xF0;
  chip328Memory[76] = 0x80;
  chip328Memory[77] = 0xF0;
  chip328Memory[78] = 0x80;
  chip328Memory[79] = 0x80;
}

uint8_t chip328PutPixel(uint8_t x, uint8_t y, uint8_t pixel){
  uint8_t offsetByte=x/8;
  uint8_t offsetBit=x%8;
  uint8_t collision = 0;

  if(pixel){
    if(chip328Display[offsetByte][y]&(0b10000000>>offsetBit)) collision = 1;
    chip328Display[offsetByte][y] = chip328Display[offsetByte][y]^(0b10000000>>offsetBit);
  }
  return collision;
}

void chip328Emulate(){
  //Dxyn - DRW Vx, Vy, nibble
  //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
  if(chip328Memory[PC]>>4 == 0xD){
    uint8_t i,j, offsetByte=0, offsetBit=0;;
    uint8_t x = V[chip328Memory[PC]&0x0F];
    uint8_t y = V[chip328Memory[PC+1]>>4];
    V[15]=0x00;
    for(i=0;i<(chip328Memory[PC+1]&0x0F);i++){
      for(j=0;j<8;j++){
        if(chip328PutPixel(x+j,y+i,chip328Memory[I+i]&(0b10000000>>j))){
          V[15]=0x01;
        }
      }
    }
    PC=PC+2;
  }
  //JP
  if(chip328Memory[PC]>>4 == 0x1){
    PC = ((chip328Memory[PC]<<8) + chip328Memory[PC+1])&0x0FFF;
    return;
  }
  //7xkk - ADD Vx, byte
  //Set Vx = Vx + kk.
  if(chip328Memory[PC]>>4 == 0x7){
    V[chip328Memory[PC]&0b00001111] = V[chip328Memory[PC]&0b00001111] + chip328Memory[PC+1];
    PC=PC+2;
    return;
  }
  //0nnn - SYS addr
  if(chip328Memory[PC]>>4 == 0x0 && chip328Memory[PC+1]>>4 != 0xE){
    PC=PC+2;
    return;
  }
  //00E0 - CLS
  if(chip328Memory[PC] == 0x00 && chip328Memory[PC+1] == 0xE0){
    uint8_t i,j;
    for(i=0;i<8;i++){
      for(j=0;j<32;j++){
        chip328Display[i][j]=0x00;
      }
    }
    PC=PC+2;
    return;
  }
  //00EE - RET
  if(chip328Memory[PC] == 0x00 && chip328Memory[PC+1] == 0xEE){
    PC = ((chip328Memory[SP]<<8) + chip328Memory[SP+1])&0x0FFF;
    if(SP==0x0FFE){
      SP=0x0000;
    }else{
      SP=SP+2;
    }
    PC=PC+2;
    return;
  }
  //2nnn - CALL addr
  if(chip328Memory[PC]>>4 == 0x2){
    if(SP==0x0000){
      SP=0x0FFE;
    }else{
      SP=SP-2;
    }
    chip328Memory[SP]=PC>>8;
    chip328Memory[SP+1]=PC&0x00FF;
    PC=((chip328Memory[PC]<<8) + chip328Memory[PC+1])&0x0FFF;
    return;
  }
  //6xkk - LD Vx, byte
  //Set Vx = kk
  if(chip328Memory[PC]>>4==0x6){
    V[chip328Memory[PC]&0x0F]=chip328Memory[PC+1];
    PC+=2;
    return;
  }
  //Annn - LD I, addr
  //Set I = nnn
  if(chip328Memory[PC]>>4==0xA){
    I = ((chip328Memory[PC]<<8) + chip328Memory[PC+1])&0x0FFF;
    PC+=2;
    return;
  }
  //3xkk - SE Vx, byte
  //Skip next instruction if Vx = kk.
  if(chip328Memory[PC]>>4==0x3){
    if(V[chip328Memory[PC]&0x0F]==chip328Memory[PC+1]){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx07 - LD Vx, DT
  //Set Vx = delay timer value.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x07){
    uint8_t x = chip328Memory[PC]&0x0F;
    V[x] = DT;
    PC+=2;
    return;
  }
  //Contém erros
  //Fx33 - LD B, Vx
  //Store BCD representation of Vx in memory locations I, I+1, and I+2.
  //Por exemplo o número 128 segue a seguinte ordem, 1->[I], 2->[I+1], 8->[I+2]
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x33){
    uint8_t n = V[chip328Memory[PC]&0x0F];
    chip328Memory[I+2]=n%10;
    n=n/10;
    chip328Memory[I+1]=n%10;
    n=n/10;
    chip328Memory[I]=n&0x0F%10;
    PC+=2;
    return;
  }
  //Fx65 - LD Vx, [I]
  //Read registers V0 through Vx from memory starting at location I.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x65){
    uint8_t i;
    uint8_t x = chip328Memory[PC]&0x0F;
    for(i=0;i<=x;i++){
      V[i]=chip328Memory[I+i];
    }
    PC+=2;
    return;
  }
  //Fx29 - LD F, Vx
  //Set I = location of sprite for digit Vx.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x29){
    uint8_t i;
    uint8_t x = V[chip328Memory[PC]&0x0F];
    if(x == 0x00) I = 0;
    if(x == 0x01) I = 5;
    if(x == 0x02) I = 10;
    if(x == 0x03) I = 15;
    if(x == 0x04) I = 20;
    if(x == 0x05) I = 25;
    if(x == 0x06) I = 30;
    if(x == 0x07) I = 35;
    if(x == 0x08) I = 40;
    if(x == 0x09) I = 45;
    if(x == 0x0A) I = 50;
    if(x == 0x0B) I = 55;
    if(x == 0x0C) I = 60;
    if(x == 0x0D) I = 65;
    if(x == 0x0E) I = 70;
    if(x == 0x0F) I = 75;
    PC+=2;
    return;
  }
  //Fx1E - ADD I, Vx
  //Set I = I + Vx.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x1E){
    uint8_t i;
    uint8_t x = chip328Memory[PC]&0x0F;
    I = I + V[x];
    PC+=2;
    return;
  }
  //Fx15 - LD DT, Vx
  //Set delay timer = Vx.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x15){
    uint8_t x = chip328Memory[PC]&0x0F;
    DT = V[x];
    PC+=2;
    return;
  }
  //Cxkk - RND Vx, byte
  //Set Vx = random byte AND kk.
  if(chip328Memory[PC]>>4==0xC){
    uint8_t x = chip328Memory[PC]&0x0F;
    V[x] = chip328Memory[PC+1]&&(rand() % 256);
    PC+=2;
    return;
  }
  //PARCIALMENTE IMPLEMENTADA
  //ExA1 - SKNP Vx
  //Skip next instruction if key with the value of Vx is not pressed.
  if(chip328Memory[PC]>>4==0xE && chip328Memory[PC+1]==0xA1){
    uint8_t x = chip328Memory[PC]&0x0F;
    if(1){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //8xy2 - AND Vx, Vy
  //Set Vx = Vx AND Vy.
  if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x2){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC+1]>>4;
    V[x] = V[x]&V[y];
    PC+=2;
    return;
  }
  //8xy3 - XOR Vx, Vy
  //Set Vx = Vx XOR Vy.
  if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x3){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC+1]>>4;
    V[x] = V[x]^V[y];
    PC+=2;
    return;
  }
  //PODE CONTER ERROS <<<<<<<<<
  //8xy4 - ADD Vx, Vy
  //Set Vx = Vx + Vy set VF = carry.
  if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x4){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC+1]>>4;
    if((V[x]+V[y])>0xFF) V[15]=0x01;
    V[x] = V[x]+V[y];
    PC+=2;
    return;
  }
  //4xkk - SNE Vx, byte
  //Skip next instruction if Vx != kk.
  if(chip328Memory[PC]>>4==0x4){
    if(V[chip328Memory[PC]&0x0F]!=chip328Memory[PC+1]){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx0A - LD Vx, K
  //Wait for a key press, store the value of the key in Vx.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x0A){
    PC=PC+2;
    return;
  }
  //8xy7 - SUBN Vx, Vy
  //Set Vx = Vy - Vx, set VF = NOT borrow.
  if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x7){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC+1]>>4;
    if(V[y]>V[x]){
      V[15]=0x01;
    }else{
      V[15]=0x00;
    }
    V[x] = V[x]-V[y];
    PC+=2;
    return;
  }
  //8xy0 - LD Vx, Vy
  //Set Vx = Vy.
  if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x0){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC+1]>>4;
    V[x] = V[y];
    PC+=2;
    return;
  }
  //NÃO IMPLEMENTADA????
  //8xy6 - SHR Vx {, Vy}
  //Set Vx = Vx SHR 1.
  //If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
  /*if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x6){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC]>>4;
    if((V[y]&0b10000000)==0x10000000){
      V[15]=0x01;
    }else{
      V[15]=0x00;
    }
    V[y]=V[y]>>1;
    V[x]=V[y];
    PC+=2;
    return;
  }*/
  //8xy5 - SUB Vx, Vy
  //Set Vx = Vx - Vy, set VF = NOT borrow.
  if(chip328Memory[PC]>>4==0x8 && (chip328Memory[PC+1]&0x0F)==0x5){
    uint8_t x = chip328Memory[PC]&0x0F;
    uint8_t y = chip328Memory[PC+1]>>4;
    if(V[x]>V[y]){
      V[15]=0x01;
    }else{
      V[15]=0x00;
    }
    V[x] = V[x]-V[y];
    PC+=2;
    return;
  }
  //Fx18 - LD ST, Vx
  //Set sound timer = Vx.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x18){
    uint8_t x = chip328Memory[PC]&0x0F;
    ST = V[x];
    PC+=2;
    return;
  }
  //PARCIALMENTE IMPLEMENTADO
  //Ex9E - SKP Vx
  //Skip next instruction if key with the value of Vx is pressed.
  if(chip328Memory[PC]>>4==0xE && chip328Memory[PC+1]==0x9E){
    uint8_t x = chip328Memory[PC]&0x0F;
    if(1){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx55 - LD [I], Vx
  //Store registers V0 through Vx in memory starting at location I.
  if(chip328Memory[PC]>>4==0xF && chip328Memory[PC+1]==0x55){
    uint8_t i;
    uint8_t x = chip328Memory[PC]&0x0F;
    for(i=0;i<=x;i++){
      chip328Memory[I+i]=V[i];
    }
    PC+=2;
    return;
  }
}
