/*
 * Boarduart_test.cpp
 *
 *  Created on: 3 may. 2022
 *      Author: mauur
 */

#include <Board_uart/Boarduart.h>
#include <Board_sw/Boardsw.h>
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
#include <array>
#include "Boardled.h"
/* Includes */
#include "sys_delay.h"
#include <fsl_lpsci.h>

using namespace std;
/*******************************************************************************
 * Defines
 ******************************************************************************/
//#define BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
//#define UART_NON_BLOQ
#define UART_MAX_BUFF 30

#define UART_RECEIVE_DATA_TEST 5
/*******************************************************************************
 * Static functions
 ******************************************************************************/
#ifdef UART_CUSTOM_CB
static void uart_callback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
#ifdef UART_CUSTOM_CB
Board_uart uart_0(UART0, uart_callback);
#else
Board_uart uart_0(UART0);
#endif
static constexpr array<uint8_t, 16> uart_data_test{
	'U','A','R','T',' ','T','e','s','t','\n','\r','\0','\0'
};
static constexpr size_t uart_buff_size = 30;
#ifdef UART_CUSTOM_CB
static bool uart_flag = false;
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef __cplusplus
extern "C"{
#endif

void UserTick_Handler(void)
{
}

void PORTC_PORTD_IRQHandler(void){
}

#ifdef __cplusplus
}
#endif

#ifdef UART_CUSTOM_CB
static void uart_callback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData){

	uart_flag = true;
}
#endif
/*
 * @brief   Application entry point.
 */
int main(void) {

	uint8_t uart_buff[uart_buff_size];
	uint8_t idx = 0;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Program start\n");

    uint32_t ret = SysTick_Config(SystemCoreClock / 1000U);
    if(ret != 0){

    	while(1);
    }

    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	//Sends uart_data_test string and waits until finished
    	uart_0.uart_data_send((uint8_t*)&uart_data_test, uart_data_test.size());
    	uart_0.uart_send_check_bloq();
    	//Receives UART_RECEIVE_DATA_TEST characters
    	uart_0.uart_data_get(uart_buff, UART_RECEIVE_DATA_TEST);
    	//Polling for data recieved ready
    	while(!uart_0.uart_recv_ready()){

    		SysTick_DelayTicks(300);
    	}
    	//Echo
    	uart_0.uart_data_send(uart_buff, UART_RECEIVE_DATA_TEST);
    	uart_0.uart_send_check_bloq();

    	uart_buff[0] = '\n';
    	uart_buff[1] = '\r';
    	uart_0.uart_data_send(uart_buff, 2);
    	uart_0.uart_send_check_bloq();
    	//Send formated string
    	uart_0.uart_write("PRUEBA WRITE %d \n\r", idx);
    	idx++;

    	SysTick_DelayTicks(5000);
    }
    return 0 ;
}
