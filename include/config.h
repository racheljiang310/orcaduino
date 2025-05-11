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
#define SCREEN_WIDTH    8
#define SCREEN_HEIGHT   8
#define OLED_RESET     -1

/***** Startup Defaults *********************************************/

// Default: (x,y) = (0,0): Top, Left of the OLED Screen
#define X_INIT     0
#define Y_INIT     0
#define NOP        '.' 

/***** System Commands **********************************************/

// Arithmetic Operations
#define ADD        'A' // [IN @ (x-1, y)] + [IN @ (x+1, y)] => [OUT] @ (x, y+1)
#define SUB        'B' // [IN @ (x-1, y)] - [IN @ (x+1, y)] => [OUT] @ (x, y+1)
#define IF         'F' // [IN @ (x-1, y)] == [IN @ (x+1, y)] => [OUT] @ (x, y+1) | if 1 * else .

#define CLOCK      'C' // [R @ (x-1, y)] C [M @ (x+1, y)] => [OUT] @ (x, y+1) | [0:M) @ 1/R rate
#define DELAY      'D' // [RATE] D [MAX] => BANG on MODE of frame
#define RIGHT      'E' // EAST

#define GENER      'G' // generator (x, y, len)
#define HALT       'H' // HALT whatever @ (x, y + 1)
#define INC        'I' // increment @ (x, y + 1)

#define KONCAT     'K' // read multiple variables
#define MIN        'L' // OUT min(inputs[])
#define MULT       'M' // OUT in[1] x in[2]
#define UP         'N' // NORTH
#define READ       'O' // (x, y, read)
#define PUSH       'P' // writes RIGHT operand
#define RAND       'R' // outputs random vlaue (min, max)
#define DOWN       'S' // move pc @ (x, y+1)
#define TRACK      'T' // reads operand with offset (x, y, len)
#define UCLID      'U' // (step max), bangs on euclidean rhythm
#define VAR        'V' // (write, read) reads and writes variables
#define LEFT       'W' // moves WEST aka left

#define LERP       'Z' // (rate, target) transitions operand to input
#define BANG       '*' // bangs neighboring operands
#define COMMENT    '#' // halts a line

// Teleportation Operations [OOH AHH]
#define JYMP       'J' // PUT (x, y-1) => (x, x, y+1) Moves top element to bottom
#define JXMP       'Y' // PUT (x-1, y) => (x+1, y) Moves left element to right
#define WRITE      'X' // [x-off] [y-off] X [val] => Moves element to offset
#define QUERY      'Q' // [x-off] [y-off] [# bytes] Q => Reads elements from offset

#endif