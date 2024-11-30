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
#define OSTREAM_VER_MINOR    8
#define OSTREAM_VER_FIX      1

#include "StreamBuffer.h"

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/
/**
 * @brief This lable shows OStream Libraty is enabled or not
 */
#define OSTREAM                     (1 && STREAM_WRITE && STREAM_PENDING_BYTES)
/**
 * @brief enable checkTransmit function
 */
#define OSTREAM_CHECK_TRANSMIT      1
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
    StreamBuffer            Buffer;         /**< stream buffer */
    OStream_TransmitFn      transmit;       /**< transmit function */
#if OSTREAM_CHECK_TRANSMIT
    OStream_CheckTransmitFn checkTransmit;  /**< check transmit function */
#endif
#if OSTREAM_FLUSH_CALLBACK
    OStream_FlushCallbackFn flushCallback;  /**< flush callback */
#endif
};


void                OStream_init(OStream* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size);
#define             OStream_deinit(STREAM)                                  memset(STREAM, 0, sizeof(OStream))


/* Output Bytes of OStream */
Stream_Result       OStream_handle(OStream* stream, Stream_LenType len);
Stream_Result       OStream_flush(OStream* stream);
Stream_Result       OStream_flushBlocking(OStream* stream);
Stream_Result       OStream_transmitByte(OStream* stream);
Stream_Result       OStream_transmitBytes(OStream* stream, Stream_LenType len);

#define             OStream_spaceUncheck(STREAM)                            Stream_space(&(STREAM)->Buffer)
Stream_LenType      OStream_space(OStream* stream);

#define             OStream_outgoingBytes(STREAM)                           Stream_getPendingBytes(&(STREAM)->Buffer)

#define             OStream_setFlushMode(STREAM, MODE)                      Stream_setFlushMode(&(STREAM)->Buffer, (MODE))

#define             OStream_inTransmit(STREAM)                              Stream_inTransmit(&(STREAM)->Buffer)

#if STREAM_ARGS
    #define         OStream_setArgs(STREAM, ARGS)                           Stream_setArgs(&(STREAM)->Buffer, (ARGS))
    #define         OStream_getArgs(STREAM)                                 Stream_getArgs(&(STREAM)->Buffer)
#endif // OSTREAM_ARGS

#if OSTREAM_CHECK_TRANSMIT
    void            OStream_setCheckTransmit(OStream* stream, OStream_CheckTransmitFn fn);
#endif // OSTREAM_CHECK_TRANSMIT

#if OSTREAM_FLUSH_CALLBACK
    void            OStream_setFlushCallback(OStream* stream, OStream_FlushCallbackFn fn);
#endif

#if STREAM_WRITE_LOCK_CUSTOM
    #define         OStream_lock(STREAM, LOCK, LEN)                         Stream_lockWriteCustom(&(STREAM)->Buffer, &(LOCK)->Buffer, (LEN), sizeof(OStream))
    #define         OStream_unlock(STREAM, LOCK)                            Stream_unlockWrite(&(STREAM)->Buffer, &(LOCK)->Buffer);
    #define         OStream_unlockIgnore(STREAM)                            Stream_unlockWriteIgnore(&(STREAM)->Buffer)
    #define         OStream_lockLen(STREAM, LOCK)                           Stream_lockWriteLen(&(STREAM)->Buffer, &(LOCK)->Buffer)
#endif // OSTREAM_LOCK

/**
 * @brief return number of bytes waiting for transmit 
 */
#define             OStream_pendingBytes(STREAM)                            Stream_available(&((STREAM)->Buffer))

#define             OStream_getDataPtr(STREAM)                              Stream_getReadPtr(&((STREAM)->Buffer))
#define             OStream_getBufferSize(STREAM)                           Stream_getBufferSize(&((STREAM)->Buffer))

#define             OStream_clear(STREAM)                                   Stream_clear(&((STREAM)->Buffer))
#define             OStream_reset(STREAM)                                   Stream_reset(&((STREAM)->Buffer))
#define             OStream_resetIO(STREAM)                                 Stream_resetIO(&((STREAM)->Buffer))

#if STREAM_BYTE_ORDER
    #define         OStream_getSystemByteOrder()                            Stream_getSystemByteOrder()
    #define         OStream_setByteOrder(STREAM, ORDER)                     Stream_setByteOrder(&((STREAM)->Buffer), ORDER)
    #define         OStream_getByteOrder(STREAM)                            Stream_getByteOrder(&((STREAM)->Buffer))
#endif

#if STREAM_WRITE_LIMIT
    #define         OStream_setLimit(STREAM, LEN)                           Stream_setWriteLimit(&((STREAM)->Buffer), (LEN))
    #define         OStream_getLimit(STREAM)                                Stream_getWriteLimit(&((STREAM)->Buffer))
    #define         OStream_isLimited(STREAM)                               Stream_isWriteLimited(&((STREAM)->Buffer))
#endif

#if STREAM_CURSOR
    #define         OStream_getCursor(STREAM, CUR)                          Stream_getCursor(&((STREAM)->Buffer), (CUR))
    #define         OStream_len(STREAM, CUR)                                Stream_getWriteLen(&((STREAM)->Buffer), (CUR))
#endif

#define             OStream_pop(STREAM, LEN)                                Stream_moveReadPos(&((STREAM)->Buffer), (LEN))
#define             OStream_ignore(STREAM, LEN)                             Stream_moveWritePos(&((STREAM)->Buffer), (LEN))

/* ------------------------------------ General Write APIs ---------------------------------- */
#if STREAM_WRITE
#define             OStream_writeBytes(STREAM, VAL, LEN)                    Stream_writeBytes(&((STREAM)->Buffer), (VAL), (LEN))     
#if STREAM_WRITE_REVERSE
    #define         OStream_writeBytesReverse(STREAM, VAL, LEN)             Stream_writeBytesReverse(&((STREAM)->Buffer), (VAL), (LEN))
    #define         OStream_write(STREAM, VAL, LEN)                         Stream_write(&((STREAM)->Buffer), (VAL), (LEN))
#else
    #define         OStream_write(STREAM, VAL, LEN)                         Stream_write(&((STREAM)->Buffer), (VAL), (LEN))
#endif
#if STREAM_WRITE_STREAM
    #define         OStream_writeStream(STREAM, VAL, LEN)                   Stream_writeStream(&((STREAM)->Buffer), (VAL), (LEN))
#endif
#if STREAM_WRITE_PADDING
    #define         OStream_writePadding(STREAM, VAL, LEN)                  Stream_writePadding(&((STREAM)->Buffer), (VAL), (LEN))
#endif
#endif // STREAM_WRITE
/* ------------------------------------ Write Value APIs ---------------------------------- */
#if STREAM_WRITE_VALUE
#define             OStream_writeStr(STREAM, VAL)                           Stream_writeStr(&((STREAM)->Buffer), (VAL))
#define             OStream_writeValue(STREAM, VAL, LEN)                    Stream_writeValue(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         OStream_writeChar(STREAM, VAL)                          Stream_writeChar(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT8
    #define         OStream_writeUInt8(STREAM, VAL)                         Stream_writeUInt8(&((STREAM)->Buffer), VAL)
    #define         OStream_writeInt8(STREAM, VAL)                          Stream_writeInt8(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT16
    #define         OStream_writeUInt16(STREAM, VAL)                        Stream_writeUInt16(&((STREAM)->Buffer), VAL)
    #define         OStream_writeInt16(STREAM, VAL)                         Stream_writeInt16(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT32
    #define         OStream_writeUInt32(STREAM, VAL)                        Stream_writeUInt32(&((STREAM)->Buffer), VAL)
    #define         OStream_writeInt32(STREAM, VAL)                         Stream_writeInt32(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT64
    #define         OStream_writeUInt64(STREAM, VAL)                        Stream_writeUInt64(&((STREAM)->Buffer), VAL)
    #define         OStream_writeInt64(STREAM, VAL)                         Stream_writeInt64(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_FLOAT
    #define         OStream_writeFloat(STREAM, VAL)                         Stream_writeFloat(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_DOUBLE
    #define         OStream_writeDouble(STREAM, VAL)                        Stream_writeDouble(&((STREAM)->Buffer), VAL)
#endif
#endif // STREAM_WRITE_VALUE
/* ------------------------------------ Write Value Array APIs ---------------------------------- */
#if STREAM_WRITE_ARRAY
#define             OStream_writeArray(STREAM, VAL, ILEN, LEN)              Stream_writeArray(&((STREAM)->Buffer), (VAL), (ILEN), (LEN))
#if STREAM_WRITE_VALUE
    #define         OStream_writeValueArray(STREAM, VAL, LEN)               Stream_writeValueArray(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         OStream_writeCharArray(STREAM, VAL, LEN)                Stream_writeCharArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT8
    #define         OStream_writeUInt8Array(STREAM, VAL, LEN)               Stream_writeUInt8Array(&((STREAM)->Buffer), VAL, LEN)
    #define         OStream_writeInt8Array(STREAM, VAL, LEN)                Stream_writeInt8Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT16
    #define         OStream_writeUInt16Array(STREAM, VAL, LEN)              Stream_writeUInt16Array(&((STREAM)->Buffer), VAL, LEN)
    #define         OStream_writeInt16Array(STREAM, VAL, LEN)               Stream_writeInt16Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT32
    #define         OStream_writeUInt32Array(STREAM, VAL, LEN)              Stream_writeUInt32Array(&((STREAM)->Buffer), VAL, LEN)
    #define         OStream_writeInt32Array(STREAM, VAL, LEN)               Stream_writeInt32Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT64
    #define         OStream_writeUInt64Array(STREAM, VAL, LEN)              Stream_writeUInt64Array(&((STREAM)->Buffer), VAL, LEN)
    #define         OStream_writeInt64Array(STREAM, VAL, LEN)               Stream_writeInt64Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_FLOAT
    #define         OStream_writeFloatArray(STREAM, VAL, LEN)               Stream_writeFloatArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_DOUBLE
    #define         OStream_writeDoubleArray(STREAM, VAL, LEN)              Stream_writeDoubleArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#endif // STREAM_WRITE_VALUE
#endif // STREAM_WRITE_ARRAY
/* --------------------------------------- Set API------------------------------------------ */
#if STREAM_SET_AT
#define             OStream_setBytesAt(STREAM, IDX, VAL, LEN)               Stream_setBytesAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#if STREAM_SET_AT_BYTES_REVERSE
    #define         OStream_setBytesReverseAt(STREAM, IDX, VAL, LEN)        Stream_setBytesReverseAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
    #define         OStream_setAt(STREAM, IDX, VAL, LEN)                    Stream_setAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#else
    #define         OStream_getAt(STREAM, IDX, VAL, LEN)                    Stream_getAt(&((STREAM)->Buffer), (IDX), (VAL), (LEN))
#endif
#endif // STREAM_SET_AT
/* ------------------------------------ SetAt Value APIs ---------------------------------- */
#if STREAM_SET_AT_VALUE
#define             OStream_setValueAt(STREAM, IDX, VAL)                    Stream_setValueAt(&((STREAM)->Buffer), IDX, VAL)
#if STREAM_CHAR
    #define         OStream_setCharAt(STREAM, IDX, VAL)                     Stream_setCharAt(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT8
    #define         OStream_setUInt8At(STREAM, IDX, VAL)                    Stream_setUInt8At(&((STREAM)->Buffer), IDX, VAL)
    #define         OStream_setInt8At(STREAM, IDX, VAL)                     Stream_setInt8At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT16
    #define         OStream_setUInt16At(STREAM, IDX, VAL)                   Stream_setUInt16At(&((STREAM)->Buffer), IDX, VAL)
    #define         OStream_setInt16At(STREAM, IDX, VAL)                    Stream_setInt16At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT32
    #define         OStream_setUInt32At(STREAM, IDX, VAL)                   Stream_setUInt32At(&((STREAM)->Buffer), IDX, VAL)
    #define         OStream_setInt32At(STREAM, IDX, VAL)                    Stream_setInt32At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT64
    #define         OStream_setUInt64At(STREAM, IDX, VAL)                   Stream_setUInt64At(&((STREAM)->Buffer), IDX, VAL)
    #define         OStream_setInt64At(STREAM, IDX, VAL)                    Stream_setInt64At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_FLOAT
    #define         OStream_setFloatAt(STREAM, IDX, VAL)                    Stream_setFloatAt(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_DOUBLE
    #define         OStream_setDoubleAt(STREAM, IDX, VAL)                   Stream_setDoubleAt(&((STREAM)->Buffer), IDX, VAL)
#endif
#endif // STREAM_SET_AT_VALUE
/* ------------------------------------ SetAt Value Array APIs ---------------------------------- */
#if STREAM_SET_AT_ARRAY
#define             OStream_setArrayAt(STREAM, IDX, VAL, ILEN, LEN)         Stream_setArrayAt(&((STREAM)->Buffer), (IDX), (VAL), (ILEN), (LEN))
#if STREAM_SET_AT_VALUE
    #define         OStream_setValueArrayAt(STREAM, IDX, VAL, LEN)          Stream_setValueArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#if STREAM_CHAR
    #define         OStream_setCharArrayAt(STREAM, IDX, VAL, LEN)           Stream_setCharArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT8
    #define         OStream_setUInt8ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setUInt8ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt8ArrayAt(STREAM, IDX, VAL, LEN)           Stream_setInt8ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT16
    #define         OStream_setUInt16ArrayAt(STREAM, IDX, VAL, LEN)         Stream_setUInt16ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt16ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setInt16ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT32
    #define         OStream_setUInt32ArrayAt(STREAM, IDX, VAL, LEN)         Stream_setUInt32ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt32ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setInt32ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT64
    #define         OStream_setUInt64ArrayAt(STREAM, IDX, VAL, LEN)         Stream_setUInt64ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt64ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setInt64ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_FLOAT
    #define         OStream_setFloatArrayAt(STREAM, IDX, VAL, LEN)          Stream_setFloatArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_DOUBLE
    #define         OStream_setDoubleArrayAt(STREAM, IDX, VAL, LEN)         Stream_setDoubleArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#endif // STREAM_SET_AT_VALUE
#endif // STREAM_SET_AT_ARRAY
/* ------------------------------------ General Set APIs ---------------------------------- */
#if STREAM_SET_VALUE
#define             OStream_setBytes(STREAM, VAL, LEN)                      Stream_setBytes(&((STREAM)->Buffer), (VAL), (LEN))
#define             OStream_setBytes(STREAM, VAL, LEN)                      Stream_setBytes(&((STREAM)->Buffer), (VAL), (LEN))
#if STREAM_SET_BYTES_REVERSE
    #define         OStream_setBytesReverse(STREAM, VAL, LEN)               Stream_setBytesReverse(&((STREAM)->Buffer), (VAL), (LEN))
    #define         OStream_set(STREAM, VAL, LEN)                           Stream_set(&((STREAM)->Buffer), (VAL), (LEN))
#else
    #define         OStream_set(STREAM, VAL, LEN)                           Stream_set(&((STREAM)->Buffer), (VAL), (LEN))
#endif
#endif // STREAM_SET_VALUE
/* ------------------------------------ Set Value Safe APIs ---------------------------------- */
#if STREAM_SET_VALUE
    #define         OStream_setValue(STREAM, LEN, IDX)                      Stream_setValue(&((STREAM)->Buffer), LEN, IDX)
#if STREAM_CHAR
    #define         OStream_setChar(STREAM, VAL)                            Stream_setChar(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT8
    #define         OStream_setUInt8(STREAM, VAL)                           Stream_setUInt8(&((STREAM)->Buffer), VAL)
    #define         OStream_setInt8(STREAM, VAL)                            Stream_setInt8(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT16
    #define         OStream_setUInt16(STREAM, VAL)                          Stream_setUInt16(&((STREAM)->Buffer), VAL)
    #define         OStream_setInt16(STREAM, VAL)                           Stream_setInt16(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT32
    #define         OStream_setUInt32(STREAM, VAL)                          Stream_setUInt32(&((STREAM)->Buffer), VAL)
    #define         OStream_setInt32(STREAM, VAL)                           Stream_setInt32(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT64
    #define         OStream_setUInt64(STREAM, VAL)                          Stream_setUInt64(&((STREAM)->Buffer), VAL)
    #define         OStream_setInt64(STREAM, VAL)                           Stream_setInt64(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_FLOAT
    #define         OStream_setFloat(STREAM, VAL)                           Stream_setFloat(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_DOUBLE
    #define         OStream_setDouble(STREAM, VAL)                          Stream_setDouble(&((STREAM)->Buffer), VAL)
#endif
#endif // STREAM_SET_VALUE
/* ------------------------------------ Set Value Array APIs ---------------------------------- */
#if STREAM_SET_AT_ARRAY
#define             OStream_setArray(STREAM, VAL, ILEN, LEN)                Stream_setArray(&((STREAM)->Buffer), VAL, ILEN, LEN)
#if STREAM_SET_AT_VALUE
    #define         OStream_setValueArray(STREAM, IDX, VAL, LEN)            Stream_setValueArray(&((STREAM)->Buffer), IDX, VAL, LEN)
#if STREAM_CHAR
    #define         OStream_setCharArray(STREAM, IDX, VAL, LEN)             Stream_setCharArray(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT8
    #define         OStream_setUInt8Array(STREAM, IDX, VAL, LEN)            Stream_setUInt8Array(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt8Array(STREAM, IDX, VAL, LEN)             Stream_setInt8Array(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT16
    #define         OStream_setUInt16Array(STREAM, IDX, VAL, LEN)           Stream_setUInt16Array(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt16Array(STREAM, IDX, VAL, LEN)            Stream_setInt16Array(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT32
    #define         OStream_setUInt32Array(STREAM, IDX, VAL, LEN)           Stream_setUInt32Array(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt32Array(STREAM, IDX, VAL, LEN)            Stream_setInt32Array(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT64
    #define         OStream_setUInt64Array(STREAM, IDX, VAL, LEN)           Stream_setUInt64Array(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         OStream_setInt64Array(STREAM, IDX, VAL, LEN)            Stream_setInt64Array(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_FLOAT
    #define         OStream_setFloatArray(STREAM, IDX, VAL, LEN)            Stream_setFloatArray(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_DOUBLE
    #define         OStream_setDoubleArray(STREAM, IDX, VAL, LEN)           Stream_setDoubleArray(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#endif // STREAM_SET_VALUE
#endif // STREAM_SET_ARRAY
/* ------------------------------------ Transpose APIs ---------------------------------- */
#if STREAM_TRANSPOSE_AT
    #define         OStream_transposeAt(STREAM, OFF, LEN, TMP, CLEN, TP, A) Stream_transposeAt(&((STREAM)->Buffer), OFF, LEN, TMP, CLEN, T, A)
#endif
#if STREAM_TRANSPOSE
    #define         OStream_transpose(STREAM, LEN, BUF, CLEN, TP, ARGS)     Stream_transpose(&((STREAM)->Buffer), LEN, BUF, CLEN, TP, ARGS)
#endif


#ifdef __cplusplus
};
#endif

#endif // _OUTPUT_STREAM_H_
