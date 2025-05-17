#include "frame.h"
#include "instr.h"
#include "stdint.h"
#include "config.h"
#include <Arduino.h>
 
extern int memory;
extern uint8_t cycle;
extern bool bangers[Y_MAX * X_MAX];
extern char grid_screen[Y_MAX * X_MAX];
extern uint8_t grid_color[Y_MAX * X_MAX];
extern uint8_t get_index(int x_pos, int y_pos);

void update_frame(){
    uint8_t boost = X_MAX;
    for(uint8_t i = 0; i < X_MAX; i++){
        for(uint8_t j = 0; j < Y_MAX; j++){
            uint8_t index = j*boost + i;
            if(ISOP(grid_screen[index])){
                if(j-1 >= 0 && grid_screen[(j-1)*boost + i] != HALT){
                    switch(grid_screen[index]){
                        uint8_t aval, bval;
                        case ADD:
                            aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
                            bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);
                            grid_screen[(j+1)*boost + i] = UNDIGIFY((aval + bval) % 36);
                            break;
                        case SUB:
                            aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
                            bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);
                            grid_screen[(j+1)*boost + i] = UNDIGIFY(abs(aval - bval));
                            break;
                        case MULT:
                            aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
                            bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);
                            grid_screen[(j+1)*boost + i] = UNDIGIFY((aval * bval)% 36);
                        case BANG:
                            if(bangers[index]){
                                bangers[index] = false;
                            }
                            else{
                                grid_screen[index] = NOP;
                            }
                            break;
                        case RIGHT:
                            if(bangers[index]){
                                bangers[index] = false;
                            }
                            else{
                                grid_screen[index] = NOP;
                                if(i+1 < X_MAX){
                                    grid_screen[index+1] = RIGHT;
                                    bangers[index+1] = true;
                                }
                            }
                            break;
                        case LEFT:
                            if(bangers[index]){
                                bangers[index] = false;
                            }
                            else{
                                grid_screen[index] = NOP;
                                if(i-1 >= 0){
                                    grid_screen[index-1] = LEFT;
                                }
                            }
                            break;
                        case UP: 
                            if(bangers[index]){
                                bangers[index] = false;
                            }
                            else{
                                grid_screen[index] = NOP;
                                if(j-1 >= 0){
                                    grid_screen[(j-1)*boost + i] = UP;
                                }
                            }
                            break;
                        case DOWN:
                            if(bangers[index]){
                                bangers[index] = false;
                            }
                            else{
                                grid_screen[index] = NOP;
                                if(j+1 < Y_MAX){
                                    grid_screen[(j+1)*boost + i] = DOWN;
                                    bangers[(j+1)*boost + i] = true;
                                }
                            }
                            break;
                        // case CLOCK: // increments counter, worry about speed later
                        //     char max_c = grid_screen[row*boost + col+1];
                        //     memory = UNDIGIFY((DIGIFY(grid_screen[(row+1)*boost + col]) + 1) & 36);
                        //     memory = memory >= max_c ? '0' : memory;
                        //     grid_screen[(row+1)*boost + col] = memory;
                        //     break;
                        // case DELAY:
                        //     memory = UNDIGIFY(grid_screen[row*boost + col+1]); // get bang framerate
                        //     uint8_t curr = cycle % memory;
                        //     uint8_t post = (cycle-1) % memory;
    
                        //     if(curr == 0){ // bang
                        //         grid_screen[((row+1)*boost)+col] = BANG;
                        //         grid_color[(row+1)*boost + col] = 3;
                        //     }
                        //     else if(post == 0){ // revert the bang
                        //         grid_screen[((row+1)*boost)+col] = NOP;
                        //         grid_color[(row+1)*boost + col] = 0;
                        //     }
                        //     break;
                        // case RAND: // choose another random character in between bounds
                        //     uint8_t r_min = DIGIFY(grid_screen[row*boost + col-1]);
                        //     uint8_t r_max = DIGIFY(grid_screen[row*boost + col+1]);
                        //     memory = random(r_min, r_max);
                        //     grid_screen[((row+1)*boost)+col] = UNDIGIFY(memory);
                        //     break;
                        // case LERP: // increment until target
                        //     uint8_t lt = DIGIFY(grid_screen[row*boost + col+1]);
                        //     uint8_t lc = DIGIFY(grid_screen[(row+1)*boost + col]);
                        //     if (lc < lt) lc++;
                        //     grid_screen[(row+1)*boost + col] = UNDIGIFY(lc);
                        //     break;
                        // case INC:
                        //     uint8_t istp = DIGIFY(grid_screen[row*boost+col-1]);
                        //     uint8_t imx = DIGIFY(grid_screen[row*boost+col+1]);
                        //     uint8_t icr = DIGIFY(grid_screen[(row+1)*boost+col]);
                        //     if (icr < imx) icr = (icr + istp) % imx;
                        //     grid_screen[(row+1)*boost + col] = UNDIGIFY(icr);
                        //     break;
                        default:
                            break;
                    }
                }
            }
        }   
    }
}