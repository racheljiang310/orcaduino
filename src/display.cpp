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
extern char variables[36];
extern uint8_t bangers[Y_MAX * X_MAX];
extern char grid_screen[Y_MAX * X_MAX];
extern uint8_t grid_color[Y_MAX * X_MAX];

/// @brief initializes grid so it's not weird...
void init_grid(){
    for(int i = 0; i < (Y_MAX * X_MAX); i++){
        grid_screen[i] = NOP;
        grid_color[i] = 0;
    }
    for(int j = 0; j < 36; j++){
        variables[j] = '?';
    }
}

/// @brief initializes bangers to all 0
void init_bangers(){
    for(int i = 0; i < (Y_MAX * X_MAX); i++){
        bangers[i] = 0;
    }
}