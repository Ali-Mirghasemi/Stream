/**
 * @file InputStream.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gamil.com)
 * @brief this library implement input stream over stream buffer
 * @version 0.4
 * @date 2021-09-01
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef _INPUT_STREAM_H_
#define _INPUT_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ISTREAM_VER_MAJOR    0
#define ISTREAM_VER_MINOR    5
#define ISTREAM_VER_FIX      0

#include "StreamBuffer.h"

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/

/**
 * @brief enable user argument in OStream
 */
#define ISTREAM_ARGS                1
/**
 * @brief enable checkTransmit function
 */
#define ISTREAM_CHECK_RECEIVE       1
/**
 * @brief enable lock read feature
 */
#define ISTREAM_LOCK                STREAM_READ_LOCK


/************************************************************************/

/**
 * @brief show stream version in string format
 */
#define ISTREAM_VER_STR                     _STREAM_VER_STR(ISTREAM_VER_MAJOR, ISTREAM_VER_MINOR, ISTREAM_VER_FIX)
/**
 * @brief show stream version in integer format, ex: 0.2.0 -> 200
 */
#define ISTREAM_VER                         ((ISTREAM_VER_MAJOR * 10000UL) + (ISTREAM_VER_MINOR * 100UL) + (ISTREAM_VER_FIX))

/* Pre-defined variables */
struct __IStream;
typedef struct __IStream IStream;
/**
 * @brief receive function, OStream_space
 * @param stream IStream 
 * @param buff uint8_t*
 * @param len Stream_LenType
 * @return Stream_Result 
 */
typedef Stream_Result (*IStream_ReceiveFn)(IStream* stream, uint8_t* buff, Stream_LenType len);
/**
 * @brief check how many bytes received, this functions allows to check 
 * how many bytes received in available function, it's good to work with DMA
 * @param stream IStream
 * @return Stream_LenType
 */
typedef Stream_LenType (*IStream_CheckReceiveFn)(IStream* stream);

/**
 * @brief hold InputStream properties
 */
struct __IStream {
    Stream                  Buffer;         /**< hold stream buffer */
    IStream_ReceiveFn       receive;        /**< receive function */
#if ISTREAM_ARGS
    void*                   Args;           /**< hold user defined arguments */
#endif
#if ISTREAM_CHECK_RECEIVE
    IStream_CheckReceiveFn  checkReceive;   /**< check receive function */
#endif
    Stream_LenType          IncomingBytes;  /**< hold how many bytes are coming */
};


void IStream_init(IStream* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size);
void IStream_deinit(IStream* stream);


/* Input Bytes of IStream */
Stream_Result IStream_handle(IStream* stream, Stream_LenType len);
Stream_Result IStream_receive(IStream* stream);
Stream_Result IStream_receiveByte(IStream* stream, uint8_t val);
Stream_Result IStream_receiveBytes(IStream* stream, uint8_t* val, Stream_LenType len);

#define IStream_availableUncheck(STREAM)            Stream_available(&(STREAM)->Buffer)
Stream_LenType IStream_available(IStream* stream);

Stream_LenType IStream_incomingBytes(IStream* stream);

#define IStream_inReceive(STREAM)                   Stream_inReceive(&(STREAM)->Buffer)

#if ISTREAM_ARGS
    void  IStream_setArgs(IStream* stream, void* args);
    void* IStream_getArgs(IStream* stream);
#endif // ISTREAM_ARGS

#if ISTREAM_CHECK_RECEIVE
    void IStream_setCheckReceive(IStream* stream, IStream_CheckReceiveFn fn);
#endif // ISTREAM_CHECK_RECEIVE

#if ISTREAM_LOCK
    Stream_Result IStream_lock(IStream* stream, IStream* lock, Stream_LenType len);
    #define IStream_unlock(STREAM, LOCK)    Stream_unlockRead(&(STREAM)->Buffer, &(LOCK)->Buffer);
    #define IStream_unlockIgnore(STREAM)    Stream_unlockReadIgnore(&(STREAM)->Buffer)
    #define IStream_lockLen(STREAM, LOCK)   Stream_lockReadLen(&(STREAM)->Buffer, &(LOCK)->Buffer)
#endif // ISTREAM_LOCK

#define IStream_getDataPtr(STREAM)                  Stream_getWritePtr(&((STREAM)->Buffer))

#define IStream_clear(STREAM)                       Stream_clear(&((STREAM)->Buffer))
#define IStream_reset(STREAM)                       Stream_reset(&((STREAM)->Buffer))
#define IStream_resetIO(STREAM)                     Stream_resetIO(&((STREAM)->Buffer))

#if STREAM_BYTE_ORDER
    #define IStream_getSystemByteOrder()            Stream_getSystemByteOrder()
    #define IStream_setByteOrder(STREAM, ORDER)     Stream_setByteOrder(&((STREAM)->Buffer), ORDER)
    #define IStream_getByteOrder(STREAM)            Stream_getByteOrder(&((STREAM)->Buffer))
#endif

#if STREAM_READ_LIMIT
    #define IStream_setLimit(STREAM, LEN)           Stream_setReadLimit(&((STREAM)->Buffer), (LEN))
    #define IStream_getLimit(STREAM)                Stream_getReadLimit(&((STREAM)->Buffer))
    #define IStream_isLimited(STREAM)               Stream_isReadLimited(&((STREAM)->Buffer))
#endif

#if STREAM_CURSOR
    #define IStream_getCursor(STREAM, CUR)          Stream_getCursor(&((STREAM)->Buffer), (CUR))
    #define IStream_len(STREAM, CUR)                Stream_getReadLen(&((STREAM)->Buffer), (CUR))
#endif

#define IStream_ignore(STREAM, LEN)                 Stream_moveReadPos(&((STREAM)->Buffer), (LEN))

/* Read function same as Stream read functions */
#define IStream_read(STREAM)                        Stream_read(&((STREAM)->Buffer))
#define IStream_readBytes(STREAM, VAL, LEN)         Stream_readBytes(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readBytesReverse(STREAM, VAL, LEN)  Stream_readBytesReverse(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readStream(IN, OUT, LEN)            Stream_readStream(&((IN)->Buffer), &((OUT)->Buffer), (LEN))
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

#define IStream_readCharArray(STREAM, VAL, LEN)     Stream_readCharArray(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readUInt8Array(STREAM, VAL, LEN)    Stream_readUInt8Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readInt8Array(STREAM, VAL, LEN)     Stream_readInt8Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readUInt16Array(STREAM, VAL, LEN)   Stream_readUInt16Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readInt16Array(STREAM, VAL, LEN)    Stream_readInt16Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readUInt32Array(STREAM, VAL, LEN)   Stream_readUInt32Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readInt32Array(STREAM, VAL, LEN)    Stream_readInt32Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_readFloatArray(STREAM, VAL, LEN)    Stream_readFloatArray(&((STREAM)->Buffer), (VAL), (LEN))

#if STREAM_UINT64
    #define IStream_readUInt64Array(STREAM,VAL,LEN) Stream_readUInt64Array(&((STREAM)->Buffer), (VAL), (LEN))
    #define IStream_readInt64Array(STREAM,VAL,LEN)  Stream_readInt64Array(&((STREAM)->Buffer), (VAL), (LEN))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    #define IStream_readDoubleArray(STREAM,VAL,LEN) Stream_readDoubleArray(&((STREAM)->Buffer), (VAL), (LEN))
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

#define IStream_getCharArray(STREAM, VAL, LEN)      Stream_getCharArray(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getUInt8Array(STREAM, VAL, LEN)     Stream_getUInt8Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getInt8Array(STREAM, VAL, LEN)      Stream_getInt8Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getUInt16Array(STREAM, VAL, LEN)    Stream_getUInt16Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getInt16Array(STREAM, VAL, LEN)     Stream_getInt16Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getUInt32Array(STREAM, VAL, LEN)    Stream_getUInt32Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getInt32Array(STREAM, VAL, LEN)     Stream_getInt32Array(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_getFloatArray(STREAM, VAL, LEN)     Stream_getFloatArray(&((STREAM)->Buffer), (VAL), (LEN))

#if STREAM_UINT64
    #define IStream_getUInt64Array(STREAM,VAL,LEN)  Stream_getUInt64Array(&((STREAM)->Buffer), (VAL), (LEN))
    #define IStream_getInt64Array(STREAM,VAL,LEN)   Stream_getInt64Array(&((STREAM)->Buffer), (VAL), (LEN))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    #define IStream_getDoubleArray(STREAM,VAL,LEN)  Stream_getDoubleArray(&((STREAM)->Buffer), (VAL), (LEN))
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

#define IStream_getCharArrayAt(STREAM, IDX, VAL, LEN)       Stream_getCharArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getUInt8ArrayAt(STREAM, IDX, VAL, LEN)      Stream_getUInt8ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getInt8ArrayAt(STREAM, IDX, VAL, LEN)       Stream_getInt8ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getUInt16ArrayAt(STREAM, IDX, VAL, LEN)     Stream_getUInt16ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getInt16ArrayAt(STREAM, IDX, VAL, LEN)      Stream_getInt16ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getUInt32ArrayAt(STREAM, IDX, VAL, LEN)     Stream_getUInt32ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getInt32ArrayAt(STREAM, IDX, VAL, LEN)      Stream_getInt32ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#define IStream_getFloatArrayAt(STREAM, IDX, VAL, LEN)      Stream_getFloatArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))

#if STREAM_UINT64
    #define IStream_getUInt64ArrayAt(STREAM,IDX,VAL,LEN)    Stream_getUInt64ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
    #define IStream_getInt64ArrayAt(STREAM,IDX,VAL,LEN)     Stream_getInt64ArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    #define IStream_getDoubleArrayAt(STREAM,IDX,VAL,LEN)    Stream_getDoubleArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#endif // STREAM_DOUBLE

#if STREAM_FIND_FUNCTIONS
    #define IStream_findByte(STREAM, VAL)                                           Stream_findByte(&((STREAM)->Buffer), (VAL))
    #define IStream_findByteAt(STREAM, IDX, VAL)                                    Stream_findByteAt(&((STREAM)->Buffer))
    #define IStream_findPattern(STREAM, PAT, PAT_LEN)                               Stream_findPattern(&((STREAM)->Buffer), (PAT), (PAT_LEN))
    #define IStream_findPatternAt(STREAM, IDX, PAT, PAT_LEN)                        Stream_findPatternAt(&((STREAM)->Buffer), (IDX), (PAT), (PAT_LEN))
    #define IStream_readBytesUntil(STREAM, END, VAL, LEN)                           Stream_readBytesUntil(&((STREAM)->Buffer), (END), (VAL), (LEN))
    #define IStream_readBytesUntilAt(STREAM, IDX, END, VAL, LEN)                    Stream_readBytesUntil(&((STREAM)->Buffer), (IDX), (END), (VAL), (LEN))
    #define IStream_readBytesUntilPattern(STREAM, PAT, PAT_LEN, VAL, LEN)           Stream_readBytesUntilPattern(&((STREAM)->Buffer), (PAT), (PAT_LEN), (VAL), (LEN))
    #define IStream_readBytesUntilPatternAt(STREAM, IDX, PAT, PAT_LEN, VAL, LEN)    Stream_readBytesUntilPatternAt(&((STREAM)->Buffer), (IDX), (PAT), (PAT_LEN), (VAL), (LEN))
    
    #define IStream_findUInt8(STREAM, VAL)                                          Stream_findUInt8(&((STREAM)->Buffer), (VAL))
    #define IStream_findInt8(STREAM, VAL)                                           Stream_findInt8(&((STREAM)->Buffer), (VAL))
    #define IStream_findUInt16(STREAM, VAL)                                         Stream_findUInt16(&((STREAM)->Buffer), (VAL))
    #define IStream_findInt16(STREAM, VAL)                                          Stream_findInt16(&((STREAM)->Buffer), (VAL))
    #define IStream_findUInt32(STREAM, VAL)                                         Stream_findUInt32(&((STREAM)->Buffer), (VAL))
    #define IStream_findInt32(STREAM, VAL)                                          Stream_findInt32(&((STREAM)->Buffer), (VAL))
#if STREAM_UINT64
    #define IStream_findUInt64(STREAM, VAL)                                         Stream_findUInt64(&((STREAM)->Buffer), (VAL))
    #define IStream_findInt64(STREAM, VAL)                                          Stream_findInt64(&((STREAM)->Buffer), (VAL))
#endif
    #define IStream_findFloat(STREAM, VAL)                                          Stream_findFloat(&((STREAM)->Buffer), (VAL))
#if STREAM_DOUBLE
    #define IStream_findDouble(STREAM, VAL)                                         Stream_findDouble(&((STREAM)->Buffer), (VAL))
#endif 

#if STREAM_FIND_AT_FUNCTIONS
    #define IStream_findUInt8At(STREAM, IDX, VAL)                                   Stream_findUInt8(&((STREAM)->Buffer), (IDX), (VAL))
    #define IStream_findInt8At(STREAM, IDX, VAL)                                    Stream_findInt8(&((STREAM)->Buffer), (IDX), (VAL))
    #define IStream_findUInt16At(STREAM, IDX, VAL)                                  Stream_findUInt16(&((STREAM)->Buffer), (IDX), (VAL))
    #define IStream_findInt16At(STREAM, IDX, VAL)                                   Stream_findInt16(&((STREAM)->Buffer), (IDX), (VAL))
    #define IStream_findUInt32At(STREAM, IDX, VAL)                                  Stream_findUInt32(&((STREAM)->Buffer), (IDX), (VAL))
    #define IStream_findInt32At(STREAM, IDX, VAL)                                   Stream_findInt32(&((STREAM)->Buffer), (IDX), (VAL))
#if STREAM_UINT64
    #define IStream_findUInt64At(STREAM, IDX, VAL)                                  Stream_findUInt64(&((STREAM)->Buffer), (IDX), (VAL))
    #define IStream_findInt64At(STREAM, IDX, VAL)                                   Stream_findInt64(&((STREAM)->Buffer), (IDX), (VAL))
#endif
    #define IStream_findFloatAt(STREAM, IDX, VAL)                                   Stream_findFloat(&((STREAM)->Buffer), (IDX), (VAL))
#if STREAM_DOUBLE
    #define IStream_findDoubleAt(STREAM, IDX, VAL)                                  Stream_findDouble(&((STREAM)->Buffer), (IDX), (VAL))
#endif 
#endif // STREAM_FIND_AT_FUNCTIONS

#endif // STREAM_FIND_FUNCTIONS

#define IStream_compare(STREAM, VAL, LEN)                                           Stream_compareAt(&((STREAM)->Buffer), (VAL), (LEN))
#define IStream_compareAt(STREAM, IDX, VAL, LEN)                                    Stream_compareAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))

#ifdef __cplusplus
};
#endif

#endif /* _INPUT_STREAM_H_ */
