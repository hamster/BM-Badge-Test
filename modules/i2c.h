//
// Created by hamster on 5/23/19.
//

#ifndef I2C_H
#define I2C_H

void twi_master_init(void);
void i2cMasterTransmit(uint16_t addr, uint8_t const * pdata, size_t size);

#endif //I2C_H