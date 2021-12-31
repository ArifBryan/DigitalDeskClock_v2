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

enum transition{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
}transition;

uint8_t dispBuffer[8 * 4];
char strBuffer[20];

Time timeNow;
Dates dateNow;
uint32_t dispTimer;
uint32_t updateTimer;
uint32_t transitionTimer;
uint8_t menu;
uint8_t dispMenu;
uint8_t lSeconds;
uint8_t setMode;
uint8_t scrollMode;
uint8_t alsEnable = 1;
int8_t xPos, yPos;

void UserInterface_TypeDef::Init(){
	disp.Init();
	disp.SetBuffer(dispBuffer);
}

void UserInterface_TypeDef::Handler(){
	if(System.Ticks() - updateTimer >= 100){
		if(!setMode){
			rtc.GetTime(&timeNow.Seconds, &timeNow.Minutes, &timeNow.Hours);
			rtc.GetDate(&dateNow.Day, &dateNow.Date, &dateNow.Month, &dateNow.Year);
		}
		
		if(alsEnable){
			disp.Write(5, MAX7219_REG_INTENSITY, Input.AmbientLight() * 0.08);
		}
		else{
			disp.Write(5, MAX7219_REG_INTENSITY, 2);
		}
		
// 		disp.Cursor(5, 0);
// 		sprintf(strBuffer, "%4d  ", Input.AmbientLight());
// 		disp.Print(strBuffer);
		
		updateTimer = System.Ticks();
	}
	
	uint8_t dispUpdate = Input.Button1() || Input.Button2() || Input.Button3();
	switch(menu){
		// Main display
		case 0 ... 3:
			if(Input.Button3()){
				menu = 4;
				transition = UP;
			}
		break;
		// Set Time
		case 4:
			if(Input.Button3()){
				menu = 7;
				transition = UP;
			}
			if(Input.Button2()){
				menu = 0;
				transition = DOWN;
			}
			if(Input.Button1()){
				menu = 5;
				setMode = 1;
				transition = LEFT;
			}
		break;
		// Set Time -> Hour
		case 5:
			if(Input.Button3()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				if(-- timeNow.Hours > 23){timeNow.Hours = 23;}
			}
			if(Input.Button1()){
				if(++ timeNow.Hours > 23){timeNow.Hours = 0;}
			}
		break;
		// Set Time -> Minutes
		case 6:
			if(Input.Button3()){
				menu = 4;
				transition = RIGHT;
				rtc.SetTime(0, timeNow.Minutes, timeNow.Hours);
				setMode = 0;
			}
			if(Input.Button2()){
				if(-- timeNow.Minutes > 59){timeNow.Minutes = 59;}
			}
			if(Input.Button1()){
				if(++ timeNow.Minutes > 59){timeNow.Minutes = 0;}
			}
		break;
		// Set Date
		case 7:
			if(Input.Button3()){
				menu = 12;
				transition = UP;
			}
			if(Input.Button2()){
				menu = 0;
				transition = DOWN;
			}
			if(Input.Button1()){
				menu = 8;
				setMode = 1;
				transition = LEFT;
			}
		break;
		// Set Date -> Day
		case 8:
			if(Input.Button3()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				if(-- dateNow.Day < 1){dateNow.Day = 7;}
			}
			if(Input.Button1()){
				if(++ dateNow.Day > 7){dateNow.Day = 1;}
			}
		break;
		// Set Date -> Date
		case 9:
			if(Input.Button3()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				if(-- dateNow.Date < 1){dateNow.Date = 31;}
			}
			if(Input.Button1()){
				if(++ dateNow.Date > 31){dateNow.Date = 1;}
			}
		break;
		// Set Date -> Month
		case 10:
			if(Input.Button3()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				if(-- dateNow.Month < 1){dateNow.Month = 12;}
			}
			if(Input.Button1()){
				if(++ dateNow.Month > 12){dateNow.Month = 1;}
			}
		break;
		// Set Date -> Year
		case 11:
			if(Input.Button3()){
				menu = 7;
				transition = RIGHT;
				rtc.SetDate(dateNow.Day, dateNow.Date, dateNow.Month, dateNow.Year);
				setMode = 0;
			}
			if(Input.Button2()){
				if(-- dateNow.Year > 99){dateNow.Year = 99;}
			}
			if(Input.Button1()){
				if(++ dateNow.Year > 99){dateNow.Year = 0;}
			}
		break;
		// Ambient Light Sensor
		case 12:
			if(Input.Button3()){
				menu = 0;
				transition = UP;
			}
			if(Input.Button2()){
				menu = 0;
				transition = DOWN;
			}
			if(Input.Button1()){
				menu = 13;
				transition = LEFT;
			}
		break;
		// Ambient Light Sensor -> On/Off
		case 13:
			if(Input.Button3()){
				menu = 12;
				transition = RIGHT;
			}
			if(Input.Button2()){
				alsEnable = 0;
			}
			if(Input.Button1()){
				alsEnable = 1;
			}
		break;
	}
	
	if(menu < 4){if((timeNow.Seconds == 30 || timeNow.Seconds == 0) && !scrollMode){scrollMode = 1;}}
	else{scrollMode = 0;}
		
	if(timeNow.Seconds != lSeconds && scrollMode){
		lSeconds = timeNow.Seconds;
		if(timeNow.Seconds % 10 == 0){menu = 1; transition = RIGHT;}
		if(timeNow.Seconds % 10 == 3){menu = 2; transition = DOWN;}
		if(timeNow.Seconds % 10 == 5){menu = 3; transition = DOWN;}
		if(timeNow.Seconds % 10 == 7){menu = 0; transition = LEFT; scrollMode = 0;}
	}
	
	if(transition != NONE){
		if(transition == UP){
			if(dispMenu != menu){
				if(System.Ticks() - transitionTimer >= 50){
					yPos ++;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(yPos >= 8){
					yPos = -8;
					dispMenu = menu;
				}
			}
			else{
				if(System.Ticks() - transitionTimer >= 50){
					yPos ++;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(yPos == 0){transition = NONE;}
			}
		}
		else if(transition == DOWN){
			if(dispMenu != menu){
				if(System.Ticks() - transitionTimer >= 50){
					yPos --;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(yPos <= -8){
					yPos = 8;
					dispMenu = menu;
				}
			}
			else{
				if(System.Ticks() - transitionTimer >= 50){
					yPos --;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(yPos == 0){transition = NONE;}
			}
		}
		else if(transition == LEFT){
			if(dispMenu != menu){
				if(System.Ticks() - transitionTimer >= 25){
					xPos --;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(xPos <= -32){
					xPos = 32;
					dispMenu = menu;
				}
			}
			else{
				if(System.Ticks() - transitionTimer >= 25){
					xPos --;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(xPos == 0){transition = NONE;}
			}
		}
		else if(transition == RIGHT){
			if(dispMenu != menu){
				if(System.Ticks() - transitionTimer >= 25){
					xPos ++;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(xPos >= 32){
					xPos = -32;
					dispMenu = menu;
				}
			}
			else{
				if(System.Ticks() - transitionTimer >= 25){
					xPos ++;
					dispUpdate = 1;
					transitionTimer = System.Ticks();
				}
				if(xPos == 0){transition = NONE;}
			}
		}
	}
	else{
		xPos = 0;
		yPos = 0;
		dispMenu = menu;
	}
	
	if(System.Ticks() - dispTimer >= 100 || dispUpdate){
		dispUpdate = 0;
		disp.ClearBuffer();
		switch(dispMenu){
			case 0:
				DisplayTime(xPos, yPos);
			break;
			case 1:
				DisplayDay(xPos, yPos);
			break;
			case 2:
				DisplayDate(xPos, yPos);
			break;
			case 3:
				DisplayYear(xPos, yPos);
			break;
			// Set Time
			case 4:
				disp.Cursor(xPos + 6, yPos);
				sprintf(strBuffer, "Time");
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Time -> Hour
			case 5:
				disp.Cursor(xPos + 7, yPos);
				sprintf(strBuffer, "H:%d ", timeNow.Hours);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Time -> Minutes
			case 6:
				disp.Cursor(xPos + 7, yPos);
				sprintf(strBuffer, "M:%02d ", timeNow.Minutes);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Date
			case 7:
				disp.Cursor(xPos + 6, yPos);
				sprintf(strBuffer, "Date");
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Date -> Day			
			case 8:
				disp.Cursor(xPos + 2, yPos);
				sprintf(strBuffer, "Dy:%s ", dayString[dateNow.Day - 1]);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Date -> Date
			case 9:
				disp.Cursor(xPos + 4, yPos);
				sprintf(strBuffer, "Dt:%d ", dateNow.Date);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Date -> Month
			case 10:
				disp.Cursor(xPos + 2, yPos);
				sprintf(strBuffer, "Mt:%s ", monthString[dateNow.Month - 1]);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Set Date -> Year
			case 11:
				disp.Cursor(xPos + 1, yPos);
				sprintf(strBuffer, "Y:20%02d ", dateNow.Year);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Ambient Light Sensor
			case 12:
				disp.Cursor(xPos + 8, yPos);
				sprintf(strBuffer, "ALS");
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			// Ambient Light Sensor -> On/Off
			case 13:
				disp.Cursor(xPos + 8, yPos);
				sprintf(strBuffer, "%s", offOnString[alsEnable]);
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
		}
		
		dispTimer = System.Ticks();
	}
}

void UserInterface_TypeDef::DisplayTime(uint8_t x, uint8_t y){
	if(timeNow.Hours % 10 == 1){x += 1;}
	if(timeNow.Hours / 10 == 1){x += 1;}
	if(timeNow.Hours < 10){x += 4;}
	if(timeNow.Minutes % 10 == 1){x += 1;}
	if(timeNow.Minutes / 10 == 1){x += 1;}
	disp.Cursor((int8_t)x + 3, y + 0);
	sprintf(strBuffer, "%d%c%02d ", timeNow.Hours, (timeNow.Seconds % 2 ? ':' : ' '), timeNow.Minutes);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
void UserInterface_TypeDef::DisplayDay(uint8_t x, uint8_t y){
	disp.Cursor((int8_t)x + 8, y + 0);
	sprintf(strBuffer, "%s ", dayString[dateNow.Day - 1]);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
void UserInterface_TypeDef::DisplayDate(uint8_t x, uint8_t y){
	if(dateNow.Date % 10 == 1){x += 1;}
	if(dateNow.Date / 10 == 1){x += 1;}
	if(dateNow.Date < 10){x += 4;}
	disp.Cursor((int8_t)x + 2, y + 0);
	sprintf(strBuffer, "%d %s ", dateNow.Date, monthString[dateNow.Month - 1]);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
void UserInterface_TypeDef::DisplayYear(uint8_t x, uint8_t y){
	if(dateNow.Year % 10 == 1){x += 1;}
	if(dateNow.Year / 10 == 1){x += 1;}
	disp.Cursor((int8_t)x + 5, y + 0);
	sprintf(strBuffer, "20%02d ", dateNow.Year);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
