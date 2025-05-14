#include "frame.h"
#include "stdint.h"
#include "config.h"
#include <Arduino.h>

extern int x; 
extern int y; 
extern int memory;
extern char grid_screen[SCREEN_HEIGHT * SCREEN_WIDTH];
extern uint8_t grid_color[SCREEN_HEIGHT * SCREEN_WIDTH];

void update_frame(){
    uint8_t boost = SCREEN_WIDTH;
    for(uint8_t row = 0; row < SCREEN_WIDTH; row++){
        for(uint8_t col = 0; col < SCREEN_HEIGHT; col++){
            if(ISOP(grid_screen[row*boost + col])){
                switch(grid_screen[row*boost + col]){
                    case BANG:
                        Serial.println("BANG");
                        break;
                    default:
                        break;
                }

            }
        }   
    }
}