/*
 * DS3231.h
 *
 * Created: 04/07/2020 5:37:02 PM
 *  Author: Arif Bryan
 */


#ifndef _DS3231_H_
#define _DS3231_H_

#include "I2CHandle.h"

#define DS3231_I2C_ADDRESS	0b11010000
#define DS3231_REG_SECONDS	0x00
#define DS3231_REG_MINUTES	0x01
#define DS3231_REG_HOUR		0x02
#define DS3231_REG_DAY		0x03
#define DS3231_REG_DATE		0x04
#define DS3231_REG_MONTH	0x05
#define DS3231_REG_YEAR		0x06
#define DS3231_REG_AL1SEC	0x07
#define DS3231_REG_AL1MIN	0x08
#define DS3231_REG_AL1HRS	0x09
#define DS3231_REG_AL1DAT	0x0A
#define DS3231_REG_AL2MIN	0x0B
#define DS3231_REG_AL2HRS	0x0C
#define DS3231_REG_AL2DAT	0x0D
#define DS3231_REG_CONTROL	0x0E
#define DS3231_REG_STATUS	0x0F
#define DS3231_REG_AGING	0x10
#define DS3231_REG_TEMPMSB	0x11
#define DS3231_REG_TEMPLSB	0x12

#define DS3231_AL1MODE_EverySecond		0b01111
#define DS3231_AL1MODE_SecondMatch		0b01110
#define DS3231_AL1MODE_MinSecMatch		0b01100
#define DS3231_AL1MODE_HrsMinSecMatch	0b01000
#define DS3231_AL1MODE_DateTimeMatch	0b00000
#define DS3231_AL1MODE_DayTimeMatch		0b10000

#define DS3231_AL2MODE_EveryMinute		0b0111
#define DS3231_AL2MODE_MinuteMatch		0b0110
#define DS3231_AL2MODE_HrsMinMatch		0b0100
#define DS3231_AL2MODE_DateTimeMatch	0b0000
#define DS3231_AL2MODE_DayTimeMatch		0b1000

struct DS3231 {
public:
    DS3231(I2CHandleTypeDef *i2c) {
        i2cdev = i2c;
    }
    uint8_t SetTime(uint8_t Sec, uint8_t Min, uint8_t Hour);
    uint8_t SetDate(uint8_t Day, uint8_t Date, uint8_t Month, uint8_t Year);
    uint8_t GetTime(uint8_t *Sec, uint8_t *Min, uint8_t *Hour);
    uint8_t GetDate(uint8_t *Day, uint8_t *Date, uint8_t *Month, uint8_t *Year);
    int16_t GetTemperature(void);
    uint8_t ConvertTemperature(void);
    uint8_t InterruptAlarm1(uint8_t Enable);
    uint8_t InterruptAlarm2(uint8_t Enable);
    uint8_t IsActiveFlagAlarm1(void);
    uint8_t IsActiveFlagAlarm2(void);
    uint8_t ClearFlagAlarm1(void);
    uint8_t ClearFlagAlarm2(void);
    uint8_t SetAlarm1(uint8_t Sec, uint8_t Min, uint8_t Hour, uint8_t DayDate, uint8_t Mode);
    uint8_t SetAlarm2(uint8_t Min, uint8_t Hour, uint8_t DayDate, uint8_t Mode);
private:
    uint8_t Write(uint8_t regAddr, uint8_t data[], uint8_t dataLen);
    uint8_t Read(uint8_t regAddr, uint8_t data[], uint8_t dataLen);
    uint8_t Write(uint8_t regAddr, uint8_t data) {
        uint8_t t[1] = {data};
        return (Write(regAddr, t, 1));
    }
    uint8_t Read(uint8_t regAddr) {
        uint8_t r[1];
        Read(regAddr, r, 1);
        return (r[0]);
    }

    I2CHandleTypeDef *i2cdev;
};

#endif /* _DS3231_H_ */