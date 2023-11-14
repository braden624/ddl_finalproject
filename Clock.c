#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>


//define registers for RTC
//define registers for LCD pins


// custom bit patterns
char custom_data[8][5] = {{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111}, 
{0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100}, 
{0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111}, 
{0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11111, 0b11111, 0b11111}, 
{0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b11111, 0b11111, 0b11111}, 
{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00111, 0b00111, 0b00111}, 
{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11100, 0b11100, 0b11100}, 
{0b00000, 0b00000, 0b01110, 0b01110, 0b01110, 0b00000, 0b00000, 0b00000}};

// number code
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

// number positions
char hour_tens[] = {0xC1, 0xC2, 0x95, 0x96, 0xD5, 0xD6};
char hour_ones[] = {0xC4, 0xC5, 0x98, 0x99, 0xD8, 0xD9};
char hm_colon[] = {0xC6, 0x9A, 0xDA};
char minute_tens[] = {0xC7, 0xC8, 0x9B, 0x9C, 0xDB, 0xDC};
char minute_ones[] = {0xCA, 0xCB, 0x9E, 0x9F, 0xDE, 0xDF};
char ms_colon[] = {0xCC, 0xA0, 0xE0};
char second_tens[] = {0xCD, 0xCE, 0xA1, 0xA2, 0xE1, 0xE2};
char second_ones[] = {0xD0, 0xD1, 0xA4, 0xA5, 0xE4, 0xE5};

void wait_ms(int milliseconds){
    int ticks = milliseconds * 366.3;
    while(ticks != 0){
        ticks--;
    }
}

void set_alarm (void) {

}

void set_clock (void) {

}

void stopwatch (void) {

}

void init_LCD(void) {
    // Config pins for RS, R/W, E, DB0-7
    wait_ms(4);
    write_LCD_command(0x38);
    write_LCD_command(0x06);
    write_LCD_command(0x0C);
    write_LCD_command(0x01);
    wait_ms(4);

    // Program custom characters
    int custom_registers[] = {0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78}
    for(int i = 0, i < 8; i++ ){
        write_LCD_command(custom_registers[i]);
            for(int j = 0; j < 5; i++){
                write_LCD_data(custom_data[i][j]);
            }
        write_LCD_command(0x80);
    }
}

void write_LCD_command(char command) {
    // Set DB0-7 to match command
    // Drive read/write low
    // Drive RS low
    // Drive E low
    wait_ms(100); 
}

void write_LCD_data(char data) {
    // Set DB0-7 to match data
    // Drive read/write low
    // Drive RS high
    // Drive E low
    wait_ms(100);
}

int main(void) {

    init_LCD();
    
    while(1) {
        
    }
    return 0 ;
}