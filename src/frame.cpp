#include "frame.h"
#include "instr.h"
#include "stdint.h"
#include "config.h"
#include <Arduino.h>
 
extern int memory;
extern uint8_t cycle;
extern char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];
extern uint8_t grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];
extern char op(int x_pos = 0, int y_pos = 0);

// extern void comment();
// extern void konkat(char len);
// extern void jxmper(char val);
// extern void jymper(char val);
// extern void add(char a, char b);
// extern void sub(char a, char b);
// extern void multiply(char a, char b);
// extern void lesser(char a, char b);
// extern void branch_if(char a, char b);
// extern void variable(char name, char value);
// extern void track(char idx, char len, char val);
// extern void push(char key, char len, char val);
// extern void read(char row, char col);
// extern void query(char row, char col, char len);
// extern void write(char off_x, char off_y, char val);
// extern void generator(char x_v, char y_v, char len);

char increment_b36(char original){
    if(original == '9'){
        original = 'a';
    }
    else if (original == 'z'){
        original = '0';
    }
    else original++;
    return original;
}

void update_frame(){
    uint8_t boost = SCREEN_WIDTH;
    for(uint8_t row = 0; row < SCREEN_WIDTH; row++){
        for(uint8_t col = 0; col < SCREEN_HEIGHT; col++){
            if(ISOP(grid_screen[row*boost + col]) && grid_screen[(row-1)*boost + col] != HALT){
                switch(grid_screen[row*boost + col]){
                    case BANG:
                        Serial.println("BANG");
                        grid_screen[row*boost + col] = NOP;
                        grid_color[row*boost + col] = 0;
                        break;
                    case RIGHT: // shift the letter right by one
                        grid_screen[row*boost + col] = NOP;
                        grid_color[row*boost + col] = 0;
                        grid_screen[row*boost + col+1] = RIGHT;
                        break;
                    case UP: // shift the letter up by one
                        grid_screen[row*boost + col] = NOP;
                        grid_color[row*boost + col] = 0;
                        grid_screen[(row-1)*boost + col] = UP;
                        break;
                    case DOWN: // shift the letter down by one
                        grid_screen[row*boost + col] = NOP;
                        grid_color[row*boost + col] = 0;
                        grid_screen[(row+1)*boost + col] = DOWN;
                        break;
                    case LEFT:  // shift the letter left by one
                        grid_screen[row*boost + col] = NOP;
                        grid_color[row*boost + col] = 0;
                        grid_screen[row*boost + col-1] = LEFT;
                        break;
                    case CLOCK: // increments counter, worry about speed later
                        char max_c = grid_screen[row*boost + col+1];
                        memory = increment_b36(grid_screen[(row+1)*boost + col]);
                        memory = memory >= max_c ? '0' : memory;
                        grid_screen[(row+1)*boost + col] = memory;
                        break;
                    case DELAY:
                        memory = UNDIGIFY(grid_screen[row*boost + col+1]); // get bang framerate
                        uint8_t curr = cycle % memory;
                        uint8_t post = (cycle-1) % memory;

                        if(curr == 0){ // bang
                            grid_screen[((row+1)*boost)+col] = BANG;
                            grid_color[(row+1)*boost + col] = 3;
                        }
                        else if(post == 0){ // revert the bang
                            grid_screen[((row+1)*boost)+col] = NOP;
                            grid_color[(row+1)*boost + col] = 0;
                        }
                        break;
                    case RAND: // choose another random character in between bounds
                        uint8_t r_min = DIGIFY(grid_screen[row*boost + col-1]);
                        uint8_t r_max = DIGIFY(grid_screen[row*boost + col+1]);
                        memory = random(r_min, r_max);
                        grid_screen[((row+1)*boost)+col] = UNDIGIFY(memory);
                        break;
                    case LERP: // increment until target
                        uint8_t lt = DIGIFY(grid_screen[row*boost + col+1]);
                        uint8_t lc = DIGIFY(grid_screen[(row+1)*boost + col]);
                        if (lc < lt) lc++;
                        grid_screen[(row+1)*boost + col] = UNDIGIFY(lc);
                        break;
                    case INC:
                        uint8_t istp = DIGIFY(grid_screen[row*boost+col-1]);
                        uint8_t imx = DIGIFY(grid_screen[row*boost+col+1]);
                        uint8_t icr = DIGIFY(grid_screen[(row+1)*boost+col]);
                        if (icr < imx) icr = (icr + istp) % imx;
                        grid_screen[(row+1)*boost + col] = UNDIGIFY(icr);
                        break;
                    default:
                        break;
                }

            }
        }   
    }
}