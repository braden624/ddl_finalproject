#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>


void set_alarm (void) {

}

void set_clock (void) {

}

void stopwatch (void) {

}

void init_LCD(void) {
    // Config pins for RS, R/W, E, DB0-7
    // wait 4ms
    // Write command 0x38
    // Write command 0x06
    // Write command 0x0c, 0x0e, or 0x0f 
    // Write command 0x01
    // Wait 4 ms

    // Define custom characters
    // Write command 0x40
    // Write data for each row
    // Write cursor command 0x80
    // Repeat last 3 line for all custom characters, addresses: 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78

}

void write_LCD_command(/*command*/) {
    // Set DB0-7 to match command
    // Drive read/write low
    // Drive RS low
    // Drive E low
    // wait 100 ms 
}

void write_LCD_data(/*data*/) {
    // Set DB0-7 to match data
    // Drive read/write low
    // Drive RS high
    // Drive E low
    // wait 100 ms
}

int main(void) {

    init_LCD();
    
    while(1) {
        
    }
    return 0 ;
}