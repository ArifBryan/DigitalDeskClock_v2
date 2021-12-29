#ifndef _SPIHandle_H
#define _SPIHandle_H

#include <inttypes.h>

#define SPI_MODE_MASTER			1U
#define SPI_MODE_SLAVE			0U
#define SPI_DATAALIGN_LSBFirst	1U
#define SPI_DATAALIGN_MSBFirst	0U
#define SPI_CLOCKPOLARITY_HIGH	0U
#define SPI_CLOCKPOLARITY_LOW	1U
#define SPI_CLOCKPHASE_LEADEDGE		0U
#define SPI_CLOCKPHASE_TRAILEDGE	1U
#define SPI_PRESCALER_4			0b000U
#define SPI_PRESCALER_16		0b001U
#define SPI_PRESCALER_64		0b010U
#define SPI_PRESCALER_128		0b011U
#define SPI_PRESCALER_2			0b100U
#define SPI_PRESCALER_8			0b101U
#define SPI_PRESCALER_32		0b110U

struct SPIInitTypeDef {
	uint8_t Mode;
	uint8_t InterruptEnable;
	uint8_t DataAlign;
	uint8_t ClockPolarity;
	uint8_t ClockPhase;
	uint8_t Prescaler;
};

struct SPIHandleTypeDef {
public:
	virtual void Init(SPIInitTypeDef*) = 0;
	virtual void Transmit(uint8_t) = 0;
	virtual uint8_t IsTransmissionComplete() = 0;
	virtual uint8_t Receive() = 0;
};

#endif // !_SPIHandle_H
