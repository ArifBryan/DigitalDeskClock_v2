/*
 * SystemConfig.h
 *
 * Created: 2021-12-24 11:45:29 PM
 *  Author: Arif Bryan
 */ 

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "I2CHandle.h"
#include "I2CPeriph.h"
#include "SPIHandle.h"
#include "SPIPeriph.h"

I2CPeriph i2c;
SPIPeriph spi;

extern volatile uint32_t _ticks;

struct System{
	void Init();
	void GPIO_Init();
	void I2C_Init();
	void SPI_Init();
	void Timer_Init();
	void Interrupt_Init();
	uint32_t Ticks();
}System;

void System::Init(){
	GPIO_Init();
	I2C_Init();
	SPI_Init();
	Timer_Init();
	Interrupt_Init();
}

void System::GPIO_Init(){
	PORTB = 0b00010001;
	DDRB = 0b00101110;
	PORTC = 0b0001111;
	DDRC = 0b0000000;
	PORTD = 0b11111101;
	DDRD = 0b00000010;
}

void System::I2C_Init(){
	I2CInitTypeDef I2CInitStruct;
	
	I2CInitStruct.CPUFreq = F_CPU;
	I2CInitStruct.GeneralCallEnable = 0U;
	I2CInitStruct.InterruptEnable = 0U;
	I2CInitStruct.OwnAddress = 0xFE;
	I2CInitStruct.SCLFreq = 400000UL;
	
	i2c.Init(&I2CInitStruct);
}

void System::SPI_Init(){
	SPIInitTypeDef SPIInitStruct;
	
	SPIInitStruct.ClockPhase = SPI_CLOCKPHASE_LEADEDGE;
	SPIInitStruct.ClockPolarity = SPI_CLOCKPOLARITY_HIGH;
	SPIInitStruct.DataAlign = SPI_DATAALIGN_MSBFirst;
	SPIInitStruct.InterruptEnable = 0U;
	SPIInitStruct.Mode = SPI_MODE_MASTER;
	SPIInitStruct.Prescaler = SPI_PRESCALER_32;
	
	spi.Init(&SPIInitStruct);
}

void System::Timer_Init(){
	TCCR0A = 0x00;
	TCCR0B = (0b011 << CS00);
	TIMSK0 = (1 << OCIE0A);
	OCR0A = 250;
}

void System::Interrupt_Init(){
	sei();
}

uint32_t System::Ticks(){
	uint32_t t;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		t = _ticks;
	}
	
	return t;
}
#endif /* SYSTEMCONFIG_H_ */