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
extern uint8_t get_index(int x_pos, int y_pos);
extern char grid_screen[Y_MAX * X_MAX];
extern uint8_t grid_color[Y_MAX * X_MAX];
extern int variables[36];
extern int x, y;
extern int memory;

/***** All things BASIC defined here *********************************/

/***** Dynamic *******************************************************/

/// @brief bangs
void bang(){
    grid_screen[y*X_MAX+x] = BANG;
    grid_color[y*X_MAX+x] = 1;
}

/// @brief Halts the operation @ (x, y+1) if it exists
/// TODO: for each frame update, check if grid_screen[x, y-1] == HALT
void halt(){
    grid_screen[y*X_MAX+x] = HALT;
    grid_color[y*X_MAX+x] = 2;
}

/// @brief moves an 'E' to the right until it goes off screen
void east(){
    // update screen with operator
    grid_screen[y*X_MAX+x] = RIGHT;
    grid_color[y*X_MAX+x] = 2;
}
/// @brief moves an 'E' to the right until it goes off screen
void north(){
    // update screen with operator
    grid_screen[y*X_MAX+x] = UP;
    grid_color[y*X_MAX+x] = 2;
}
/// @brief moves an 'S' downwards until it goes off screen
void south(){
    grid_screen[y*X_MAX+x] = DOWN;
    grid_color[y*X_MAX+x] = 2;

}
/// @brief moves an 'W' leftwards until it goes off screen
void west(){
    grid_screen[y*X_MAX+x] = LEFT;
    grid_color[y*X_MAX+x] = 2;
}

/// @brief creates a counter of some sense
void clock(){

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = '0';
    grid_color[idx] = 3; 

    idx = get_index(1, 0);
    grid_color[idx] = 2;
    idx = get_index(-1, 0);
    grid_color[idx] = 2;

    idx = get_index(0, 0);
    grid_screen[idx] = CLOCK;
    grid_color[idx] = 1;
}
/// @brief creates a delay in a "bang"
void delay_b(){
    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = '.';
    grid_color[idx] = 3; 

    idx = get_index(1, 0);
    grid_color[idx] = 2;
    idx = get_index(-1, 0);
    grid_color[idx] = 2;

    idx = get_index(0, 0);
    grid_screen[idx] = DELAY;
    grid_color[idx] = 1;
}

/// @brief random # inclusive of min and max values
/// @param minx min value's index
/// @param maxx max value's index
void rando(uint8_t minx, uint8_t maxx){

    uint8_t mi = DIGIFY(grid_screen[minx]);
    uint8_t ma = DIGIFY(grid_screen[maxx]);
    grid_color[minx] = grid_color[maxx] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = random(DIGIFY(mi), DIGIFY(ma));
    grid_color[idx] = 3; 

    idx = get_index(0, 0);
    grid_screen[idx] = RAND;
    grid_color[idx] = 1; 
}

/// @brief increments until it reaches 'target', wraps around too
void lerp(){
    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = '0';
    grid_color[idx] = 3; 

    idx = get_index(1, 0);
    grid_color[idx] = 2;
    idx = get_index(-1, 0);
    grid_color[idx] = 2;

    idx = get_index(0, 0);
    grid_screen[idx] = LERP;
    grid_color[idx] = 1; 
}

/// @brief Increments by 'step' from [0, mod)
void increment(){
    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = '0';
    grid_color[idx] = 3; 

    idx = get_index(1, 0);
    grid_color[idx] = 2;
    idx = get_index(-1, 0);
    grid_color[idx] = 2;

    idx = get_index(0, 0);
    grid_screen[idx] = INC;
    grid_color[idx] = 1;
}

/***** Static ********************************************************/

/// @brief comments, we can handle this later
void comment(){

}

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
/// @param val index containing value
void jxmper(uint8_t val){
    uint8_t res = grid_screen[val];
    grid_color[val] = 2;

    uint8_t idx = get_index(1, 0);
    grid_screen[idx] = res;
    grid_color[idx] = 3;

    idx = get_index(0, 0);
    grid_screen[idx] = JXMP;
    grid_color[idx] = 1;
}

/// @brief Takes value from north and teleports it to south
/// @param val the value's index in question
void jymper(uint8_t val){
    uint8_t res = grid_screen[val];
    grid_color[val] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = res;
    grid_color[idx] = 3;

    idx = get_index(0, 0);
    grid_screen[idx] = JYMP;
    grid_color[idx] = 1;
}

/// @brief grid[a] + grid[b] => result
/// @param a index to number
/// @param b index to number
void add(uint8_t a, uint8_t b){

    uint8_t aval = grid_screen[a] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[a]);
    uint8_t bval = grid_screen[b] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[b]);
    grid_color[a] = grid_color[b] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = UNDIGIFY(aval + bval);
    grid_color[idx] = 3; 

    idx = get_index(0, 0);
    grid_screen[idx] = ADD;
    grid_color[idx] = 1;
}

/// @brief grid[a] - grid[b] => result
/// @param a index to number
/// @param b index to number
void sub(uint8_t a, uint8_t b){
    
    uint8_t aval = grid_screen[a] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[a]);
    uint8_t bval = grid_screen[b] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[b]);
    grid_color[a] = grid_color[b] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = UNDIGIFY(aval - bval);
    grid_color[idx] = 3; 

    idx = get_index(0, 0);
    grid_screen[idx] = SUB;
    grid_color[idx] = 1;

}
/// @brief Multiplies a and b, then mod by base 36
/// @param a left value in question
/// @param b right value in question
void multiply(uint8_t a, uint8_t b){

    uint8_t aval = grid_screen[a] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[a]);
    uint8_t bval = grid_screen[b] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[b]);
    grid_color[a] = grid_color[b] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = UNDIGIFY(aval * bval);
    grid_color[idx] = 3; 

    idx = get_index(0, 0);
    grid_screen[idx] = MULT;
    grid_color[idx] = 1;
}

/// @brief output smaller value
/// @param a left val
/// @param b right val
void lesser(uint8_t a, uint8_t b){

    uint8_t aval = grid_screen[a] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[a]);
    uint8_t bval = grid_screen[b] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[b]);
    grid_color[a] = grid_color[b] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = UNDIGIFY(aval < bval ? aval : bval);
    grid_color[idx] = 3; 

    idx = get_index(0, 0);
    grid_screen[idx] = MIN;
    grid_color[idx] = 1;
}

/// @brief bang if a == b
/// @param a index to value
/// @param b index to value
void branch_if(uint8_t a, uint8_t b){

    uint8_t aval = grid_screen[a] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[a]);
    uint8_t bval = grid_screen[b] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[b]);
    grid_color[a] = grid_color[b] = 2;

    uint8_t idx = get_index(0, 1);
    grid_screen[idx] = aval == bval ? '*' : '.'; // edge case for frame updates
    grid_color[idx] = 3; 

    idx = get_index(0, 0);
    grid_screen[idx] = IF;
    grid_color[idx] = 1;
}

/// @brief creates/reads a variable
/// @param name idx
/// @param value idx
void variable(uint8_t name, uint8_t value){

    uint8_t aval = grid_screen[name];
    uint8_t bval = grid_screen[value];
    

    uint8_t idx = get_index(0, 0);
    grid_screen[idx] = VAR;
    grid_color[idx] = 1;
    grid_color[name] = grid_color[value] = 2;


    // if we're declaring or reading a variable
    if(variables[value] != 100){
        idx = get_index(0, 1);
        grid_screen[idx] = variables[bval];
        grid_color[idx] = 3; 
    }
    else{
        variables[aval] = DIGIFY(bval);
    }
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
    memory = grid_screen[(x*X_MAX)+y+1+id];
    grid_screen[(x*X_MAX)+y] = memory;
    grid_color[(x*X_MAX)+y] = 3; 
}
/// @brief writes eastward operand
/// @param key index
/// @param len # slots
/// @param val what to write
void push(uint8_t key, uint8_t len, uint8_t val){
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

/// @brief read cell given offset
/// @param row x offset
/// @param col y offset
void read(uint8_t row, uint8_t col){

    uint8_t x_off = DIGIFY(grid_screen[row])+1;
    uint8_t y_off = DIGIFY(grid_screen[col]);
    grid_color[row] = grid_color[col] = 2;

    uint8_t idx = get_index(0, 0);
    grid_screen[idx] = READ;
    grid_color[idx] = 1;

    idx = get_index(x_off, y_off);
    memory = grid_screen[idx]; // read
    grid_color[idx] = 2;

    idx = get_index(0, 1);
    grid_screen[idx] = memory; // write
    grid_color[idx] = 3; 
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
        memory = DIGIFY(grid_screen[(x+x_off)*X_MAX+y+y_off]);
        grid_color[(x+x_off)*X_MAX+y+y_off] = 2;

        // write the byte into result
        grid_screen[(start_r)*X_MAX+start_c] = memory;
        grid_color[(start_r)*X_MAX+start_c] = 3;
        start_c++;
    }
}
void write(uint8_t off_x, uint8_t off_y, uint8_t val){
    // update screen with operator
    grid_screen[x*X_MAX+y] = WRITE;
    grid_color[x*X_MAX+y] = 1; 
    grid_color[x*X_MAX+y-2] = grid_color[x*X_MAX+y-1] = grid_color[x*X_MAX+y+1] = 2; 

    // update screen with result & colors
    uint8_t x_off = DIGIFY(off_x);
    uint8_t y_off = DIGIFY(off_y); // one after operator

    // value to write
    memory = DIGIFY(val);

    // put value into offsetted box
    grid_screen[(x+1+y_off)*X_MAX+y+x_off] = memory;
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
    memory = DIGIFY(len);
    uint8_t xv = DIGIFY(x_v);
    uint8_t yv = DIGIFY(y_v);
    for(uint8_t i = 0; i < memory; i++){
        grid_screen[((x+yv)*X_MAX)+(xv)+i] = grid_screen[x*X_MAX+y+i+1]; 
        grid_color[x*X_MAX+y+i+1] = 2;
        grid_color[((x+yv)*X_MAX)+(xv)+i] = 3; 
    }
}

/***** All things SPECIAL defined here ************************(******/

void self(char cmd){

}
void midi(char ch, char oct, char note, char velocity){

}
void midi_cc(char ch, char knob, char val){

}