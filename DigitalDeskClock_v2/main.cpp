/*
 * DIgitalDeskClock_v2.cpp
 *
 * Created: 2021-12-24 11:37:55 PM
 * Author : Arif Bryan
 */ 

#include "SystemConfig.h"
#include "DS3231.h"
#include "MAX7219.h"

DS3231 rtc(&i2c);
MAX7219 disp(&spi, 4, &PORTB, 2);

struct Time{
	uint8_t Seconds;
	uint8_t Minutes;
	uint8_t Hours;
};

uint32_t timer;
uint8_t toggle;
Time tim;
uint8_t dispBuffer[8 * 4];

int main(void){
	System.Init();
	disp.Init();
	disp.SetBuffer(dispBuffer);
	
    while(1){
		Time timeNow;
		rtc.GetTime(&timeNow.Seconds, &timeNow.Minutes, &timeNow.Hours);
		
		if(System.Ticks() - timer >= 100){
			disp.DrawPix(toggle ++, 0, 1);
			
			timer = System.Ticks();
		}
		
		disp.DrawBuffer();
    }
}

