/*
 * CFile1.c
 *
 * Created: 2021-12-29 12:00:04 AM
 *  Author: Arif Bryan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t _ticks;
ISR(TIMER0_COMPA_vect){
	_ticks ++;
}
