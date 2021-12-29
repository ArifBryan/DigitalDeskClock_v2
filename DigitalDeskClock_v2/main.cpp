/*
 * DIgitalDeskClock_v2.cpp
 *
 * Created: 2021-12-24 11:37:55 PM
 * Author : Arif Bryan
 */ 

#include "SystemConfig.h"
#include "DS3231.h"
#include "MAX7219_7Seg.h"

DS3231 rtc(&i2c);
MAX7219_7Seg disp(&spi, 4, &PORTB, 2);

struct Time{
	uint8_t Seconds;
	uint8_t Minutes;
	uint8_t Hours;
};

uint32_t timer;
uint8_t toggle;
Time tim;

int main(void){
	System.Init();
	disp.Init();
	
    while(1){
		Time timeNow;
		rtc.GetTime(&timeNow.Seconds, &timeNow.Minutes, &timeNow.Hours);
		if(timeNow.Seconds != tim.Seconds){
			disp.Write(5, 1, 0xFF * toggle);
			toggle = !toggle;
			
			tim.Seconds = timeNow.Seconds;
		}
    }
}

