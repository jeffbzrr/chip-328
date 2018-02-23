void chip8Begin(){
//Dxyn
  PC = 0x0200;
  I = 0x0000;
  SP = 0X0000;
  DT = 0;
  ST = 0;
  interfaceLoadROM();
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
