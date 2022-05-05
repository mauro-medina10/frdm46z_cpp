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

	data_recv_done = false;
	data_send_done = false;
}

//Methods
void Board_uart::uart_data_get(uint8_t* data, size_t size){

	xfer.data 		= data;

	xfer.dataSize 	= size;

	data_recv_done = false;

	LPSCI_TransferReceiveNonBlocking(base, &handle, &xfer, &receivedBytes);

	if(receivedBytes >= size) data_recv_done = true;
}

void Board_uart::uart_data_send(uint8_t* data, size_t size){

	xfer.data 		= data;

	xfer.dataSize 	= size;

	data_send_done = false;

	LPSCI_TransferSendNonBlocking(base, &handle, &xfer);
}

bool Board_uart::uart_recv_ready() const{

	return data_recv_done;
}

bool Board_uart::uart_send_ready() const{

	return data_send_done;
}

void Board_uart::uart_recv_check_bloq() const{

	while(!data_recv_done);
}

void Board_uart::uart_send_check_bloq() const{

	while(!data_send_done);
}

void uart_data_callback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData){

	switch(status){

		case kStatus_LPSCI_RxIdle:

			reinterpret_cast<Board_uart*>(userData)->data_recv_done = true;
			break;
		case kStatus_LPSCI_TxIdle:

			reinterpret_cast<Board_uart*>(userData)->data_send_done = true;
			break;

		default:
			break;
	}
}

//D-tor
Board_uart::~Board_uart() {
	// TODO Auto-generated destructor stub
}

