/**********************************************************************
 * frame.cpp 
 * 
 * This will contain function definitions to the frame update functions
 * that will update the LCD display based on the 2D program created
**********************************************************************/

#include "frame.h"
#include "instr.h"
#include "stdint.h"
#include "config.h"
#include <Arduino.h>
 
/*********************************************************************/

extern uint8_t cycle;
extern uint8_t bangers[Y_MAX * X_MAX];
extern char grid_screen[Y_MAX * X_MAX];
extern uint8_t grid_color[Y_MAX * X_MAX];

/*********************************************************************/

void clear_colors(){
    for(int i = 0; i < (Y_MAX * X_MAX); i++){
        grid_color[i] = 0;
    }
}
void update_frame(){
    uint8_t boost = X_MAX;
    clear_colors();
    for(uint8_t i = 0; i < X_MAX; i++){
        for(uint8_t j = 0; j < Y_MAX; j++){
            uint8_t index = j*boost + i;
            if(!ISOP(grid_screen[index])){
                continue;
            }
            if (bangers[index] == 1){
                bangers[index] = 0;
                continue;
            }
            else if (bangers[index] == 2){
                continue;
            }
            if((j == 0) || (j-1 >= 0 && grid_screen[(j-1)*boost + i] != HALT)){
                switch(grid_screen[index]){
                    uint8_t aval, bval;
                    case ADD:
                        add(i, j);
                        break;
                    case SUB:
                        sub(i, j);
                        break;
                    case MULT:
                        multiply(i, j);
                        break;
                    case BANG:
                        bang(index);
                        break;
                    case RIGHT:
                        east(i, j);
                        break;
                    case LEFT:
                        west(i, j);
                        break;
                    case UP: 
                        north(i, j);
                        break;
                    case DOWN:
                        south(i, j);
                        break;
                    case JXMP:
                        jxmper(i, j);
                        break;
                    case JYMP:
                        jymper(i, j);
                        break;
                    case CLOCK:
                        clock(i, j);
                        break;
                    case DELAY:
                        delay_b(i, j);
                        break;
                    case LERP:
                        lerp(i, j);
                        break;
                    case MIN:
                        lesser(i, j);
                        break;
                    case IF:
                        branch_if(i, j);
                        break;
                    case RAND:
                        rando(i, j);
                        break;
                    case VAR:
                        variable(i,j);
                        break;
                    case INC:
                        increment(i,j);
                        break;
                    case READ:
                        read(i, j);
                        break;
                    case PUSH:
                        push(i, j);
                        break;
                    case KONCAT:
                        konkat(i, j);
                        break;
                    case TRACK:
                        track(i, j);
                        break;
                    case QUERY:
                        query(i, j);
                        break;
                    case WRITE:
                        write(i, j);
                        break;
                    default:
                        break;
                }
            }
        }   
    }
}