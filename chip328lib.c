#include "chip328lib.h"

uint8_t V[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t PC, SP_, I;
uint8_t DT, ST;

uint8_t chip328MemStack[16];
uint8_t chip328Memory[300];
uint8_t chip328MemSys[80] =
{
   0xF0,
   0x90,
   0x90,
   0x90,
   0xF0,
  //1
   0x20,
   0x60,
   0x20,
   0x20,
   0x70,
  //2
   0xF0,
   0x10,
   0xF0,
   0x80,
   0xF0,
  //3
   0xF0,
   0x10,
   0xF0,
   0x10,
   0xF0,
  //4
   0x90,
   0x90,
   0xF0,
   0x10,
   0x10,
  //5
   0xF0,
   0x80,
   0xF0,
   0x10,
   0xF0,
  //6
   0xF0,
   0x80,
   0xF0,
   0x90,
   0xF0,
  //7
   0xF0,
   0x10,
   0x20,
   0x40,
   0x40,
  //8
   0xF0,
   0x90,
   0xF0,
   0x90,
   0xF0,
  //9
   0xF0,
   0x90,
   0xF0,
   0x10,
   0xF0,
  //A
   0xF0,
   0x90,
   0xF0,
   0x90,
   0x90,
  //B
   0xE0,
   0x90,
   0xE0,
   0x90,
   0xE0,
  //C
   0xF0,
   0x80,
   0x80,
   0x80,
   0xF0,
  //D
   0xE0,
   0x90,
   0x90,
   0x90,
   0xE0,
  //E
   0xF0,
   0x80,
   0xF0,
   0x80,
   0xF0,
  //F
   0xF0,
   0x80,
   0xF0,
   0x80,
   0x80
};

void chip328Begin(){
  PC = 0x0200;
  I = 0x0000;
  SP_ = 0X0000;
  DT = 0;
  ST = 0;
}

uint8_t chip328MemoryRead(uint16_t address){
  if(address<512) return(chip328MemSys[address]);
  if(address>=4080) return(chip328MemStack[address-4080]);
  return(chip328Memory[address-512]);
}

void chip328MemoryWrite(uint16_t address, uint8_t data){
   if(address>=4080) { chip328MemStack[address-4080]=data; return; }
   chip328Memory[address-512]=data;
}

void chip328Emulate(){
  //Dxyn - DRW Vx, Vy, nibble
  //Display n-byte SP_rite starting at memory location I at (Vx, Vy), set VF = collision.
  if(chip328MemoryRead(PC)>>4 == 0xD){
    uint8_t i,j, offsetByte=0, offsetBit=0;;
    uint8_t x = V[chip328MemoryRead(PC)&0x0F];
    uint8_t y = V[chip328MemoryRead(PC+1)>>4];
    V[15]=0x00;
    for(i=0;i<(chip328MemoryRead(PC+1)&0x0F);i++){
      for(j=0;j<8;j++){
        if(interfacePutPixel(x+j,y+i,chip328MemoryRead(I+i)&(0b10000000>>j))){
          V[15]=0x01;
        }
      }
    }
    PC=PC+2;
  }
  //JP
  if(chip328MemoryRead(PC)>>4 == 0x1){
    PC = ((chip328MemoryRead(PC)<<8) + chip328MemoryRead(PC+1))&0x0FFF;
    return;
  }
  //7xkk - ADD Vx, byte
  //Set Vx = Vx + kk.
  if(chip328MemoryRead(PC)>>4 == 0x7){
    V[chip328MemoryRead(PC)&0b00001111] = V[chip328MemoryRead(PC)&0b00001111] + chip328MemoryRead(PC+1);
    PC=PC+2;
    return;
  }
  //0nnn - SYS addr
  if(chip328MemoryRead(PC)>>4 == 0x0 && chip328MemoryRead(PC+1)>>4 != 0xE){
    PC=PC+2;
    return;
  }
  //00E0 - CLS
  if(chip328MemoryRead(PC) == 0x00 && chip328MemoryRead(PC+1) == 0xE0){
    uint8_t i,j;
    #if defined(__AVR_ATmega328P__)
      for(i=0;i<256;i++){
        chip328Display[i]=0x00;
      }
    #else
      for(i=0;i<8;i++){
        for(j=0;j<32;j++){
          chip328Display[i][j]=0x00;
        }
      }
    #endif
    PC=PC+2;
    return;
  }
  //00EE - RET
  if(chip328MemoryRead(PC) == 0x00 && chip328MemoryRead(PC+1) == 0xEE){
    PC = ((chip328MemoryRead(SP_)<<8) + chip328MemoryRead(SP_+1))&0x0FFF;
    //if(SP_==0x03FE){
    if(SP_==4094) {
      SP_=0x0000;
    }else{
      SP_=SP_+2;
    }
    PC=PC+2;
    return;
  }
  //2nnn - CALL addr
  if(chip328MemoryRead(PC)>>4 == 0x2){
    /*if(SP_==0x0000){
      //SP_=0x03F3;
      SP_=798;
    }else{
      SP_=SP_-2;
    }*/
    if(SP_==0x0000){
      //SP_=0x03F3;
      SP_=4094;
    }else{
      SP_=SP_-2;
    }
    chip328MemoryWrite(SP_, PC>>8);
    chip328MemoryWrite(SP_+1, PC&0x00FF);
    PC=((chip328MemoryRead(PC)<<8) + chip328MemoryRead(PC+1))&0x0FFF;
    return;
  }
  //6xkk - LD Vx, byte
  //Set Vx = kk
  if(chip328MemoryRead(PC)>>4==0x6){
    V[chip328MemoryRead(PC)&0x0F]=chip328MemoryRead(PC+1);
    PC+=2;
    return;
  }
  //Annn - LD I, addr
  //Set I = nnn
  if(chip328MemoryRead(PC)>>4==0xA){
    I = ((chip328MemoryRead(PC)<<8) + chip328MemoryRead(PC+1))&0x0FFF;
    PC+=2;
    return;
  }
  //3xkk - SE Vx, byte
  //Skip next instruction if Vx = kk.
  if(chip328MemoryRead(PC)>>4==0x3){
    if(V[chip328MemoryRead(PC)&0x0F]==chip328MemoryRead(PC+1)){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx07 - LD Vx, DT
  //Set Vx = delay timer value.
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x07){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    V[x] = DT;
    PC+=2;
    return;
  }
  //Contém erros
  //Fx33 - LD B, Vx
  //Store BCD representation of Vx in memory locations I, I+1, and I+2.
  //Por exemplo o número 128 segue a seguinte ordem, 1->[I], 2->[I+1], 8->[I+2]
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x33){
    uint8_t n = V[chip328MemoryRead(PC)&0x0F];
    chip328MemoryWrite(I+2,n%10);
    n=n/10;
    chip328MemoryWrite(I+1,n%10);
    n=n/10;
    chip328MemoryWrite(I, n%10);
    PC+=2;
    return;
  }
  //Fx65 - LD Vx, [I]
  //Read registers V0 through Vx from memory starting at location I.
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x65){
    uint8_t i;
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    for(i=0;i<=x;i++){
      V[i]=chip328MemoryRead(I+i);
    }
    PC+=2;
    return;
  }
  //Fx29 - LD F, Vx
  //Set I = location of SP_rite for digit Vx.
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x29){
    uint8_t i;
    uint8_t x = V[chip328MemoryRead(PC)&0x0F];
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
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x1E){
    uint8_t i;
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    I = I + V[x];
    PC+=2;
    return;
  }
  //Fx15 - LD DT, Vx
  //Set delay timer = Vx.
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x15){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    DT = V[x];
    PC+=2;
    return;
  }
  //Cxkk - RND Vx, byte
  //Set Vx = random byte AND kk.
  if(chip328MemoryRead(PC)>>4==0xC){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    V[x] = chip328MemoryRead(PC+1)&&(rand() % 256);
    PC+=2;
    return;
  }
  //PARCIALMENTE IMPLEMENTADA
  //ExA1 - SKNP Vx
  //Skip next instruction if key with the value of Vx is not pressed.
  if(chip328MemoryRead(PC)>>4==0xE && chip328MemoryRead(PC+1)==0xA1){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    if(1){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //8xy2 - AND Vx, Vy
  //Set Vx = Vx AND Vy.
  if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x2){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC+1)>>4;
    V[x] = V[x]&V[y];
    PC+=2;
    return;
  }
  //8xy3 - XOR Vx, Vy
  //Set Vx = Vx XOR Vy.
  if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x3){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC+1)>>4;
    V[x] = V[x]^V[y];
    PC+=2;
    return;
  }
  //PODE CONTER ERROS <<<<<<<<<
  //8xy4 - ADD Vx, Vy
  //Set Vx = Vx + Vy set VF = carry.
  if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x4){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC+1)>>4;
    if((V[x]+V[y])>0xFF) V[15]=0x01;
    V[x] = V[x]+V[y];
    PC+=2;
    return;
  }
  //4xkk - SNE Vx, byte
  //Skip next instruction if Vx != kk.
  if(chip328MemoryRead(PC)>>4==0x4){
    if(V[chip328MemoryRead(PC)&0x0F]!=chip328MemoryRead(PC+1)){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx0A - LD Vx, K
  //Wait for a key press, store the value of the key in Vx.
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x0A){
    PC=PC+2;
    return;
  }
  //8xy7 - SUBN Vx, Vy
  //Set Vx = Vy - Vx, set VF = NOT borrow.
  if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x7){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC+1)>>4;
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
  if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x0){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC+1)>>4;
    V[x] = V[y];
    PC+=2;
    return;
  }
  //NÃO IMPLEMENTADA????
  //8xy6 - SHR Vx {, Vy}
  //Set Vx = Vx SHR 1.
  //If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
  /*if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x6){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC)>>4;
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
  if(chip328MemoryRead(PC)>>4==0x8 && (chip328MemoryRead(PC+1)&0x0F)==0x5){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    uint8_t y = chip328MemoryRead(PC+1)>>4;
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
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x18){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    ST = V[x];
    PC+=2;
    return;
  }
  //PARCIALMENTE IMPLEMENTADO
  //Ex9E - SKP Vx
  //Skip next instruction if key with the value of Vx is pressed.
  if(chip328MemoryRead(PC)>>4==0xE && chip328MemoryRead(PC+1)==0x9E){
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    if(1){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx55 - LD [I], Vx
  //Store registers V0 through Vx in memory starting at location I.
  if(chip328MemoryRead(PC)>>4==0xF && chip328MemoryRead(PC+1)==0x55){
    uint8_t i;
    uint8_t x = chip328MemoryRead(PC)&0x0F;
    for(i=0;i<=x;i++){
      chip328MemoryWrite(I+i, V[i]);
    }
    PC+=2;
    return;
  }
}
