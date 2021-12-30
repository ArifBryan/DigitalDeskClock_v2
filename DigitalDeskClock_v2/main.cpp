/*
 * DIgitalDeskClock_v2.cpp
 *
 * Created: 2021-12-24 11:37:55 PM
 * Author : Arif Bryan
 */ 

#include "SystemConfig.h"
#include "Inputs.h"
#include "UserInterface.h"

int main(void){
	System.Init();
	Input.Init();
	UserInterface.Init();
	
    while(1){
		Input.Handler();
		UserInterface.Handler();
		
// 		if(Input.Button1()){
// 			//sprintf(strBuffer, "   %2d%c%02d  ", timeNow.Hours, (timeNow.Seconds % 2 ? ':' : ' '), timeNow.Minutes);
// 			disp.Cursor(5, 0);
// 			sprintf(strBuffer, "%2d", 12);
// 			disp.Print(strBuffer);
// 			disp.Cursor(15, 0);
// 			disp.Print((timeNow.Seconds % 2 ? ':' : ' '));
// 			disp.Cursor(17, 0);
// 			sprintf(strBuffer, "%02d", 34);
// 			disp.Print(strBuffer);
// 			disp.DrawBuffer(0);
// 			if(shift >= 32){shift = -20;}
// 			
// 			timer = System.Ticks();
// 		}
    }
}

