/*
 * Boardled_test.cpp
 *
 *  Created on: 5 abr. 2022
 *      Author: mauur
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
/* cpp includes */
#include <iostream>
#include "Boardled.h"
/* Includes */
#include "sys_delay.h"
/* TODO: insert other definitions and declarations here. */
/*******************************************************************************
 * Variables
 ******************************************************************************/
Board_led red_led;
Board_led green_led;

/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void UserTick_Handler(void)
{
	red_led.led_tick_callback();
	green_led.led_tick_callback();
}

#ifdef __cplusplus
}
#endif

/*
 * @brief   Application entry point.
 */
int main(void) {

	uint32_t ret = 0;
	uint32_t period = 1000;
	bool aux = true;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    red_led = Board_led(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN_MASK, period);
    green_led = Board_led(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN_MASK);

    PRINTF("Hello World\n");

    ret = SysTick_Config(SystemCoreClock / 1000U);
    if(ret != 0){

    	while(1);
    }
    //std::cout << "COUT: test\n";

    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */
    green_led.led_period_set(period / 2);
    green_led.led_state_set(LED_ON);

    green_led.led_blink_start();
    red_led.led_blink_start();

    while(1) {
    	//Red led
    	PRINTF("Red led %s.\n", red_led.led_state_get() ? "On" : "Off");
    	//Green led
    	PRINTF("Green led %s.\n", green_led.led_state_get() ? "On" : "Off");
    	//Delay
        SysTick_DelayTicks(5000U);

        if(aux){

        	aux = !aux;
        	green_led.led_period_set(period * 2);
        }

    }
    return 0 ;
}
