//
// Keyboard routines
//
//

#include "common.h"
#include "keyboard.h"

/**
 * Initialize the keyboard interface
 */
void keyboardInit(void){
    nrf_gpio_cfg_input(KEYBOARD_INT_PIN, NRF_GPIO_PIN_NOPULL);
}

/**
 * Determine if an interrupt is occuring
 */
bool keyboardInterrupt(void){
    return nrf_gpio_pin_read(KEYBOARD_INT_PIN) == 0;
}