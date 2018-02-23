//#include <windows.h>  // for MS Windows
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>  // GLUT, include glu.h and gl.h

int i = 0;
int cycleCount = 0;

//chip8 register, ram memory and video memory
uint8_t chip8Memory[4096];
uint8_t chip8Display[8][32];
uint8_t V[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t I = 0x0000;
uint16_t PC = 0x0000;
uint16_t SP = 0x0000;
uint8_t DT = 0;
uint8_t ST = 0;

int refreshMillis = 10;

void chip8Begin(){
//Dxyn
  PC = 0x0200;
  I = 0x0000;
  SP = 0X0000;

  int i;
  uint8_t temp[280];
  FILE *rom;
  rom = fopen("BRIX", "r");
  int ab;
  for(ab=0;ab<280;ab++){
    fread(&temp,280,1,rom);
  }
  fclose(rom);
  for(i=0;i<280;i++){
    chip8Memory[i+512]=temp[i];
  }
  //0
  chip8Memory[0] = 0xF0;
  chip8Memory[1] = 0x90;
  chip8Memory[2] = 0x90;
  chip8Memory[3] = 0x90;
  chip8Memory[4] = 0xF0;
  //1
  chip8Memory[5] = 0x20;
  chip8Memory[6] = 0x60;
  chip8Memory[7] = 0x20;
  chip8Memory[8] = 0x20;
  chip8Memory[9] = 0x70;
  //2
  chip8Memory[10] = 0xF0;
  chip8Memory[11] = 0x10;
  chip8Memory[12] = 0xF0;
  chip8Memory[13] = 0x80;
  chip8Memory[14] = 0xF0;
  //3
  chip8Memory[15] = 0xF0;
  chip8Memory[16] = 0x10;
  chip8Memory[17] = 0xF0;
  chip8Memory[18] = 0x10;
  chip8Memory[19] = 0xF0;
  //4
  chip8Memory[20] = 0x90;
  chip8Memory[21] = 0x90;
  chip8Memory[22] = 0xF0;
  chip8Memory[23] = 0x10;
  chip8Memory[24] = 0x10;
  //5
  chip8Memory[25] = 0xF0;
  chip8Memory[26] = 0x80;
  chip8Memory[27] = 0xF0;
  chip8Memory[28] = 0x10;
  chip8Memory[29] = 0xF0;
  //6
  chip8Memory[30] = 0xF0;
  chip8Memory[31] = 0x80;
  chip8Memory[32] = 0xF0;
  chip8Memory[33] = 0x90;
  chip8Memory[34] = 0xF0;
  //7
  chip8Memory[35] = 0xF0;
  chip8Memory[36] = 0x10;
  chip8Memory[37] = 0x20;
  chip8Memory[38] = 0x40;
  chip8Memory[39] = 0x40;
  //8
  chip8Memory[40] = 0xF0;
  chip8Memory[41] = 0x90;
  chip8Memory[42] = 0xF0;
  chip8Memory[43] = 0x90;
  chip8Memory[44] = 0xF0;
  //9
  chip8Memory[45] = 0xF0;
  chip8Memory[46] = 0x90;
  chip8Memory[47] = 0xF0;
  chip8Memory[48] = 0x10;
  chip8Memory[49] = 0xF0;
  //A
  chip8Memory[50] = 0xF0;
  chip8Memory[51] = 0x90;
  chip8Memory[52] = 0xF0;
  chip8Memory[53] = 0x90;
  chip8Memory[54] = 0x90;
  //B
  chip8Memory[55] = 0xE0;
  chip8Memory[56] = 0x90;
  chip8Memory[57] = 0xE0;
  chip8Memory[58] = 0x90;
  chip8Memory[59] = 0xE0;
  //C
  chip8Memory[60] = 0xF0;
  chip8Memory[61] = 0x80;
  chip8Memory[62] = 0x80;
  chip8Memory[63] = 0x80;
  chip8Memory[64] = 0xF0;
  //D
  chip8Memory[65] = 0xE0;
  chip8Memory[66] = 0x90;
  chip8Memory[67] = 0x90;
  chip8Memory[68] = 0x90;
  chip8Memory[69] = 0xE0;
  //E
  chip8Memory[70] = 0xF0;
  chip8Memory[71] = 0x80;
  chip8Memory[72] = 0xF0;
  chip8Memory[73] = 0x80;
  chip8Memory[74] = 0xF0;
  //F
  chip8Memory[75] = 0xF0;
  chip8Memory[76] = 0x80;
  chip8Memory[77] = 0xF0;
  chip8Memory[78] = 0x80;
  chip8Memory[79] = 0x80;

}

void putPixel(int x, int y){
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  glVertex2f(-1+0.03125*x, 1-0.0625*(y+1));
  glVertex2f(-1+0.03125*(x+1), 1-0.0625*(y+1));
  glVertex2f(-1+0.03125*(x+1), 1-0.0625*y);
  glVertex2f(-1+0.03125*x, 1-0.0625*y);
  glEnd();
}

void chip8Debug(){
  printf("Ciclo: %d\n\n",cycleCount);
  printf("PC: %x PC_Value: %x %x SP: %x I: %x\n\n",PC,chip8Memory[PC],chip8Memory[PC+1],SP,I);
  printf("V0:%x V1:%x V2:%x V3:%x V4:%x V5:%x V6:%x V7:%x\n\n",V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]);
  printf("V8:%x V9:%x VA:%x VB:%x VC:%x VD:%x VE:%x VF:%x\n\n",V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]);
  for(int i = 4080;i<4096;i++){
    printf("%x ",chip8Memory[i]);
  }
  printf("\n\n");
  for(int i = 1812;i<2000;i++){
    printf("%d:0x%x ",i,chip8Memory[i]);
  }
  printf("\n");
  cycleCount++;
  //if(cycleCount>885) getchar();
}

uint8_t chip8PutDisplay(uint8_t x, uint8_t y, uint8_t pixel){
  uint8_t offsetByte=x/8;
  uint8_t offsetBit=x%8;
  uint8_t collision = 0;

  if(pixel){
    if(chip8Display[offsetByte][y]&(0b10000000>>offsetBit)) collision = 1;
    chip8Display[offsetByte][y] = chip8Display[offsetByte][y]^(0b10000000>>offsetBit);
    //chip8Display[offsetByte][y] = chip8Display[offsetByte][y]^(0b00000000>>offsetBit);
    /*chip8Display[offsetByte][y] = 1^0=1
                                  0^0=0
                                  0101^0000=0101
                                  0001^0000=0001*/
    //chip8Display[offsetByte][y] = chip8Display[offsetByte][y]&~(0b10000000>>offsetBit);
  //}else{
    //if(chip8Display[offsetByte][y]&(0b10000000>>offsetBit)) collision = 1;
    //chip8Display[offsetByte][y] = chip8Display[offsetByte][y]^(0b10000000>>offsetBit);
  }
  return collision;
}

void chip8Emulate(){
  //Dxyn - DRW Vx, Vy, nibble
  //Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
  if(chip8Memory[PC]>>4 == 0xD){
    uint8_t i,j, offsetByte=0, offsetBit=0;;
    uint8_t x = V[chip8Memory[PC]&0x0F];
    uint8_t y = V[chip8Memory[PC+1]>>4];
    V[15]=0x00;
    for(i=0;i<(chip8Memory[PC+1]&0x0F);i++){
      for(j=0;j<8;j++){
        if(chip8PutDisplay(x+j,y+i,chip8Memory[I+i]&(0b10000000>>j))){
          V[15]=0x01;
        }
      }
    }
    PC=PC+2;
  }
  //JP
  if(chip8Memory[PC]>>4 == 0x1){
    PC = ((chip8Memory[PC]<<8) + chip8Memory[PC+1])&0x0FFF;
    return;
  }
  //7xkk - ADD Vx, byte
  //Set Vx = Vx + kk.
  if(chip8Memory[PC]>>4 == 0x7){
    V[chip8Memory[PC]&0b00001111] = V[chip8Memory[PC]&0b00001111] + chip8Memory[PC+1];
    PC=PC+2;
    return;
  }
  //0nnn - SYS addr
  if(chip8Memory[PC]>>4 == 0x0 && chip8Memory[PC+1]>>4 != 0xE){
    PC=PC+2;
    return;
  }
  //00E0 - CLS
  if(chip8Memory[PC] == 0x00 && chip8Memory[PC+1] == 0xE0){
    uint8_t i,j;
    for(i=0;i<8;i++){
      for(j=0;j<32;j++){
        chip8Display[i][j]=0x00;
      }
    }
    PC=PC+2;
    return;
  }
  //00EE - RET
  if(chip8Memory[PC] == 0x00 && chip8Memory[PC+1] == 0xEE){
    PC = ((chip8Memory[SP]<<8) + chip8Memory[SP+1])&0x0FFF;
    if(SP==0x0FFE){
      SP=0x0000;
    }else{
      SP=SP+2;
    }
    PC=PC+2;
    return;
  }
  //2nnn - CALL addr
  if(chip8Memory[PC]>>4 == 0x2){
    if(SP==0x0000){
      SP=0x0FFE;
    }else{
      SP=SP-2;
    }
    chip8Memory[SP]=PC>>8;
    chip8Memory[SP+1]=PC&0x00FF;
    PC=((chip8Memory[PC]<<8) + chip8Memory[PC+1])&0x0FFF;
    return;
  }
  //6xkk - LD Vx, byte
  //Set Vx = kk
  if(chip8Memory[PC]>>4==0x6){
    V[chip8Memory[PC]&0x0F]=chip8Memory[PC+1];
    PC+=2;
    return;
  }
  //Annn - LD I, addr
  //Set I = nnn
  if(chip8Memory[PC]>>4==0xA){
    I = ((chip8Memory[PC]<<8) + chip8Memory[PC+1])&0x0FFF;
    PC+=2;
    return;
  }
  //3xkk - SE Vx, byte
  //Skip next instruction if Vx = kk.
  if(chip8Memory[PC]>>4==0x3){
    if(V[chip8Memory[PC]&0x0F]==chip8Memory[PC+1]){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx07 - LD Vx, DT
  //Set Vx = delay timer value.
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x07){
    uint8_t x = chip8Memory[PC]&0x0F;
    V[x] = DT;
    PC+=2;
    return;
  }
  //Contém erros
  //Fx33 - LD B, Vx
  //Store BCD representation of Vx in memory locations I, I+1, and I+2.
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x33){
    chip8Memory[I]=V[chip8Memory[PC]&0x0F]/100;
    chip8Memory[I+1]=V[chip8Memory[PC]&0x0F]/10;
    chip8Memory[I+2]=V[chip8Memory[PC]&0x0F];
    PC+=2;
    return;
  }
  //Fx65 - LD Vx, [I]
  //Read registers V0 through Vx from memory starting at location I.
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x65){
    uint8_t i;
    uint8_t x = chip8Memory[PC]&0x0F;
    for(i=0;i<=x;i++){
      V[i]=chip8Memory[I+i];
    }
    PC+=2;
    return;
  }
  //Fx29 - LD F, Vx
  //Set I = location of sprite for digit Vx.
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x29){
    uint8_t i;
    uint8_t x = V[chip8Memory[PC]&0x0F];
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
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x1E){
    uint8_t i;
    uint8_t x = chip8Memory[PC]&0x0F;
    I = I + V[x];
    PC+=2;
    return;
  }
  //Fx15 - LD DT, Vx
  //Set delay timer = Vx.
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x15){
    uint8_t x = chip8Memory[PC]&0x0F;
    DT = V[x];
    PC+=2;
    return;
  }
  //Cxkk - RND Vx, byte
  //Set Vx = random byte AND kk.
  if(chip8Memory[PC]>>4==0xC){
    uint8_t x = chip8Memory[PC]&0x0F;
    V[x] = chip8Memory[PC+1]&&(rand() % 256);
    PC+=2;
    return;
  }
  //PARCIALMENTE IMPLEMENTADA
  //ExA1 - SKNP Vx
  //Skip next instruction if key with the value of Vx is not pressed.
  if(chip8Memory[PC]>>4==0xE && chip8Memory[PC+1]==0xA1){
    uint8_t x = chip8Memory[PC]&0x0F;
    if(1){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //8xy2 - AND Vx, Vy
  //Set Vx = Vx AND Vy.
  if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x2){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC+1]>>4;
    V[x] = V[x]&V[y];
    PC+=2;
    return;
  }
  //8xy3 - XOR Vx, Vy
  //Set Vx = Vx XOR Vy.
  if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x3){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC+1]>>4;
    V[x] = V[x]^V[y];
    PC+=2;
    return;
  }
  //PODE CONTER ERROS <<<<<<<<<
  //8xy4 - ADD Vx, Vy
  //Set Vx = Vx + Vy set VF = carry.
  if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x4){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC+1]>>4;
    if((V[x]+V[y])>0xFF) V[15]=0x01;
    V[x] = V[x]+V[y];
    PC+=2;
    return;
  }
  //4xkk - SNE Vx, byte
  //Skip next instruction if Vx != kk.
  if(chip8Memory[PC]>>4==0x4){
    if(V[chip8Memory[PC]&0x0F]!=chip8Memory[PC+1]){
      PC+=4;
    }else{
      PC+=2;
    }
    return;
  }
  //Fx0A - LD Vx, K
  //Wait for a key press, store the value of the key in Vx.
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x0A){
    PC=PC+2;
    return;
  }
  //8xy7 - SUBN Vx, Vy
  //Set Vx = Vy - Vx, set VF = NOT borrow.
  if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x7){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC+1]>>4;
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
  if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x0){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC+1]>>4;
    V[x] = V[y];
    PC+=2;
    return;
  }
  //NÃO IMPLEMENTADA????
  //8xy6 - SHR Vx {, Vy}
  //Set Vx = Vx SHR 1.
  //If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
  /*if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x6){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC]>>4;
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
  if(chip8Memory[PC]>>4==0x8 && (chip8Memory[PC+1]&0x0F)==0x5){
    uint8_t x = chip8Memory[PC]&0x0F;
    uint8_t y = chip8Memory[PC+1]>>4;
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
  if(chip8Memory[PC]>>4==0xF && chip8Memory[PC+1]==0x18){
    uint8_t x = chip8Memory[PC]&0x0F;
    ST = V[x];
    PC+=2;
    return;
  }
}

void chip8WriteDisplay(){
  int i=0,j=0;
  for(j=0;j<32;j++){
    for(i=0;i<8;i++){
      if((chip8Display[i][j]>>7)&0b00000001) putPixel(0+i*8,j);
      if((chip8Display[i][j]>>6)&0b00000001) putPixel(1+i*8,j);
      if((chip8Display[i][j]>>5)&0b00000001) putPixel(2+i*8,j);
      if((chip8Display[i][j]>>4)&0b00000001) putPixel(3+i*8,j);
      if((chip8Display[i][j]>>3)&0b00000001) putPixel(4+i*8,j);
      if((chip8Display[i][j]>>2)&0b00000001) putPixel(5+i*8,j);
      if((chip8Display[i][j]>>1)&0b00000001) putPixel(6+i*8,j);
      if((chip8Display[i][j])&0b00000001)    putPixel(7+i*8,j);
    }
  }
}

void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
  glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
  chip8WriteDisplay();
  glFlush();
}

void chip8(int value){
    chip8Debug();
    chip8Emulate();
    chip8WriteDisplay();
    glutPostRedisplay();    // Post a paint request to activate display()
    glutTimerFunc(refreshMillis, chip8, 0); // subsequent timer call at milliseconds
}

void chip8DelayTimer(int value){
    if(DT!=0) DT--;
    glutTimerFunc(17, chip8DelayTimer, 0);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
  chip8Begin();
   glutInit(&argc, argv);                 // Initialize GLUT
   glutInitWindowSize(960, 480);   // Set the window's initial width & height
   glutCreateWindow("Chip8 Alive Emulator"); // Create a window with the given title
   glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
   glutDisplayFunc(display); // Register display callback handler for window re-paint
   glutTimerFunc(0, chip8, 0);
   glutTimerFunc(0, chip8DelayTimer, 0);
   glutMainLoop();           // Enter the event-processing loop
   return 0;
}
