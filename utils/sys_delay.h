/*
 * sys_delay.h
 *
 *  Created on: 5 abr. 2022
 *      Author: mauur
 */

#ifndef SYS_DELAY_H_
#define SYS_DELAY_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>

void UserTick_Handler(void);

void SysTick_DelayTicks(uint32_t n);

#ifdef __cplusplus
}
#endif

#endif /* SYS_DELAY_H_ */
