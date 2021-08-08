#ifndef _INPUT_STREAM_H_
#define _INPUT_STREAM_H_

#if __cplusplus
extern "C" {
#endif

#include "Stream.h"

struct __IStream;
typedef struct __IStream IStream;

typedef void (*IStream_ReceiveFn)(IStream* stream, uint8_t* buff, Stream_LenType len);


struct __IStream {
    Stream                  Buffer;
    void*                   Args;
    IStream_ReceiveFn       receive;
};


void IStream_init(IStream* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size);
void IStream_deinit(IStream* stream);


/* Input Bytes of IStream */
Stream_Result IStream_handle(IStream* stream, Stream_LenType len);
Stream_Result IStream_receive(IStream* stream);
Stream_Result IStream_receiveByte(IStream* stream, uint8_t val);

void  IStream_setArgs(IStream* stream, void* args);
void* IStream_getArgs(IStream* stream);

#define IStream_available(STREAM)                   Stream_available(&((STREAM)->Buffer))

#define IStream_getDataPtr(STREAM)                  Stream_getWritePtr(&((STREAM)->Buffer))

#define IStream_clear(STREAM)                       Stream_clear(&((STREAM)->Buffer))

#if STREAM_BYTE_ORDER
    #define IStream_getSystemByteOrder()            Stream_getSystemByteOrder()
    #define IStream_setByteOrder(STREAM, ORDER)     Stream_setByteOrder(&((STREAM)->Buffer), ORDER)
    #define IStream_getByteOrder(STREAM)            Stream_getByteOrder(&((STREAM)->Buffer))
#endif

/* Read function same as Stream read functions */
#define IStream_read(STREAM)                        Stream_read(&((STREAM)->Buffer))
#define IStream_readBytes(STREAM, VAL, LEN)         Stream_readBytes(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readBytesReverse(STREAM, VAL, LEN)  Stream_readBytesReverse(&((STREAM)->Buffer), (VAL), (LEN));
#define IStream_readChar(STREAM)                    Stream_readChar(&((STREAM)->Buffer))
#define IStream_readUInt8(STREAM)                   Stream_readUInt8(&((STREAM)->Buffer))
#define IStream_readInt8(STREAM)                    Stream_readInt8(&((STREAM)->Buffer))
#define IStream_readUInt16(STREAM)                  Stream_readUInt16(&((STREAM)->Buffer))
#define IStream_readInt16(STREAM)                   Stream_readInt16(&((STREAM)->Buffer))
#define IStream_readUInt32(STREAM)                  Stream_readUInt32(&((STREAM)->Buffer))
#define IStream_readInt32(STREAM)                   Stream_readInt32(&((STREAM)->Buffer))
#define IStream_readFloat(STREAM)                   Stream_readFloat(&((STREAM)->Buffer))

#if STREAM_UINT64
    #define IStream_readUInt64(STREAM)              Stream_readUInt64(&((STREAM)->Buffer))
    #define IStream_readInt64(STREAM)               Stream_readInt64(&((STREAM)->Buffer))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    #define IStream_readDouble(STREAM)              Stream_readDouble(&((STREAM)->Buffer))
#endif // STREAM_DOUBLE

#define IStream_getBytes(STREAM, VAL, LEN)          Stream_getBytes((STREAM), (VAL), (LEN))
#define IStream_getBytesReverse(STREAM, VAL, LEN)   Stream_getBytesReverse(&((STREAM)->Buffer), (VAL), (LEN));
#define IStream_getChar(STREAM)                     Stream_getChar(&((STREAM)->Buffer))
#define IStream_getUInt8(STREAM)                    Stream_getUInt8(&((STREAM)->Buffer))
#define IStream_getInt8(STREAM)                     Stream_getInt8(&((STREAM)->Buffer))
#define IStream_getUInt16(STREAM)                   Stream_getUInt16(&((STREAM)->Buffer))
#define IStream_getInt16(STREAM)                    Stream_getInt16(&((STREAM)->Buffer))
#define IStream_getUInt32(STREAM)                   Stream_getUInt32(&((STREAM)->Buffer))
#define IStream_getInt32(STREAM)                    Stream_getInt32(&((STREAM)->Buffer))
#define IStream_getFloat(STREAM)                    Stream_getFloat(&((STREAM)->Buffer))

#if STREAM_UINT64
    #define IStream_getUInt64(STREAM)               Stream_getUInt64(&((STREAM)->Buffer))
    #define IStream_getInt64(STREAM)                Stream_getInt64(&((STREAM)->Buffer))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    #define IStream_getDouble(STREAM)               Stream_getDouble(&((STREAM)->Buffer))
#endif // STREAM_DOUBLE

#define IStream_getBytesAt(STREAM, IXD, VAL, LEN)           Stream_getBytesAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getBytesReverseAt(STREAM, IDX, VAL, LEN)    Stream_getBytesReverseAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN));
#define IStream_getCharAt(STREAM, IDX)                      Stream_getCharAt(&((STREAM)->Buffer), (IDX))
#define IStream_getUInt8At(STREAM, IDX)                     Stream_getUInt8At(&((STREAM)->Buffer), (IDX))
#define IStream_getInt8At(STREAM, IDX)                      Stream_getInt8At(&((STREAM)->Buffer), (IDX))
#define IStream_getUInt16At(STREAM, IDX)                    Stream_getUInt16At(&((STREAM)->Buffer), (IDX))
#define IStream_getInt16At(STREAM, IDX)                     Stream_getInt16At(&((STREAM)->Buffer), (IDX))
#define IStream_getUInt32At(STREAM, IDX)                    Stream_getUInt32At(&((STREAM)->Buffer), (IDX))
#define IStream_getInt32At(STREAM, IDX)                     Stream_getInt32At(&((STREAM)->Buffer), (IDX))
#define IStream_getFloatAt(STREAM, IDX)                     Stream_getFloatAt(&((STREAM)->Buffer), (IDX))

#if STREAM_UINT64
    #define IStream_getUInt64At(STREAM)                     Stream_getUInt64At(&((STREAM)->Buffer), (IDX))
    #define IStream_getInt64At(STREAM)                      Stream_getInt64At(&((STREAM)->Buffer), (IDX))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    #define IStream_getDoubleAt(STREAM)                     Stream_getDoubleAt(&((STREAM)->Buffer), (IDX))
#endif // STREAM_DOUBLE


#define IStream_compareAt(STREAM, IDX, VAL, LEN)                                Stream_compareAt(&((STREAM)->Buffer)), (IDX), (VAL), (LEN))
#define IStream_findByte(STREAM, VAL)                                           Stream_findByte(&((STREAM)->Buffer)), (VAL))
#define IStream_findByteAt(STREAM, IDX, VAL)                                    Stream_findByteAt(&((STREAM)->Buffer))
#define IStream_findPattern(STREAM, PAT, PAT_LEN)                               Stream_findPattern(&((STREAM)->Buffer), (PAT), (PAT_LEN))
#define IStream_findPatternAt(STREAM, IDX, PAT, PAT_LEN)                        Stream_findPatternAt(&((STREAM)->Buffer), (IDX), (PAT), (PAT_LEN))
#define IStream_readBytesUntil(STREAM, END, VAL, LEN)                           Stream_readBytesUntil(&((STREAM)->Buffer), (END), (VAL), (LEN))
#define IStream_readBytesUntilAt(STREAM, IDX, END, VAL, LEN)                    Stream_readBytesUntil(&((STREAM)->Buffer), (IDX), (END), (VAL), (LEN))
#define IStream_readBytesUntilPattern(STREAM, PAT, PAT_LEN, VAL, LEN)           Stream_readBytesUntilPattern(&((STREAM)->Buffer), (PAT), (PAT_LEN), (VAL), (LEN))
#define IStream_readBytesUntilPatternAt(STREAM, IDX, PAT, PAT_LEN, VAL, LEN)    Stream_readBytesUntilPatternAt(&((STREAM)->Buffer), (IDX), (PAT), (PAT_LEN), (VAL), (LEN))

#if __cplusplus
};
#endif

#endif /* _INPUT_STREAM_H_ */
