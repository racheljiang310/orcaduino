#ifndef CONFIG_H
#define CONFIG_H

/**********************************************************************
 * config.h 
 * 
 * This header will contain all macros and #define values, mostly will
 * act as a "mux" so it's easier to parse/interpret through instructions
 * essentially anything hardcoded will go right here. lmao.............
**********************************************************************/

/***** All things OLED defined here *********************************/
#define TFT_CS        10U
#define TFT_RST       8U
#define TFT_DC        9U
#define TFT_WIDE      160U
#define TFT_HIGH      128U

#define X_MAX          8
#define Y_MAX          8
#define LCD_RESET     -1

/***** Startup Defaults *********************************************/

// Default: (x,y) = (0,0): Top, Left of the OLED Screen
#define X_INIT     0
#define Y_INIT     0
#define NOP        '.' 

/***** Display Settings ********************************************/
#define TFT_CS     10
#define TFT_DC     9
#define TFT_RST    8

/***** System Commands **********************************************/

// Helpful Conversion Macros
#define NUM_DIGIFY(c) (c - 48U)  // don't use
#define CHAR_DIGIFY(c) (c - 87U) // don't use

#define UNDIGIFY(c) (c >= 0 && c <= 9) ? (c + 48U) : (c+87U) // base36 => char
#define DIGIFY(c) (c >= '0' && c <= '9') ? (NUM_DIGIFY(c)) : (CHAR_DIGIFY(c)) // char => base36
#define ISOP(c) ((c >= 'A' && c <= 'Z') || (c == '*') || (c == '#')) // indicates instruction
#define ISB36(c) (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') // if it belongs in base36

// Arithmetic Operations
#define ADD        'A'
#define SUB        'B'
#define IF         'F'
#define CLOCK      'C'
#define DELAY      'D' 
#define RIGHT      'E'
#define GENER      'G' 
#define HALT       'H' 
#define INC        'I' 
#define KONCAT     'K' 
#define MIN        'L'
#define MULT       'M'
#define UP         'N'
#define READ       'O'
#define PUSH       'P' 
#define RAND       'R' 
#define DOWN       'S'
#define TRACK      'T' 
// #define UCLID      'U' 
#define VAR        'V' 
#define LEFT       'W' 
#define LERP       'Z' 
#define BANG       '*' 
#define COMMENT    '#' 
#define JYMP       'J' 
#define JXMP       'Y' 
#define WRITE      'X' 
#define QUERY      'Q' 

#endif