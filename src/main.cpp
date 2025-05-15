#include "config.h"
#include "instr.h"
#include "frame.h"
#include "display.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

#define DEBUG 1

// control
uint8_t cycle = 0; // 8 average
uint64_t last_step = 0; // previous step
const uint64_t step_delay = 500; // tweak as needed

int variables[36];

// 2D grid with characters
char grid_screen[Y_MAX * X_MAX];

// 2D grid with colors: 0: nothing | 1: operation | 2: operands | 3: result | 4: comments
uint8_t grid_color[Y_MAX * X_MAX];

// PC
int x, y; // for col #, row #
int memory;

// display
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
int16_t ldc_max_x, ldc_max_y;

// put function declarations here:
void draw_grid();
void setup_display();
bool check_bounds(int r, int c);
bool check_instruction(char instruction);
uint8_t get_index(int x_pos, int y_pos);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  x = X_INIT; 
  y = Y_INIT; 
  init_grid(); // setup grid
  setup_display();

}

void loop() {
  check_bounds(x, y); // check bounds
  if (Serial.available()) {
    char c = Serial.read();
    Serial.println(c);
    x = 5; y = 4;
    if (check_instruction(c)){
      switch (c){
        case ADD:
          add(get_index(-1, 0), get_index(1,0));
          break;
        case SUB:
          sub(get_index(-1, 0), get_index(1,0));
          break;
        case CLOCK:
          clock();
          break;
        case DELAY:
          delay_b();
          break;
        case RIGHT:
          east();
          break;
        case IF:
          branch_if(get_index(-1, 0), get_index(1,0));
          break;
        // case GENER:
        //   generator( op(-3, 0),  op(-2, 0), op(-1, 0));
        //   break;
        case HALT:
          halt();
          break;
        case INC:
          increment();
          break;
        case JYMP:
          jymper(get_index(0, -1));
          break;
        // case KONCAT:
        //   konkat(op(-1, 0));
        //   break;
        case MIN:
          lesser(get_index(-1, 0), get_index(1,0));
          break;
        case MULT:
          multiply(get_index(-1, 0), get_index(1,0));
          break;
        case UP:
          north();
          break;
        case READ:
          read(get_index(-2, 0), get_index(-1, 0));
          break;
        // case PUSH:
        //   push(op(-2, 0), op(-1, 0), op(1, 0));
        //   break;
        // case QUERY:
        //   query(op(-3, 0), op(-2, 0), op(-1, 0));
        //   break;
        case RAND:
          rando(get_index(-1, 0), get_index(1, 0));
          break;
        case DOWN:
          south();
          break;
        // case TRACK:
        //   track(op(-2, 0), op(-1, 0), op(1, 0)); // rightmost arg not needed really
        //   break;
        case VAR:
          variable(get_index(-1, 0), get_index(1,0));
          break;
        case LEFT:
          west();
          break;
        // case WRITE:
        //   write(op(-2, 0), op(-1, 0), op(1, 0));
        //   break;
        case JXMP:
          jxmper(get_index(-1, 0));
          break;
        case LERP:
          lerp();
          break;
        case BANG:
          bang();
          break;
        // case COMMENT:
        //   comment();
        //   break;
        default:
          break;
      }
    }
    else if((c >= '0' && c <= '9' )|| (c >= 'a' && c <= 'z')){
      grid_screen[x*X_MAX+y] = c;
    }
    else{
      Serial.println("Invalid instruction");
    }
  }

  if ((millis() - last_step) > step_delay) {
    update_frame();
    draw_grid();
    last_step = millis();
  }
  cycle = (cycle + 1) % 8;
  delay(100);
}

/// @brief Draws the grid and displays on LCD
void draw_grid() {
  int16_t cellW = TFT_WIDE / X_MAX; // vertical groups spacing
  int16_t cellH = TFT_HIGH / Y_MAX; // horizontal groups spacing

  tft.setTextSize(1);
  tft.setTextWrap(false);

  for (int i = 0; i < X_MAX; i++) {
    for (int j = 0; j < Y_MAX; j++) {
      int16_t index = j * X_MAX + i;
      char val = grid_screen[index];
      uint8_t color = grid_color[index];

      if (color == 1){
        tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
      }
      else if (color == 3){
        tft.setTextColor(ST77XX_BLACK, ST77XX_CYAN);
      }
      else if (color == 2){
        tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
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
  ldc_max_x = tft.width(); // horizontal length
  ldc_max_y = tft.height(); // vertical length

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

  // don't cares: digits
  if(!ISOP(instruction)){
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
      return (y-1 >= 0) && (y + 1 < Y_MAX) && (x+1 < Y_MAX);
    case RIGHT:
    case UP:
    case DOWN:
    case LEFT:
    case BANG:
    case COMMENT:
      return (y >= 0) && (y < Y_MAX) && (x < Y_MAX) && (x >= 0);
    case GENER:
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      bool basic = (y+1 < Y_MAX) && (x-3 >= 0) && (x+a < X_MAX);
      b = DIGIFY(grid_screen[x*X_MAX+y-3]);
      c = DIGIFY(grid_screen[x*X_MAX+y-2]);
      bool bounds = (x+b < X_MAX) && (y+c < Y_MAX);
      return basic && bounds;
    case HALT:
      return (y + 1 < Y_MAX);
    case JYMP:
      return (y + 1 < Y_MAX) && (y - 1 >= 0);
    case KONCAT:
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      return (y+1 < Y_MAX) && (x-1 >= 0) && (x+a < X_MAX);
    case VAR:
    case JXMP:
      return (x-1 >= 0) && (x+1 < Y_MAX);
    case PUSH:
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      return (x-2 >= 0) && (x+a-1 < X_MAX) && (y+1 < Y_MAX);
    case READ: 
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      b = DIGIFY(grid_screen[x*X_MAX+y-2]);
      return (x-2 >= 0) && (y+1 < Y_MAX) && (x+b < X_MAX) && (y + a < Y_MAX);
    case QUERY:
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      b = DIGIFY(grid_screen[x*X_MAX+y-2]);
      c = DIGIFY(grid_screen[x*X_MAX+y-3]);
      return (x-2 >= 0) && (y+1 < Y_MAX) && (x-a >= 0) && (x+c + a < X_MAX) && (y + b < Y_MAX);
    case TRACK:
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      return (x-2 >= 0) && (x+1 < X_MAX) && (y+1 < Y_MAX) && (x + a < X_MAX);
    case WRITE:
      a = DIGIFY(grid_screen[x*X_MAX+y-1]);
      b = DIGIFY(grid_screen[x*X_MAX+y-2]);
      return (x-2 >= 0) && (x+1 < X_MAX) && (y+1+a< Y_MAX) && (x + b < X_MAX);
    default:
      break;
  }
  return true;
}