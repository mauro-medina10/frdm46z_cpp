/*
 * sys_delay.c
 *
 *  Created on: 5 abr. 2022
 *      Author: mauur
 */

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include "sys_delay.h"

#ifndef __weak
#define __weak	__attribute__((weak))
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/

volatile uint32_t g_systickCounter;

/*******************************************************************************
 * Code
 ******************************************************************************/
__weak void UserTick_Handler(void){


}

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
    UserTick_Handler();
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while(g_systickCounter != 0U)
    {
    }
}

#ifdef __cplusplus
}
#endif
