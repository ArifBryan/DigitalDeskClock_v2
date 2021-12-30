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
    }
}

