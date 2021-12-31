/*
 * UserInterface.h
 *
 * Created: 2021-12-30 10:11:43 PM
 *  Author: Arif Bryan
 */ 
#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include <inttypes.h>

const char dayString[][4] = {
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};
const char monthString[][4] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};
const char offOnString[][4] = {
	"Off",
	"On "
};

struct UserInterface_TypeDef{
	void Init();
	void Handler();
private:
	void DisplayTime(uint8_t x, uint8_t y);
	void DisplayDay(uint8_t x, uint8_t y);
	void DisplayDate(uint8_t x, uint8_t y);
	void DisplayYear(uint8_t x, uint8_t y);
};

extern UserInterface_TypeDef UserInterface;

#endif /* USERINTERFACE_H_ */