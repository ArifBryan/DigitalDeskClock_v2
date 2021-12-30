/*
 * ADC.h
 *
 * Created: 2021-12-30 11:05:40 PM
 *  Author: Arif Bryan
 */ 

#ifndef _ADC_H
#define _ADC_H

#include <avr/io.h>
#include <util/delay.h>

#define ADC_MUX_TEMP	0b1000
#define ADC_MUX_VBG		0b1110
#define ADC_MUX_GND		0b1111
#define ADC_REF_AREF	0b00
#define ADC_REF_AVCC	0b01
#define ADC_REF_INT		0b11
#define ADC_PRESC_2		0b001
#define ADC_PRESC_4		0b010
#define ADC_PRESC_8		0b011
#define ADC_PRESC_16	0b100
#define ADC_PRESC_32	0b101
#define ADC_PRESC_64	0b110
#define ADC_PRESC_128	0b111
#define ADC_TRIG_FREERUN 0b000
#define ADC_TRIG_ACOMP	0b001
#define ADC_TRIG_EXTINT	0b010

class ADC_TypeDef{
	public:
		void Init(uint8_t enable,uint8_t startconv,uint8_t autotrig,uint8_t interruptenable,uint8_t prescaler,uint8_t compmuxen,uint8_t trigsrc);
		void DisableDigitalIn(uint8_t pin);
		uint16_t Read(uint8_t mux, uint8_t vref);
};

#endif