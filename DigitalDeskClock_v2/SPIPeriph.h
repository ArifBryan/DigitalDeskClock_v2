#ifndef _SPIPeriph_H
#define _SPIPeriph_H

#include <avr/io.h>
#include "SPIHandle.h"

struct SPIPeriph : public SPIHandleTypeDef {
public:
	void Init(SPIInitTypeDef*);
	inline void Transmit(uint8_t data) {SPDR = data;}
	inline uint8_t Receive() {return (SPDR);}
	inline uint8_t IsTransmissionComplete() {return (SPSR & (1 << SPIF));}
};

#endif // !_PeriphSPI_H
