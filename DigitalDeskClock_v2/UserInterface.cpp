/*
 * UserInterface.cpp
 *
 * Created: 2021-12-30 10:16:29 PM
 *  Author: Arif Bryan
 */ 
#include "UserInterface.h"

#include "SystemConfig.h"
#include "Inputs.h"
#include "MAX7219.h"
#include <stdio.h>
#include <math.h>

UserInterface_TypeDef UserInterface;

MAX7219 disp(&spi, 4, &PORTB, 2);

uint8_t dispBuffer[8 * 4];
char strBuffer[20];
uint32_t dispTimer;

void UserInterface_TypeDef::Init(){
	disp.Init();
	disp.SetBuffer(dispBuffer);
}

void UserInterface_TypeDef::Handler(){
	if(System.Ticks() - dispTimer >= 100){
		Time timeNow;
		rtc.GetTime(&timeNow.Seconds, &timeNow.Minutes, &timeNow.Hours);
		
		disp.Write(5, MAX7219_REG_INTENSITY, Input.AmbientLight() * 0.08);
		
// 		disp.Cursor(5, 0);
// 		sprintf(strBuffer, "%4d  ", Input.AmbientLight());
// 		disp.Print(strBuffer);
		
		disp.Cursor(5, 0);
		sprintf(strBuffer, " %2d ", timeNow.Hours);
		disp.Print(strBuffer);
		disp.Cursor(15, 0);
		disp.Print((timeNow.Seconds % 2 ? ':' : ' '));
		disp.Cursor(17, 0);
		sprintf(strBuffer, "%02d ", timeNow.Minutes);
		disp.Print(strBuffer);		
		disp.DrawBuffer(0);
		
		dispTimer = System.Ticks();
	}
}