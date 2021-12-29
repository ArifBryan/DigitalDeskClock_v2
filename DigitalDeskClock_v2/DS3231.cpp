/*
 * DS3231.cpp
 *
 * Created: 05/07/2020 12:18:33 AM
 *  Author: Arif Bryan
 */

#include <string.h>
#include "DS3231.h"

uint8_t DS3231::Write(uint8_t regAddr, uint8_t data[], uint8_t dataLen) {
    uint8_t t[dataLen + 1];
    t[0] = regAddr;
    memcpy(t + 1, data, dataLen);
    return (i2cdev->Write(DS3231_I2C_ADDRESS, t, dataLen + 1, 100));
}

uint8_t DS3231::Read(uint8_t regAddr, uint8_t data[], uint8_t dataLen) {
    uint8_t t[1] = {regAddr};
    return (i2cdev->Read(DS3231_I2C_ADDRESS, t, 1, data, dataLen, 100));
}

uint8_t DS3231::SetTime(uint8_t Sec, uint8_t Min, uint8_t Hour) {
    uint8_t t[3];
    t[0] = ((Sec / 10) << 4) | (Sec % 10);
    t[1] = ((Min / 10) << 4) | (Min % 10);
    t[2] = (0 << 6) | ((Hour / 10) << 4) | (Hour % 10);
    return (Write(DS3231_REG_SECONDS, t, 3));
}

uint8_t DS3231::SetDate(uint8_t Day, uint8_t Date, uint8_t Month, uint8_t Year) {
    uint8_t t[4];
    t[0] = Day & 0b0111;
    t[1] = ((Date / 10) << 4) | (Date % 10);
    t[2] = ((Year / 100) << 7) | ((Month / 10) << 4) | (Month % 10);
    t[3] = ((Year / 10) << 4) | (Year % 10);
    return (Write(DS3231_REG_DAY, t, 4));
}

uint8_t DS3231::GetTime(uint8_t *Sec, uint8_t *Min, uint8_t *Hour) {
    uint8_t t[3];
    uint8_t status = Read(DS3231_REG_SECONDS, t, 3);
    *Sec = ((t[0] >> 4) * 10) + (t[0] & 0x0F);
    *Min = ((t[1] >> 4) * 10) + (t[1] & 0x0F);
    *Hour = ((t[2] >> 4) * 10) + (t[2] & 0x0F);
    return (status);
}

uint8_t DS3231::GetDate(uint8_t *Day, uint8_t *Date, uint8_t *Month, uint8_t *Year) {
    uint8_t t[4];
    uint8_t status = Read(DS3231_REG_DAY, t, 4);
    *Day = t[0];
    *Date = ((t[1] >> 4) * 10) + (t[1] & 0x0F);
    *Month = (((t[2] >> 4) & 0b0001) * 10) + (t[2] & 0x0F);
    *Year = ((t[3] >> 4) * 10) + (t[3] & 0x0F);
    return (status);
}

uint8_t DS3231::ConvertTemperature() {
    uint8_t t = Read(DS3231_REG_CONTROL);
    t |= (1 << 5);
    return (Write(DS3231_REG_CONTROL, t));
}

int16_t DS3231::GetTemperature() {
    uint8_t t[2];
    Read(DS3231_REG_TEMPMSB, t, 2);
    int16_t r, s;
    r = t[0] * 10;
    s = (t[1] >> 6) * 2.5;
    return (r + s);
}

uint8_t DS3231::SetAlarm1(uint8_t Sec, uint8_t Min, uint8_t Hour, uint8_t DayDate, uint8_t Mode) {
    uint8_t t[4];
    t[0] = (Mode << 7) | ((Sec / 10) << 4) | (Sec % 10);
    t[1] = ((Mode >> 1) << 7) | ((Min / 10) << 4) | (Min % 10);
    t[2] = ((Mode >> 2) << 7) | (0 << 6) | ((Hour / 10) << 4) | (Hour % 10);
    t[3] = ((Mode >> 3) << 7) | (((Mode >> 4) & 0b1) << 6) | ((DayDate / 10) << 4) | (DayDate % 10);
    ClearFlagAlarm1();
    return (Write(DS3231_REG_AL1SEC, t, 4));
}

uint8_t DS3231::SetAlarm2(uint8_t Min, uint8_t Hour, uint8_t DayDate, uint8_t Mode) {
    uint8_t t[3];;
    t[0] = (Mode << 7) | ((Min / 10) << 4) | (Min % 10);
    t[1] = ((Mode >> 1) << 7) | (0 << 6) | ((Hour / 10) << 4) | (Hour % 10);
    t[2] = ((Mode >> 2) << 7) | (((Mode >> 3) & 0b1) << 6) | ((DayDate / 10) << 4) | (DayDate % 10);
    ClearFlagAlarm2();
    return (Write(DS3231_REG_AL2MIN, t, 3));
}

uint8_t DS3231::IsActiveFlagAlarm1() {
    return ((Read(DS3231_REG_STATUS) >> 0) & 0b1);
}

uint8_t DS3231::IsActiveFlagAlarm2() {
    return ((Read(DS3231_REG_STATUS) >> 1) & 0b1);
}

uint8_t DS3231::ClearFlagAlarm1() {
    uint8_t t = Read(DS3231_REG_STATUS);
    t &= ~(1 << 0);
    return (Write(DS3231_REG_STATUS, t));
}

uint8_t DS3231::ClearFlagAlarm2() {
    uint8_t t = Read(DS3231_REG_STATUS);
    t &= ~(1 << 1);
    return (Write(DS3231_REG_STATUS, t));
}

uint8_t DS3231::InterruptAlarm1(uint8_t Enable) {
    uint8_t t = Read(DS3231_REG_CONTROL);
    if (Enable) {
        t |= (1 << 0);
    }
    else {
        t &= ~(1 << 0);
    }
    t |= (1 << 2);
    return (Write(DS3231_REG_CONTROL, t));
}

uint8_t DS3231::InterruptAlarm2(uint8_t Enable) {
    Enable &= 0b1;
    uint8_t t = Read(DS3231_REG_CONTROL);
    if (Enable) {
        t |= (1 << 1);
    }
    else {
        t &= ~(1 << 1);
    }
    t |= (1 << 2);
    return (Write(DS3231_REG_CONTROL, t));
}

