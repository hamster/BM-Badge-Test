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
 * Initialize the buttons
 */
static void button_init(void){
    // Setup the buttons
    keyboardInit();
}

/**
 * Initialize the speaker
 */
static void speaker_init(void){
    // Setup the speaker
}

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
    button_init();
    speaker_init();
    
    
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
    

    //util_gfx_draw_raw_file("/TEST/TIMEHACK.RAW", 0, 0, GFX_WIDTH, GFX_HEIGHT, NULL, false, NULL);
    
    ledsOff();

    // Configure the systick
    sysTickStart();
    
    char* ble_name = "BMBadge"; // must be 10char
    printf("advertising user: %s\n", ble_name);
    advertising_setUser(ble_name);
    ble_adv_start();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

    util_gfx_fill_screen(COLOR_BLACK);
    util_gfx_set_font(FONT_COMPUTER_12PT);
   util_gfx_set_cursor(22, 20);
    util_gfx_set_color(COLOR_WHITE);
    util_gfx_print("Press a key");

int counter = 0;
int curLED = LED_XOR;
ledsOff();
ledOn(curLED);

    while(true) {

        if(waitForButton(10)){
                util_gfx_fill_screen(COLOR_PURPLE);
                util_gfx_set_cursor(22, 20);
                util_gfx_print("Key Detected");
                nrf_delay_ms(500);
                util_gfx_fill_screen(COLOR_BLACK);
                util_gfx_set_cursor(22, 20);
                util_gfx_print("Press a key");
        }

        nrf_delay_ms(5);
        counter++;
        if(counter > 25){
                counter = 0;
                ledOff(curLED);
                curLED++;
                if(curLED == ISSI_LED_COUNT){
                        curLED = LED_XOR;
                }
                ledOn(curLED);
        }

    }


#pragma clang diagnostic pop

}

