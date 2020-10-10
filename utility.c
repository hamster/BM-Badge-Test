/*
 * @file utility.c
 *
 * @date Jul 24, 2017
 * @author hamster
 *
 *  Utility functions
 *
 */

#include <hal/nrf_gpio.h>
#include "common.h"

volatile static uint32_t systick = 0;

/**
 * Start the local time reference
 * It's seeded at bootup from the user storage, if it exists
 * @param time
 */
void sysTickStart(void){
    systick = 0;
    app_timer_create(&sysTickID, APP_TIMER_MODE_REPEATED, sysTickHandler);
    app_timer_start(sysTickID, APP_TIMER_TICKS(1000), NULL);
}

/**
 * @return number of seconds since we started counting time
 */
uint32_t getSystick(void){
    return systick;
}

/**
 * Every second, update the systick handler
 * @param p_context
 */
void sysTickHandler(void * p_context){
    systick++;
}


/**
 * @brief Determine if a button is being pressed.
 * @param waitForLongPress true if we want to wait to see if a button is long pressed
 * @return Button pressed, with long press mask if long pressed
 *
 */
uint8_t getButton(bool waitForLongPress){

	return 0;

}

/**
 * @brief wait for any button to be pressed
 * @param timeout milliseconds to busy wait for a button press, or 0 for forever
 * @return which button was pressed
 */
uint8_t waitForButton(uint8_t timeout){

    bool forever = false;
    if(timeout == 0){
        forever = true;
    }

    while(forever || timeout){
        if(keyboardInterrupt()){
            return 1;
        }
        nrf_delay_ms(1);
        if(timeout > 0){
            timeout--;
        }
    }

	return 0;

}

/**
 * Get a list of files on a path
 * @param files
 * @param path
 * @param fileMax
 * @return
 */
uint8_t getFiles(char files[][9], char *path, uint8_t fileMax){

    FRESULT ff_result;
    DIR dir;
    FILINFO fno;

    ff_result = f_opendir(&dir, path);
    if (ff_result) {
        printf("Can't open path\n");
        return 0;
    }

    uint8_t counter = 0;
    for (uint8_t i = 0; i < fileMax; i++) {
        ff_result = f_readdir(&dir, &fno);                   /* Read a directory item */
        if (ff_result != FR_OK || fno.fname[0] == 0) {
            break;  /* Break on error or end of dir */
        }
        if ((fno.fattrib & AM_DIR)) {
            // Ignore subdirs
        }
        else{
            char *ext = strrchr(fno.fname, '.') + 1;
            if (strcmp(ext, "RAW") == 0){
                // Add the file
                memcpy(&files[counter++], fno.fname, ext - fno.fname - 1);
            }
        }
    }
    f_closedir(&dir);

    return counter;
}

/**
 * Calculate the CRC on a chunk of memory
 * @param data
 * @param len
 * @return
 */
uint16_t calcCRC(uint8_t *data, uint8_t len, const uint16_t POLYNOM){
    uint16_t crc;
    uint8_t aux = 0;

    crc = 0x0000;

    while (aux < len){
        crc = crc16(crc, data[aux], POLYNOM);
        aux++;
    }

    return (crc);
}

/**
 * Calculate the crc16 of a value
 * @param crcValue
 * @param newByte
 * @return
 */
uint16_t crc16(uint16_t crcValue, uint8_t newByte, const uint16_t POLYNOM){
    uint8_t i;

    for (i = 0; i < 8; i++) {

        if (((crcValue & 0x8000) >> 8) ^ (newByte & 0x80)){
            crcValue = (crcValue << 1)  ^ POLYNOM;
        }else{
            crcValue = (crcValue << 1);
        }

        newByte <<= 1;
    }

    return crcValue;
}

#define OVERFLOW ((uint32_t)(0xFFFFFFFF/32.768))

uint32_t millis_elapsed(uint32_t currentMillis, uint32_t previousMillis)
{
    if(currentMillis < previousMillis) return(currentMillis + OVERFLOW + 1 - previousMillis);
    return(currentMillis - previousMillis);
}

uint32_t millis(void)
{
    return(app_timer_cnt_get() / 32.768);
}

