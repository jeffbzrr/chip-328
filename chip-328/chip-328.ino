#include "U8glib.h"
#include "chip328lib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);


void interfaceDelayTimer(int value){
  //if(DT!=0) DT--;
  //glutTimerFunc(17, interfaceDelayTimer, 0);
}

void interfaceLoadROM(){
  
  chip328Memory[0x200] = 0x60;
  chip328Memory[0x201] = 0x03;
  chip328Memory[0x202] = 0x61;
  chip328Memory[0x203] = 0x0F;
  chip328Memory[0x204] = 0x62;
  chip328Memory[0x205] = 0x0F;
  chip328Memory[0x206] = 0xF0;
  chip328Memory[0x207] = 0x29;
  chip328Memory[0x208] = 0xD1;
  chip328Memory[0x209] = 0x25;
  chip328Memory[0x210] = 0x12;
  chip328Memory[0x211] = 0x00;
}

void interfaceDiSP_layWrite(){
  int i=0,j=0;
  for(j=0;j<32;j++){
    for(i=0;i<8;i++){
      if((chip328Display[i][j]>>7)&0b00000001) {
        u8g.drawPixel((0+i*8)*2,j*2);
        u8g.drawPixel((0+i*8)*2+1,j*2);
        u8g.drawPixel((0+i*8)*2,j*2+1);
        u8g.drawPixel((0+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j]>>6)&0b00000001) {
        u8g.drawPixel((1+i*8)*2,j*2);
        u8g.drawPixel((1+i*8)*2+1,j*2);
        u8g.drawPixel((1+i*8)*2,j*2+1);
        u8g.drawPixel((1+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j]>>5)&0b00000001) {
        u8g.drawPixel((2+i*8)*2,j*2);
        u8g.drawPixel((2+i*8)*2+1,j*2);
        u8g.drawPixel((2+i*8)*2,j*2+1);
        u8g.drawPixel((2+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j]>>4)&0b00000001) {
        u8g.drawPixel((3+i*8)*2,j*2);
        u8g.drawPixel((3+i*8)*2+1,j*2);
        u8g.drawPixel((3+i*8)*2,j*2+1);
        u8g.drawPixel((3+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j]>>3)&0b00000001) {
        u8g.drawPixel((4+i*8)*2,j*2);
        u8g.drawPixel((4+i*8)*2+1,j*2);
        u8g.drawPixel((4+i*8)*2,j*2+1);
        u8g.drawPixel((4+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j]>>2)&0b00000001) {
        u8g.drawPixel((5+i*8)*2,j*2);
        u8g.drawPixel((5+i*8)*2+1,j*2);
        u8g.drawPixel((5+i*8)*2,j*2+1);
        u8g.drawPixel((5+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j]>>1)&0b00000001) {
        u8g.drawPixel((6+i*8)*2,j*2);
        u8g.drawPixel((6+i*8)*2+1,j*2);
        u8g.drawPixel((6+i*8)*2,j*2+1);
        u8g.drawPixel((7+i*8)*2+1,j*2+1);
      }
      if((chip328Display[i][j])&0b00000001) {
        u8g.drawPixel((7+i*8)*2,j*2);
        u8g.drawPixel((7+i*8)*2+1,j*2);
        u8g.drawPixel((7+i*8)*2,j*2+1);
        u8g.drawPixel((7+i*8)*2+1,j*2+1);
      }
    }
  }
}

void setup(void) {
    Serial.begin(9600);
    u8g.setColorIndex(1);
    chip328Begin();
}

void loop(void) {
  chip328Emulate();
  //u8g.firstPage();  
  //do {
    interfaceDiSP_layWrite();
  //} while( u8g.nextPage() );
}

