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

#ifdef UART_NON_BLOQ
#define UART_WRITE(d) 	LPSCI_TransferSendNonBlocking(UART0, &uart_hand, &d)
#define UART_READ(d) 	LPSCI_TransferReceiveNonBlocking(UART0, d.data, d.dataSize)
#else
#define UART_WRITE(d) 	LPSCI_WriteBlocking(UART0, d.data, d.dataSize)
#define UART_READ(d) 	LPSCI_ReadBlocking(UART0, d.data, d.dataSize)
#endif
#define UART_DATA_SET(d, ...) d.dataSize = sprintf((char*)d.data, __VA_ARGS__)

/*******************************************************************************
 * Variables
 ******************************************************************************/
Board_uart uart_0(UART0);


static constexpr array<uint8_t, 16> uart_data_test{
	'U','A','R','T',' ','T','e','s','t','\n','\r','\0','\0'
};
static constexpr size_t uart_buff_size = 30;
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

/*
 * @brief   Application entry point.
 */
int main(void) {

	uint8_t uart_buff[uart_buff_size];

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

    	uart_0.uart_data_send((uint8_t*)&uart_data_test, uart_data_test.size());

    	uart_0.uart_data_get(uart_buff, 5);

    	while(!uart_0.uart_data_ready()){

    		SysTick_DelayTicks(300);
    	}

    	uart_0.uart_data_send(uart_buff, 5);
    	SysTick_DelayTicks(50);
    	uart_buff[0] = '\n';
    	uart_buff[1] = '\r';
    	uart_0.uart_data_send(uart_buff, 2);

    	SysTick_DelayTicks(1000);
    }
    return 0 ;
}
