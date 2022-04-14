/*
 * Boardsw.h
 *
 *  Created on: 14 abr. 2022
 *      Author: mauur
 */

#ifndef BOARD_SW_BOARDSW_H_
#define BOARD_SW_BOARDSW_H_

#include <fsl_gpio.h>

class Board_sw {
	//Class-specific types
	typedef GPIO_Type 	sw_base;
	typedef uint32_t 	sw_mask;
public:
	//Ctor
	Board_sw(const sw_base *b, const sw_mask m);
	//Dtor
	virtual ~Board_sw();
	//Methods
	uint16_t sw_events_get() const;
	bool sw_events_pending();
	void sw_events_done();
	void sw_events_clear();
	bool sw_interrupt_handler(uint32_t flags);

private:
	const sw_base		*base;
	const sw_mask		mask;
	uint16_t 			events;
};

#endif /* BOARD_SW_BOARDSW_H_ */
