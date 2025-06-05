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

extern void clear_LED();
extern void set_LED(int index, bool on);

/***** All things BASIC defined here *********************************/

/***** Dynamic *******************************************************/

/// @brief bangs once, updates bangers[]
/// @param index index
/// @param type 0 == regular, 3 == LED_color
void bang(uint8_t index, uint8_t type){
    grid_screen[index] = NOP;
    grid_color[index] = 0;
    if(type == 3){
        clear_LED();
    }
    else {
        set_LED(BANG_LED, 0);
    }
}

/// @brief moves an 'E' to the right until it goes off screen
/// @param i index 
/// @param j index 
void east(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(i+1 < X_MAX && grid_screen[index+1] == '.'){
        grid_screen[index+1] = RIGHT;
        bangers[index+1] = 1;
        set_LED(YELLOW_LED, HIGH);
    }
    else if (i + 1 == X_MAX){
        set_LED(YELLOW_LED, LOW);
    }
}
/// @brief moves an 'E' to the right until it goes off screen
/// @param i index 
/// @param j index 
void north(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(j-1 >= 0 && grid_screen[(j-1)*X_MAX + i] == '.'){
        grid_screen[(j-1)*X_MAX + i] = UP;
        set_LED(RED_LED, HIGH);
    }
    else if (j - 1 < 0){
        set_LED(RED_LED, LOW);
    }
}
/// @brief moves an 'S' downwards until it goes off screen
/// @param i index 
/// @param j index 
void south(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(j+1 < Y_MAX && grid_screen[(j+1)*X_MAX + i] == '.'){
        grid_screen[(j+1)*X_MAX + i] = DOWN;
        bangers[(j+1)*X_MAX + i] = 1;
        set_LED(BLUE_LED, HIGH);
    }
    else if (j+1 >= Y_MAX){
        set_LED(BLUE_LED, LOW);
    }
}
/// @brief moves an 'W' leftwards until it goes off screen
/// @param i index 
/// @param j index 
void west(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    grid_screen[index] = NOP;
    if(i-1 >= 0 && grid_screen[index-1] == '.'){
        grid_screen[index-1] = LEFT;
        set_LED(GREEN_LED, HIGH);
    }
    else if (i-1 < 0){
        set_LED(GREEN_LED, LOW);
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
    uint8_t c_max_val = !ISB36(grid_screen[c_max]) ? 10 : DIGIFY(grid_screen[c_max]);
    
    if(!ISB36(grid_screen[c_result])){
        grid_screen[c_result] = '0';
    }
    else{
        uint8_t current_val = !ISB36(grid_screen[c_result]) ? 0 : DIGIFY(grid_screen[c_result]);
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
    uint8_t left = index-1; // light color
    uint8_t right = index+1; // rate
    uint8_t result = (j+1)*X_MAX + i;

    uint8_t bang_rate = !ISB36(grid_screen[right]) ? 1 : DIGIFY(grid_screen[right]);
    uint8_t color = !ISB36(grid_screen[left]) ? 0 : (DIGIFY(grid_screen[left]) % 5);
    uint8_t curr = cycle % bang_rate;
    
    if(curr == 0){
        grid_screen[result] = BANG;
        switch(color){
            case 0:
                set_LED(BANG_LED, 0);
                set_LED(BANG_LED, 1);
                break;
            case 1:
                set_LED(BANG_LED, 0);
                set_LED(RED_LED, 1);
                break;
            case 2:
                set_LED(BANG_LED, 0);
                set_LED(GREEN_LED, 1);
                break;
            case 3:
                set_LED(BANG_LED, 0);
                set_LED(BLUE_LED, 1);
                break;
            case 4:
                set_LED(BANG_LED, 0);
                set_LED(YELLOW_LED, 1);
                break;
            default:
                set_LED(BANG_LED, 1);
                break;
        }
        grid_color[result] = 3;
        bangers[result] = 4;
    }
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

/// @brief increments until it reaches 'target' (Z)
/// @param i index
/// @param j index
void lerp(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;
    uint8_t c_start =  index-1;
    uint8_t c_max = index+1;
    uint8_t res = (j+1)*X_MAX + i;
    
    uint8_t c_max_val = ISB36(grid_screen[c_max]) ? DIGIFY(grid_screen[c_max]) : 0;
    char c_start_val = ISB36(grid_screen[c_start]) ? grid_screen[c_start] : '0';
    
    if(!ISB36(grid_screen[res]) || DIGIFY(grid_screen[res]) < DIGIFY(c_start_val)){
        grid_screen[res] = c_start_val;
    }
    else{
        uint8_t resulting_value = DIGIFY(grid_screen[res]);
        uint8_t next_val = min(resulting_value + 1, c_max_val);
        grid_screen[res] = UNDIGIFY(next_val);
    }
    grid_color[index] = 1; 
    grid_color[c_max] = grid_color[c_start] = 2;
    grid_color[res] = 3;
}

/// @brief Increments by 'left' from [0, 'right')
/// @param i index
/// @param j index
void increment(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t result = (j+1)*X_MAX + i;
    uint8_t step = ISB36(grid_screen[index - 1]) ? DIGIFY(grid_screen[index - 1]) : 1;
    uint8_t maxx = ISB36(grid_screen[index + 1]) ? DIGIFY(grid_screen[index + 1]) : DIGIFY('z');

    if(!ISB36(grid_screen[result])){
        grid_screen[result] = '0';
    }
    else {
        uint8_t current_val = !ISB36(grid_screen[result]) ? 0 : DIGIFY(grid_screen[result]);
        uint8_t next_val = (current_val + step) % maxx;
        grid_screen[result] = UNDIGIFY(next_val);
    }
    grid_color[index] = 1; 
    grid_color[index+1] = grid_color[index-1] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
}

/***** Static ********************************************************/

/// @brief [len] K [v1, v2, ...] where vars are read
/// @param i index
/// @param j index
void konkat(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t param = !ISB36(grid_screen[j*X_MAX + i - 1]) ? 1: DIGIFY(grid_screen[j*X_MAX + i - 1]);
    uint8_t vars = j*X_MAX + i + 1;
    uint8_t result = (j+1)*X_MAX + i + 1;
    grid_color[index] = 1; // operator
    grid_color[index-1] = 2; // operand

    for(uint8_t it = 0; it < param; it++){
        if(grid_screen[vars+it] != '.'){
            uint8_t var = DIGIFY(grid_screen[vars+it]);
            if((char)(variables[var]) != '?'){
                grid_screen[result+it] = (char)variables[var];
            }
            else{
                grid_screen[result+it] = '.';
            }
        }
        else{
            grid_screen[result+it] = '.';
        }
        grid_color[index+1+it] = 2; // operand
        grid_color[result+it] = 3; // result
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
    uint8_t aval = !ISB36(grid_screen[index-1]) ? 0 : DIGIFY(grid_screen[index-1]);
    uint8_t bval = !ISB36(grid_screen[index+1]) ? 0 : DIGIFY(grid_screen[index+1]);

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
    uint8_t aval = !ISB36(grid_screen[index-1]) ? 0 : DIGIFY(grid_screen[index-1]);
    uint8_t bval = !ISB36(grid_screen[index+1]) ? 0 : DIGIFY(grid_screen[index+1]);

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
    uint8_t aval = !ISB36(grid_screen[index-1]) ? 0 : DIGIFY(grid_screen[index-1]);
    uint8_t bval = !ISB36(grid_screen[index+1]) ? 0 : DIGIFY(grid_screen[index+1]);
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
    uint8_t aval = !ISB36(grid_screen[index-1]) ? 0 : DIGIFY(grid_screen[index-1]);
    uint8_t bval = !ISB36(grid_screen[index+1]) ? 0 : DIGIFY(grid_screen[index+1]);
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

    if(!ISB36(grid_screen[index+1]) && !ISB36(grid_screen[index-1])) return; // nothing

    uint8_t name = DIGIFY(grid_screen[index-1]);
    char value =grid_screen[index+1];

    if(variables[DIGIFY(value)] == '?' && ISB36(grid_screen[index-1])){ // var input, value exists => set
        variables[name] = value;
    }
    else if(variables[DIGIFY(value)] != '?' && !ISB36(grid_screen[index-1])){ // var exists => read
        grid_screen[(j+1)*X_MAX + i] = variables[DIGIFY(value)];
        grid_color[(j+1)*X_MAX + i] = 3;
        return;
    }
}

/// @brief read cell given offset
/// @bug: this exists in th original source code for orca as well, but sometimes there's a 1 cycle delay b4 output gets updated
/// @param i index
/// @param j index
void read(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t x_off = !ISB36(grid_screen[index-2]) ? 0 : DIGIFY(grid_screen[index-2]);
    uint8_t y_off = !ISB36(grid_screen[index-1]) ? 0 : DIGIFY(grid_screen[index-1]);

    uint8_t read_index = (j+y_off)*X_MAX + (i+x_off + 1);
    char value = grid_screen[read_index];
    grid_screen[(j+1)*X_MAX + i] = value;

    grid_color[index-1] = grid_color[index-2] = grid_color[read_index] = 2;
    grid_color[(j+1)*X_MAX + i] = 3;
    grid_color[index] = 1;
}

/// @brief ouputs note @ 'idx' from arbitrarily long 'val' of size 'len'
/// @param i index
/// @param j index
void track(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t idx = !ISB36(grid_screen[index-2]) ? 0 : DIGIFY(grid_screen[index-2]);
    uint8_t len = !ISB36(grid_screen[index-1]) ? 1 : DIGIFY(grid_screen[index-1]);
    char val = grid_screen[index+1+(idx % len)];

    grid_screen[(j+1)*X_MAX + i] = val;

    grid_color[index] = 1; 
    grid_color[(j+1)*X_MAX + i] = 3; 
    grid_color[index-2] = grid_color[index-1] = 2; 
    for (size_t x = 0; x < len; x++){
        grid_color[index+x+1] = 2;
    }
}
/// @brief writes value to an indexed section
/// @param i index
/// @param j index
void push(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t result = (j+1)*X_MAX + i;
    uint8_t key = !ISB36(grid_screen[index-2]) ? 0 : DIGIFY(grid_screen[index-2]);
    uint8_t len = !ISB36(grid_screen[index-1]) ? 1 : DIGIFY(grid_screen[index-1]);
    char val = grid_screen[index+1];
    uint8_t id = key % len;

    grid_screen[result+id] = val;

    grid_color[index] = 1; 
    grid_color[index-2] = grid_color[index-1] =grid_color[index+1] = 2; 
    grid_color[result+id] = 3; 
}

/// @brief reads values from offset & length.
/// @param i index
/// @param j index
void query(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t result = (j+1)*X_MAX + i;
    uint8_t x_off = ISB36(grid_screen[index-3]) ? 0 : DIGIFY(grid_screen[index-3]);
    uint8_t y_off = ISB36(grid_screen[index-2]) ? 0 : DIGIFY(grid_screen[index-2]);
    uint8_t len = ISB36(grid_screen[index-1]) ? 1 : DIGIFY(grid_screen[index-1]);

    for(size_t it = 0; it < len; it++){
        grid_screen[result-(len-it-1)] = grid_screen[(j+y_off)*X_MAX+(i+x_off+it+1)];
        grid_color[(j+y_off)*X_MAX+(i+x_off+it+1)] = 2;
        grid_color[result-(len-it-1)] = 3;
    }

    grid_color[index] = 1;
    grid_color[index-3] = grid_color[index-2] = grid_color[index-1] = 2; 
}

/// @brief writes value into slot
/// @param off_x 
/// @param off_y 
/// @param val 
void write(uint8_t i, uint8_t j){

    // fetch all the indices
    uint8_t index = j*X_MAX + i;
    uint8_t off_x = j*X_MAX + i-2; // x_offset
    uint8_t off_y = j*X_MAX + i-1; // y_offset
    char value = grid_screen[j*X_MAX + i+1]; // value to write
   

    // update screen with result & colors
    uint8_t x_off = ISB36(grid_screen[off_x]) ? 0 :DIGIFY(grid_screen[off_x]);
    uint8_t y_off = ISB36(grid_screen[off_y]) ? 0 :DIGIFY(grid_screen[off_y]);
    uint8_t result = (j+1+y_off)*X_MAX+i+x_off; // y_offset
    grid_screen[result] = value;
    
    // update color
    grid_color[index] = 1; 
    grid_color[result] = 3;
    grid_color[off_x] = grid_color[off_y] = grid_color[j*X_MAX + i+1] = 2; 
}

/// @brief [x|y|len|G|string of "len" size]
/// @param x right offset 
/// @param y down offset
/// @param len length of string
void generator(uint8_t i, uint8_t j){

    uint8_t index = j*X_MAX + i;
    uint8_t off_x = j*X_MAX + i-3; // x_offset
    uint8_t off_y = j*X_MAX + i-2; // y_offset
    uint8_t ldx = j*X_MAX + i-1; // len

    uint8_t length = !ISB36(grid_screen[ldx]) ?  1 : DIGIFY(grid_screen[ldx]);
    uint8_t x_off = !ISB36(grid_screen[off_x]) ? 0 :DIGIFY(grid_screen[off_x]);
    uint8_t y_off = !ISB36(grid_screen[off_y]) ? 0 :DIGIFY(grid_screen[off_y]);
    uint8_t base_idx = (j+1+y_off)*X_MAX+i+x_off;
    
    grid_color[index] = 1; 
    grid_color[off_x] = grid_color[off_y] = grid_color[ldx] = 2;
    for(size_t it = 0; it < length; it++){
        grid_screen[base_idx+it] = grid_screen[index+1+it]; 
        grid_color[index+1+it] = 2;
        grid_color[base_idx+it] = 3; 
    }
}

/***** All things SPECIAL defined here ************************(******/

/// @brief hits all directions like a firework
/// @param i 
/// @param j 
void starburst(uint8_t i, uint8_t j){
    uint8_t index = j*X_MAX + i;

    // set correct instructions
    grid_screen[index] = BANG;

    if(i+1 < X_MAX && grid_screen[index+1] == '.'){
        grid_screen[index+1] = RIGHT;
        grid_color[index+1] = 5;
    }
    
    if(i-1 >= 0 && grid_screen[index-1] == '.'){
        grid_screen[index-1] = LEFT;
        grid_color[index-1] = 4;
    }
    
    if(j-1 >= 0 && grid_screen[(j-1)*X_MAX + i] == '.'){
        grid_screen[(j-1)*X_MAX + i] = UP;
        grid_color[(j-1)*X_MAX + i] = 3;
    }

    if(j+1 < Y_MAX && grid_screen[(j+1)*X_MAX + i] == '.'){
        grid_screen[(j+1)*X_MAX + i] = DOWN;
        grid_color[(j+1)*X_MAX + i] = 2;
    }

    // set correct bangers
    bangers[index] = 1;
    bangers[index+1] = 1;
    bangers[(j+1)*X_MAX +i] = 1;
    set_LED(BANG_LED, 1);
}
// void self(char cmd){

// }
// void midi(char ch, char oct, char note, char velocity){

// }
// void midi_cc(char ch, char knob, char val){

// }