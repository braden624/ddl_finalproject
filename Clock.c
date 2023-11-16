#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>


//define registers for RTC

//define regisers for Alarm sound
#define FIO0DIR (*(volatile unsigned int *) 0x2009C000)
#define FIO0PIN (*(volatile unsigned int *) 0x2009C014)
#define PINSEL1 (*(volatile unsigned int *) 0x4002C004)
#define DACR (*(volatile unsigned int *) 0x4008C000)
//Switch 1 P0. , Switch 2 P0. , Switch 3 P0. 

//define registers for LCD pins
#define FIO2DIR (*(volatile unsigned int *) 0x2009C040)
#define FIO2PIN (*(volatile unsigned int *) 0x2009C054)
//DB0-7 P2.0-7, E P2.8, R/W P2.10, RS P2.11


// custom bit patterns
char custom_data[8][8] = {{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111}, 
{0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100}, 
{0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b00111}, 
{0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11111, 0b11111, 0b11111}, 
{0b00111, 0b00111, 0b00111, 0b00111, 0b00111, 0b11111, 0b11111, 0b11111}, 
{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00111, 0b00111, 0b00111}, 
{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11100, 0b11100, 0b11100}, 
{0b00000, 0b00000, 0b01110, 0b01110, 0b01110, 0b00000, 0b00000, 0b00000}};

// number code
char numbers[10][6] = {{0x00, 0x00, 0x01, 0x02, 0x03, 0x04}, 
{0x05, 0x06, 0x20, 0x01, 0x05, 0x03}, 
{0x00, 0x00, 0x00, 0x04, 0x03, 0x00},
{0x00, 0x00, 0x05, 0x04, 0x00, 0x04},
{0x06, 0x05, 0x03, 0x04, 0x20, 0x02},
{0x00, 0x00, 0x03, 0x00, 0x00, 0x04},
{0x00, 0x00, 0x03, 0x00, 0x03, 0x04},
{0x00, 0x00, 0x20, 0x02, 0x20, 0x02},
{0x00, 0x00, 0x03, 0x04, 0x03, 0x04},
{0x00, 0x00, 0x03, 0x04, 0x20, 0x02}};
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
    int ticks = milliseconds * 400;
    while(ticks > 0){
        ticks--;
    }
}

void write_LCD_command(char command) {
    FIO2PIN &= ~(255<<0);
    FIO2PIN |= command;
    FIO2PIN &= ~(1<<10);
    FIO2PIN &= ~(1<<11);
    FIO2PIN |= (1<<8);
    FIO2PIN &= ~(1<<8);
    wait_ms(100); 
}

void write_LCD_data(char data) {
    FIO2PIN &= ~(255<<0);
    FIO2PIN |= data;
    FIO2PIN &= ~(1<<10);
    FIO2PIN |= (1<<11);
    FIO2PIN |= (1<<8);
    FIO2PIN &= ~(1<<8);
    wait_ms(100);
}

void init_LCD(void) {
    FIO2DIR |= (255<<0) | (1<<8) | (1<<10) | (1<<11);
    wait_ms(4);
    write_LCD_command(0x38);
    write_LCD_command(0x06);
    write_LCD_command(0x0F);
    write_LCD_command(0x01);
    wait_ms(4);

    // Program custom characters
    int custom_char_addresses[] = {0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78};
    for(int i = 0; i < 8; i++){
        write_LCD_command(custom_char_addresses[i]);
            for(int j = 0; j < 8; j++){
                write_LCD_data(custom_data[i][j]);
            }
        write_LCD_command(0x80);
    }
}

void display(int mode, int hh, int mm, int ss) {
    int h_tens, h_ones, m_tens, m_ones, s_tens, s_ones;
    h_tens = hh/10;
    h_ones = hh%10;
    m_tens = mm/10;
    m_ones = mm%10;
    s_tens = ss/10;
    s_ones = ss%10;

    switch(mode) {
        case 0:
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_tens[i]);
                write_LCD_data(numbers[h_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_ones[i]);
                write_LCD_data(numbers[h_ones][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hm_colon[i]);
                write_LCD_data(colon[i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(minute_tens[i]);
                write_LCD_data(numbers[m_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(minute_ones[i]);
                write_LCD_data(numbers[m_ones][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(ms_colon[i]);
                write_LCD_data(colon[i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(second_tens[i]);
                write_LCD_data(numbers[s_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(second_ones[i]);
                write_LCD_data(numbers[s_ones][i]);
            }
            break;
        case 1:
        for(int i = 0; i < 6; i++){
                write_LCD_command(hour_tens[i]);
                write_LCD_data(numbers[h_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_ones[i]);
                write_LCD_data(numbers[h_ones][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hm_colon[i]);
                write_LCD_data(colon[i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(minute_tens[i]);
                write_LCD_data(numbers[m_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(minute_ones[i]);
                write_LCD_data(numbers[m_ones][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(ms_colon[i]);
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(second_tens[i]);
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(second_ones[i]);
                write_LCD_data(0x20);
            }
            break;
        case 2:
        for(int i = 0; i < 6; i++){
                write_LCD_command(hour_tens[i]);
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_ones[i]);
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hm_colon[i]);
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(minute_tens[i]);
                write_LCD_data(numbers[m_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(minute_ones[i]);
                write_LCD_data(numbers[m_ones][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(ms_colon[i]);
                write_LCD_data(colon[i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(second_tens[i]);
                write_LCD_data(numbers[s_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(second_ones[i]);
                write_LCD_data(numbers[s_ones][i]);
            }
            break;
    }
}

void set_alarm (void) {

}

void set_clock (void) {

}

void stopwatch (void) {

}

int main(void) {

    init_LCD();
    
    while(1) {
        //write_LCD_command(0x80);
        //write_LCD_data(0x42);
        display(0, 8, 0, 0);
        wait_ms(1000);

        
    }
    return 0 ;
}
