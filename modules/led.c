/*-
 * Copyright (c) 2019
 *      Bill Paul <wpaul@windriver.com>.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Bill Paul.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Bill Paul AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL Bill Paul OR THE VOICES IN HIS HEAD
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This module implements some very basic support for the LED controller
 * on the DC27 badge board. We want to use the LEDs as a progress and
 * success/failure indicator during the flash update, since we can't
 * easily use the screen. We assume that the badge firmware already
 * did most of the initialization of the controller chip and just make
 * sure here select the PWM register page so that we can write to the
 * PWM control registers. We use a single scrolling blue LED to indicate
 * flashing progress. For an error we illuminate all LEDs red, and if
 * the firmware update succeeds we set them all green.
 */

#include <common.h>
#include "led.h"
#include "i2c.h"

#include "nrf_delay.h"

static void ledSet (uint8_t, uint8_t);
static void ledPageSet (uint8_t);
static void ledRegSet (uint8_t, uint8_t);
static void ledRegGet (uint8_t);

static const uint8_t led_address[ISSI_LED_COUNT] = {
        0x70,   // LED_XOR
        0x72,   // LED_ADD
        0x74,   // LED_SUB
        0x76,   // LED_PAGE
        0x60,   // LED_BIT128
        0x62,   // LED_BIT64
        0x64,   // LED_BIT32
        0x66,   // LED_BIT16
        0x50,   // LED_BIT8
        0x52,   // LED_BIT4
        0x54,   // LED_BIT2
        0x56,   // LED_BIT1
        0x36,   // LED_MEM3
        0x34,   // LED_MEM2
        0x32,   // LED_MEM1
        0x30,   // LED_MEM0
        0x40,   // LED_USB
        0x42,   // LED_HAX
        0x44    // LED_SD

};


void ledInit (void){
    int i;
    
    /* Select function page */

    ledPageSet (ISSI_PAGE_FUNCTION);

    /* Reset the chip */

    ledRegGet (ISSI_REG_RESET);

    /* Select function page */

    ledPageSet (ISSI_PAGE_FUNCTION);

    /* Set configuration - disable soft shutdown */

    ledRegSet (ISSI_REG_CONFIG, ISSI_REG_CONFIG_SSD_OFF | ISSI_REG_CONFIG_B_EN);

    /* Set global current control*/

    ledRegSet (ISSI_REG_GCC, LED_INTENSITY);

    /* Set pullups */
    ledRegSet (ISSI_REG_SWY_PULLUP, 0xFF);

    /* Set pulldowns */
    ledRegSet (ISSI_REG_CSX_PULLDOWN, 0xFF);
    
    /* Set breath speeds */
    ledRegSet(ISSI_REG_ABM1_IN, 0x24); // .42s ramp, .42s hold
    ledRegSet(ISSI_REG_ABM1_OUT, 0x24); // .42s decline, .42s pause
    
    /* Reset timer (also updates breathing values) */
    ledRegSet(ISSI_REG_TIME, 0);

    /* Turn on LEDs */

    ledPageSet (ISSI_PAGE_LED);

    for (i = 0; i < 0x18; i++) {
        ledRegSet(i, 0xFF);
    }
    
    /* Select PWM register page */

    ledPageSet (ISSI_PAGE_PWM);
    
    return;
}

static void ledPageSet (uint8_t page){
    /* Unlock command register */

    ledRegSet (ISSI_REG_COMMAND_UNLOCK, ISSI_CMDUNLOCK_ENABLE);

    /* Select register page */

    ledRegSet (ISSI_REG_COMMAND, page);

    return;
}

static void ledRegGet (uint8_t reg){
    uint8_t rxbuf = 0;
    uint8_t txbuf;

    txbuf = reg;

    i2cMasterTransmit(ISSI_I2C_ADDR, &txbuf, 1);

}

static void ledRegSet (uint8_t reg, uint8_t val){
    uint8_t txbuf[2];

    txbuf[0] = reg;
    txbuf[1] = val;

    i2cMasterTransmit(ISSI_I2C_ADDR, txbuf, 2);

    return;
}

//void ledShow (void){
//    //old function no longer needed
//    /*
//     * Assume that we're still on the PWM control page.
//     * Write the whole LED "frame buffer" in one transaction.
//     * Note: the first element in the led_memory[] array is
//     * the offset of the first LED PWM duty cycle register,
//     * which happens to be 0. After that's set, the rest
//     * of the buffer is written to the PWM duty cycle
//     * registers in sequence using the autoincrement feature.
//     */
//
//    /*i2cMasterTransmit(ISSI_I2C_ADDR, led_memory, sizeof(led_memory));
//
//    ledPageSet(ISSI_PAGE_BREATH);
//
//    i2cMasterTransmit(ISSI_I2C_ADDR, led_breath, sizeof(led_breath));
//
//    ledPageSet(ISSI_PAGE_PWM);*/
//
//    return;
//}

static void ledSet (uint8_t index, uint8_t intensity){
    if (index > ISSI_LED_COUNT) {
        return;
    }
    
    ledPageSet(ISSI_PAGE_PWM);
    
    ledRegSet(led_address[index] , intensity);
    
    ledPageSet(ISSI_PAGE_BREATH);
    
    ledRegSet(led_address[index] , 0);
    

    //led_memory[led_address[index] + 1] = intensity;
    //led_breath[led_address[index] + 1] = 0;

    return;
}


void ledsOff (void){
    int i;
    for (i = 0; i < ISSI_LED_COUNT; i++){
        ledSet(i, 0);
    }
    return;
}

void ledsOn (void){
    int i;
    for (i = 0; i < ISSI_LED_COUNT; i++){
        ledSet(i, 0xff);
    }
    return;
}

void ledOn(LEDID id) {
    ledSet(id, 0xff);
}

void ledOff(LEDID id) {
    ledSet(id, 0);
}


void ledPulse(LEDID id) {
    if (id > ISSI_LED_COUNT) {
        return;
    }
    
    ledPageSet(ISSI_PAGE_BREATH);
    
    ledRegSet(led_address[id] , 1);
}

void ledPulseFast(LEDID id) {
    if (id > ISSI_LED_COUNT) {
        return;
    }
    
    ledPageSet(ISSI_PAGE_BREATH);
    
    ledRegSet(led_address[id] , 2);
}

//0 to 255
void ledPwm(LEDID id, uint8_t val) {
    ledSet(id, val);
}


