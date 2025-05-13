#ifndef INSTR_H
#define INSTR_H
/**********************************************************************
 * instr.h 
 * 
 * This header will contain all function prototypes for the operations
 * aka our instruction set. More descriptions will be provided in the
 * function definitions.
**********************************************************************/
#include "config.h"

/***** All things BASIC defined here *********************************/
void bang();
void halt();
void east();
void north();
void south();
void west();
void comment(); // worry about this later

void konkat(char len);
void jxmper(char val);
void jymper(char val);

void add(char a, char b);
void sub(char a, char b);
void multiply(char a, char b); 

void lesser(char a, char b); 
void branch_if(char a, char b); 

void clock(char rate=0, char mod=8); 
void delay_b(char rate, char mod); 

void rando(char min, char max); 
void uclid(char step, char max); // we'll handle this later...
void lerp(char rate, char target); 

void increment(char step, char mod);
void variable(char name, char value); 

void track(char idx, char len, char val); 
void push(char key, char len, char val); 

void read(char row, char col); 
void query(char row, char col, char len); 
void write(char off_x, char off_y, char val); 
void generator(char x_v, char y_v, char len); 

/***** All things SPECIAL defined here *******************************/

void self(char cmd);
void midi(char ch, char oct, char note, char velocity);
void midi_cc(char ch, char knob, char val);
// void pitch(char oct, char note);
// void byte(char high, char low);
// void play(char ch, char oct, char note, char velocity);

#endif