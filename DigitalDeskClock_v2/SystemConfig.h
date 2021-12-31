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
#include "I2CHandle.h"
#include "I2CPeriph.h"
#include "SPIHandle.h"
#include "SPIPeriph.h"
#include "DS3231.h"

struct System_TypeDef{
	void Init();
	void GPIO_Init();
	void I2C_Init();
	void SPI_Init();
	void Timer_Init();
	void Interrupt_Init();
	uint32_t Ticks();
};

struct Time{
	uint8_t Seconds;
	uint8_t Minutes;
	uint8_t Hours;
};
struct Dates{
	uint8_t Day;
	uint8_t Date;
	uint8_t Month;
	uint8_t Year;
};

extern I2CPeriph i2c;
extern SPIPeriph spi;
extern DS3231 rtc;
extern System_TypeDef System;

#endif /* SYSTEMCONFIG_H_ */