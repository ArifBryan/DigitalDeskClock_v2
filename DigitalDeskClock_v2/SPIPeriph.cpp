#include "SPIPeriph.h"

void SPIPeriph::Init(SPIInitTypeDef *spiInit) {
	SPCR = (spiInit->InterruptEnable << SPIE) | (1 << SPE) | (spiInit->DataAlign << DORD) | (spiInit->Mode << MSTR) |
			(spiInit->ClockPolarity << CPOL) | (spiInit->ClockPhase << CPHA) | ((spiInit->Prescaler & 0b011) << SPR0);
	SPSR = (((spiInit->Prescaler >> 2) & 1) << SPI2X);
}