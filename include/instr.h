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
void bang(uint8_t index, uint8_t type);             // PASS
void east(uint8_t i, uint8_t j);                    // PASS
void north(uint8_t i, uint8_t j);                   // PASS
void south(uint8_t i, uint8_t j);                   // PASS
void west(uint8_t i, uint8_t j);                    // PASS
void add(uint8_t i, uint8_t j);                     // PASS
void sub(uint8_t i, uint8_t j);                     // PASS
void jxmper(uint8_t i, uint8_t j);                  // PASS
void jymper(uint8_t i, uint8_t j);                  // PASS
void multiply(uint8_t i, uint8_t j);                // PASS
void lerp(uint8_t i, uint8_t j);                    // PASS
void clock(uint8_t i, uint8_t j);                   // FUNC (ignored 'left')
void delay_b(uint8_t i, uint8_t j);                 // FUNC
void increment(uint8_t i, uint8_t j);               // PASS
void lesser(uint8_t i, uint8_t j);                  // PASS
void branch_if(uint8_t i, uint8_t j);               // PASS
void rando(uint8_t i, uint8_t j);                   // PASS
void variable(uint8_t i, uint8_t j);                // PASS

void konkat(uint8_t i, uint8_t j);                  // PASS
void read(uint8_t i, uint8_t j);                    // PASS
void push(uint8_t i, uint8_t j);                    // PASS
void track(uint8_t i, uint8_t j);                   // PASS
void query(uint8_t i, uint8_t j);                   // PASS
void write(uint8_t i, uint8_t j);                   // PASS
void generator(uint8_t i, uint8_t j);               // PASS

/***** All things SPECIAL defined here *******************************/
void starburst(uint8_t i, uint8_t j);

 
#endif