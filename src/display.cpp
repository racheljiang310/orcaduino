#include "display.h"
#include "stdint.h"
#include <config.h>
/**********************************************************************
 * display.cpp 
 * 
 * This file will define all function declarations made in "display.h"
**********************************************************************/
extern int x; 
extern int y; 
extern int memory;
extern int variables[36];
extern char grid_screen[Y_MAX * X_MAX];
extern uint8_t grid_color[Y_MAX * X_MAX];

void init_grid(){
    for(int i = 0; i < 64; i++){
        grid_screen[i] = NOP;
        grid_color[i] = 0;
    }
    for(int j = 0; j < 36; j++){
        variables[j] = 100;
    }
}