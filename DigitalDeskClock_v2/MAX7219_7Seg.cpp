/*
 * MAX7219_7Seg.cpp
 *
 * Created: 14/08/2020 12:08:25 AM
 *  Author: Arif Bryan
 */

#include "MAX7219_7Seg.h"

void MAX7219_7Seg::Write(uint8_t Chip, uint8_t Address, uint8_t Data) {
    if (Chip <= chipCnt) {Chip = chipCnt - Chip;}
    *csPort &= ~(1 << csBit);
    for (uint8_t j = 0; j < chipCnt; j++) {
        if (j == Chip || Chip > chipCnt) {dev->Transmit(Address);}
        else {dev->Transmit(MAX7219_REG_NOOP);}
        while (!dev->IsTransmissionComplete());
        dev->Transmit(Data);
        while (!dev->IsTransmissionComplete());
    }
    *csPort |= (1 << csBit);
}

void MAX7219_7Seg::Init() {
    Write(chipCnt + 1, MAX7219_REG_SHUTDOWN, 0);
    for (uint8_t i = 0; i < 8; i++) {
        Write(chipCnt + 1, i + 1, 0);
    }
    Write(chipCnt + 1, MAX7219_REG_SCANLIMIT, 7);
    Write(chipCnt + 1, MAX7219_REG_DISPTEST, 0);
    Write(chipCnt + 1, MAX7219_REG_INTENSITY, 7);
    Write(chipCnt + 1, MAX7219_REG_SHUTDOWN, 1);
}
