/*
 * Boardled.h
 *
 *  Created on: 5 abr. 2022
 *      Author: mauur
 */

#ifndef BOARDLED_H_
#define BOARDLED_H_

#include <fsl_gpio.h>

enum led_state{
	LED_OFF = false,
	LED_ON 	= true,
};

class Board_led {
public:
	Board_led();
	Board_led(GPIO_Type *b, uint32_t m);
	Board_led(GPIO_Type *b, uint32_t m, uint32_t p);
	Board_led(GPIO_Type *b, uint32_t m, uint32_t p, bool blk);
	virtual ~Board_led();
	void led_toggle(void);
	bool led_state_get(void);
	void led_state_set(bool st);
	void led_period_set(uint32_t p);
	uint32_t led_period_get(void);
	void led_blink_start(void);
	void led_blink_stop(void);
	void led_tick_callback(void);
private:
	GPIO_Type 	*base;
	uint32_t	mask;
	bool		state;
	uint32_t 	period;
	bool		blink;
	uint32_t	count;
};

#endif /* BOARDLED_H_ */
