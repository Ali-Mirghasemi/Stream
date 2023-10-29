#include "UARTStream.h"

/**
 * @brief Initialize UARTStream object
 *
 * @param stream
 * @param huart
 * @param rxBuff
 * @param rxBuffSize
 * @param txBuff
 * @param txBuffSize
 */
bool UARTStream_init(
	UARTStream* stream, const char* ip, uint16_t port, const char* name,
	uint8_t* rxBuff, Stream_LenType rxBuffSize,
	uint8_t* txBuff, Stream_LenType txBuffSize
) {

    void * socketInit(void* args,const char* ip, uint16_t port, const char* name,void (*txComp)(void*),void (*rxComp)(void*));
    void * ptr = socketInit(stream,ip,port,name,UARTStream_txHandle,UARTStream_rxHandle);
    if(ptr == NULL){
        return false;
    }
    stream->uartOverSock = ptr;

    IStream_init(&stream->Input, UARTStream_receive, rxBuff, rxBuffSize);
    IStream_setCheckReceive(&stream->Input, UARTStream_checkReceive);
    IStream_setArgs(&stream->Input, stream);

    OStream_init(&stream->Output, UARTStream_transmit, txBuff, txBuffSize);
    OStream_setCheckTransmit(&stream->Output, UARTStream_checkTransmit);
    OStream_setArgs(&stream->Output, stream);
    return true;
}

void UARTStream_deInit(UARTStream* stream){
    void socketDeInit(void *obj);
    socketDeInit(stream->uartOverSock);
}
/**
 * @brief Handle Receive Data for Input Stream, user must put it in
 * HAL_UART_RxCpltCallback or other callbacks
 *
 * @param stream
 */
void UARTStream_rxHandle(void* ptr) {
    UARTStream* stream = (UARTStream*)ptr;
    IStream_handle(&stream->Input, IStream_incomingBytes(&stream->Input));
}
/**
 * @brief Handle Transmit Data for Output Stream, user must put it in
 * HAL_UART_TxCpltCallback or other callbacks
 *
 * @param stream
 */
void UARTStream_txHandle(void* ptr) {
    UARTStream* stream = (UARTStream*)ptr;
    OStream_handle(&stream->Output, OStream_outgoingBytes(&stream->Output));
}
/**
 * @brief Check Received bytes in DMA or IT and add to stream
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType UARTStream_checkReceive(IStream* stream) {
    UARTStream* uartStream = (UARTStream*) IStream_getArgs(stream);
    // if (uartStream->HUART->hdmarx) {
    //     return IStream_incomingBytes(stream) - __HAL_DMA_GET_COUNTER(uartStream->HUART->hdmarx);
    // }
    // else {
    //     return IStream_incomingBytes(stream) - uartStream->HUART->RxXferCount;
    // }
    size_t socketinReceive(void *obj);
    return IStream_incomingBytes(stream) - socketinReceive(uartStream->uartOverSock);
}
/**
 * @brief Check Transmitted bytes in DMA or IT and removed from stream
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType UARTStream_checkTransmit(OStream* stream) {
    UARTStream* uartStream = (UARTStream*) OStream_getArgs(stream);
    // if (uartStream->HUART->hdmarx) {
    //     return OStream_outgoingBytes(stream) - __HAL_DMA_GET_COUNTER(uartStream->HUART->hdmatx);
    // }
    // else {
    //     return OStream_outgoingBytes(stream) - uartStream->HUART->TxXferCount;
    // }
    return 0;
}
/**
 * @brief Stream receive function
 *
 * @param stream
 * @param buff
 * @param len
 */
Stream_Result UARTStream_receive(IStream* stream, uint8_t* buff, Stream_LenType len) {
    // HAL_StatusTypeDef status;
    // UARTStream* uartStream = (UARTStream*) IStream_getArgs(stream);
    // if (uartStream->HUART->hdmarx) {
    //     status = HAL_UART_Receive_DMA(uartStream->HUART, buff, len);
    // }
    // else {
    //     status = HAL_UART_Receive_IT(uartStream->HUART, buff, len);
    // }
    // return status == HAL_OK ? Stream_Ok : Stream_CustomError | status;
    UARTStream* uartStream = (UARTStream*) IStream_getArgs(stream);
    void socketReceive(void *obj, uint8_t *buffer, size_t len);
    socketReceive(uartStream->uartOverSock,buff,len);
    return 0;
}
/**
 * @brief Stream transmit function
 *
 * @param stream
 * @param buff
 * @param len
 */
Stream_Result UARTStream_transmit(OStream* stream, uint8_t* buff, Stream_LenType len) {
    // HAL_StatusTypeDef status;
    // UARTStream* uartStream = (UARTStream*) OStream_getArgs(stream);
    // if (uartStream->HUART->hdmatx) {
    //     status = HAL_UART_Transmit_DMA(uartStream->HUART, buff, len);
    // }
    // else {
    //     status = HAL_UART_Transmit_IT(uartStream->HUART, buff, len);
    // }
    // return status == HAL_OK ? Stream_Ok : Stream_CustomError | status;
    UARTStream* uartStream = (UARTStream*) OStream_getArgs(stream);
    void socketTransmit(void *obj, uint8_t *buffer, size_t len);
    //printf("len = %d ",len);
    socketTransmit(uartStream->uartOverSock,buff,len);
    return 0;
}
