/*
 * Inputs.cpp
 *
 * Created: 2021-12-30 10:26:15 PM
 *  Author: Arif Bryan
 */ 
#include "Inputs.h"
#include "SystemConfig.h"
#include "ADC.h"

#define ALS_FILTER_COEFF	5

Input_TypeDef Input;
ADC_TypeDef adc;

uint32_t sampleTimer;
uint16_t alsRawFiltered = 1023;

void Input_TypeDef::Init(){
	adc.Init(1, 1, 0, 0, ADC_PRESC_64, 0, ADC_TRIG_FREERUN);
}

void Input_TypeDef::Handler(){
	btn1 = 0;
	if(!((PIND >> PIND5) & 1) && lbtn1){
		btn1 = 1;
		lbtn1 = 0;
	}
	else if(((PIND >> PIND5) & 1)){
		lbtn1 = 1;
	}
	
	btn2 = 0;
	if(!((PIND >> PIND3) & 1) && lbtn2){
		btn2 = 1;
		lbtn2 = 0;
	}
	else if(((PIND >> PIND3) & 1)){
		lbtn2 = 1;
	}
	
	btn3 = 0;
	if(!((PIND >> PIND2) & 1) && lbtn2){
		btn3 = 1;
		lbtn3 = 0;
	}
	else if(((PIND >> PIND2) & 1)){
		lbtn3 = 1;
	}
	
	if(System.Ticks() - sampleTimer >= 500){
		alsRawFiltered = (alsRawFiltered * ALS_FILTER_COEFF) + adc.Read(7, ADC_REF_AVCC);
		alsRawFiltered /= (ALS_FILTER_COEFF + 1);
		ambientLight = ((1023 - alsRawFiltered) / 3);
		ambientLight = (ambientLight > 100 ? 100 : ambientLight);
		
		sampleTimer = System.Ticks();
	}
}
