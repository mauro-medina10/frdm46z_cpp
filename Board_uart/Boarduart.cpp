/*
 * Boarduart.cpp
 *
 *  Created on: 3 may. 2022
 *      Author: mauur
 */

#include <Board_uart/Boarduart.h>
#include <fsl_lpsci.h>

//C-tor
Board_uart::Board_uart(UART0_Type* b) : base(b) {

	LPSCI_TransferCreateHandle(base, &handle, NULL, NULL);

	handle.rxRingBuffer 	= ring_buff;
	handle.rxRingBufferSize = buffer_size;

	data_flag = false;
}

//Methods
void Board_uart::uart_data_get(uint8_t* data, size_t size){

	xfer.data 		= data;

	xfer.dataSize 	= size;

	LPSCI_TransferReceiveNonBlocking(base, &handle, &xfer, &receivedBytes);
}

void Board_uart::uart_data_send(uint8_t* data, size_t size){

	xfer.data 		= data;

	xfer.dataSize 	= size;

	LPSCI_TransferSendNonBlocking(base, &handle, &xfer);
}

//D-tor
Board_uart::~Board_uart() {
	// TODO Auto-generated destructor stub
}

