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
void halt();                                        // PASS
void bang(uint8_t index);                           // PASS
void east(uint8_t i, uint8_t j);                    // PASS 
void north(uint8_t i, uint8_t j);                   // FUNCTIONAL (need to get rid of lag)
void south(uint8_t i, uint8_t j);                   // PASS
void west(uint8_t i, uint8_t j);                    // FUNCTIONAL (need to get rid of lag)
void add(uint8_t i, uint8_t j);                     // PASS
void sub(uint8_t i, uint8_t j);                     // PASS
void jxmper(uint8_t i, uint8_t j);                  // PASS
void jymper(uint8_t i, uint8_t j);                  // PASS
void multiply(uint8_t i, uint8_t j);                // PASS
void lerp(uint8_t i, uint8_t j);                    // FUNCTIONAL (just a wierd function overall)
void clock(uint8_t i, uint8_t j);                   // FUNCTIONAL (need to incorporate 'left'+timing a bit off)
void delay_b(uint8_t i, uint8_t j);                 //  (need to incorporate 'left')
void increment(uint8_t i, uint8_t j);               // PASS
void lesser(uint8_t i, uint8_t j);                  // PASS
void branch_if(uint8_t i, uint8_t j);               // PASS
void rando(uint8_t i, uint8_t j);                   // PASS
void variable(uint8_t i, uint8_t j);                // PASS

void konkat(uint8_t i, uint8_t j);                  // PASS
void read(uint8_t i, uint8_t j);                    // FUNCTIONAL (besides the cycle delay at times)
void push(uint8_t i, uint8_t j);                    // PASS
void track(uint8_t i, uint8_t j);                   // TO TEST
void query(uint8_t i, uint8_t j);                   // TO TEST
void write(uint8_t i, uint8_t j);                   // TO TEST
void generator(uint8_t i, uint8_t j); 

// void comment(); // worry about this later

/***** All things SPECIAL defined here *******************************/

// void self(char cmd);
// void midi(char ch, char oct, char note, char velocity);
// void midi_cc(char ch, char knob, char val);
// void pitch(char oct, char note);
// void byte(char high, char low);
// void play(char ch, char oct, char note, char velocity);

#endif