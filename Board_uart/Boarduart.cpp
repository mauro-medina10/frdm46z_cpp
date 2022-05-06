/*
 * Boarduart.cpp
 *
 *  Created on: 3 may. 2022
 *      Author: mauur
 */

#include <Board_uart/Boarduart.h>

void uart_data_callback(UART0_Type *base, lpsci_handle_t *handle, status_t status, void *userData);

static void uart_format_cb(char *buf, int32_t *indicator, char val, int len){

	int i = 0;

	for (i = 0; i < len; i++)
	{
        if ((*indicator + 1) >= MAX_BUFF_SIZE)
        {
            *indicator = 0U;
        }

		buf[*indicator] = val;
		(*indicator)++;
	}
}
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

Board_uart::Board_uart(UART0_Type* b, lpsci_transfer_callback_t cb) : base(b){

	LPSCI_TransferCreateHandle(base, &handle, NULL, NULL);

	handle.rxRingBuffer 	= ring_buff;
	handle.rxRingBufferSize = buffer_size;

	handle.callback = cb;

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

void Board_uart::uart_write(const char *_s, ...){

    va_list ap;
    size_t logLength = 0U;

    va_start(ap, _s);
    /* format print log first */
    logLength = (size_t) StrFormatPrintf(_s, ap, reinterpret_cast<char*>(write_buff), uart_format_cb);
    /* print log */
    uart_data_send(write_buff, logLength);

    va_end(ap);
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
