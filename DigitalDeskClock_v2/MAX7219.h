/*
 * MAX7219.h
 *
 * Created: 13/08/2020 8:57:55 PM
 *  Author: Arif Bryan
 */


#ifndef MAX7219_H_
#define MAX7219_H_

#include "SPIHandle.h"
#include <string.h>
#include "font1.h"

//const uint8_t font = _font1_ascii1;

#define MAX7219_REG_NOOP		0x00
#define MAX7219_REG_DECODE		0x09
#define MAX7219_REG_INTENSITY	0x0A
#define MAX7219_REG_SCANLIMIT	0x0B
#define MAX7219_REG_SHUTDOWN	0x0C
#define MAX7219_REG_DISPTEST	0x0F

struct MAX7219 {
public:
    MAX7219(SPIHandleTypeDef *SPIdev, uint8_t ChipStack, volatile uint8_t *csPORT, uint8_t csBIT) {
        dev = SPIdev;
        chipCnt = ChipStack;
        csPort = csPORT;
        csBit = csBIT;
    }
    void Write(uint8_t Chip, uint8_t Address, uint8_t Data);
    void Init();
	void SetBuffer(uint8_t *buffer){this->buffer = buffer;}
	void DrawPix(uint16_t x, uint8_t y, uint8_t state);
	void DrawBuffer(uint8_t rotation);
	void ClearBuffer(){memset(buffer, 0, chipCnt * 8);}
	void Cursor(uint16_t x, uint8_t y){cx = x; cy = y;}
	void Print(char c);
	void Print(const char *str){
		uint8_t i = 0;
		while(str[i] != 0){Print(str[i++]);}
	}
private:
    uint8_t chipCnt;
    volatile uint8_t *csPort;
    uint8_t csBit;
    SPIHandleTypeDef *dev;
	uint8_t *buffer;
	uint16_t cx;
	uint8_t cy;
};

#endif /* MAX7219_H_ */