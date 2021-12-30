/*
 * UserInterface.h
 *
 * Created: 2021-12-30 10:11:43 PM
 *  Author: Arif Bryan
 */ 
#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include <inttypes.h>

struct UserInterface_TypeDef{
	void Init();
	void Handler();
private:
	void DisplayTime(uint8_t x, uint8_t y);
};

extern UserInterface_TypeDef UserInterface;

#endif /* USERINTERFACE_H_ */