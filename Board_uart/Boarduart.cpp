/*
 * Boarduart.cpp
 *
 *  Created on: 3 may. 2022
 *      Author: mauur
 */

#include <Board_uart/Boarduart.h>
#include <fsl_lpsci.h>

void uart_data_callback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData);

//C-tor
Board_uart::Board_uart(UART0_Type* b) : base(b) {

	LPSCI_TransferCreateHandle(base, &handle, NULL, NULL);

	handle.rxRingBuffer 	= ring_buff;
	handle.rxRingBufferSize = buffer_size;

	handle.callback = reinterpret_cast<lpsci_transfer_callback_t>(uart_data_callback);
	handle.userData = reinterpret_cast<void*>(this);

	data_flag = false;
}

//Methods
void Board_uart::uart_data_get(uint8_t* data, size_t size){

	xfer.data 		= data;

	xfer.dataSize 	= size;

	data_flag = false;

	LPSCI_TransferReceiveNonBlocking(base, &handle, &xfer, &receivedBytes);

	if(receivedBytes >= size) data_flag = true;
}

void Board_uart::uart_data_send(uint8_t* data, size_t size){

	xfer.data 		= data;

	xfer.dataSize 	= size;

	LPSCI_TransferSendNonBlocking(base, &handle, &xfer);
}

bool Board_uart::uart_data_ready() const{

	return data_flag;
}

void Board_uart::uart_set_data_flag(){

	data_flag = true;
}

void uart_data_callback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData){

	if(status == kStatus_LPSCI_RxIdle){

		//data_flag = true;
		reinterpret_cast<Board_uart*>(userData)->uart_set_data_flag();
	}
}

//D-tor
Board_uart::~Board_uart() {
	// TODO Auto-generated destructor stub
}

