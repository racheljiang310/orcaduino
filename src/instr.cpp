#include "instr.h"
#include "config.h"
#include "Arduino.h"
/**********************************************************************
 * instr.h 
 * 
 * This header will contain all function prototypes for the operations
 * aka our instruction set. More descriptions will be provided in the
 * function definitions.
**********************************************************************/
extern char grid_screen[Y_MAX * X_MAX];
extern uint8_t grid_color[Y_MAX * X_MAX];
extern uint8_t bangers[Y_MAX * X_MAX];
extern uint8_t variables[36];
extern uint8_t cycle;
extern int x, y;

/***** All things BASIC defined here *********************************/

/***** Dynamic *******************************************************/

/// @brief bangs once, updates bangers[]
/// @param index index
void bang(uint8_t index){
    grid_color[index] = 0;
    grid_screen[index] = NOP;
    bangers[index] = 0;
}

/// @brief Halts the operation @ (x, y+1) if it exists
void halt(){
    grid_screen[y*X_MAX+x] = HALT; // honestly don't even need this line either
}

/// @brief moves an 'E' to the right until it goes off screen
/// @param i index 
/// @param j index 
void east(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(i+1 < X_MAX){
        grid_screen[index+1] = RIGHT;
        bangers[index+1] = 1;
    }
}
/// @brief moves an 'E' to the right until it goes off screen
/// @param i index 
/// @param j index 
void north(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(j-1 >= 0){
        grid_screen[(j-1)*X_MAX + i] = UP;
        bangers[(j-1)*X_MAX + i] = 1;
    }
}
/// @brief moves an 'S' downwards until it goes off screen
/// @param i index 
/// @param j index 
void south(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(j+1 < Y_MAX){
        grid_screen[(j+1)*X_MAX + i] = DOWN;
        bangers[(j+1)*X_MAX + i] = 1;
    }
}
/// @brief moves an 'W' leftwards until it goes off screen
/// @param i index 
/// @param j index 
void west(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(i-1 >= 0){
        grid_screen[index-1] = LEFT;
        bangers[index-1] = 1;
    }
}

/// @brief counts from 0 to the max value specified in right (exclusive)
/// @param i index
/// @param j index
void clock(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    uint8_t c_max = j*X_MAX + i+1;
    uint8_t rate =  j*X_MAX + i-1; // don't worry about this for now
    uint8_t c_result = (j+1)*X_MAX + i;
    uint8_t c_max_val = DIGIFY(grid_screen[c_max]);
    
    if (c_max_val == 0) c_max_val = 1;
    if(ISB36(grid_screen[c_result])==false){
        grid_screen[c_result] = '0';
    }
    else{
        uint8_t current_val = DIGIFY(grid_screen[c_result]);
        uint8_t next_val = (current_val + 1) % c_max_val;
        grid_screen[c_result] = UNDIGIFY(next_val);
    }
    grid_color[index] = 1; 
    grid_color[c_max] = grid_color[rate] = 2;
    grid_color[c_result] = 3;
}
/// @brief creates a delay in a "bang" (max = 8)
/// @param i index
/// @param j index
void delay_b(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t left = j*X_MAX + i-1; // don't worry for now
    uint8_t right = j*X_MAX + i+1; // rate
    uint8_t result = (j+1)*X_MAX + i;

    uint8_t bang_rate = DIGIFY(grid_screen[right]);
    if(bang_rate == 0) bang_rate = 1;

    uint8_t curr = cycle % bang_rate;
    if(curr == 0){ // bang
        grid_screen[result] = BANG;
        grid_color[result] = 3;
        bangers[result] = 1;
    }
    else{ /* bang() takes care of the un-bang */ }
    grid_color[index] = 1; 
    grid_color[left] = grid_color[right] = 2;
}

/// @brief random # inclusive of min and max values
/// @param i index
/// @param j index
void rando(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t minx = ISB36(grid_screen[index - 1]) ? DIGIFY(grid_screen[index - 1]) : 0;
    uint8_t maxx = ISB36(grid_screen[index + 1]) ? DIGIFY(grid_screen[index + 1]) : 9;
    uint8_t new_val = (uint8_t)(random(minx, maxx));
    grid_screen[(j + 1) * X_MAX + i] = UNDIGIFY(new_val);

    grid_color[index] = 1; 
    grid_color[index+1] = grid_color[index-1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
}

/// @brief increments until it reaches 'target'
/// @param i index
/// @param j index
void lerp(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    uint8_t c_max = j*X_MAX + i+1;
    uint8_t c_start =  j*X_MAX + i-1;
    uint8_t c_result = (j+1)*X_MAX + i;
    uint8_t c_max_val = ISB36(grid_screen[c_max]) ? DIGIFY(grid_screen[c_max]) : 0;
    char c_start_val = ISB36(grid_screen[c_start]) ? grid_screen[c_start] : '0';
    
    if(ISB36(grid_screen[c_result])==false){
        grid_screen[c_result] = c_start_val;
    }
    else{
        uint8_t current_val = DIGIFY(grid_screen[c_result]);
        uint8_t next_val = (current_val + 1 > c_max_val) ? c_max_val : (current_val + 1);
        grid_screen[c_result] = UNDIGIFY(next_val);
    }
    grid_color[index] = 1; 
    grid_color[c_max] = grid_color[c_start] = 2;
    grid_color[c_result] = 3;
}

/// @brief Increments by 'left' from [0, 'right')
/// @param i index
/// @param j index
void increment(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t result = (j+1)*X_MAX + i;
    uint8_t step = ISB36(grid_screen[index - 1]) ? DIGIFY(grid_screen[index - 1]) : 1;
    uint8_t maxx = ISB36(grid_screen[index + 1]) ? DIGIFY(grid_screen[index + 1]) : DIGIFY('z');

    if(ISB36(grid_screen[result])==false){
        grid_screen[result] = '0';
    }
    else {
        uint8_t current_val = DIGIFY(grid_screen[result]);
        uint8_t next_val = (current_val + step) % maxx;
        grid_screen[result] = UNDIGIFY(next_val);
    }
    grid_color[index] = 1; 
    grid_color[index+1] = grid_color[index-1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
}

/***** Static ********************************************************/

/// @brief [len] K [variables...] => values to vars
/// @param len length of variables to read
void konkat(uint8_t len){
    // update screen with operator
    grid_screen[x*X_MAX+y] = JYMP;
    // update screen with colors
    grid_color[x*X_MAX+y] = 1; // operator
    grid_color[x*X_MAX+y-1] = 2; // operand

    for(uint8_t i = 0; i < DIGIFY(len); i++){
        grid_color[(x*X_MAX)+y+1+i] = 2; // operand
        grid_color[((x+1)*X_MAX)+y+1+i] = 3; // result
        uint8_t var = DIGIFY(grid_screen[((x)*X_MAX)+y+1+i]);
        grid_screen[((x+1)*X_MAX)+y+1+i] = UNDIGIFY(variables[var]); // values
    }
}

/// @brief teleports a value left to right
/// @param i index 
/// @param j index 
void jxmper(uint8_t i, uint8_t j){
    uint8_t aval = j*X_MAX + i-1; // west
    uint8_t bval = j*X_MAX + i+1; // east
    grid_screen[bval] = grid_screen[aval];

    grid_color[j*X_MAX + i] = 1;
    grid_color[aval] = 2; 
    grid_color[bval] = 3;
}

/// @brief Takes value from north and teleports it to south
/// @param i index 
/// @param j index 
void jymper(uint8_t i, uint8_t j){
    uint8_t aval = (j-1)*X_MAX + i; // north
    uint8_t bval = (j+1)*X_MAX + i; // south
    grid_screen[bval] = grid_screen[aval];

    grid_color[j*X_MAX + i] = 1;
    grid_color[aval] = 2; 
    grid_color[bval] = 3;
}

/// @brief executes addition instruction
/// @param i index
/// @param j index
void add(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
    uint8_t bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);

    grid_screen[(j+1)*X_MAX + i] = UNDIGIFY((aval + bval) % 36);

    grid_color[index+1] = grid_color[index-1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief executes subtraction instruction (consider not having abs())
/// @param i index 
/// @param j index 
void sub(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
    uint8_t bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);

    grid_screen[(j+1)*X_MAX + i] = UNDIGIFY(abs(aval - bval) % 36);

    grid_color[index+1] = grid_color[index-1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief multiplication
/// @param i index 
/// @param j index 
void multiply(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
    uint8_t bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);
    grid_screen[(j+1)*X_MAX + i] = UNDIGIFY((aval * bval)% 36);

    grid_color[index+1] = grid_color[index-1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief output smaller value
/// @param i index
/// @param j index
void lesser(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
    uint8_t bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);
    grid_screen[(j+1)*X_MAX + i] = UNDIGIFY(aval < bval ? aval : bval);

    grid_color[index-1] = grid_color[index+1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief bang if a == b
/// @param i index
/// @param j index
void branch_if(uint8_t i, uint8_t j){ // Progress
    uint8_t index = j*X_MAX + i;
    char aval = grid_screen[index-1];
    char bval = grid_screen[index+1];
    char result = aval == bval ? '*' : '.';
    grid_screen[(j+1)*X_MAX + i] = result;
    if(result == '*'){
        bangers[(j+1)*X_MAX + i] = 2;
    }
    
    grid_color[index-1] = grid_color[index+1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief creates/reads a variable
/// @param i index
/// @param j index
void variable(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;

    grid_color[index] = 1;
    grid_color[index-1] = grid_color[index+1] = 2;
    grid_screen[(j+1)*X_MAX + i] = NOP;
    grid_color[(j+1)*X_MAX + i] = 0;

    if(ISB36(grid_screen[index+1]) == false && ISB36(grid_screen[index-1]) == false) return; // nothing

    uint8_t name = DIGIFY(grid_screen[index-1]);
    char value =grid_screen[index+1];

    if(variables[DIGIFY(value)] == '?' && ISB36(grid_screen[index-1]) == true){ // var input, value exists => set
        variables[name] = value;
    }
    else if(variables[DIGIFY(value)] != '?' && ISB36(grid_screen[index-1]) == false){ // var exists => read
        grid_screen[(j+1)*X_MAX + i] = variables[DIGIFY(value)];
        grid_color[(j+1)*X_MAX + i] = 3;
        return;
    }
}

/// @brief read cell given offset
/// @param i index
/// @param j index
void read(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t x_off = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-2]);
    uint8_t y_off = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);

    uint8_t read_index = (j+y_off)*X_MAX + i+x_off;
    char value = grid_screen[read_index];
    grid_screen[(j+1)*X_MAX + i] = value;

    grid_color[index-1] = grid_color[index-2] = grid_color[read_index] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief ouputs note @ 'idx' from arbitrarily long 'val' of size 'len'
/// @param idx the index % len value
/// @param len size of val
/// @param val val in questions
void track(uint8_t idx, uint8_t len, uint8_t val){
    grid_screen[x*X_MAX+y] = TRACK;
    grid_color[x*X_MAX+y] = 1; 

    // update screen with result & colors
    uint8_t id = DIGIFY(idx) % DIGIFY(len);
    grid_color[(x*X_MAX)+y-1] = 2; 
    grid_color[(x*X_MAX)+y-2] = 2; 
    grid_color[(x*X_MAX)+y+1] = 2; 
    // result
    // memory = grid_screen[(x*X_MAX)+y+1+id];
    // grid_screen[(x*X_MAX)+y] = memory;
    grid_color[(x*X_MAX)+y] = 3; 
}
/// @brief writes eastward operand
/// @param key index
/// @param len # slots
/// @param val what to write
void push(uint8_t key, uint8_t len, uint8_t val){

    // uint8_t index = j*X_MAX + i;
    // uint8_t key = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-2]);
    // uint8_t len = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);

    grid_screen[x*X_MAX+y] = PUSH;
    grid_color[x*X_MAX+y] = 1; 

    // update screen with result & colors
    uint8_t id = DIGIFY(key) % DIGIFY(len);
    grid_color[(x*X_MAX)+y-1] = 2; 
    grid_color[(x*X_MAX)+y-2] = 2; 
    grid_color[(x*X_MAX)+y+1] = 2; 
    // result
    grid_screen[((x+1)*X_MAX)+y+id] = val;
    grid_color[((x+1)*X_MAX)+y+id] = 3; 
}

/// @brief reads values from offset & length.
/// @param x x-offset
/// @param y y-offset
/// @param len # slots to query
void query(uint8_t row, uint8_t col, uint8_t len){
    grid_screen[x*X_MAX+y] = QUERY;
    grid_color[x*X_MAX+y] = 1;

    // update screen with result & colors
    uint8_t x_off = DIGIFY(row);
    uint8_t y_off = DIGIFY(col+1); // one after operator

    uint8_t size = DIGIFY(len); // one after operator
    uint8_t start_r = DIGIFY(x+1);
    uint8_t start_c = DIGIFY(y-size); // one after operator

    grid_color[x*X_MAX+y-3] = grid_color[x*X_MAX+y-2] = grid_color[x*X_MAX+y-1] = 2; 

    for(uint8_t i = 0; i < size; i){
        // read the byte
        // memory = DIGIFY(grid_screen[(x+x_off)*X_MAX+y+y_off]);
        grid_color[(x+x_off)*X_MAX+y+y_off] = 2;

        // write the byte into result
        // grid_screen[(start_r)*X_MAX+start_c] = memory;
        grid_color[(start_r)*X_MAX+start_c] = 3;
        start_c++;
    }
}

/// @brief writes value into slot
/// @param off_x 
/// @param off_y 
/// @param val 
void write(uint8_t i, uint8_t j){

    // fetch all the indices
    uint8_t index = j*X_MAX + i;
    uint8_t off_x = j*X_MAX + i-2;
    uint8_t off_y = j*X_MAX + i-1;
    uint8_t value = j*X_MAX + i+1;

    // update screen with operator
    grid_screen[x*X_MAX+y] = WRITE;
    grid_color[x*X_MAX+y] = 1; 
    grid_color[x*X_MAX+y-2] = grid_color[x*X_MAX+y-1] = grid_color[x*X_MAX+y+1] = 2; 

    // update screen with result & colors
    uint8_t x_off = DIGIFY(off_x);
    uint8_t y_off = DIGIFY(off_y); // one after operator

    // value to write
    // memory = DIGIFY(val);

    // put value into offsetted box
    // grid_screen[(x+1+y_off)*X_MAX+y+x_off] = memory;
    grid_color[(x+1+y_off)*X_MAX+y+x_off] = 3;
}

/// @brief [x|y|len|G|string of "len" size]
/// @param x right offset 
/// @param y down offset
/// @param len length of string
void generator(uint8_t x_v, uint8_t y_v, uint8_t len){
    // update screen with operator
    grid_screen[x*X_MAX+y] = GENER;
    grid_color[x*X_MAX+y] = 1; 
    // update screen with result & colors
    // memory = DIGIFY(len);
    // uint8_t xv = DIGIFY(x_v);
    // uint8_t yv = DIGIFY(y_v);
    // for(uint8_t i = 0; i < memory; i++){
    //     grid_screen[((x+yv)*X_MAX)+(xv)+i] = grid_screen[x*X_MAX+y+i+1]; 
    //     grid_color[x*X_MAX+y+i+1] = 2;
    //     grid_color[((x+yv)*X_MAX)+(xv)+i] = 3; 
    // }
}

/// @brief comments, we can handle this later
// void comment(){

// }

/***** All things SPECIAL defined here ************************(******/

// void self(char cmd){

// }
// void midi(char ch, char oct, char note, char velocity){

// }
// void midi_cc(char ch, char knob, char val){

// }