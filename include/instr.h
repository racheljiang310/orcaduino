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
void east(); // DONE
void north();
void south();
void west();
void comment();

void konkat(char len);
void jumper(char val);
void jymper(char val);

void add(char a, char b); // DONE
void sub(char a, char b); // DONE
void multiply(char a, char b);

void lesser(char a, char b);
void branch_if(char a, char b); // DONE

void clock(char rate=0, char mod=8); // DONE
void delay_b(char rate, char mod); // DONE

void random(char min, char max);
void uclid(char step, char max);
void lerp(char rate, char target);

void increment(char step, char mod);
char variable(char write, char read);

void track(char key, char len, char val);
void push(char len, char key, char val);

void read(char x, char y, char read);
void query(char x, char y, char len);
void write(char x, char y, char val);
void generator(char x_v, char y_v, char len); // DONE

/***** All things SPECIAL defined here ************************(******/

void self(char cmd);
void midi(char ch, char oct, char note, char velocity);
void midi_cc(char ch, char knob, char val);
// void pitch(char oct, char note);
// void byte(char high, char low);
// void play(char ch, char oct, char note, char velocity);

#endif