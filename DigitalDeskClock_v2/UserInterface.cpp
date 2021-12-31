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
int8_t yPos = 0;

void UserInterface_TypeDef::Init(){
	disp.Init();
	disp.SetBuffer(dispBuffer);
}

void UserInterface_TypeDef::Handler(){
	if(System.Ticks() - updateTimer >= 100){
		rtc.GetTime(&timeNow.Seconds, &timeNow.Minutes, &timeNow.Hours);
		rtc.GetDate(&dateNow.Day, &dateNow.Date, &dateNow.Month, &dateNow.Year);
		
		disp.Write(5, MAX7219_REG_INTENSITY, Input.AmbientLight() * 0.08);
		
// 		disp.Cursor(5, 0);
// 		sprintf(strBuffer, "%4d  ", Input.AmbientLight());
// 		disp.Print(strBuffer);
		
		updateTimer = System.Ticks();
	}
	
	uint8_t dispUpdate = Input.Button1() || Input.Button2() || Input.Button3();
	switch(menu){
		case 0:
			if(Input.Button1()){
				menu ++;
				transition = UP;
			}
		break;
		case 1:
			if(Input.Button1()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				menu --;
				transition = DOWN;
			}
		break;
		case 2:
			if(Input.Button1()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				menu --;
				transition = DOWN;
			}
		break;
		case 3:
			if(Input.Button1()){
				menu ++;
				transition = UP;
			}
			if(Input.Button2()){
				menu --;
				transition = DOWN;
			}
		break;
		case 4:
			if(Input.Button1()){
				//menu ++;
				//transition = UP;
			}
			if(Input.Button2()){
				menu --;
				transition = DOWN;
			}
		break;
	}
	if(timeNow.Seconds != lSeconds && ((timeNow.Seconds >= 30 && timeNow.Seconds < 40) || (timeNow.Seconds >= 0 && timeNow.Seconds < 10))){
		lSeconds = timeNow.Seconds;
		if(timeNow.Seconds % 10 == 0){menu = 1; transition = DOWN;}
		if(timeNow.Seconds % 10 == 2){menu = 2; transition = DOWN;}
		if(timeNow.Seconds % 10 == 4){menu = 3; transition = DOWN;}
		if(timeNow.Seconds % 10 == 6){menu = 0; transition = UP;}
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
	}
	if(System.Ticks() - dispTimer >= 100 || dispUpdate){
		dispUpdate = 0;
		disp.ClearBuffer();
		switch(dispMenu){
			case 0:
				DisplayTime(0, yPos);
			break;
			case 1:
				DisplayDay(0, yPos);
			break;
			case 2:
				DisplayDate(0, yPos);
			break;
			case 3:
				DisplayYear(0, yPos);
			break;
			case 4:
				disp.Cursor(6, yPos);
				sprintf(strBuffer, "Time");
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			case 5:
				disp.Cursor(6, yPos);
				sprintf(strBuffer, "Date");
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			case 6:
				disp.Cursor(6, yPos);
				sprintf(strBuffer, "Disp");
				disp.Print(strBuffer);
				disp.DrawBuffer(0);
			break;
			case 7:
				disp.Cursor(8, yPos);
				sprintf(strBuffer, "ALS");
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
	disp.Cursor(x + 3, y + 0);
	sprintf(strBuffer, "%d%c%02d ", timeNow.Hours, (timeNow.Seconds % 2 ? ':' : ' '), timeNow.Minutes);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
void UserInterface_TypeDef::DisplayDay(uint8_t x, uint8_t y){
	disp.Cursor(x + 8, y + 0);
	sprintf(strBuffer, "%s ", dayString[dateNow.Day - 1]);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
void UserInterface_TypeDef::DisplayDate(uint8_t x, uint8_t y){
	if(dateNow.Date % 10 == 1){x += 1;}
	if(dateNow.Date / 10 == 1){x += 1;}
	if(dateNow.Date < 10){x += 4;}
	disp.Cursor(x + 2, y + 0);
	sprintf(strBuffer, "%d %s ", dateNow.Date, monthString[dateNow.Month - 1]);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
void UserInterface_TypeDef::DisplayYear(uint8_t x, uint8_t y){
	if(dateNow.Year % 10 == 1){x += 1;}
	if(dateNow.Year / 10 == 1){x += 1;}
	disp.Cursor(x + 5, y + 0);
	sprintf(strBuffer, "20%02d ", dateNow.Year);
	disp.Print(strBuffer);
	disp.DrawBuffer(0);
}
