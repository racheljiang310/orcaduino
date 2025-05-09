#include "instr.h"
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
extern int x, y;
extern int memory;

/***** All things BASIC defined here *********************************/
void bang(){

}
void halt(){

}

/// @brief moves an 'E' to the right until it goes off screen
/// @details frame updates @ grid[(x*SCREEN_WIDTH)+y] => mv right/frame
void east(){
    // update screen with operator
    grid_screen[x*SCREEN_WIDTH+y] = RIGHT;
    grid_color[x*SCREEN_WIDTH+y] = 2;
}
void north(){

}
void south(){

}
void west(){

}
void comment(){

}

void konkat(char len){

}
void jumper(char val){

}
void jymper(char val){

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
void multiply(char a, char b){

}

void lesser(char a, char b){

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

void random(char min, char max){

}
void uclid(char step, char max){

}
void lerp(char rate, char target){

}

void increment(char step, char mod){

}
char variable(char write, char read){

}

void track(char key, char len, char val){

}
void push(char len, char key, char val){

}

void read(char x, char y, char read){

}
void query(char x, char y, char len){

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