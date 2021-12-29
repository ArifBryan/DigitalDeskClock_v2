#ifndef _I2CPeriph_H
#define _I2CPeriph_H

#include <util/twi.h>
#include "I2CHandle.h"

struct I2CPeriph : public I2CHandleTypeDef {
public:
	void Init(I2CInitTypeDef*);
	inline void GenerateStart() {TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (twieBit << TWIE);}
	inline void GenerateStop() {TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) | (twieBit << TWIE); }
	inline void Transmit(uint8_t data) {TWDR = data;	TWCR = (1 << TWINT) | (1 << TWEN) | (twieBit << TWIE); }
	inline void SetACK(uint8_t ack) {tweaBit = ack & 1;}
	inline uint8_t Receive()
		{uint8_t data = TWDR; TWCR = (1 << TWINT) | (twieBit << TWIE) | (tweaBit << TWEA) | (1 << TWEN); return (data);}
	inline uint8_t IsStartCondition() {return (((TWSR & 0xF8) == TW_START) | ((TWSR & 0xF8) == TW_REP_START));}
	inline uint8_t IsTransmissionComplete() {return (TWCR & (1 << TWINT));}
	inline uint8_t IsBusy() {return (TWSR & (1 << TWSTO));}
	inline uint8_t IsACK() {
		return (((TWSR & 0xF8) == TW_MT_SLA_ACK) | ((TWSR & 0xF8) == TW_MT_DATA_ACK) | ((TWSR & 0xF8) == TW_MR_SLA_ACK));}
	uint8_t Read(uint8_t, uint8_t[], uint8_t, uint8_t[], uint16_t, uint16_t);
	uint8_t Write(uint8_t, uint8_t[], uint16_t, uint16_t);
private:
	uint8_t twieBit;
	uint8_t tweaBit;
	uint32_t cpuFreq;
};

#endif // !_periphI2C_H

