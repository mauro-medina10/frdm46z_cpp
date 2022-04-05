/*
 * Boardled.cpp
 *
 *  Created on: 5 abr. 2022
 *      Author: mauur
 */

#include <Boardled.h>

Board_led::Board_led() {

	blink	= false;
}

Board_led::~Board_led() {
	// TODO Auto-generated destructor stub
}

Board_led::Board_led(GPIO_Type *b, uint32_t m){

	base 	= b;
	mask 	= m;
	blink	= false;
	state 	= LED_OFF;

	GPIO_PortSet(base, mask);
}

Board_led::Board_led(GPIO_Type *b, uint32_t m, uint32_t p){

	base 	= b;
	mask 	= m;
	period 	= p;
	blink	= false;
	state 	= LED_OFF;

	GPIO_PortSet(base, mask);
}

Board_led::Board_led(GPIO_Type *b, uint32_t m, uint32_t p, bool blk){

	base 	= b;
	mask 	= m;
	period 	= p;
	blink	= blk;
	state 	= LED_OFF;

	GPIO_PortSet(base, mask);
}

void Board_led::led_toggle(void) {

	GPIO_PortToggle(base, mask);
	state = !state;
}

bool Board_led::led_state_get(void){

	return state;
}

void Board_led::led_period_set(uint32_t p){

	period = p;
}

uint32_t Board_led::led_period_get(void){

	return period;
}

void Board_led::led_state_set(bool st){

	state = st;
	if(state == LED_ON){

		GPIO_PortClear(base, mask);
	}else{

		GPIO_PortSet(base, mask);
	}
}

void Board_led::led_blink_start(void){

	count = period;
	blink = true;
}

void Board_led::led_blink_stop(void){

	blink = false;
}

void Board_led::led_tick_callback(void){

	if(blink){

		count--;
		if(count == 0){

			this->led_toggle();
			count = period;
		}
	}
}

