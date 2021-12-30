/*
 * MAX7219_7Seg.cpp
 *
 * Created: 14/08/2020 12:08:25 AM
 *  Author: Arif Bryan
 */

#include "MAX7219.h"
#include <avr/pgmspace.h>

void MAX7219::Write(uint8_t Chip, uint8_t Address, uint8_t Data) {
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

void MAX7219::Init() {
    Write(chipCnt + 1, MAX7219_REG_SHUTDOWN, 0);
    for (uint8_t i = 0; i < 8; i++) {
        Write(chipCnt + 1, i + 1, 0);
    }
    Write(chipCnt + 1, MAX7219_REG_SCANLIMIT, 7);
    Write(chipCnt + 1, MAX7219_REG_DISPTEST, 0);
    Write(chipCnt + 1, MAX7219_REG_INTENSITY, 7);
    Write(chipCnt + 1, MAX7219_REG_SHUTDOWN, 1);
}

void MAX7219::Print(char c){
	uint8_t chr = c - ' ';
	uint8_t lx = pgm_read_byte(&_font1_ascii1[chr][5]);
	for(uint8_t i = 0; i < lx; i ++){
		if(i + (int16_t)cx >= chipCnt * 8){break;}
		if(i + (int16_t)cx >= 0){
			for(uint8_t j = 0; j < 8; j ++){
				if((7 - j) + (int8_t)cy < 8 && (7 - j) + (int8_t)cy >= 0){
					DrawPix(i + cx, (7 - j) + cy, (pgm_read_byte(&_font1_ascii1[chr][i]) >> j) & 1);
				}
			}
		}
	}
	if(lx + cx < chipCnt * 8){
		for(uint8_t j = 0; j < 8; j ++){
			if((7 - j) + (int8_t)cy >= 8 && (7 - j) + (int8_t)cy < 0){break;}
			DrawPix(lx + cx , j + cy, 0);
		}
	}
	cx += lx + 1;
}

void MAX7219::DrawPix(uint16_t x, uint8_t y, uint8_t state){
	uint16_t px = chipCnt * 8 - 1 - x;
	uint8_t py = 7 - y;
	uint16_t p = px / 8 + py * chipCnt;
	p = (p >= chipCnt * 8 ? chipCnt * 8 - 1 : p);
	uint8_t tmp = buffer[p];
	
	if(state){
		tmp |= (1 << (px % 8));
	}
	else{
		tmp &= ~(1 << (px % 8));
	}
	
	buffer[p] = tmp;
}

void MAX7219::DrawBuffer(uint8_t rotation){
	if(rotation){
		for(uint8_t i = 0; i < 8; i ++){
			*csPort &= ~(1 << csBit);
			for (uint8_t j = 0; j < chipCnt; j ++) {
				uint8_t t = 0;
				for(uint8_t k = 0; k < 8; k ++){
					t |= ((buffer[(7 - i) * chipCnt + j] >> (7 - k)) & 1) << k;
				}
				dev->Transmit(i + 1);
				while (!dev->IsTransmissionComplete());
				dev->Transmit(t);
				while (!dev->IsTransmissionComplete());
			}
			*csPort |= (1 << csBit);
		}
	}
	else{
		for(uint8_t i = 0; i < 8; i ++){
			*csPort &= ~(1 << csBit);
			for (uint8_t j = chipCnt + 1; j > 0; j--) {
				dev->Transmit(i + 1);
				while (!dev->IsTransmissionComplete());
				dev->Transmit(buffer[i * chipCnt + (j - 1)]);
				while (!dev->IsTransmissionComplete());
			}
			*csPort |= (1 << csBit);
		}
	}
}
