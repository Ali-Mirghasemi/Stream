/**
 * @file UARTStream.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This library implement Stream library for STM32Fxxx Based on HAL
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _UART_STREAM_H_
#define _UART_STREAM_H_

#include "InputStream.h"
#include "OutputStream.h"
#include "stdbool.h"

/******************************************************************************/
/*                                Configuration                               */
/******************************************************************************/
// #define UARTSTREAM_MCU_F0		0x00
// #define UARTSTREAM_MCU_F1		0x01
// #define UARTSTREAM_MCU_F2		0x02
// #define UARTSTREAM_MCU_F3		0x03
// #define UARTSTREAM_MCU_F4		0x04
// #define UARTSTREAM_MCU_F7		0x07
// #define UARTSTREAM_MCU_H7		0x17

// /**
//  * @brief define your MCU series
//  */
// #define UARTSTREAM_MCU			UARTSTREAM_MCU_F4

// /******************************************************************************/

// #if   UARTSTREAM_MCU == UARTSTREAM_MCU_F0
// 	#include "stm32f0xx.h"
// #elif UARTSTREAM_MCU == UARTSTREAM_MCU_F1
// 	#include "stm32f1xx.h"
// #elif UARTSTREAM_MCU == UARTSTREAM_MCU_F2
// 	#include "stm32f2xx.h"
// #elif UARTSTREAM_MCU == UARTSTREAM_MCU_F3
// 	#include "stm32f3xx.h"
// #elif UARTSTREAM_MCU == UARTSTREAM_MCU_F4
// 	#include "stm32f4xx.h"
// #elif UARTSTREAM_MCU == UARTSTREAM_MCU_F7
// 	#include "stm32f7xx.h"
// #elif UARTSTREAM_MCU == UARTSTREAM_MCU_H7
// 	#include "stm32h7xx.h"
// #else
// 	#error "Wrong MCU series!"
// #endif

typedef struct {
	void * uartOverSock;
	IStream						Input;
	OStream						Output;
} UARTStream;

bool UARTStream_init(
	UARTStream* stream, const char* ip, uint16_t port,const char* name,
	uint8_t* rxBuff, Stream_LenType rxBuffSize, 
	uint8_t* txBuff, Stream_LenType txBuffSize
);
void UARTStream_deInit(UARTStream* stream);

void UARTStream_rxHandle(void* ptr);
void UARTStream_txHandle(void* ptr);

Stream_LenType UARTStream_checkReceive(IStream* stream);
Stream_LenType UARTStream_checkTransmit(OStream* stream);
Stream_Result UARTStream_receive(IStream* stream, uint8_t* buff, Stream_LenType len);
Stream_Result UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len);

#endif // _UART_STREAM_H_

