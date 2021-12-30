/*
 * ADC.cpp
 *
 * Created: 2021-12-30 11:05:40 PM
 *  Author: Arif Bryan
 */ 
#include "ADC.h"

void ADC_TypeDef::Init(uint8_t enable,uint8_t startconv,uint8_t autotrig,uint8_t interruptenable,uint8_t prescaler,uint8_t compmuxen,uint8_t trigsrc){
	ADCSRA = (enable << ADEN)|(startconv << ADSC)|(autotrig << ADATE)|(interruptenable << ADIE)|((prescaler/4%2) << ADPS2)|((prescaler/2%2) << ADPS1)|((prescaler%2) << ADPS0);
	ADCSRB = (compmuxen << ACME)|((trigsrc/4%2) << ADTS2)|((trigsrc/2%2) << ADTS1)|((trigsrc%2)<< ADTS0);
}
void ADC_TypeDef::DisableDigitalIn(uint8_t pin){
	DIDR0 = pin;
}
uint16_t ADC_TypeDef::Read(uint8_t mux,uint8_t vref){
	uint8_t adcsra_t;
	adcsra_t = ADCSRA;
	ADMUX = ((vref/2%2)<<REFS1)|((vref%2)<<REFS0)|(0<<ADLAR)|((mux/8%2)<<MUX3)|((mux/4%2)<<MUX2)|((mux/2%2)<<MUX1)|((mux%2)<<MUX0);
	_delay_us(20);
	ADCSRA = (1<<ADSC)|adcsra_t;
	while((ADCSRA >> ADSC) & 1);
	return ADCL|(ADCH<<8);
}