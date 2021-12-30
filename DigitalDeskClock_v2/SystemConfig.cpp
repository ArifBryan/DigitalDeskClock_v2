/*
 * SystemConfig.cpp
 *
 * Created: 2021-12-30 10:33:16 PM
 *  Author: Arif Bryan
 */ 
#include "SystemConfig.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

I2CPeriph i2c;
SPIPeriph spi;
DS3231 rtc(&i2c);
System_TypeDef System;

extern volatile uint32_t _ticks;

void System_TypeDef::Init(){
	GPIO_Init();
	I2C_Init();
	SPI_Init();
	Timer_Init();
	Interrupt_Init();
}

void System_TypeDef::GPIO_Init(){
	PORTB = 0b00010001;
	DDRB = 0b00101110;
	PORTC = 0b0001111;
	DDRC = 0b0000000;
	PORTD = 0b11111101;
	DDRD = 0b00000010;
}

void System_TypeDef::I2C_Init(){
	I2CInitTypeDef I2CInitStruct;
	
	I2CInitStruct.CPUFreq = F_CPU;
	I2CInitStruct.GeneralCallEnable = 0U;
	I2CInitStruct.InterruptEnable = 0U;
	I2CInitStruct.OwnAddress = 0xFE;
	I2CInitStruct.SCLFreq = 400000UL;
	
	i2c.Init(&I2CInitStruct);
}

void System_TypeDef::SPI_Init(){
	SPIInitTypeDef SPIInitStruct;
	
	SPIInitStruct.ClockPhase = SPI_CLOCKPHASE_LEADEDGE;
	SPIInitStruct.ClockPolarity = SPI_CLOCKPOLARITY_HIGH;
	SPIInitStruct.DataAlign = SPI_DATAALIGN_MSBFirst;
	SPIInitStruct.InterruptEnable = 0U;
	SPIInitStruct.Mode = SPI_MODE_MASTER;
	SPIInitStruct.Prescaler = SPI_PRESCALER_32;
	
	spi.Init(&SPIInitStruct);
}

void System_TypeDef::Timer_Init(){
	TCCR0A = 0x00;
	TCCR0B = (0b011 << CS00);
	TIMSK0 = (1 << OCIE0A);
	OCR0A = 250;
}

void System_TypeDef::Interrupt_Init(){
	sei();
}

uint32_t System_TypeDef::Ticks(){
	uint32_t t;
	
	ATOMIC_BLOCK(ATOMIC_FORCEON){
		t = _ticks;
	}
	
	return t;
}