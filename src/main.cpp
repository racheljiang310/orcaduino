#include "config.h"
#include "instr.h"
#include "frame.h"
#include "display.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define DEBUG 0

/***** All things CONTROLS *********************************/

int x, y; // col, row
uint8_t cycle = 0; // 8 average
uint64_t last_step = 0; // previous step
const uint64_t step_delay = 10; // tweak as needed

/* 
  colors: 0: nothing | 1: operation | 2: operands | 
          3: result  | 4: cursor    | 5: comments | 
*/
uint8_t grid_color[Y_MAX * X_MAX];
uint8_t bangers[Y_MAX * X_MAX];  // controls bang timing

/***** All things CONTANTS *********************************/
char variables[36];  // stores vars
char grid_screen[Y_MAX * X_MAX]; // 2D grid with characters

/***** All things DISPLAY **********************************/
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

/***** All things FUNC DECL ********************************/
void draw_grid();
void setup_display();

bool check_bounds(int r, int c);
bool check_instruction(char instruction);

uint8_t get_index(int x_pos, int y_pos);
/***** All things FUNC DEFN ********************************/

/// @brief setup
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  x = X_INIT; 
  y = Y_INIT; 
  init_grid(); // setup grid
  init_bangers(); // setup bangers
  setup_display();
  Serial.println("Setup Complete...");
}

/// @brief main loop
void loop() {
  check_bounds(x, y); // check bounds
  uint8_t index = y*X_MAX+x;
  if (Serial.available()) {
    char c = Serial.read();

    #if DEBUG == 1 
    Serial.println(c);
    #endif

    if(c == '[') x = (x - 1 < 0) ? 0 : x-1;
    else if(c == ']') x = (x + 1 == X_MAX) ? x : x+1;
    else if(c == '=') y = (y - 1 < 0) ? 0 : y-1;
    else if(c == '\'') y = (y + 1 == Y_MAX) ? y : y+1;
    else if((c >= '0' && c <= '9' ) || (c >= 'a' && c <= 'z')) grid_screen[index] = c;
    else if (ISOP(c) && check_instruction(c) == 1)  {
      if (c == RIGHT || c == LEFT || c == BANG || c == UP || c == DOWN){
        bangers[index] = 1;
      }
      grid_screen[index] = c; 
    }
    else if(c == '.'){
      grid_screen[index] = c; 
    }
    else {
      Serial.print(c);
      Serial.println(" => Invalid instruction");
    }

  }
  if ((millis() - last_step) > step_delay) {
    update_frame();
    draw_grid();
    last_step = millis();
  }
  cycle = (cycle + 1) % 8;
}

/// @brief Draws the grid and displays on LCD
void draw_grid() {
  int16_t cellW = TFT_WIDE / X_MAX; // vertical groups spacing
  int16_t cellH = TFT_HIGH / Y_MAX; // horizontal groups spacing
  tft.setTextWrap(false);

  for (int i = 0; i < X_MAX; i++) {
    for (int j = 0; j < Y_MAX; j++) {
      int16_t index = j * X_MAX + i;
      char val = grid_screen[index];
      uint8_t color = grid_color[index];

      if(x == i && y == j){
        tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
      }
      else if (color == 1){
        tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
      }
      else if (color == 2){
        tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
      }
      else if (color == 3){
        tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
      }
      else {
        tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
      }

      int16_t x0 = i * cellW;
      int16_t y0 = j * cellH;

      tft.setCursor(x0, y0);
      tft.print(val);
    }
  }
}

/// @brief configurations for LCD
void setup_display(){
  tft.setSPISpeed(8000000); 
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN);

  // control logic
  int16_t ldc_max_x = tft.width(); // horizontal length
  int16_t ldc_max_y = tft.height(); // vertical length

  #if DEBUG == 1
  char buffer[30];
  sprintf(buffer, "TFT: x = %d, y = %d", ldc_max_x, ldc_max_y);
  Serial.println(buffer);
  #endif

  draw_grid();
}

/// @brief checks that cursor location isn't OOB
/// @param r x
/// @param c y
/// @return true of good, false if OOB
bool check_bounds(int r, int c){
  if(c >= Y_MAX || c < 0){ return false;}
  if(r >= X_MAX || r < 0){ return false;}
  return true;
}

/// @brief fetches the grid index based on offset
// @detail bc grid is 1D, ++ when walking the row
/// @param x_pos offset for x 
/// @param y_pos offset for y
/// @return 
uint8_t get_index(int x_pos, int y_pos){
  return ((y+y_pos)*X_MAX) + x+x_pos;
}

/// @brief verifies the bounds are possible
/// @param instruction 
/// @return instruction enum
bool check_instruction(char instruction){
  uint8_t a, b, c;
  uint8_t index = y*X_MAX+x;
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
      return (y + 1 < Y_MAX) && (x+1 < X_MAX) && (x-1 >= 0);
    case RIGHT:
    case UP:
    case DOWN:
    case LEFT:
    case BANG:
      return (y >= 0) && (y < Y_MAX) && (x < Y_MAX) && (x >= 0);
    case GENER:
      bool basic = x-3 >= 0;
      a = grid_screen[index-1] == '.' ? 0 : DIGIFY(grid_screen[index-1]);
      basic &= (x+a+1 < X_MAX);
      b = grid_screen[index-2] == '.' ? 0 : DIGIFY(grid_screen[index-2]);
      basic &= (y+b+1 < Y_MAX);      
      return basic;
    case HALT:
      return (y + 1 < Y_MAX);
    case JYMP:
      return (y + 1 < Y_MAX) && (y - 1 >= 0);
    case KONCAT:
      a = grid_screen[index-1] == '.' ? 0 : DIGIFY(grid_screen[index-1]);
      return (y+1 < Y_MAX) && (x-1 >= 0) && (x+a+1 < X_MAX);
    case VAR:
    case JXMP:
      return (x-1 >= 0) && (x+1 < Y_MAX);
    case PUSH:
      a = grid_screen[index-1] == '.' ? 0 : DIGIFY(grid_screen[index-1]);
      return (x-2 >= 0) && (x+a < X_MAX) && (y+1 < Y_MAX);
    case READ: 
      a = grid_screen[index-1] == '.' ? 0 : DIGIFY(grid_screen[index-1]);
      b = grid_screen[index-2] == '.' ? 1 : DIGIFY(grid_screen[index-2]);
      return (x-2 >= 0) && (y+1 < Y_MAX) && (x+b < X_MAX) && (y+a < Y_MAX);
    case QUERY:
      a = grid_screen[index-1] == '.' ? 1 : DIGIFY(grid_screen[index-1]);
      b = grid_screen[index-2] == '.' ? 0 : DIGIFY(grid_screen[index-2]);
      c = grid_screen[index-3] == '.' ? 0 : DIGIFY(grid_screen[index-3]);
      return (y+1 < Y_MAX) && (x-3 >= 0) 
            && (x+c+a < X_MAX) && (y+b < Y_MAX);
    case TRACK:
      a = grid_screen[index-1] == '.' ? 1 : DIGIFY(grid_screen[index-1]);
      return (x-2 >= 0) && (y+1 < Y_MAX) && (x + a < X_MAX);
    case WRITE:
      a = grid_screen[index-1] == '.' ? 1 : DIGIFY(grid_screen[index-1]);
      b = grid_screen[index-2] == '.' ? 0 : DIGIFY(grid_screen[index-2]);
      return (x-2 >= 0) && (x+1 < X_MAX) && (y+a< Y_MAX) && (x+b < X_MAX);
    default:
      break;
  }
  return true;
}