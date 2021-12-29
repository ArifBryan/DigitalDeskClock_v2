#include "I2CPeriph.h"

void I2CPeriph::Init(I2CInitTypeDef *i2cInit) {
    cpuFreq = i2cInit->CPUFreq;
    TWBR = (i2cInit->CPUFreq / i2cInit->SCLFreq - 16) / 2;
    TWSR = (0 << TWPS1) | (0 << TWPS0);
    TWAR = (i2cInit->OwnAddress & 0b11111110) | (i2cInit->GeneralCallEnable & 1);
    twieBit = i2cInit->InterruptEnable;
    TWCR = (1 << TWEN) | (twieBit << TWIE);
}

uint8_t I2CPeriph::Read(uint8_t devAddr, uint8_t regAddr[], uint8_t regAddrLen, uint8_t data[], uint16_t dataLen, uint16_t tout = 100) {
    uint8_t status = 0;
    tout *= TWBR * dataLen;

    //Start
    GenerateStart();
    while (!IsStartCondition() && tout > 0) {
        tout--;
    }
    //Device Address (Write)
    Transmit(devAddr);
    while (!IsTransmissionComplete() && tout > 0) {
        tout--;
    }
    if (IsACK()) {
        status = 1;
        for (uint8_t i = 0; i < regAddrLen; i++) {
            //Register Address
            Transmit(regAddr[i]);
            while (!IsTransmissionComplete() && tout > 0) {
                tout--;
            }
            status &= IsACK();
        }
        //Repeated Start
        GenerateStart();
        while (!IsStartCondition() && tout > 0) {
            tout--;
        }
        //Device Address (Read)
        Transmit(devAddr | 1);
        while (!IsTransmissionComplete() && tout > 0) {
            tout--;
        }
        status &= IsACK();
        if (dataLen == 1) {
            SetACK(0);
            Receive();
            while (!IsTransmissionComplete() && tout > 0) {
                tout--;
            }
            //Stop
            GenerateStop();
            data[0] = Receive();
        }
        else {
            uint16_t i;
            SetACK(1);
            Receive();
            while (!IsTransmissionComplete() && tout > 0) {
                tout--;
            }
            dataLen -= 1;
            for (i = 0; i < dataLen; i++) {
                SetACK(i + 1 < dataLen);
                data[i] = Receive();
                while (!IsTransmissionComplete() && tout > 0) {
                    tout--;
                }
            }
            GenerateStop();
            data[i] = Receive();
        }

    }
    else {
        //Stop
        GenerateStop();
    }

    return (status & (tout > 0));
}

uint8_t I2CPeriph::Write(uint8_t devAddr, uint8_t data[], uint16_t dataLen, uint16_t tout = 100) {
    uint8_t status = 0;
    tout *= TWBR * dataLen;

    //Start
    GenerateStart();
    while (!IsStartCondition() && tout > 0) {
        tout--;
    }
    //Device Address (Write)
    Transmit(devAddr);
    while (!IsTransmissionComplete() && tout > 0) {
        tout--;
    }
    if (IsACK()) {
        status = 1;
        for (uint16_t i = 0; i < dataLen; i++) {
            Transmit(data[i]);
            while (!IsTransmissionComplete() && tout > 0) {
                tout--;
            }
            status &= IsACK();
        }
    }
    //Stop
    GenerateStop();

    return (status & (tout > 0));
}