/*
 * Boarduart.h
 *
 *  Created on: 3 may. 2022
 *      Author: mauur
 */

#ifndef BOARD_UART_BOARDUART_H_
#define BOARD_UART_BOARDUART_H_

#include <fsl_lpsci.h>

class Board_uart {
public:
	//Ctor
	Board_uart(UART0_Type* b);
	//Dtor
	virtual ~Board_uart();
	//Methods
	void uart_data_get(uint8_t* data, size_t size);
	void uart_data_send(uint8_t* data, size_t size);

private:
	//non-copy
	Board_uart(const Board_uart&) = delete;
	const Board_uart& operator=(const Board_uart&) = delete;
	//members
	static constexpr size_t buffer_size = 50;

	UART0_Type* 		base;
	lpsci_handle_t 		handle;
	lpsci_transfer_t	xfer;
	size_t				receivedBytes;
	bool 				data_flag;
	uint8_t 			ring_buff[buffer_size];
};

#endif /* BOARD_UART_BOARDUART_H_ */