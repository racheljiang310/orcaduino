#include "instr.h"
#include "Arduino.h"
#include "stdint.h"
/**********************************************************************
 * instr.h 
 * 
 * This header will contain all function prototypes for the operations
 * aka our instruction set. More descriptions will be provided in the
 * function definitions.
**********************************************************************/
extern char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];
extern uint8_t grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];
extern int variables[36];
extern int x, y;
extern int memory;

/***** All things BASIC defined here *********************************/
void bang(){

}

/// @brief Halts the operation @ (x, y+1) if it exists
/// @details for each frame update, check if grid_screen[x, y-1] == HALT
void halt(){
    grid_screen[x*SCREEN_WIDTH+y] = HALT;
    grid_color[x*SCREEN_WIDTH+y] = 2;
}

/// @brief moves an 'E' to the right until it goes off screen
/// @details frame updates @ grid[(x*SCREEN_WIDTH)+y] => mv right/frame
void east(){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = RIGHT;
    grid_color[x*SCREEN_WIDTH+y] = 2;
}
/// @brief moves an 'E' to the right until it goes off screen
/// @details frame updates @ grid[(x*SCREEN_WIDTH)+y] => mv up/frame
void north(){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = UP;
    grid_color[x*SCREEN_WIDTH+y] = 2;
}
void south(){

}
void west(){

}
void comment(){

}

/// @brief [len] K [variables...]
/// @param len length of concatenation
void konkat(char len){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = JYMP;
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand

    for(uint8_t i = 0; i < static_cast<int>(len); i++){
        grid_color[(x*SCREEN_WIDTH)+y+1+i] = 2; // operand
        grid_color[((x+1)*SCREEN_WIDTH)+y+1+i] = 3; // result
        grid_screen[((x+1)*SCREEN_WIDTH)+y+1+i] = static_cast<char>(variables[static_cast<int>(grid_screen[((x)*SCREEN_WIDTH)+y+1+i])]); // values
    }
}

void jumper(char val){

}

/// @brief Takes value from north and teleports it to south
/// @param val the value in question
void jymper(char val){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = JYMP;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = val;
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[(x-1)*SCREEN_WIDTH+y] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}

/// @brief a - b => result
/// @param a a number
/// @param b a number
void add(char a, char b){
    // calculation
    memory = static_cast<int>(a)+static_cast<int>(b);
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = ADD;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = static_cast<char>(memory);
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}

/// @brief a + b => result
/// @param a a number
/// @param b a number
void sub(char a, char b){
    // calculation
    memory = static_cast<int>(a)-static_cast<int>(b);
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = SUB;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = static_cast<char>(memory);
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result

}
/// @brief Multiplies a and b, then mod by base 36
/// @param a left value in question
/// @param b right value in question
void multiply(char a, char b){
    memory = (static_cast<int>(a) * static_cast<int>(b)) % 36;
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = MULT;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = memory;
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}

/// @brief output smaller value
/// @param a left val
/// @param b right val
void lesser(char a, char b){
    memory = static_cast<int>(a) < static_cast<int>(b) ? a : b;
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = MIN;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = memory;
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}

/// @brief bang if a == b
/// @param a a char
/// @param b a char
void branch_if(char a, char b){
    memory = a - b;
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = IF;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = memory == 0 ? '*' : '.';
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}

/// @brief creates a counter of some sense
/// @details frame updates @ grid[((x+1)*SCREEN_WIDTH)+y]
/// @param rate initial delay of rate frames
/// @param mod counts -> [0,mod)
void clock(char rate=0, char mod=8){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = CLOCK;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = static_cast<int>(0);
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}
/// @brief creates a delay in a "bang"
/// @details frame updates @ grid[((x+1)*SCREEN_WIDTH)+y]
/// @param rate initial delay of rate frames
/// @param mod bangs once every "mod" frames
void delay_b(char rate=0, char mod=8){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = DELAY;
    // update screen with result
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = '.'; // will go to '*' on bang
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}

/// @brief random number inclusive of min and max values
/// @details frame updates @ grid[((x+1)*SCREEN_WIDTH)+y]
/// @param min min value
/// @param max max value
void rando(char min, char max){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = RAND;
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator

    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand

    // update screen with result, starts @ min
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = random(static_cast<int>(min), static_cast<int>(max)); // TODO: ensure that this works with letters too
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result  
}
void uclid(char step, char max){

}
void lerp(char rate, char target){

}

/// @brief Increments by 'step' from [0, mod)
/// @details frame updates @ grid[((x+1)*SCREEN_WIDTH)+y]
/// @param step increment value
/// @param mod upper bound (not inclusive)
void increment(char step, char mod){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = INC;
    // update screen with result, starts @ 1
    grid_screen[((x+1)*SCREEN_WIDTH)+y] = '0';
    // update screen with colors
    grid_color[x*SCREEN_WIDTH+y] = 1; // operator
    grid_color[x*SCREEN_WIDTH+y+1] = grid_color[x*SCREEN_WIDTH+y-1] = 2; // operand
    grid_color[((x+1)*SCREEN_WIDTH)+y] = 3; // result
}
char variable(char write, char read){

}

void track(char key, char len, char val){

}
/// @brief writes eastward operand
/// @details frame updates @ id + 1 % len
/// @param key index
/// @param len # slots
/// @param val what to write
void push(char key, char len, char val){
    grid_screen[x*SCREEN_WIDTH+y] = PUSH;
    grid_color[x*SCREEN_WIDTH+y] = 1; 

    // update screen with result & colors
    uint8_t id = static_cast<int>(key) % static_cast<int>(len);
    grid_color[(x*SCREEN_WIDTH)+y-1] = 2; 
    grid_color[(x*SCREEN_WIDTH)+y-2] = 2; 
    grid_color[(x*SCREEN_WIDTH)+y+1] = 2; 
    // result
    grid_screen[((x+1)*SCREEN_WIDTH)+y+id] = val;
    grid_color[((x+1)*SCREEN_WIDTH)+y+id] = 3; 
}

void read(char row, char col){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = READ;
    grid_color[x*SCREEN_WIDTH+y] = 1; 
    // update screen with result & colors
    uint8_t x_off = static_cast<int>(row);
    uint8_t y_off = static_cast<int>(col)+1; // one after operator
    grid_color[x*SCREEN_WIDTH+y-2] = grid_color[x*SCREEN_WIDTH+y-1] = 2; 

    // read the offsetted value
    memory = static_cast<int>(grid_screen[(x+x_off)*SCREEN_WIDTH+y+y_off]);
    grid_color[(x+x_off)*SCREEN_WIDTH+y+y_off] = 2;

    // put read value into south box
    grid_screen[(x+1)*SCREEN_WIDTH+y] = memory;
    grid_color[(x+1)*SCREEN_WIDTH+y] = 3;
    
}
/// @brief reads values from offset & length.
/// @param x x-offset
/// @param y y-offset
/// @param len # slots to query
void query(char row, char col, char len){
    grid_screen[x*SCREEN_WIDTH+y] = QUERY;
    grid_color[x*SCREEN_WIDTH+y] = 1;

    // update screen with result & colors
    uint8_t x_off = static_cast<int>(row);
    uint8_t y_off = static_cast<int>(col+1); // one after operator

    uint8_t size = static_cast<int>(len); // one after operator
    uint8_t start_r = static_cast<int>(x+1);
    uint8_t start_c = static_cast<int>(y-size); // one after operator

    grid_color[x*SCREEN_WIDTH+y-3] = grid_color[x*SCREEN_WIDTH+y-2] = grid_color[x*SCREEN_WIDTH+y-1] = 2; 

    for(uint8_t i = 0; i < size; i){
        // read the byte
        memory = static_cast<int>(grid_screen[(x+x_off)*SCREEN_WIDTH+y+y_off]);
        grid_color[(x+x_off)*SCREEN_WIDTH+y+y_off] = 2;

        // write the byte into result
        grid_screen[(start_r)*SCREEN_WIDTH+start_c] = memory;
        grid_color[(start_r)*SCREEN_WIDTH+start_c] = 3;
        start_c++;
    }
}
void write(char x, char y, char val){

}

/// @brief [x|y|len|G|string of "len" size]
/// @param x right offset 
/// @param y down offset
/// @param len length of string
void generator(char x_v, char y_v, char len){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = GENER;
    grid_color[x*SCREEN_WIDTH+y] = 1; 
    // update screen with result & colors
    memory = static_cast<int>(len);
    uint8_t xv = static_cast<int>(x_v);
    uint8_t yv = static_cast<int>(y_v);
    for(uint8_t i = 0; i < memory; i++){
        grid_screen[((x+yv)*SCREEN_WIDTH)+(xv)+i] = grid_screen[x*SCREEN_WIDTH+y+i+1]; 
        grid_color[x*SCREEN_WIDTH+y+i+1] = 2;
        grid_color[((x+yv)*SCREEN_WIDTH)+(xv)+i] = 3; 
    }
}

/***** All things SPECIAL defined here ************************(******/

void self(char cmd){

}
void midi(char ch, char oct, char note, char velocity){

}
void midi_cc(char ch, char knob, char val){

}