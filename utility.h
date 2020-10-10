/*
 * @file utility.h
 *
 * @date Jul 24, 2017
 * @author hamster
 *
 * Utility functions
 *
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#define CRC_SEED_DC26		0x0801
#define CRC_SEED_DC27		0x0180

uint16_t calcCRC(uint8_t *data, uint8_t len, const uint16_t POLYNOM);
uint16_t crc16(uint16_t crcValue, uint8_t newByte, const uint16_t POLYNOM);

uint8_t getButton(bool waitForLongPress);
uint8_t waitForButton(uint8_t timeout);

APP_TIMER_DEF(sysTickID);
void sysTickStart(void);
void sysTickHandler(void * p_context);
uint32_t getSystick(void);

uint32_t millis_elapsed(uint32_t currentMillis, uint32_t previousMillis);
uint32_t millis();

uint8_t getFiles(char files[][9], char *path, uint8_t fileMax);

#endif /* UTILITY_H_ */
