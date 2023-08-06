/**
 * @file OutputStream.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief this library implement output stream over stream buffer
 * @version 0.4
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

#define OSTREAM_VER_MAJOR    0
#define OSTREAM_VER_MINOR    5
#define OSTREAM_VER_FIX      0

#include "StreamBuffer.h"

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/

/**
 * @brief enable user argument in OStream
 */
#define OSTREAM_ARGS                1
/**
 * @brief enable checkTransmit function
 */
#define OSTREAM_CHECK_TRANSMIT      1
/**
 * @brief enable lock write feature
 */
#define OSTREAM_LOCK                STREAM_WRITE_LOCK
/**
 * @brief set default flush mode
 * Stream_FlushMode_Single:     flush only pending bytes before call flush function
 * Stream_FlushMode_Continue:   after flush complete if there is any pending bytes transmit pending bytes again
 */
#define OSTREAM_FLUSH_MODE          Stream_FlushMode_Continue
/**
 * @brief enable flush complete callback
 */
#define OSTREAM_FLUSH_CALLBACK      1

/************************************************************************/

/**
 * @brief show stream version in string format
 */
#define OSTREAM_VER_STR                     _STREAM_VER_STR(OSTREAM_VER_MAJOR, OSTREAM_VER_MINOR, OSTREAM_VER_FIX)
/**
 * @brief show stream version in integer format, ex: 0.2.0 -> 200
 */
#define OSTREAM_VER                         ((OSTREAM_VER_MAJOR * 10000UL) + (OSTREAM_VER_MINOR * 100UL) + (OSTREAM_VER_FIX))

/* Pre-defined variables */
struct __OStream;
typedef struct __OStream OStream;

/**
 * @brief transmit function, this function allow stream to work with hardware
 * @param stream OStream
 * @param buff uint8_t*
 * @param len Stream_LenType
 */
typedef Stream_Result (*OStream_TransmitFn)(OStream* stream, uint8_t* buff, Stream_LenType len);
/**
 * @brief check how many bytes transmitted, this functions allow stream to check 
 * how many bytes transmitted over hardware, it's good for work with DMA 
 * @param stream OStream
 * @return Stream_LenType return how many bytes transmitted
 */
typedef Stream_LenType (*OStream_CheckTransmitFn)(OStream* stream);
/**
 * @brief flush callback, call after flush completed
 * 
 */
typedef void (*OStream_FlushCallbackFn)(OStream* stream);

/**
 * @brief hold OutputStream properties
 */
struct __OStream {
    StreamBuffer                  Buffer;         /**< stream buffer */
    OStream_TransmitFn      transmit;       /**< transmit function */
#if OSTREAM_ARGS
    void*                   Args;           /**< user argument */
#endif
#if OSTREAM_CHECK_TRANSMIT
    OStream_CheckTransmitFn checkTransmit;  /**< check transmit function */
#endif
#if OSTREAM_FLUSH_CALLBACK
    OStream_FlushCallbackFn flushCallback;  /**< flush callback */
#endif
    Stream_LenType          OutgoingBytes;  /**< outgoing bytes */
};


void OStream_init(OStream* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size);
void OStream_deinit(OStream* stream);


/* Output Bytes of OStream */
Stream_Result OStream_handle(OStream* stream, Stream_LenType len);
Stream_Result OStream_flush(OStream* stream);
Stream_Result OStream_flushBlocking(OStream* stream);
Stream_Result OStream_transmitByte(OStream* stream);
Stream_Result OStream_transmitBytes(OStream* stream, Stream_LenType len);

#define OStream_spaceUncheck(STREAM)                Stream_space(&(STREAM)->Buffer)
Stream_LenType OStream_space(OStream* stream);

Stream_LenType OStream_outgoingBytes(OStream* stream);

#define OStream_setFlushMode(STREAM, MODE)          Stream_setFlushMode(&(STREAM)->Buffer, (MODE))

#define OStream_inTransmit(STREAM)                  Stream_inTransmit(&(STREAM)->Buffer)

#if OSTREAM_ARGS
    void  OStream_setArgs(OStream* stream, void* args);
    void* OStream_getArgs(OStream* stream);
#endif // OSTREAM_ARGS

#if OSTREAM_CHECK_TRANSMIT
    void OStream_setCheckTransmit(OStream* stream, OStream_CheckTransmitFn fn);
#endif // OSTREAM_CHECK_TRANSMIT

#if OSTREAM_FLUSH_CALLBACK
    void OStream_setFlushCallback(OStream* stream, OStream_FlushCallbackFn fn);
#endif

#if OSTREAM_LOCK
    Stream_Result   OStream_lock(OStream* stream, OStream* lock, Stream_LenType len);
    #define OStream_unlock(STREAM, LOCK)    Stream_unlockWrite(&(STREAM)->Buffer, &(LOCK)->Buffer);
    #define OStream_unlockIgnore(STREAM)    Stream_unlockWriteIgnore(&(STREAM)->Buffer)
    #define OStream_lockLen(STREAM, LOCK)   Stream_lockWriteLen(&(STREAM)->Buffer, &(LOCK)->Buffer)
#endif // OSTREAM_LOCK

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
#define OStream_writeStr(STREAM, VAL)               Stream_writeStr(&((STREAM)->Buffer), (VAL))
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

#define OStream_writeCharArray(STREAM, VAL, LEN)    Stream_writeCharArray(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeUInt8Array(STREAM, VAL, LEN)   Stream_writeUInt8Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeInt8Array(STREAM, VAL, LEN)    Stream_writeInt8Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeUInt16Array(STREAM, VAL, LEN)  Stream_writeUInt16Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeInt16Array(STREAM, VAL, LEN)   Stream_writeInt16Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeUInt32Array(STREAM, VAL, LEN)  Stream_writeUInt32Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeInt32Array(STREAM, VAL, LEN)   Stream_writeInt32Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeFloatArray(STREAM, VAL, LEN)   Stream_writeFloatArray(&((STREAM)->Buffer), (VAL), (LEN))
#if STREAM_UINT64
#define OStream_writeUInt64Array(STREAM, VAL, LEN)  Stream_writeUInt64Array(&((STREAM)->Buffer), (VAL), (LEN))
#define OStream_writeInt64Array(STREAM, VAL, LEN)   Stream_writeInt64Array(&((STREAM)->Buffer), (VAL), (LEN))
#endif // STREAM_UINT64
#if STREAM_DOUBLE
#define OStream_writeDoubleArray(STREAM, VAL, LEN)  Stream_writeDoubleArray(&((STREAM)->Buffer), (VAL), (LEN))
#endif // STREAM_DOUBLE



#ifdef __cplusplus
};
#endif

#endif // _OUTPUT_STREAM_H_
