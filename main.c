/**
 *
 * @file main.c
 *
 * @date May 24, 2017
 * @author hamster
 *
 * @brief This is the entry point for the DC801 DC26 party badge
 *
 */

#include <stdint.h>
#include "common.h"

#define XVAL(x) #x
#define VAL(x)  XVAL(x)

/*
#pragma message ("TIMER_ENABLED=" VAL(TIMER_ENABLED))
#pragma message ("TIMER1_ENABLED=" VAL(TIMER1_ENABLED))
#pragma message ("NRFX_TIMER_ENABLED=" VAL(NRFX_TIMER_ENABLED))
#pragma message ("NRFX_TIMER1_ENABLED=" VAL(NRFX_TIMER1_ENABLED))
*/


/**
 * Initialize the logging backend for logging over JTAG
 */
static void log_init(void){
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**
 * Handler to show the LEDs during bootup animation
 * @param frame
 * @param p_data
 */
static void bootCallback(uint8_t frame, void *p_data){
}


/**
 * @brief Main app
 * @return Not used
 */
int main(void){

    // Setup the system
    log_init();

    keyboard_init();

    // Timers
    app_timer_init();
    //usb_serial_init();
    
    // BLE
    //gap_params_init();
    ble_stack_init();
    scan_start();

    // Init the display
    ili9341_init();
    ili9341_start();
	util_gfx_init();

    // Init the random number generator
    nrf_drv_rng_init(NULL);

    // Setup the battery monitor
    //adc_configure();
    //adc_start();

    // Setup the UART
    //uart_init();

    // Setup I2C
    twi_master_init();

    // Setup LEDs
    ledInit();
    ledsOff();
    
    if(!util_sd_init()){
        //util_sd_error();
    }

    // Boot! Boot! Boot!
    printf("Booted!\n");
    // printf goes to the RTT_Terminal.log after you've fired up debug.sh
    
    ledsOff();

    // Configure the systick
    sysTickStart();
    
    char* ble_name = "BMBadge"; // must be 10char
    printf("advertising user: %s\n", ble_name);
    advertising_setUser(ble_name);
    ble_adv_start();

    char keymap[33];
    memset(keymap, '^', 27);
    keymap[27] = '\0';

    util_gfx_fill_screen(COLOR_BLACK);

    util_gfx_fill_rect(0, 0, 25, 25, COLOR_CYAN);
    util_gfx_fill_rect(295, 0, 25, 25, COLOR_YELLOW);
    util_gfx_fill_rect(0, 215, 25, 25, COLOR_PURPLE);
    util_gfx_fill_rect(295, 215, 25, 25, COLOR_BLUE);

    util_gfx_set_font(FONT_MONO55_8PT);
    util_gfx_set_color(COLOR_WHITE);
    util_gfx_set_cursor(65, 90);
    util_gfx_print("DC801 BM Badge Fab Tester");
    util_gfx_set_cursor(115, 110);
    util_gfx_print("Toggle a key");
    util_gfx_set_cursor(122, 140);
    util_gfx_print("Key States");
    util_gfx_set_cursor(55, 160);
    util_gfx_print(keymap);

    util_gfx_set_cursor(130, 210);
    util_gfx_print("Rev 1.1");

    int LEDCounter = 0;
    int curLED = LED_XOR;
    ledsOn();
    ledOff(curLED);

    bool sawKey = false;
    int sawKeyCounter = 0;

    uint32_t keyboardMask = 0;
    uint32_t oldKeyboardMask = 0;



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    while(true) {

	// Check if the interrupt is set
        if(is_keyboard_interrupt()){
            util_gfx_fill_rect(140, 0, 25, 25, COLOR_RED);
        }
        else{
            util_gfx_fill_rect(140, 0, 25, 25, COLOR_BLACK);
        }


        // Get the current keyboard mask
        keyboardMask = get_keyboard_mask();

        for(int i = 0; i < 27; i++){
            if(((keyboardMask >> i) & 0x01) == 1){
                keymap[i] = '_';
            }
            else{
                keymap[i] = '^';
            }
            keymap[27] = '\0';
        }

        // See if a key has changed state
        if(oldKeyboardMask != keyboardMask){

            if(!sawKey) {
                util_gfx_fill_rect(100, 108, 150, 20, COLOR_BLACK);
                util_gfx_set_cursor(102, 110);
                util_gfx_set_color(COLOR_RED);
                util_gfx_print("Key State Change!");
                sawKey = true;
            }
            sawKeyCounter = 0;

            util_gfx_set_color(COLOR_WHITE);
            util_gfx_fill_rect(35, 158, 300, 20, COLOR_BLACK);
            util_gfx_set_cursor(55, 160);
            util_gfx_print(keymap);
        }

        // Save the old keymask for next loop
        oldKeyboardMask = keyboardMask;

        // Clear the 'key detected' if it was set and the timeout is reached
        if(sawKey){
            if(sawKeyCounter++ > 50){
                //util_gfx_fill_screen(COLOR_BLACK);
                util_gfx_fill_rect(100, 108, 150, 20, COLOR_BLACK);
                util_gfx_set_cursor(115, 110);
                util_gfx_set_color(COLOR_WHITE);
                util_gfx_print("Toggle a key");
                sawKey = false;
            }
        }

        if(LEDCounter++ > 10){
                LEDCounter = 0;
                ledOn(curLED);
                curLED++;
                if(curLED == ISSI_LED_COUNT){
                        curLED = LED_XOR;
                }
                ledOff(curLED);
        }

        nrf_delay_ms(5);

    }


#pragma clang diagnostic pop

}

