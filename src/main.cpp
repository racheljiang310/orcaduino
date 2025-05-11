#include "instr.h"
#include "display.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define BACKLIGHT_PIN 3
#define TFT_CS     10
#define TFT_DC     9
#define TFT_RST    8

// control
bool auto_mode = true; // start in auto mode
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

// LDC
#define TFT_CS     10
#define TFT_RST    9  // Or set to -1 and connect to Arduino RESET
#define TFT_DC     8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// put function declarations here:
bool check_bounds(int r, int c);
bool check_instruction(char instruction);

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
    char super_left = grid_screen[x*SCREEN_WIDTH+y-2];
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
        break;
      case IF:
        branch_if(left, right);
        break;
      case GENER:
        generator(super_left, left, right);
        break;
      case HALT:
        halt();
        break;
      case INC:
        increment(left, right); // ops not needed for this foo
        break;
      case JYMP:
        jymper(up);
        break;
      case KONCAT:
        konkat(left);
        break;
      case MIN:
        lesser(left, right);
        break;
      case MULT:
        multiply(left, right);
        break;
      case UP:
        north();
        break;
      case READ:
        read(super_left, left);
        break;
      case PUSH:
        push(super_left, left, right);
        break;
      case QUERY:
        query(grid_screen[x*SCREEN_WIDTH+y-3], super_left, left);
        break;
      case RAND:
        rando(left, right);
        break;
      default:
        // if it's a numerically represented value
        if((c >= '0' && c <= '9' )|| (c >= 'a' && c <= 'z')){
          grid_screen[x*SCREEN_WIDTH+y] = c;
        }
        break;
    }
  }

  // if (auto_mode && ((millis() - last_step) > step_delay)) {
  //   // executeCurrentCommand();
  //   last_step = millis();
  // }

  delay(100);
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