#ifndef DISPLAY_H
#define DISPLAY_H

/**********************************************************************
 * display.h 
 * 
 * This header will contain global data, declarations, and definitions 
 * required or wanted for the OLED display. The OLED display will be
 * separate from the Serial Monitor, but the input through Serial will
 * be visualized in a special way via the OLED
**********************************************************************/
void init_grid();
void init_bangers();
// idea: surround OLED with LEDs, bang x and y mapping, otherwise, if it moves towards a direction, blink NWSE until run out of LEDS?
#endif