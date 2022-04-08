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
	//Class-specific types
	typedef GPIO_Type 	led_base;
	typedef uint32_t 	led_mask;
	//Ctor
	Board_led(const led_base *b, const led_mask m);
	Board_led(const led_base *b, const led_mask m, uint32_t p);
	Board_led(const led_base *b, const led_mask m, uint32_t p, bool blk);
	//Destructor
	virtual ~Board_led();
	//Methods
	void led_toggle();
	bool led_state_get() const;
	void led_state_set(bool st);
	void led_period_set(uint32_t p);
	uint32_t led_period_get() const;
	void led_blink_start();
	void led_blink_stop();
	void led_blink_toggle();
	void led_tick_handler();
private:
	const led_base 	*base;
	const led_mask	mask;
	bool			state;
	uint32_t 		period;
	bool			blink;
	uint32_t		count;
};

#endif /* BOARDLED_H_ */
