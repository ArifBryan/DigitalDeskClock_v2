/*
 * MAX7219_7Seg.h
 *
 * Created: 13/08/2020 8:57:55 PM
 *  Author: Arif Bryan
 */


#ifndef MAX7219_7SEG_H_
#define MAX7219_7SEG_H_

#include "SPIHandle.h"

#define MAX7219_REG_NOOP		0x00
#define MAX7219_REG_DECODE		0x09
#define MAX7219_REG_INTENSITY	0x0A
#define MAX7219_REG_SCANLIMIT	0x0B
#define MAX7219_REG_SHUTDOWN	0x0C
#define MAX7219_REG_DISPTEST	0x0F

struct MAX7219_7Seg {
public:
    MAX7219_7Seg(SPIHandleTypeDef *SPIdev, uint8_t ChipStack, volatile uint8_t *csPORT, uint8_t csBIT) {
        dev = SPIdev;
        chipCnt = ChipStack;
        csPort = csPORT;
        csBit = csBIT;
    }
    void Write(uint8_t Chip, uint8_t Address, uint8_t Data);
    void Init();
private:
    uint8_t chipCnt;
    volatile uint8_t *csPort;
    uint8_t csBit;
    SPIHandleTypeDef *dev;
};

#endif /* MAX7219_7SEG_H_ */