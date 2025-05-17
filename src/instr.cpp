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
extern bool bangers[Y_MAX * X_MAX];
extern int variables[36];
extern uint8_t cycle;
extern int x, y;
extern int memory;

/***** All things BASIC defined here *********************************/

/***** Dynamic *******************************************************/

/// @brief bangs once, updates bangers[]
/// @param index index
void bang(uint8_t index){
    if(bangers[index] == true){
        grid_color[index] = 0;
        grid_screen[index] = NOP;
        bangers[index] = false;
    }
    else {
        grid_color[index] = 1;
        bangers[index] = true;
    }
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
    if(bangers[index]){ // already banged
        bangers[index] = false;
        grid_screen[index] = NOP;
        if(i+1 < X_MAX){
            grid_screen[index+1] = RIGHT;
            bangers[index+1] = true;
        }
    }
    else{ // not yet banged
        bangers[index] = true;
    }
}
/// @brief moves an 'E' to the right until it goes off screen
/// @param i index 
/// @param j index 
void north(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    if(bangers[index]){ // already banged
        bangers[index] = false;
        grid_screen[index] = NOP;
        if(j-1 >= 0){
            grid_screen[(j-1)*X_MAX + i] = UP;
            bangers[(j-1)*X_MAX + i] = true;
        }
    }
    else{ // not yet banged
        bangers[index] = true;
    }
}
/// @brief moves an 'S' downwards until it goes off screen
/// @param i index 
/// @param j index 
void south(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    if(bangers[index]){ // already banged
        bangers[index] = false;
        grid_screen[index] = NOP;
        if(j+1 < Y_MAX){
            grid_screen[(j+1)*X_MAX + i] = DOWN;
            bangers[(j+1)*X_MAX + i] = true;
        }
    }
    else{ // not yet banged
        bangers[index] = true;
    }
}
/// @brief moves an 'W' leftwards until it goes off screen
/// @param i index 
/// @param j index 
void west(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;

    if(bangers[index]){ // already banged
        bangers[index] = false;
        grid_screen[index] = NOP;
        if(i-1 >= 0){
            grid_screen[index-1] = LEFT;
            bangers[index-1] = true;
        }
    }
    else{ // not yet banged
        bangers[index] = true;
    }
}

/// @brief counts from 0 to the max value specified in right
/// @param i index
/// @param j index
void clock(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    uint8_t c_max = j*X_MAX + i+1;
    uint8_t c_result = (j+1)*X_MAX + i;
    uint8_t rate = j*X_MAX + i-1; // don't worry about this for now
    if(!ISB36(grid_screen[c_result])){
        grid_screen[c_result] = '0';
    }
    if(grid_screen[c_result] + 1 > grid_screen[c_max]){
        grid_screen[c_result] = '0';
    }
    else{
        grid_screen[c_result] += 1;
    }
    grid_color[index] = 1; 
    grid_color[c_max] = grid_color[rate] = 2;
    grid_color[c_result] = 3;
}
/// @brief creates a delay in a "bang"
void delay_b(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t left = j*X_MAX + i-1;
    uint8_t right = j*X_MAX + i+1; // rate
    uint8_t result = (j+1)*X_MAX + i;

    uint8_t curr = cycle % memory;
    uint8_t post = (cycle-1) % memory;

    if(curr == 0){ // bang
        grid_screen[result] = BANG;
        grid_color[result] = 3;
    }
    else if(post == 0){ // revert the bang
        grid_screen[index] = NOP;
        grid_color[result] = 0;
    }

    grid_color[index] = 1; 
    grid_color[left] = grid_color[right] = 2;
    grid_color[result] = 3;
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

/// @brief increments until it reaches 'target'
/// @param i index
/// @param j index
void lerp(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    uint8_t lt = DIGIFY(grid_screen[index-1]);
    uint8_t lc = DIGIFY(grid_screen[index+1]);

    if (lc < lt) lc++;
    grid_screen[(j+1)*X_MAX + i] = UNDIGIFY(lc);

    grid_color[index] = 1; 
    grid_color[grid_screen[index+1]] = grid_color[grid_screen[index-1]] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
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
    uint8_t aval = (j-1)*X_MAX + i; // west
    uint8_t bval = (j+1)*X_MAX + i; // east
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

/// @brief executes subtraction instruction
/// @param i index 
/// @param j index 
void sub(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t aval = grid_screen[index-1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index-1]);
    uint8_t bval = grid_screen[index+1] == '.' ? DIGIFY('0') : DIGIFY(grid_screen[index+1]);

    grid_screen[(j+1)*X_MAX + i] = UNDIGIFY((aval - bval) % 36);

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
    uint8_t aval = grid_screen[index-1];
    uint8_t bval = grid_screen[index+1];
    char result = aval == bval ?'*' : '.';
    grid_screen[(j+1)*X_MAX + i] = result;
    
    grid_color[index-1] = grid_screen[index+1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
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