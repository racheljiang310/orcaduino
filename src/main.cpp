#include "config.h"
#include "instr.h"
#include "frame.h"
#include "display.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// control
uint8_t cycle = 0; // 8 average
uint64_t last_step = 0; // previous step
const uint64_t step_delay = 500; // tweak as needed

int variables[36];

// 2D grid with characters
char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];

// 2D grid with colors: 0: nothing | 1: operation | 2: operands | 3: result | 4: comments
uint8_t grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];

// PC
int x, y;
// somewhat of a "register"
int memory;

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// put function declarations here:
bool check_bounds(int r, int c);
bool check_instruction(char instruction);
char op(int x_pos = 0, int y_pos = 0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // pinMode(LED_BUILTIN, OUTPUT);
  tft.initR(); 
  delay(1000);
  tft.fillScreen(ST77XX_WHITE);
  delay(500);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(5);
  tft.setCursor(5, 5);
  tft.println("LCD OK!");
  
  x = X_INIT; 
  y = Y_INIT; 
  init_grid(); // setup grid

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, HIGH);  // turn on
  delay(500);
  digitalWrite(8, LOW);   // turn off
  delay(500);
  // digitalWrite(LED_BUILTIN, HIGH); // Turn on LED
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);  // Turn off LED
  // delay(1000);
  check_bounds(x, y); // check bounds
  if (Serial.available()) {
    char c = Serial.read();

    if (check_instruction(c)){
      switch (c){
        case ADD:
          add(op(-1,0), op(1,0));
          break;
        case SUB:
          sub(op(-1,0), op(1,0));
          break;
        case CLOCK:
          clock(op(-1, 0), op(1, 0)); // ops not needed for this foo
          break;
        case DELAY:
          delay_b(op(-1, 0), op(1, 0)); // ops not needed for this foo
          break;
        case RIGHT:
          east();
          break;
        case IF:
          branch_if(op(-1, 0), op(1, 0));
          break;
        case GENER:
          generator( op(-3, 0),  op(-2, 0), op(-1, 0));
          break;
        case HALT:
          halt();
          break;
        case INC:
          increment(op(-1, 0), op(1, 0)); // ops not needed for this foo
          break;
        case JYMP:
          jymper(op(0, -1));
          break;
        case KONCAT:
          konkat(op(-1, 0));
          break;
        case MIN:
          lesser(op(-1, 0), op(1, 0));
          break;
        case MULT:
          multiply(op(-1, 0), op(1, 0));
          break;
        case UP:
          north();
          break;
        case READ:
          read(op(-2, 0), op(-1, 0));
          break;
        case PUSH:
          push(op(-2, 0), op(-1, 0), op(1, 0));
          break;
        case QUERY:
          query(op(-3, 0), op(-2, 0), op(-1, 0));
          break;
        case RAND:
          rando(op(-1, 0), op(1, 0));
          break;
        case DOWN:
          south();
          break;
        case TRACK:
          track(op(-2, 0), op(-1, 0), op(1, 0)); // rightmost arg not needed really
          break;
        case VAR:
          variable(op(-1, 0), op(1, 0));
          break;
        case LEFT:
          west();
          break;
        case WRITE:
          write(op(-2, 0), op(-1, 0), op(1, 0));
          break;
        case JXMP:
          jxmper(op(-1, 0));
          break;
        case LERP:
          lerp(op(-1, 0), op(1, 0)); // ops not needed for this foo
          break;
        case BANG:
          bang();
          break;
        case COMMENT:
          comment();
          break;
        default:
          // if it's a numerically represented value
          if((c >= '0' && c <= '9' )|| (c >= 'a' && c <= 'z')){
            grid_screen[x*SCREEN_WIDTH+y] = c;
          }
          break;
      }
    }
    else{
      Serial.println("Invalid instruction");
    }
  }

  if ((millis() - last_step) > step_delay) {
    update_frame();
    last_step = millis();
  }
  cycle = (cycle + 1) % 8;
  delay(100);
}

/// @brief checks that cursor location isn't OOB
/// @param r x
/// @param c y
/// @return true of good, false if OOB
bool check_bounds(int r, int c){
  if(c >= SCREEN_HEIGHT || c < 0){ return false;}
  if(r >= SCREEN_WIDTH || r < 0){ return false;}
  return true;
}

/// @brief fetches the character at position wrp cursor loc
/// @param x_pos 
/// @param y_pos 
/// @return 
char op(int x_pos = 0, int y_pos = 0){
  return grid_screen[(x+x_pos)*SCREEN_WIDTH+y+y_pos];
}

/// @brief verifies the bounds are possible
/// @param instruction 
/// @return instruction enum
bool check_instruction(char instruction){

  // don't cares: digits
  if(instruction >= '0' && instruction <= '9'){
    return true;
  }
  uint8_t a, b, c;
  // typically capital letters
  switch (instruction){
    case ADD:
    case SUB:
    case CLOCK:
    case DELAY:
    case IF:
    case INC:
    case MIN:
    case MULT:
    case RAND:
    case LERP:
      return (y-1 >= 0) && (y + 1 < SCREEN_HEIGHT) && (x+1 < SCREEN_HEIGHT);
    case RIGHT:
    case UP:
    case DOWN:
    case LEFT:
    case BANG:
    case COMMENT:
      return (y >= 0) && (y < SCREEN_HEIGHT) && (x < SCREEN_HEIGHT) && (x >= 0);
    case GENER:
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      bool basic = (y+1 < SCREEN_HEIGHT) && (x-3 >= 0) && (x+a < SCREEN_WIDTH);
      b = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-3]);
      c = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-2]);
      bool bounds = (x+b < SCREEN_WIDTH) && (y+c < SCREEN_HEIGHT);
      return basic && bounds;
    case HALT:
      return (y + 1 < SCREEN_HEIGHT);
    case JYMP:
      return (y + 1 < SCREEN_HEIGHT) && (y - 1 >= 0);
    case KONCAT:
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      return (y+1 < SCREEN_HEIGHT) && (x-1 >= 0) && (x+a < SCREEN_WIDTH);
    case VAR:
    case JXMP:
      return (x-1 >= 0) && (x+1 < SCREEN_HEIGHT);
    case PUSH:
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      return (x-2 >= 0) && (x+a-1 < SCREEN_WIDTH) && (y+1 < SCREEN_HEIGHT);
    case READ: 
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      b = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-2]);
      return (x-2 >= 0) && (y+1 < SCREEN_HEIGHT) && (x+b < SCREEN_WIDTH) && (y + a < SCREEN_HEIGHT);
    case QUERY:
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      b = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-2]);
      c = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-3]);
      return (x-2 >= 0) && (y+1 < SCREEN_HEIGHT) && (x-a >= 0) && (x+c + a < SCREEN_WIDTH) && (y + b < SCREEN_HEIGHT);
    case TRACK:
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      return (x-2 >= 0) && (x+1 < SCREEN_WIDTH) && (y+1 < SCREEN_HEIGHT) && (x + a < SCREEN_WIDTH);
    case WRITE:
      a = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-1]);
      b = DIGIFY(grid_screen[x*SCREEN_WIDTH+y-2]);
      return (x-2 >= 0) && (x+1 < SCREEN_WIDTH) && (y+1+a< SCREEN_HEIGHT) && (x + b < SCREEN_WIDTH);
    default:
      break;
  }
  return true;
}

// #include <Adafruit_GFX.h>
// #include <Adafruit_ST7735.h>
// #include <SPI.h>

// #define TFT_CS        10U
// #define TFT_RST       8U
// #define TFT_DC        9U

// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// void setup() {
//   tft.setSPISpeed(8000000); 
//   tft.initR();
//   tft.fillScreen(ST77XX_BLACK);
//   tft.setCursor(10, 10);
//   tft.setTextColor(ST77XX_WHITE);
//   tft.setTextSize(1);
//   tft.println("Hello ST7735!");
// }

// void loop() {
// }