#ifndef _OUTPUT_STREAM_H_
#define _OUTPUT_STREAM_H_

#if __cplusplus
extern "C" {
#endif

#include "StreamBuffer.h"

struct __OStream;
typedef struct __OStream OStream;

typedef void (*OStream_TransmitFn)(OStream* stream, uint8_t* buff, Stream_LenType len);


struct __OStream {
    Stream                  Buffer;
    void*                   Args;
    OStream_TransmitFn      transmit;
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

#define OStream_pendingBytes(STREAM)                Stream_available(&((STREAM)->Buffer))
#define OStream_space(STREAM)                       Stream_space(&((STREAM)->Buffer))

#define OStream_getDataPtr(STREAM)                  Stream_getReadPtr(&((STREAM)->Buffer))

#define OStream_clear(STREAM)                       Stream_clear(&((STREAM)->Buffer))

#if STREAM_BYTE_ORDER
    #define OStream_getSystemByteOrder()            Stream_getSystemByteOrder()
    #define OStream_setByteOrder(STREAM, ORDER)     Stream_setByteOrder(&((STREAM)->Buffer), ORDER)
    #define OStream_getByteOrder(STREAM)            Stream_getByteOrder(&((STREAM)->Buffer))
#endif

#define OStream_writeBytes(STREAM, VAL, LEN)        Stream_writeBytes(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeBytesReverse(STREAM, VAL, LEN) Stream_writeBytesReverse(&((STREAM)->Buffer), (VAL), (LEN))
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






#if __cplusplus
};
#endif

#endif // _OUTPUT_STREAM_H_
