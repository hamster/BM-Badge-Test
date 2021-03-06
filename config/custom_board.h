/**
 *
 * @file custom_board.h
 *
 * @date May 24, 2017
 * @author hamster
 *
 * This is the custom board defines for the LEDs and etc on the DC25 badge
 *
 */

#ifndef DC801_H
#define DC801_H

#include "nrf52840.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defines for the SDK

#define BUTTONS_NUMBER		0
#define BUTTONS_ACTIVE_STATE 	NRF_GPIO_PIN_SENSE_LOW
#define USER_BUTTON_NONE        0	

// Keyboard config
#define KEYBOARD_ADDRESS	0x23
#define KEYBOARD_INT_PIN	18


// LEDs definitions

#define VOLTAGE_SENSE		4

#define RX_PIN_NUMBER  		8
#define TX_PIN_NUMBER  		6
#define CTS_PIN_NUMBER 		NRF_UART_PSEL_DISCONNECTED
#define RTS_PIN_NUMBER 		NRF_UART_PSEL_DISCONNECTED
#define HWFC           		false


// LCD
#define LCD_SPIM_INSTANCE   3

#define ILI9341_SCK_PIN		28
#define ILI9341_MOSI_PIN	30
#define ILI9341_PIN_CS		31
#define ILI9341_PIN_DC		29
#define ILI9341_PIN_LED		0
#define ILI9341_PIN_RESET	10+32

#define LCD_WIDTH  			320
#define LCD_HEIGHT 			240
#define LCD_X_OFFSET		0
#define LCD_Y_OFFSET		0

// I2C configuration
#define I2C_INSTANCE        0

#define I2C_SCL_PIN         23
#define I2C_SDA_PIN         22

// SD card interface
#define SDC_SCK_PIN         17
#define SDC_MOSI_PIN        21
#define SDC_MISO_PIN        16
#define SDC_CS_PIN          19

// Clock
// Low frequency clock source to be used by the SoftDevice
// We're using the internal RC osc
#define NRF_CLOCK_LFCLKSRC      {.source        = NRF_CLOCK_LF_SRC_RC, \
                                 .rc_ctiv       = 16, \
                                 .rc_temp_ctiv  = 2, \
                                 .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM}


#ifdef __cplusplus
}
#endif

#endif // DC801_H
