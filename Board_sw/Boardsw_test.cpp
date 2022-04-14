/*
 * Boardsw_test.cpp
 *
 *  Created on: 14 abr. 2022
 *      Author: mauur
 */

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
namespace sw_objects{

	Board_sw sw_1(GPIOC, BOARD_SW1_PIN_MASK);
	Board_sw sw_3(GPIOC, BOARD_SW3_GPIO_PIN_MASK);
}
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

	uint32_t flags = GPIO_PortGetInterruptFlags(GPIOC);

	PRINTF("SW %ld interrupt handler \n", flags);

    /* Clear external interrupt flag. */
	sw_objects::sw_1.sw_interrupt_handler(flags);
	sw_objects::sw_3.sw_interrupt_handler(flags);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
      exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif

	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
}

#ifdef __cplusplus
}
#endif

/*
 * @brief   Application entry point.
 */
int main(void) {

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

    	if(sw_objects::sw_1.sw_events_get() == 0 && sw_objects::sw_3.sw_events_get() == 0){

    		PRINTF("NO events\n");
    	}
    	if(sw_objects::sw_1.sw_events_pending()){

    		PRINTF("SW1 event\n");
    	}
    	if(sw_objects::sw_3.sw_events_get() > 0){

    		PRINTF("SW3 %d events\n", sw_objects::sw_3.sw_events_get());

    		sw_objects::sw_3.sw_events_clear();
    	}

    	SysTick_DelayTicks(3000);
    }
    return 0 ;
}
