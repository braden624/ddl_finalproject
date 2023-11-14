#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

// bit pattern
char custom0[] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111};
char custom1[] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100};
char custom2[] = {0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111};
char custom3[] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11111, 0b11111, 0b11111};
char custom4[] = {0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b11111, 0b11111, 0b11111};
char custom5[] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00111, 0b00111, 0b00111};
char custom6[] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11100, 0b11100, 0b11100};
char custom7[] = {0b00000, 0b00000, 0b01110, 0b01110, 0b01110, 0b00000, 0b00000, 0b00000};


// number data
char zero[] = {0x00, 0x00, 0x01, 0x02, 0x03, 0x04};
char one[] = {0x05, 0x06, 0x20, 0x01, 0x05, 0x03};
char two[] = {0x00, 0x00, 0x00, 0x04, 0x03, 0x00};
char three[] = {0x00, 0x00, 0x05, 0x04, 0x00, 0x04};
char four[] = {0x06, 0x05, 0x03, 0x04, 0x20, 0x02};
char five[] = {0x00, 0x00, 0x03, 0x00, 0x00, 0x04};
char six[] = {0x00, 0x00, 0x03, 0x00, 0x03, 0x04};
char seven[] = {0x00, 0x00, 0x20, 0x02, 0x20, 0x02};
char eight[] = {0x00, 0x00, 0x03, 0x04, 0x03, 0x04};
char nine[] = {0x00, 0x00, 0x03, 0x04, 0x20, 0x02};
char colon[] = {0x20, 0x07, 0x07};

//number commands
char hour_tens[] = {0xC1, 0xC2, 0x95, 0x96, 0xD5, 0xD6};
char hour_ones[] = {0xC4, 0xC5, 0x98, 0x99, 0xD8, 0xD9};
char hm_colon[] = {0xC6, 0x9A, 0xDA};
char minute_tens[] = {0xC7, 0xC8, 0x9B, 0x9C, 0xDB, 0xDC};
char minute_ones[] = {0xCA, 0xCB, 0x9E, 0x9F, 0xDE, 0xDF};
char ms_colon[] = {0xCC, 0xA0, 0xE0};
char second_tens[] = {0xCD, 0xCE, 0xA1, 0xA2, 0xE1, 0xE2};
char second_ones[] = {0xD0, 0xD1, 0xA4, 0xA5, 0xE4, 0xE5};



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
    // Repeat previous 3 lines for all custom characters, addresses: 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78

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