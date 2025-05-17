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
void bang(uint8_t index);                           // TO TEST
void halt();                                        // TO TEST
void east(uint8_t i, uint8_t j);                    // TO TEST
void north(uint8_t i, uint8_t j);                   // TO TEST
void south(uint8_t i, uint8_t j);                   // TO TEST
void west(uint8_t i, uint8_t j);                    // TO TEST

void konkat(uint8_t len);
void add(uint8_t i, uint8_t j);                     // TO TEST
void sub(uint8_t i, uint8_t j);                     // TO TEST
void jxmper(uint8_t i, uint8_t j);                  // TO TEST
void jymper(uint8_t i, uint8_t j);                  // TO TEST
void multiply(uint8_t i, uint8_t j);                // TO TEST

void lerp(uint8_t i, uint8_t j);                    // TO TEST
void clock(uint8_t i, uint8_t j);                   // TO TEST
void delay_b(uint8_t i, uint8_t j);                 // TO TEST
void increment();

void lesser(uint8_t i, uint8_t j);                  // TO TEST
void branch_if(uint8_t i, uint8_t j);               // TO TEST
void rando(uint8_t minx, uint8_t maxx);             // TO IMPLEMENT


void variable(uint8_t name, uint8_t value); 
void push(uint8_t key, uint8_t len, uint8_t val); 
void track(uint8_t idx, uint8_t len, uint8_t val); 

void read(uint8_t row, uint8_t col); 
void query(uint8_t row, uint8_t col, uint8_t len); 
void write(uint8_t off_x, uint8_t off_y, uint8_t val); 
void generator(uint8_t x_v, uint8_t y_v, uint8_t len); 

// void comment(); // worry about this later
// void uclid(char step, char max); => we'll handle this later...

/***** All things SPECIAL defined here *******************************/

// void self(char cmd);
// void midi(char ch, char oct, char note, char velocity);
// void midi_cc(char ch, char knob, char val);
// void pitch(char oct, char note);
// void byte(char high, char low);
// void play(char ch, char oct, char note, char velocity);

#endif