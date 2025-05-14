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
extern char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];
extern uint8_t grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];

void init_grid(){
    for(int row = 0; row < SCREEN_WIDTH; row++){
        for(int col = 0; col < SCREEN_HEIGHT; col++){
            grid_screen[row*SCREEN_WIDTH + col] = NOP;
            grid_color[row*SCREEN_WIDTH + col] = NOP;
        }
    }
}