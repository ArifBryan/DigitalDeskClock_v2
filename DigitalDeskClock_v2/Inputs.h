/*
 * Inputs.h
 *
 * Created: 2021-12-30 9:42:31 PM
 *  Author: Arif Bryan
 */ 
#ifndef INPUTS_H_
#define INPUTS_H_

#include <avr/io.h>

struct Input_TypeDef{
	void Init();
	void Handler();
	uint16_t AmbientLight(){return ambientLight;};
	uint8_t Button1(){return btn1;};
	uint8_t Button2(){return btn2;};
	uint8_t Button3(){return btn3;};
private:
	uint8_t btn1 : 1;
	uint8_t btn2 : 1;
	uint8_t btn3 : 1;
	uint8_t lbtn1 : 1;
	uint8_t lbtn2 : 1;
	uint8_t lbtn3 : 1;
	uint16_t ambientLight;
};

extern Input_TypeDef Input;

#endif /* INPUTS_H_ */