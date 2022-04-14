/*
 * Boardsw.cpp
 *
 *  Created on: 14 abr. 2022
 *      Author: mauur
 */

#include <Board_sw/Boardsw.h>
#include <core_cm0plus.h>

Board_sw::Board_sw(const sw_base *b, const sw_mask m) : base(b), mask(m) {

	events = 0;

    NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

Board_sw::~Board_sw() {
	// TODO Auto-generated destructor stub
}

uint16_t Board_sw::sw_events_get(void) const{

	return events;
}

void Board_sw::sw_events_done(){

	events--;
}

void Board_sw::sw_events_clear(){

	events = 0;
}

bool Board_sw::sw_events_pending(){

	if(events > 0){

		events = 0;

		return true;
	}else{

		return false;
	}
}

bool Board_sw::sw_interrupt_handler(uint32_t flags){

	if(flags & mask){

		events++;

		GPIO_PortClearInterruptFlags((GPIO_Type *) base, (uint32_t) mask);

		return true;
	}

	return false;
}
