/*
 * Boardled.cpp
 *
 *  Created on: 5 abr. 2022
 *      Author: mauur
 */

#include <Boardled.h>

using namespace std;

Board_led::~Board_led() {
	// TODO Auto-generated destructor stub
}

Board_led::Board_led(const led_base *b,const led_mask m) : base(b), mask(m){

	blink	= false;
	state 	= LED_OFF;
	period 	= 0;

	GPIO_PortSet((GPIO_Type*)base, mask);
}

Board_led::Board_led(const led_base *b,const led_mask m, uint32_t p) : base(b), mask(m){

	blink	= false;
	period 	= p;
	state 	= LED_OFF;

	GPIO_PortSet((GPIO_Type*)base, mask);
}

Board_led::Board_led(const led_base *b,const led_mask m, uint32_t p, bool blk) : base(b), mask(m){

	period 	= p;
	blink	= blk;
	state 	= LED_OFF;

	GPIO_PortSet((GPIO_Type*)base, mask);
}

void Board_led::led_toggle() {

	GPIO_PortToggle((GPIO_Type*)base, mask);
	state = !state;
}

bool Board_led::led_state_get() const{

	return state;
}

void Board_led::led_state_set(bool st) {

	state = st;
	if(state == LED_ON){

		GPIO_PortClear((GPIO_Type*)base, mask);
	}else{

		GPIO_PortSet((GPIO_Type*)base, (uint32_t)mask);
	}
}

void Board_led::led_period_set(uint32_t p){

	period = p;
}

uint32_t Board_led::led_period_get() const{

	return period;
}

void Board_led::led_blink_start(){

	if(period != 0){

		count = period;
		blink = true;
	}
}

void Board_led::led_blink_stop() {

	blink = false;

	led_state_set(LED_OFF);
}

void Board_led::led_blink_toggle(){

	if(blink){

		led_blink_stop();
	}else{

		led_blink_start();
	}
}

void Board_led::led_tick_handler(){

	if(blink && period != 0){

		count--;
		if(count == 0){

			led_toggle();

			count = period;
		}
	}
}

