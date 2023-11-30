#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>

//define registers for RTC
#define ILR (*(volatile unsigned int *) 0x40024000)
#define CCR (*(volatile unsigned int *) 0x40024008)
#define CIIR (*(volatile unsigned int *) 0x4002400C)
#define AMR (*(volatile unsigned int *) 0x40024010)
#define CTIME0 (*(volatile unsigned int *) 0x40024014)
#define SEC (*(volatile unsigned int *) 0x40024020)
#define MIN (*(volatile unsigned int *) 0x40024024)
#define HOUR (*(volatile unsigned int *) 0x40024028)
#define ALSEC (*(volatile unsigned int *) 0x40024060)
#define ALMIN (*(volatile unsigned int *) 0x40024064)
#define ALHOUR (*(volatile unsigned int *) 0x40024068)
#define ISER0 (*(volatile unsigned int *) 0xE000E100)
#define PCONP (*(volatile unsigned int *) 0xE000E100)

//define registers for buttons
#define FIO0DIR (*(volatile unsigned int *) 0x2009C000)
#define FIO0PIN (*(volatile unsigned int *) 0x2009C014)
#define PINMODE0 (*(volatile unsigned int *) 0x4002C040)
//Button 1 P0.0 , Button 2 P0.1 , Button 3 P0.2

//define registers for PWM
#define T0IR (*(volatile unsigned int *) 0x40004000)
#define T0TCR (*(volatile unsigned int *) 0x40004004)
#define T0TC (*(volatile unsigned int *) 0x40004008)
#define T0PR (*(volatile unsigned int *) 0x4000400C)
#define T0MCR (*(volatile unsigned int *) 0x40004014)
#define T0MR0 (*(volatile unsigned int *) 0x40004018)
#define T0CTCR (*(volatile unsigned int *) 0x40004070)

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

int clockMode = 0;
int alarmStatus = 1;
int displayMode = 0;
int stopStatus = 0;
int stopHour = 0;
int stopMin = 0;
int stopSec = 0;
int alarmHour = 23;
int alarmMin = 59;
int alarmSec = 59;

int longCount = 1000;
int alarmCount = 0;
int alarmCounter = 1000;
int activateAlarm = 0;

void display(int mode, int hh, int mm, int ss);
void write_LCD_command(char command);
void displayTop(char text[]);

void wait_ms(int milliseconds){
    int ticks = milliseconds * 400;
    while(ticks > 0){
        ticks--;
    }
}

void RTC_IRQHandler(void) {
    if((ILR >> 0) & 1) {
    	if(clockMode != 2){
    		write_LCD_command(0x80);
    		if((clockMode == 0) & (displayMode == 0)){
    			if(alarmStatus == 1){
    				displayTop("Clock      Alarm: On");
    			} else {
    				displayTop("Clock     Alarm: Off");
    			}
    		} else if((clockMode == 0) & (displayMode == 1)){
    			if(alarmStatus == 1){
    			    displayTop("Needs fixed         ");
    			} else {
    			    displayTop("Clock     Alarm: Off");
    			}
    		} else if(clockMode == 1){
    			displayTop("Stopwatch           ");
    		}

    	}
        if(clockMode == 0){
            display(clockMode, (HOUR&31), (MIN&63), (SEC&63));
        }
        else if(clockMode == 1){
        	if(stopStatus == 1){
        		stopSec++;
        		if(stopSec > 59){
        			stopSec = 0;
        			stopMin++;
        			if(stopMin > 99){
        				stopMin = 0;
        			}
        		}
        	}
            display(clockMode, stopHour, stopMin, stopSec);
        }
        ILR |= (1<<0);
    }
    if ((ILR >> 1) & 1) {
    	activateAlarm = 1;
    	ILR |= (1<<1);
    }
}

void TIMER0_IRQHandler(void) {
    if((T0IR >> 0) & 1) {
        alarmCount++;
        if ((alarmCount < 200) & activateAlarm){
            if ((FIO0PIN >> 3) & 1){
                FIO0PIN &= ~(1<<3);
            } else {
                FIO0PIN |= (1<<3);
            }
        }
        else{
            FIO0PIN |= (1<<3);
            if(alarmCount > 500){
                alarmCount = 0;
            }
        }
        T0IR |= (1<<0);
    }
}

void RTCinterruptInitialize(void){
    CIIR |= (1<<0);
    AMR |= (248<<0);
    ILR |= (3<<0);
    ISER0 |= (1<<17);
}

void TIMER0interruptInitialize(void){
	PCONP |= (1<<1);
	T0TCR &= ~(1<<0);
    T0CTCR &= ~(1<<0);
    T0MCR |= (1<<0) | (1<<1);
    T0MR0 = alarmCounter;
    T0TC = 1;
    T0PR = 1;
    ISER0 |= (1<<1);
    T0IR |= (1<<0);
    T0TCR |= (1<<0);
}

void buttonChirp(){
	activateAlarm = 0;
    for(int i = 0; i < 10; i++){
        FIO0PIN &= ~(1<<3);
        wait_ms(1);
        FIO0PIN |=  (1<<3);
        wait_ms(1);
    }
}

void write_LCD_clear(char command) {
    FIO2PIN &= ~(255<<0);
    FIO2PIN |= command;
    FIO2PIN &= ~(1<<10);
    FIO2PIN &= ~(1<<11);
    FIO2PIN |= (1<<8);
    FIO2PIN &= ~(1<<8);
    wait_ms(100);
}

void write_LCD_command(char command) {
    FIO2PIN &= ~(255<<0);
    FIO2PIN |= command;
    FIO2PIN &= ~(1<<10);
    FIO2PIN &= ~(1<<11);
    FIO2PIN |= (1<<8);
    FIO2PIN &= ~(1<<8);
    wait_ms(2);
}

void write_LCD_data(char data) {
    FIO2PIN &= ~(255<<0);
    FIO2PIN |= data;
    FIO2PIN &= ~(1<<10);
    FIO2PIN |= (1<<11);
    FIO2PIN |= (1<<8);
    FIO2PIN &= ~(1<<8);
    wait_ms(2);
}

void init_LCD(void) {
    FIO2DIR |= (255<<0) | (1<<8) | (1<<10) | (1<<11);
    wait_ms(4);
    write_LCD_command(0x38);
    write_LCD_command(0x06);
    write_LCD_command(0x0C);
    write_LCD_clear(0x01);
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
        case 0: case 2:
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_tens[i]);
                write_LCD_data(numbers[h_tens][i]);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_ones[i]);
                write_LCD_data(numbers[h_ones][i]);
            }
            for(int i = 0; i < 3; i++){
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
            for(int i = 0; i < 3; i++){
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
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 6; i++){
                write_LCD_command(hour_ones[i]);
                write_LCD_data(0x20);
            }
            for(int i = 0; i < 3; i++){
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
            for(int i = 0; i < 3; i++){
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

void displayTop(char text[]){
    write_LCD_command(0x80);
    for(int i = 0; i < 20; i++){
        write_LCD_data(text[i]);
    }
}

void set_alarm (void) {
    int alarmTime[] = {ALHOUR, ALMIN, ALSEC};
    int index = 0;
    int mod = 23;
    while(index > -1  && index < 4){
    	int count = 0;
        // Button 1
        if((FIO0PIN >> 0) & 1){
            buttonChirp();
        	while((FIO0PIN >> 0) & 1){
        		count++;
        	    wait_ms(1);
        	}
            if(count > longCount){
                index--;
            } else {
            	index++;
            }
            switch(index){
            	case 0: mod = 23; break;
            	case 1: case 2: mod = 59; break;
            }
        }
        // Button 2
        if((FIO0PIN >> 1) & 1){
            buttonChirp();
        	while((FIO0PIN >> 1) & 1){
        		count++;
        	    wait_ms(1);
        	}

        	if(count > longCount){
                alarmTime[index] = alarmTime[index] + 10;
            } else {
            	alarmTime[index]++;;
            }
        	if(alarmTime[index] > mod){
        		alarmTime[index] = alarmTime[index] - mod - 1;
        	}
        }
        // Button 3
        if((FIO0PIN >> 2) & 1){
            buttonChirp();
        	while((FIO0PIN >> 2) & 1){
        		count++;
        	    wait_ms(1);
        	}
        	if(count > longCount){
                alarmTime[index] = alarmTime[index] - 10;
            } else {
            	alarmTime[index]--;
            }
        	if(alarmTime[index] < 0){
        		alarmTime[index] = alarmTime[index] + mod + 1;
        	}
        }
        switch(index) {
            case 0: displayTop("Set Alarm -> Hour   "); break;
            case 1: displayTop("Set Alarm -> Minute "); break;
            case 2: displayTop("Set Alarm -> Second "); break;
        }
        display(clockMode, alarmTime[0], alarmTime[1], alarmTime[2]);
    }
    ALHOUR = alarmTime[0];
    ALMIN = alarmTime[1];
    ALSEC = alarmTime[2];
    return;
}

void set_clock (void) {
    int clockTime[] = {HOUR, MIN, SEC};
    int index = 0;
    int mod = 23;
    while(index < 4 && index > -1){
    	int count = 0;
        // Button 1
        if((FIO0PIN >> 0) & 1){
            buttonChirp();
        	while((FIO0PIN >> 0) & 1){
        		count++;
        	    wait_ms(1);
        	}
        	if(count > longCount){
                index--;
            } else {
            	index++;
            }
        	switch(index){
        		case 0: mod = 23; break;
        		case 1: case 2: mod = 59; break;
        	}
        }
        // Button 2
        if((FIO0PIN >> 1) & 1){
            buttonChirp();
        	while((FIO0PIN >> 1) & 1){
        		count++;
        	    wait_ms(1);
        	}
        	if(count > longCount){
                clockTime[index] = clockTime[index] + 10;
            } else {
            	clockTime[index]++;
            }
        	if(clockTime[index] > mod){
        		clockTime[index] = clockTime[index] - mod - 1;
        	}
        }
        // Button 3
        if((FIO0PIN >> 2) & 1){
            buttonChirp();
        	while((FIO0PIN >> 0) & 1){
        		count++;
        	    wait_ms(1);
        	}
        	if(count > longCount){
                clockTime[index] = clockTime[index] - 10;
            } else {
            	clockTime[index]--;
            }
        	if(clockTime[index] < 0){
        		clockTime[index] = clockTime[index] + mod + 1;
        	}
        }
        switch(index) {
            case 0: displayTop("Set Clock -> Hour   "); break;
            case 1: displayTop("Set Clock -> Minute "); break;
            case 2: displayTop("Set Clock -> Second "); break;
        }
        display(clockMode, clockTime[0], clockTime[1], clockTime[2]);
    }
    HOUR = clockTime[0];
    MIN = clockTime[1];
    SEC = clockTime[2];
    return;
}

int main(void) {
    CCR |= (1<<0);

    RTCinterruptInitialize();
    TIMER0interruptInitialize();

    HOUR = 12;
    MIN = 0;
    SEC = 0;
    ALHOUR = 12;
    ALMIN = 00;
    ALSEC = 30;

    init_LCD();

    FIO0DIR &= ~(1u<<0) & ~(1u<<1) & ~(1u<<2);
    FIO0DIR |= (1<<3);
    PINMODE0 |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5);

    clockMode = 0;

    while(1) {
        //write_LCD_command(0x80);
        //write_LCD_data(0x42);
        //display(0, 8, 0, 0);
        //wait_ms(1000);

    	int count = 0;
        // Button 1
        if((FIO0PIN >> 0) & 1){
            buttonChirp();
        	while((FIO0PIN >> 0) & 1){
        		count++;
        		wait_ms(1);
        	}
            if(count > longCount){
                if(alarmStatus == 0){
                    alarmStatus = 1;
                } else {
                    alarmStatus = 0;
                }
            } else {
            	if(clockMode == 0){
            		clockMode = 1;
            	} else {
            		clockMode = 0;
            	}
            }
        }
        // Button 2
        if((FIO0PIN >> 1) & 1){
            buttonChirp();
        	while((FIO0PIN >> 1) & 1){
        	        		count++;
        	        		wait_ms(1);
        	        	}
            if(count > longCount){
                if(clockMode == 0){
                    clockMode = 2;
                    set_clock();
                    clockMode = 0;
                } else if(clockMode == 1){
                    stopHour = 0;
                    stopMin = 0;
                    stopSec = 0;
                }
            } else {
            	if(clockMode == 0){
            		if(displayMode == 0){
            			displayMode = 1;
            		} else {
            			displayMode = 0;
            		}
            	} else if (clockMode = 1){
            		if(stopStatus == 0){
            			stopStatus = 1;
            		} else {
            			stopStatus = 0;
            		}
            	}
            }
        }
        // Button 3
        if((FIO0PIN >> 2) & 1){
            buttonChirp();
        	while((FIO0PIN >> 2) & 1){
        		count++;
        	    wait_ms(1);
        	}
            if(count > longCount){
            	if (clockMode == 0){
            		clockMode = 2;
            		set_alarm();
            		clockMode = 0;
            	}

            }
        }
    }
    return 0 ;
}

//Questions
// Should stopwatch reset and pause when cycling modes
// Should short and long press be shorter in time length

//To Do
// Display top line alarm time
// Alarm interrupt
