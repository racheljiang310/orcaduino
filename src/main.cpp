#include "instr.h"
#include "display.h"
#include <Arduino.h>

// control
bool auto_mode = true; // start in auto mode
uint64_t last_step = 0; // previous step
const uint64_t step_delay = 500; // tweak as needed

// 2D grid with characters
char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];

// 2D grid with colors: 0: nothing | 1: operation | 2: operands | 3: result | 4: comments
uint8_t grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];

// PC
int x, y;
// somewhat of a "register"
int memory;

// put function declarations here:
bool check_bounds(int r, int c);
bool check_instruction(char instruction);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  x = X_INIT; 
  y = Y_INIT; 
  init_grid(); // setup grid

}

void loop() {
  // put your main code here, to run repeatedly:
  check_bounds(x, y); // check bounds
  
  if (Serial.available()) {
    char c = Serial.read();
    char left = grid_screen[x*SCREEN_WIDTH+y-1];
    char right = grid_screen[x*SCREEN_WIDTH+y+1];
    char up = grid_screen[(x-1)*SCREEN_WIDTH+y];
    char down = grid_screen[(x+1)*SCREEN_WIDTH+y];
    
    switch (c){
      case ADD:
        add(left, right);
        break;
      case SUB:
        sub(left, right);
        break;
      case CLOCK:
        clock(left, right); // ops not needed for this foo
        break;
      case DELAY:
        delay_b(left, right); // ops not needed for this foo
        break;
      case RIGHT:
        east();
      default:
        // if it's a number, then store it
        if(c >= '0' && c <= '9'){
          grid_screen[x*SCREEN_WIDTH+y] = c;
        }
        break;
    }
  }

  if (auto_mode && ((millis() - last_step) > step_delay)) {
    // executeCurrentCommand();
    last_step = millis();
  }

  delay(step_delay);
  // draw grid
}

// put function definitions here:
bool check_bounds(int r, int c){
  if(c >= SCREEN_HEIGHT || c < 0){ return false;}
  if(r >= SCREEN_WIDTH || r < 0){ return false;}
  return true;
}
bool check_instruction(char instruction){
  if(instruction >= '0' && instruction <= '9'){
    return true;
  }
  return true;
}