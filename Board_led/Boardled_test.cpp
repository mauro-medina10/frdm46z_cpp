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
#include <string>
#include "Boardled.h"
/* Includes */
#include "sys_delay.h"
#include <fsl_lpsci.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/
//#define BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
//#define UART_NON_BLOQ
#define UART_MAX_BUFF 30

#ifdef UART_NON_BLOQ
#define UART_WRITE(d) LPSCI_TransferSendNonBlocking(UART0, &uart_hand, &d)
#define UART_READ(d) LPSCI_TransferReceiveNonBlocking(UART0, d.data, d.dataSize)
#else
#define UART_WRITE(d) LPSCI_WriteBlocking(UART0, d.data, d.dataSize)
#define UART_READ(d) LPSCI_ReadBlocking(UART0, d.data, d.dataSize)
#endif
#define UART_DATA_SET(d, ...) d.dataSize = sprintf((char*)d.data, __VA_ARGS__)
/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t period = 1000;
Board_led red_led(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN_MASK, period);
Board_led green_led(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN_MASK);

static const uint8_t period_menu[] = {'E','n','t','e','r',' ','p','e','r','i','o','d',':',' ','\0','\0'};
/*******************************************************************************
 * Code
 ******************************************************************************/
#ifdef __cplusplus
extern "C"{
#endif

void UserTick_Handler(void)
{
	red_led.led_tick_handler();
	green_led.led_tick_handler();
}

#ifdef __cplusplus
}
#endif

static void led_menu_write(void){

	uint8_t str[250];
	lpsci_transfer_t menu_data = {str, sizeof(str)};

	UART_DATA_SET(menu_data,"%s%s%s%s%s%s%s%s%s%s","\n\r1- Red led ON.\n\r",
							"2- Red led OFF\n\r",
							"3- Red led Toggle\n\r",
							"4- Red led blink toggle\n\r",
							"5- Red led blink period set\n\r",
							"6- Green led ON\n\r",
							"7- Green led OFF",
							"8- Green led toggle\n\r",
							"9- Green blink toggle\n\r",
							"a- Green led blink period set\n\r");
	UART_WRITE(menu_data);
}
/*
 * @brief   Application entry point.
 */
int main(void) {

	uint32_t 	ret = 0;
	uint8_t 	menu_select = 0;
	uint8_t 	period_data[] = {'0','0','0','0','\n','\r','\0'};
	std::string period_aux;

	lpsci_transfer_t 	menu_info = {&menu_select, 1};
	lpsci_transfer_t 	period_info = {(uint8_t*)period_menu, sizeof(period_menu)};
	lpsci_handle_t 		uart_hand;

	LPSCI_TransferCreateHandle(UART0, &uart_hand, NULL, NULL);

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Program start\n");

    ret = SysTick_Config(SystemCoreClock / 1000U);
    if(ret != 0){

    	while(1);
    }

    /* Force the counter to be placed into memory. */

    /* Enter an infinite loop, just incrementing a counter. */
    green_led.led_period_set(period / 2);

    while(1) {

    	led_menu_write();

		SysTick_DelayTicks(100U);

		UART_READ(menu_info);

		menu_select -= 48;

		if(menu_select == 17 || menu_select == 49)menu_select = 10;

		switch(menu_select){

		case 1:
			red_led.led_state_set(LED_ON);

			break;

		case 2:
			red_led.led_state_set(LED_OFF);

			break;

		case 3:
			red_led.led_toggle();

			break;

		case 4:
			red_led.led_blink_toggle();

			break;

		case 5:
			UART_WRITE(period_info);

			SysTick_DelayTicks(100U);

			period_info.data = period_data;
			period_info.dataSize = 4;

			UART_READ(period_info);

			period_info.dataSize = sizeof(period_data);

			UART_WRITE(period_info); //echo

			period_aux = (char*)period_data;

			std::cout << period_aux << std::endl; //DEBUG

			ret = (uint32_t)stoi(period_aux);

			red_led.led_period_set(ret);

			period_info.data = (uint8_t*) period_menu;
			period_info.dataSize = sizeof(period_menu);

			break;

		case 6:
			green_led.led_state_set(LED_ON);

			break;

		case 7:
			green_led.led_state_set(LED_OFF);

			break;

		case 8:
			green_led.led_toggle();

			break;

		case 9:
			green_led.led_blink_toggle();

			break;

		case 10:
			UART_WRITE(period_info);

			SysTick_DelayTicks(100U);

			period_info.data = period_data;
			period_info.dataSize = 4;

			UART_READ(period_info);

			period_info.dataSize = sizeof(period_data);

			UART_WRITE(period_info); //echo

			period_aux = (char*)period_data;

			std::cout << period_aux << std::endl; //DEBUG

			ret = (uint32_t)stoi(period_aux);

			green_led.led_period_set(ret);

			period_info.data = (uint8_t*) period_menu;
			period_info.dataSize = sizeof(period_menu);

			break;
		}

    }
    return 0 ;
}
