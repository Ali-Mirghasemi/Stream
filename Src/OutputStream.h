/**
 * @file OutputStream.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief this library implement output stream over stream buffer
 * @version 0.2
 * @date 2021-09-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _OUTPUT_STREAM_H_
#define _OUTPUT_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "StreamBuffer.h"

struct __OStream;
typedef struct __OStream OStream;

typedef void (*OStream_TransmitFn)(OStream* stream, uint8_t* buff, Stream_LenType len);
typedef Stream_LenType (*OStream_CheckTransmitFn)(OStream* stream);


struct __OStream {
    Stream                  Buffer;
    void*                   Args;
    OStream_TransmitFn      transmit;
    OStream_CheckTransmitFn checkTransmit;
    Stream_LenType          OutgoingBytes;
};


void OStream_init(OStream* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size);
void OStream_deinit(OStream* stream);


/* Output Bytes of OStream */
Stream_Result OStream_handle(OStream* stream, Stream_LenType len);
Stream_Result OStream_flush(OStream* stream);
Stream_Result OStream_transmitByte(OStream* stream);
Stream_Result OStream_transmitBytes(OStream* stream, Stream_LenType len);

void  OStream_setArgs(OStream* stream, void* args);
void* OStream_getArgs(OStream* stream);

void OStream_setCheckTransmit(OStream* stream, OStream_CheckTransmitFn fn);

Stream_LenType OStream_space(OStream* stream);
Stream_LenType OStream_outgoingBytes(OStream* stream);

/**
 * @brief return number of bytes waiting for transmit 
 */
#define OStream_pendingBytes(STREAM)                Stream_available(&((STREAM)->Buffer))

#define OStream_getDataPtr(STREAM)                  Stream_getReadPtr(&((STREAM)->Buffer))

#define OStream_clear(STREAM)                       Stream_clear(&((STREAM)->Buffer))
#define OStream_reset(STREAM)                       Stream_reset(&((STREAM)->Buffer))
#define OStream_resetIO(STREAM)                     Stream_resetIO(&((STREAM)->Buffer))

#if STREAM_BYTE_ORDER
    #define OStream_getSystemByteOrder()            Stream_getSystemByteOrder()
    #define OStream_setByteOrder(STREAM, ORDER)     Stream_setByteOrder(&((STREAM)->Buffer), ORDER)
    #define OStream_getByteOrder(STREAM)            Stream_getByteOrder(&((STREAM)->Buffer))
#endif

#if STREAM_WRITE_LIMIT
    #define OStream_setLimit(STREAM, LEN)           Stream_setWriteLimit(&((STREAM)->Buffer), (LEN))
    #define OStream_getLimit(STREAM)                Stream_getWriteLimit(&((STREAM)->Buffer))
    #define OStream_isLimited(STREAM)               Stream_isWriteLimited(&((STREAM)->Buffer))
#endif

#if STREAM_CURSOR
    #define OStream_getCursor(STREAM, CUR)          Stream_getCursor(&((STREAM)->Buffer), (CUR))
    #define OStream_len(STREAM, CUR)                Stream_getWriteLen(&((STREAM)->Buffer), (CUR))
#endif

#define OStream_ignore(STREAM, LEN)                 Stream_moveWritePos(&((STREAM)->Buffer), (LEN))

#define OStream_writeBytes(STREAM, VAL, LEN)        Stream_writeBytes(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeBytesReverse(STREAM, VAL, LEN) Stream_writeBytesReverse(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeStream(OUT, IN, LEN)           Stream_writeStream(&((OUT)->Buffer), &((IN)->Buffer), (LEN))
#define OStream_writePadding(STREAM, VAL, LEN)      Stream_writePadding(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeChar(STREAM, VAL)              Stream_writeChar(&((STREAM)->Buffer), (VAL))
#define OStream_writeUInt8(STREAM, VAL)             Stream_writeUInt8(&((STREAM)->Buffer), (VAL))
#define OStream_writeInt8(STREAM, VAL)              Stream_writeInt8(&((STREAM)->Buffer), (VAL))
#define OStream_writeUInt16(STREAM, VAL)            Stream_writeUInt16(&((STREAM)->Buffer), (VAL))
#define OStream_writeInt16(STREAM, VAL)             Stream_writeInt16(&((STREAM)->Buffer), (VAL))
#define OStream_writeUInt32(STREAM, VAL)            Stream_writeUInt32(&((STREAM)->Buffer), (VAL))
#define OStream_writeInt32(STREAM, VAL)             Stream_writeInt32(&((STREAM)->Buffer), (VAL))
#define OStream_writeFloat(STREAM, VAL)             Stream_writeFloat(&((STREAM)->Buffer), (VAL))
#if STREAM_UINT64
#define OStream_writeUInt64(STREAM, VAL)            Stream_writeUInt64(&((STREAM)->Buffer), (VAL))
#define OStream_writeInt64(STREAM, VAL)             Stream_writeInt64(&((STREAM)->Buffer), (VAL))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
#define OStream_writeDouble(STREAM, VAL)            Stream_writeDouble(&((STREAM)->Buffer), (VAL))
#endif // STREAM_DOUBLE






#ifdef __cplusplus
};
#endif

#endif // _OUTPUT_STREAM_H_
