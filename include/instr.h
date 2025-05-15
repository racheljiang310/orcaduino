#ifndef INSTR_H
#define INSTR_H
/**********************************************************************
 * instr.h 
 * 
 * This header will contain all function prototypes for the operations
 * aka our instruction set. More descriptions will be provided in the
 * function definitions.
**********************************************************************/
#include "stdint.h"
/***** All things BASIC defined here *********************************/
void bang();
void halt();
void east();
void north();
void south();
void west();
void comment(); // worry about this later

void konkat(uint8_t len);
void jxmper(uint8_t val);
void jymper(uint8_t val);

void add(uint8_t a, uint8_t b);
void sub(uint8_t a, uint8_t b);
void multiply(uint8_t a, uint8_t b); 

void lesser(uint8_t a, uint8_t b); 
void branch_if(uint8_t a, uint8_t b); 

void clock(); 
void delay_b(); 

void rando(uint8_t minx, uint8_t maxx); 
// void uclid(char step, char max); => we'll handle this later...
void lerp(); 

void increment();
void variable(uint8_t name, uint8_t value); 
void track(uint8_t idx, uint8_t len, uint8_t val); 
void push(uint8_t key, uint8_t len, uint8_t val); 

void read(uint8_t row, uint8_t col); 
void query(uint8_t row, uint8_t col, uint8_t len); 
void write(uint8_t off_x, uint8_t off_y, uint8_t val); 
void generator(uint8_t x_v, uint8_t y_v, uint8_t len); 

/***** All things SPECIAL defined here *******************************/

void self(char cmd);
void midi(char ch, char oct, char note, char velocity);
void midi_cc(char ch, char knob, char val);
// void pitch(char oct, char note);
// void byte(char high, char low);
// void play(char ch, char oct, char note, char velocity);

#endif