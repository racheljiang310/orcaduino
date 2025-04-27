#include "display.h"
/**********************************************************************
 * display.cpp 
 * 
 * This file will define all function declarations made in "display.h"
**********************************************************************/

void init_grid(){
    for(int row = 0; row < SCREEN_WIDTH; row++){
        for(int col = 0; col < SCREEN_HEIGHT; col++){
            grid_screen[row*SCREEN_WIDTH + col] = '*';
        }
    }
}