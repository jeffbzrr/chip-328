/*
* gcc chip328.c chip328lib.c -o chip328 -lglut -lGL -lGLU -I/chip328.c
*/

//#include <windows.h>  // for MS Windows

#if !defined(__AVR_ATmega328P__)

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "chip328lib.h"

int cycleCount = 0;
int refreshMillis = 1;

//Main function. Inicia e configura o openGL
void interfaceDelayTimer(int value){
  if(DT!=0) DT--;
  glutTimerFunc(17, interfaceDelayTimer, 0);
}

void interfaceLoadROM(){
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
    chip328Memory[i]=temp[i];
  }
}

void interfaceDebug(){
  printf("Ciclo: %d\n\n",cycleCount);
  printf("PC: %x PC_Value: %x %x SP_: %x I: %x\n\n",PC,chip328Memory[PC],chip328Memory[PC+1],SP_,I);
  printf("V0:%x V1:%x V2:%x V3:%x V4:%x V5:%x V6:%x V7:%x\n\n",V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]);
  printf("V8:%x V9:%x VA:%x VB:%x VC:%x VD:%x VE:%x VF:%x\n\n",V[8],V[9],V[10],V[11],V[12],V[13],V[14],V[15]);
  //for(int i = 4080;i<4096;i++){
  //  printf("%x ",chip328Memory[i]);
  //}
  /*printf("\n\n");
  for(int i = 1812;i<2000;i++){
    printf("%d:0x%x ",i,chip328Memory[i]);
  }*/
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  cycleCount++;
  //if(cycleCount>885) getchar();
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

void interfaceDisplayWrite(){
  int i=0,j=0;
  for(j=0;j<32;j++){
    for(i=0;i<8;i++){
      if((chip328Display[i][j]>>7)&0b00000001) putPixel(0+i*8,j);
      if((chip328Display[i][j]>>6)&0b00000001) putPixel(1+i*8,j);
      if((chip328Display[i][j]>>5)&0b00000001) putPixel(2+i*8,j);
      if((chip328Display[i][j]>>4)&0b00000001) putPixel(3+i*8,j);
      if((chip328Display[i][j]>>3)&0b00000001) putPixel(4+i*8,j);
      if((chip328Display[i][j]>>2)&0b00000001) putPixel(5+i*8,j);
      if((chip328Display[i][j]>>1)&0b00000001) putPixel(6+i*8,j);
      if((chip328Display[i][j])&0b00000001)    putPixel(7+i*8,j);
    }
  }
}

void mainLoop(int value) {
  interfaceDebug();
  chip328Emulate();
  interfaceDisplayWrite();
  glutPostRedisplay();    // Post a paint request to activate display()
  glutTimerFunc(refreshMillis, mainLoop, 0); // subsequent timer call at milliseconds
}

void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  interfaceDisplayWrite();
  glFlush();
}

int main(int argc, char** argv) {
  chip328Begin();
  interfaceLoadROM();
  glutInit(&argc, argv);
  glutInitWindowSize(960, 480);
  glutCreateWindow("Chip-328 Emulator");
  glutInitWindowPosition(0, 0);
  glutDisplayFunc(display); // Register display callback handler for window re-paint
  glutTimerFunc(0, mainLoop, 0);
  glutTimerFunc(0, interfaceDelayTimer, 0);
  glutMainLoop(); // Enter the event-processing loop
  return 0;
}

#endif
