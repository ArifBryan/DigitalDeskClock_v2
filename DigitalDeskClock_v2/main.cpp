/*
 * DIgitalDeskClock_v2.cpp
 *
 * Created: 2021-12-24 11:37:55 PM
 * Author : Arif Bryan
 */ 

#include "SystemConfig.h"
#include "DS3231.h"
#include "MAX7219.h"
#include <stdio.h>

DS3231 rtc(&i2c);
MAX7219 disp(&spi, 4, &PORTB, 2);

struct Time{
	uint8_t Seconds;
	uint8_t Minutes;
	uint8_t Hours;
};

uint32_t timer;
int16_t shift;
Time tim;
uint8_t dispBuffer[8 * 4];
char strBuffer[20];

int main(void){
	System.Init();
	disp.Init();
	disp.SetBuffer(dispBuffer);
	//rtc.SetTime(0, 56, 10);
	
    while(1){
		Time timeNow;
		rtc.GetTime(&timeNow.Seconds, &timeNow.Minutes, &timeNow.Hours);
		
		if(System.Ticks() - timer >= 50){
			sprintf(strBuffer, "  %2d%c%02d  ", timeNow.Hours, (timeNow.Seconds % 2 ? ':' : ' '), timeNow.Minutes);
			disp.Cursor(shift++, 0);
			disp.Print(strBuffer);
			disp.DrawBuffer(0);
			if(shift >= 32){shift = -20;}
			
			timer = System.Ticks();
		}
    }
}

