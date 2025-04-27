#include "display.h"
#include <Arduino.h>

// control
bool auto_mode = true; // start in auto mode
uint64_t last_step = 0; // previous step
const uint64_t step_delay = 500; // tweak as needed

// 2D grid
char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];
int grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];
// 0: nothing | 1: operation | 2: operands | 3: comments

// PC
int x, y;
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
    switch (c){
      case ADD:
        memory = grid_screen[x*SCREEN_WIDTH+y-1] + grid_screen[x*SCREEN_WIDTH+y+1];
        grid_screen[x*SCREEN_WIDTH+y] = ADD;
        grid_screen[((x+1)*SCREEN_WIDTH)+y] = memory;
        break;
      case SUB:
        memory = grid_screen[x*SCREEN_WIDTH+y-1] - grid_screen[x*SCREEN_WIDTH+y+1];
        grid_screen[x*SCREEN_WIDTH+y] = SUB;
        grid_screen[((x+1)*SCREEN_WIDTH)+y] = memory;
        break;
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