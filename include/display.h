#ifndef DISPLAY_H
#define DISPLAY_H
#include <config.h>

/**********************************************************************
 * display.h 
 * 
 * This header will contain global data, declarations, and definitions 
 * required or wanted for the OLED display. The OLED display will be
 * separate from the Serial Monitor, but the input through Serial will
 * be visualized in a special way via the OLED
**********************************************************************/

extern int x; // x coordinate
extern int y; // y coordinate
extern int memory; // a register
extern char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH]; // what will be displayed on the screen

void init_grid();

#endif