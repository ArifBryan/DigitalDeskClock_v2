#ifndef _I2CHandle_
#define _I2CHandle_

#include <stdint.h>

struct I2CInitTypeDef {
	uint32_t CPUFreq;
	uint32_t SCLFreq;
	uint8_t OwnAddress;
	uint8_t GeneralCallEnable;
	uint8_t InterruptEnable;
};

struct I2CHandleTypeDef {
public:
	virtual void Init(I2CInitTypeDef*) = 0;
	virtual void GenerateStart() = 0;
	virtual void GenerateStop() = 0;
	virtual void Transmit(uint8_t) = 0;
	virtual void SetACK(uint8_t) = 0;
	virtual uint8_t Receive() = 0;
	virtual uint8_t IsStartCondition() = 0;
	virtual uint8_t IsTransmissionComplete() = 0;
	virtual uint8_t IsACK() = 0;
	virtual uint8_t IsBusy() = 0;
	virtual uint8_t Read(uint8_t, uint8_t[], uint8_t, uint8_t[], uint16_t, uint16_t) = 0;
	virtual uint8_t Write(uint8_t, uint8_t[], uint16_t, uint16_t) = 0;
};

#endif
