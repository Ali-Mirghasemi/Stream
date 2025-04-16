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
#define ISTREAM_VER_MINOR    8
#define ISTREAM_VER_FIX      3

#include "StreamBuffer.h"

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/
/**
 * @brief This lable shows OStream Libraty is enabled or not
 */
#define ISTREAM                     (1 && STREAM_READ && STREAM_PENDING_BYTES)
/**
 * @brief enable checkTransmit function
 */
#define ISTREAM_CHECK_RECEIVE       1
/**
 * @brief Enable Full Callback
 */
#define ISTREAM_FULL_CALLBACK        1


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
 * @brief when buffer got full, this function will call
 * @param stream IStream
 * @return Stream_LenType
 */
typedef void (*IStream_OnFullFn)(IStream* stream);

/**
 * @brief hold InputStream properties
 */
struct __IStream {
    StreamBuffer            Buffer;         /**< hold stream buffer */
    IStream_ReceiveFn       receive;        /**< receive function */
#if ISTREAM_CHECK_RECEIVE
    IStream_CheckReceiveFn  checkReceive;   /**< check receive function */
#endif
#if ISTREAM_FULL_CALLBACK
    IStream_OnFullFn        onFull;   /**< full callback */
#endif
};


void                IStream_init(IStream* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size);
#define             IStream_deinit(STREAM)                                  memset(STREAM, 0, sizeof(IStream))

/* Input Bytes of IStream */
Stream_Result       IStream_handle(IStream* stream, Stream_LenType len);
Stream_Result       IStream_receive(IStream* stream);
#define             IStream_receiveByte(STREAM, VAL)                        Stream_writeUInt8(&((STREAM)->Buffer), VAL)
#define             IStream_receiveBytes(STREAM, VAL, LEN)                  Stream_writeBytes(&((STREAM)->Buffer), VAL, LEN)

#define             IStream_availableUncheck(STREAM)                        Stream_available(&(STREAM)->Buffer)
Stream_LenType      IStream_available(IStream* stream);

#define             IStream_incomingBytes(STREAM)                           Stream_getPendingBytes(&(STREAM)->Buffer)

#define             IStream_inReceive(STREAM)                               Stream_inReceive(&(STREAM)->Buffer)

#if STREAM_ARGS
    #define         IStream_setArgs(STREAM, ARGS)                           Stream_setArgs(&(STREAM)->Buffer, (ARGS))
    #define         IStream_getArgs(STREAM)                                 Stream_getArgs(&(STREAM)->Buffer)
#endif // OSTREAM_ARGS

#if ISTREAM_CHECK_RECEIVE
    void            IStream_setCheckReceive(IStream* stream, IStream_CheckReceiveFn fn);
#endif // ISTREAM_CHECK_RECEIVE
#if ISTREAM_FULL_CALLBACK
    void            IStream_onFull(IStream* stream, IStream_OnFullFn fn);
#endif

#if STREAM_READ_LOCK_CUSTOM
    #define         IStream_lock(STREAM, LOCK, LEN)                         Stream_lockReadCustom(&(STREAM)->Buffer, &(LOCK)->Buffer, LEN, sizeof(IStream))
    #define         IStream_unlock(STREAM, LOCK)                            Stream_unlockRead(&(STREAM)->Buffer, &(LOCK)->Buffer);
    #define         IStream_unlockIgnore(STREAM)                            Stream_unlockReadIgnore(&(STREAM)->Buffer)
    #define         IStream_lockLen(STREAM, LOCK)                           Stream_lockReadLen(&(STREAM)->Buffer, &(LOCK)->Buffer)
#endif // STREAM_READ_LOCK

#define             IStream_getDataPtr(STREAM)                              Stream_getWritePtr(&((STREAM)->Buffer))
#define             IStream_getBufferSize(STREAM)                           Stream_getBufferSize(&((STREAM)->Buffer))

#define             IStream_clear(STREAM)                                   Stream_clear(&((STREAM)->Buffer))
#define             IStream_reset(STREAM)                                   Stream_reset(&((STREAM)->Buffer))
#define             IStream_resetIO(STREAM)                                 Stream_resetIO(&((STREAM)->Buffer))

#if STREAM_BYTE_ORDER
    #define         IStream_getSystemByteOrder()                            Stream_getSystemByteOrder()
    #define         IStream_setByteOrder(STREAM, ORDER)                     Stream_setByteOrder(&((STREAM)->Buffer), ORDER)
    #define         IStream_getByteOrder(STREAM)                            Stream_getByteOrder(&((STREAM)->Buffer))
#endif

#if STREAM_READ_LIMIT
    #define         IStream_setLimit(STREAM, LEN)                           Stream_setReadLimit(&((STREAM)->Buffer), (LEN))
    #define         IStream_getLimit(STREAM)                                Stream_getReadLimit(&((STREAM)->Buffer))
    #define         IStream_isLimited(STREAM)                               Stream_isReadLimited(&((STREAM)->Buffer))
#endif

#if STREAM_CURSOR
    #define         IStream_getCursor(STREAM, CUR)                          Stream_getCursor(&((STREAM)->Buffer), (CUR))
    #define         IStream_len(STREAM, CUR)                                Stream_getReadLen(&((STREAM)->Buffer), (CUR))
#endif

#define             IStream_ignore(STREAM, LEN)                             Stream_moveReadPos(&((STREAM)->Buffer), (LEN))

/* ------------------------------------ General Read APIs ---------------------------------- */
#if STREAM_READ
#define             IStream_readBytes(STREAM, VAL, LEN)                     Stream_readBytes(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_READ_REVERSE
    #define         IStream_readBytesReverse(STREAM, VAL, LEN)              Stream_readBytesReverse(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_read(STREAM, VAL, LEN)                          Stream_read(&((STREAM)->Buffer), VAL, LEN)
#else
    #define         IStream_read(STREAM, VAL, LEN)                          Stream_read(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_READ_STREAM
    #define        IStream_readStream(IN, OUT, LEN)                         Stream_readStream(&((IN)->Buffer), &((OUT)->Buffer), LEN)
#endif
#endif // STREAM_READ
/* ------------------------------------ Read Value APIs ---------------------------------- */
#if STREAM_READ_VALUE
#define             IStream_readValue(STREAM, LEN)                          Stream_readValue(&((STREAM)->Buffer), LEN)
#if STREAM_CHAR
    #define         IStream_readChar(STREAM)                                Stream_readChar(&((STREAM)->Buffer))
#endif
#if STREAM_UINT8
    #define         IStream_readUInt8(STREAM)                               Stream_readUInt8(&((STREAM)->Buffer))
    #define         IStream_readInt8(STREAM)                                Stream_readInt8(&((STREAM)->Buffer))
#endif
#if STREAM_UINT16
    #define         IStream_readUInt16(STREAM)                              Stream_readUInt16(&((STREAM)->Buffer))
    #define         IStream_readInt16(STREAM)                               Stream_readInt16(&((STREAM)->Buffer))
#endif
#if STREAM_UINT32
    #define         IStream_readUInt32(STREAM)                              Stream_readUInt32(&((STREAM)->Buffer))
    #define         IStream_readInt32(STREAM)                               Stream_readInt32(&((STREAM)->Buffer))
#endif
#if STREAM_UINT64
    #define         IStream_readUInt64(STREAM)                              Stream_readUInt64(&((STREAM)->Buffer))
    #define         IStream_readInt64(STREAM)                               Stream_readInt64(&((STREAM)->Buffer))
#endif
#if STREAM_FLOAT
    #define         IStream_readFloat(STREAM)                               Stream_readFloat(&((STREAM)->Buffer))
#endif
#if STREAM_DOUBLE
    #define         IStream_readDouble(STREAM)                              Stream_readDouble(&((STREAM)->Buffer))
#endif
#endif // STREAM_READ_VALUE
/* ------------------------------------ Read Value Safe APIs ---------------------------------- */
#if STREAM_READ_VALUE_SAFE
    #define         IStream_readValueSafe(STREAM, VAL, LEN)                 Stream_readValueSafe(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         IStream_readCharSafe(STREAM, VAL)                       Stream_readCharSafe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT8
    #define         IStream_readUInt8Safe(STREAM, VAL)                      Stream_readUInt8Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_readInt8Safe(STREAM, VAL)                       Stream_readInt8Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT16
    #define         IStream_readUInt16Safe(STREAM, VAL)                     Stream_readUInt16Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_readInt16Safe(STREAM, VAL)                      Stream_readInt16Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT32
    #define         IStream_readUInt32Safe(STREAM, VAL)                     Stream_readUInt32Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_readInt32Safe(STREAM, VAL)                      Stream_readInt32Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT64
    #define         IStream_readUInt64Safe(STREAM, VAL)                     Stream_readUInt64Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_readInt64Safe(STREAM, VAL)                      Stream_readInt64Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_FLOAT
    #define         IStream_readFloatSafe(STREAM, VAL)                      Stream_readFloatSafe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_DOUBLE
    #define         IStream_readDoubleSafe(STREAM, VAL)                     Stream_readDoubleSafe(&((STREAM)->Buffer), VAL)
#endif
#endif // STREAM_READ_VALUE_SAFE
/* ------------------------------------ Read Value Array APIs ---------------------------------- */
#if STREAM_READ_ARRAY
#define             IStream_readArray(STREAM, VAL, ILEN, LEN)               Stream_readArray(&((STREAM)->Buffer), (VAL), (ILEN), (LEN))
#if STREAM_READ_VALUE
    #define         IStream_readValueArray(STREAM, VAL, LEN)                Stream_readValueArray(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         IStream_readCharArray(STREAM, VAL, LEN)                 Stream_readCharArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT8
    #define         IStream_readUInt8Array(STREAM, VAL, LEN)                Stream_readUInt8Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_readInt8Array(STREAM, VAL, LEN)                 Stream_readInt8Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT16
    #define         IStream_readUInt16Array(STREAM, VAL, LEN)               Stream_readUInt16Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_readInt16Array(STREAM, VAL, LEN)                Stream_readInt16Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT32
    #define         IStream_readUInt32Array(STREAM, VAL, LEN)               Stream_readUInt32Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_readInt32Array(STREAM, VAL, LEN)                Stream_readInt32Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT64
    #define         IStream_readUInt64Array(STREAM, VAL, LEN)               Stream_readUInt64Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_readInt64Array(STREAM, VAL, LEN)                Stream_readInt64Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_DOUBLE
    #define         IStream_readDoubleArray(STREAM, VAL, LEN)               Stream_readDoubleArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_FLOAT
    #define         IStream_readFloatArray(STREAM, VAL, LEN)                Stream_readFloatArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#endif // STREAM_READ_VALUE
#endif // STREAM_READ_ARRAY
/* ------------------------------------ General GetAt APIs ---------------------------------- */
#if STREAM_GET_AT
#define             IStream_getBytesAt(STREAM, IDX, VAL, LEN)               Stream_getBytesAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#if STREAM_GET_AT_BYTES_REVERSE
    #define         IStream_getBytesReverseAt(STREAM, IDX, VAL, LEN)        Stream_getBytesReverseAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         IStream_getAt(STREAM, IDX, VAL, LEN)                    Stream_getAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#else
    #define         IStream_getAt(STREAM, IDX, VAL, LEN)                    Stream_getAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#endif // STREAM_GET_AT
/* ------------------------------------ GetAt Value APIs ---------------------------------- */
#if STREAM_GET_AT_VALUE
#define             IStream_getValueAt(STREAM, IDX, LEN)                    Stream_getValueAt(&((STREAM)->Buffer), IDX, LEN)
#if STREAM_CHAR
    #define         IStream_getCharAt(STREAM, IDX)                          Stream_getCharAt(&((STREAM)->Buffer), IDX)
#endif
#if STREAM_UINT8
    #define         IStream_getUInt8At(STREAM, IDX)                         Stream_getUInt8At(&((STREAM)->Buffer), IDX)
    #define         IStream_getInt8At(STREAM, IDX)                          Stream_getInt8At(&((STREAM)->Buffer), IDX)
#endif
#if STREAM_UINT16
    #define         IStream_getUInt16At(STREAM, IDX)                        Stream_getUInt16At(&((STREAM)->Buffer), IDX)
    #define         IStream_getInt16At(STREAM, IDX)                         Stream_getInt16At(&((STREAM)->Buffer), IDX)
#endif
#if STREAM_UINT32
    #define         IStream_getUInt32At(STREAM, IDX)                        Stream_getUInt32At(&((STREAM)->Buffer), IDX)
    #define         IStream_getInt32At(STREAM, IDX)                         Stream_getInt32At(&((STREAM)->Buffer), IDX)
#endif
#if STREAM_UINT64
    #define         IStream_getUInt64At(STREAM, IDX)                        Stream_getUInt64At(&((STREAM)->Buffer), IDX)
    #define         IStream_getInt64At(STREAM, IDX)                         Stream_getInt64At(&((STREAM)->Buffer), IDX)
#endif
#if STREAM_FLOAT
    #define         IStream_getFloatAt(STREAM, IDX)                         Stream_getFloatAt(&((STREAM)->Buffer), IDX)
#endif
#if STREAM_DOUBLE
    #define         IStream_getDoubleAt(STREAM, IDX)                        Stream_getDoubleAt(&((STREAM)->Buffer), IDX)
#endif
#endif // STREAM_GET_AT_VALUE
/* ------------------------------------ GetAt Value Safe APIs ---------------------------------- */
#if STREAM_GET_AT_VALUE_SAFE
    #define         IStream_getValueAtSafe(STREAM, IDX, VAL)                Stream_getValueAtSafe(&((STREAM)->Buffer), IDX, VAL)
#if STREAM_CHAR
    #define         IStream_getCharAtSafe(STREAM, IDX, VAL)                 Stream_getCharAtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT8
    #define         IStream_getUInt8AtSafe(STREAM, IDX, VAL)                Stream_getUInt8AtSafe(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_getInt8AtSafe(STREAM, IDX, VAL)                 Stream_getInt8AtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT16
    #define         IStream_getUInt16AtSafe(STREAM, IDX, VAL)               Stream_getUInt16AtSafe(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_getInt16AtSafe(STREAM, IDX, VAL)                Stream_getInt16AtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT32
    #define         IStream_getUInt32AtSafe(STREAM, IDX, VAL)               Stream_getUInt32AtSafe(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_getInt32AtSafe(STREAM, IDX, VAL)                Stream_getInt32AtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT64
    #define         IStream_getUInt64AtSafe(STREAM, IDX, VAL)               Stream_getUInt64AtSafe(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_getInt64AtSafe(STREAM, IDX, VAL)                Stream_getInt64AtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_FLOAT
    #define         IStream_getFloatAtSafe(STREAM, IDX, VAL)                Stream_getFloatAtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_DOUBLE
    #define         IStream_getDoubleAtSafe(STREAM, IDX, VAL)               Stream_getDoubleAtSafe(&((STREAM)->Buffer), IDX, VAL)
#endif
#endif // STREAM_GET_AT_VALUE_SAFE
/* ------------------------------------ GetAt Value Array APIs ---------------------------------- */
#if STREAM_GET_AT_ARRAY
#define             IStream_getArrayAt(STREAM, IDX, VAL, ILEN, LEN)         Stream_getArrayAt(&((STREAM)->Buffer), IDX, VAL, ILEN, LEN)
#if STREAM_GET_AT_VALUE
    #define         IStream_getValueArrayAt(STREAM, IDX, VAL, LEN)          Stream_getValueArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#if STREAM_CHAR
    #define         IStream_getCharArrayAt(STREAM, IDX, VAL, LEN)           Stream_getCharArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT8
    #define         IStream_getUInt8ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getUInt8ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         IStream_getInt8ArrayAt(STREAM, IDX, VAL, LEN)           Stream_getInt8ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT16
    #define         IStream_getUInt16ArrayAt(STREAM, IDX, VAL, LEN)         Stream_getUInt16ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         IStream_getInt16ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getInt16ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT32
    #define         IStream_getUInt32ArrayAt(STREAM, IDX, VAL, LEN)         Stream_getUInt32ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         IStream_getInt32ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getInt32ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_UINT64
    #define         IStream_getUInt64ArrayAt(STREAM, IDX, VAL, LEN)         Stream_getUInt64ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
    #define         IStream_getInt64ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getInt64ArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_FLOAT
    #define         IStream_getFloatArrayAt(STREAM, IDX, VAL, LEN)          Stream_getFloatArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#if STREAM_DOUBLE
    #define         IStream_getDoubleArrayAt(STREAM, IDX, VAL, LEN)         Stream_getDoubleArrayAt(&((STREAM)->Buffer), IDX, VAL, LEN)
#endif
#endif // STREAM_GET_AT_VALUE
#endif // STREAM_GET_AT_ARRAY
/* ------------------------------------ General Get APIs ---------------------------------- */
#if STREAM_GET
#define             IStream_getBytes(STREAM, VAL, LEN)                      Stream_getBytes(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_GET_BYTES_REVERSE
    #define         IStream_getBytesReverse(STREAM, VAL, LEN)               Stream_getBytesReverse(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_get(STREAM, VAL, LEN)                           Stream_get(&((STREAM)->Buffer), VAL, LEN)
#else
    #define         IStream_get(STREAM, VAL, LEN)                           Stream_get(&((STREAM)->Buffer), VAL, LEN)
#endif
#endif // STREAM_GET
/* ------------------------------------ Get Value APIs ---------------------------------- */
#if STREAM_GET_VALUE
    #define         IStream_getValue(STREAM)                                Stream_getValue(&((STREAM)->Buffer))
#if STREAM_CHAR
    #define         IStream_getChar(STREAM)                                 Stream_getChar(&((STREAM)->Buffer))
#endif
#if STREAM_UINT8
    #define         IStream_getUInt8(STREAM)                                Stream_getUInt8(&((STREAM)->Buffer))
    #define         IStream_getInt8(STREAM)                                 Stream_getInt8(&((STREAM)->Buffer))
#endif
#if STREAM_UINT16
    #define         IStream_getUInt16(STREAM)                               Stream_getUInt16(&((STREAM)->Buffer))
    #define         IStream_getInt16(STREAM)                                Stream_getInt16(&((STREAM)->Buffer))
#endif
#if STREAM_UINT32
    #define         IStream_getUInt32(STREAM)                               Stream_getUInt32(&((STREAM)->Buffer))
    #define         IStream_getInt32(STREAM)                                Stream_getInt32(&((STREAM)->Buffer))
#endif
#if STREAM_UINT64
    #define         IStream_getUInt64(STREAM)                               Stream_getUInt64(&((STREAM)->Buffer))
    #define         IStream_getInt64(STREAM)                                Stream_getInt64(&((STREAM)->Buffer))
#endif
#if STREAM_FLOAT
    #define         IStream_getFloat(STREAM)                                Stream_getFloat(&((STREAM)->Buffer))
#endif
#if STREAM_DOUBLE
    #define         IStream_getDouble(STREAM)                               Stream_getDouble(&((STREAM)->Buffer))
#endif
#endif // STREAM_GET_VALUE
/* ------------------------------------ Get Value Safe APIs ---------------------------------- */
#if STREAM_GET_VALUE_SAFE
    #define         IStream_getValueSafe(STREAM, VAL, LEN)                  Stream_getValueSafe(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         IStream_getCharSafe(STREAM, VAL)                        Stream_getCharSafe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT8
    #define         IStream_getUInt8Safe(STREAM, VAL)                       Stream_getUInt8Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_getInt8Safe(STREAM, VAL)                        Stream_getInt8Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT16
    #define         IStream_getUInt16Safe(STREAM, VAL)                      Stream_getUInt16Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_getInt16Safe(STREAM, VAL)                       Stream_getInt16Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT32
    #define         IStream_getUInt32Safe(STREAM, VAL)                      Stream_getUInt32Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_getInt32Safe(STREAM, VAL)                       Stream_getInt32Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT64
    #define         IStream_getUInt64Safe(STREAM, VAL)                      Stream_getUInt64Safe(&((STREAM)->Buffer), VAL)
    #define         IStream_getInt64Safe(STREAM, VAL)                       Stream_getInt64Safe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_FLOAT
    #define         IStream_getFloatSafe(STREAM, VAL)                       Stream_getFloatSafe(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_DOUBLE
    #define         IStream_getDoubleSafe(STREAM, VAL)                      Stream_getDoubleSafe(&((STREAM)->Buffer), VAL)
#endif
#endif // STREAM_GET_VALUE_SAFE
/* ------------------------------------ Get Value Array APIs ---------------------------------- */
#if STREAM_GET_ARRAY
#define             IStream_getArray(STREAM, VAL, ITEM_LEN, LEN)            Stream_getArray(&((STREAM)->Buffer), VAL, ITEM_LEN, LEN)
#if STREAM_GET_VALUE
    #define         IStream_getValueArray(STREAM, VAL, LEN)                 Stream_getValueArray(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         IStream_getCharArray(STREAM, VAL, LEN)                  Stream_getCharArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT8
    #define         IStream_getUInt8Array(STREAM, VAL, LEN)                 Stream_getUInt8Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_getInt8Array(STREAM, VAL, LEN)                  Stream_getInt8Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT16
    #define         IStream_getUInt16Array(STREAM, VAL, LEN)                Stream_getUInt16Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_getInt16Array(STREAM, VAL, LEN)                 Stream_getInt16Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT32
    #define         IStream_getUInt32Array(STREAM, VAL, LEN)                Stream_getUInt32Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_getInt32Array(STREAM, VAL, LEN)                 Stream_getInt32Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_UINT64
    #define         IStream_getUInt64Array(STREAM, VAL, LEN)                Stream_getUInt64Array(&((STREAM)->Buffer), VAL, LEN)
    #define         IStream_getInt64Array(STREAM, VAL, LEN)                 Stream_getInt64Array(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_FLOAT
    #define         IStream_getFloatArray(STREAM, VAL, LEN)                 Stream_getFloatArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#if STREAM_DOUBLE
    #define         IStream_getDoubleArray(STREAM, VAL, LEN)                Stream_getDoubleArray(&((STREAM)->Buffer), VAL, LEN)
#endif
#endif // STREAM_GET_VALUE
#endif // STREAM_GET
/* ------------------------------------ General Find At APIs ---------------------------------- */
#if STREAM_FIND_AT
#define             IStream_findByteAt(STREAM, OFF, VAL)                    Stream_findByteAt(&((STREAM)->Buffer), OFF, VAL)
#define             IStream_findPatternAt(STREAM, OFF, PAT, PLEN)           Stream_findPatternAt(&((STREAM)->Buffer), OFF, PAT, PLEN)
#if STREAM_FIND_AT_VALUE
#define             IStream_findValueAt(STREAM, OFF, VAL, LEN)              Stream_findValueAt(&((STREAM)->Buffer), OFF, VAL, LEN)
#if STREAM_CHAR
    #define         IStream_findCharAt(STREAM, IDX, VAL)                    Stream_findCharAt(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT8
    #define         IStream_findUInt8At(STREAM, IDX, VAL)                   Stream_findUInt8At(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_findInt8At(STREAM, IDX, VAL)                    Stream_findInt8At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT16
    #define         IStream_findUInt16At(STREAM, IDX, VAL)                  Stream_findUInt16At(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_findInt16At(STREAM, IDX, VAL)                   Stream_findInt16At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT32
    #define         IStream_findUInt32At(STREAM, IDX, VAL)                  Stream_findUInt32At(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_findInt32At(STREAM, IDX, VAL)                   Stream_findInt32At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_UINT64
    #define         IStream_findUInt64At(STREAM, IDX, VAL)                  Stream_findUInt64At(&((STREAM)->Buffer), IDX, VAL)
    #define         IStream_findInt64At(STREAM, IDX, VAL)                   Stream_findInt64At(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_FLOAT
    #define         IStream_findFloatAt(STREAM, IDX, VAL)                   Stream_findFloatAt(&((STREAM)->Buffer), IDX, VAL)
#endif
#if STREAM_DOUBLE
    #define         IStream_findDoubleAt(STREAM, IDX, VAL)                  Stream_findDoubleAt(&((STREAM)->Buffer), IDX, VAL)
#endif
#endif // STREAM_FIND_AT_VALUE
#endif // STREAM_FIND_AT
/* ------------------------------------ General Find APIs ---------------------------------- */
#if STREAM_FIND
#define             IStream_findByte(STREAM, VAL)                           Stream_findByte(&((STREAM)->Buffer), VAL)
#define             IStream_findPattern(STREAM, PAT, PAT_LEN)               Stream_findPattern(&((STREAM)->Buffer), PAT, PAT_LEN)
#if STREAM_FIND_AT_VALUE
    #define         IStream_findValue(STREAM, VAL, LEN)                     Stream_findValue(&((STREAM)->Buffer), VAL, LEN)
#if STREAM_CHAR
    #define         IStream_findChar(STREAM, VAL)                           Stream_findChar(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT8
    #define         IStream_findUInt8(STREAM, VAL)                          Stream_findUInt8(&((STREAM)->Buffer), VAL)
    #define         IStream_findInt8(STREAM, VAL)                           Stream_findInt8(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT16
    #define         IStream_findUInt16(STREAM, VAL)                         Stream_findUInt16(&((STREAM)->Buffer), VAL)
    #define         IStream_findInt16(STREAM, VAL)                          Stream_findInt16(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT32
    #define         IStream_findUInt32(STREAM, VAL)                         Stream_findUInt32(&((STREAM)->Buffer), VAL)
    #define         IStream_findInt32(STREAM, VAL)                          Stream_findInt32(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_UINT64
    #define         IStream_findUInt64(STREAM, VAL)                         Stream_findUInt64(&((STREAM)->Buffer), VAL)
    #define         IStream_findInt64(STREAM, VAL)                          Stream_findInt64(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_FLOAT
    #define         IStream_findFloat(STREAM, VAL)                          Stream_findFloat(&((STREAM)->Buffer), VAL)
#endif
#if STREAM_DOUBLE
    #define         IStream_findDouble(STREAM, VAL)                         Stream_findDouble(&((STREAM)->Buffer), VAL)
#endif
#endif // STREAM_FIND_AT_VALUE
#endif // STREAM_FIND_AT
/* ------------------------------------ Read Bytes Until At APIs ---------------------------------- */
#if STREAM_READ_UNTIL_AT
#define             IStream_readBytesUntilAt(STREAM, OFF, END, VAL, LEN)    Stream_readBytesUntilAt(&((STREAM)->Buffer), OFF, END, VAL, LEN)
#define             IStream_readBytesUntilPatternAt(STREAM,  OFF, PAT, PLEN, VAL, LEN) Stream_readBytesUntilPatternAt(&((STREAM)->Buffer),  OFF, PAT, PLEN, VAL, LEN)
#endif // STREAM_READ_UNTIL_AT
/* ------------------------------------ Read Bytes Until APIs ---------------------------------- */
#if STREAM_READ_UNTIL
#define             IStream_readBytesUntil(STREAM, END, VAL, LEN)            Stream_readBytesUntil(&((STREAM)->Buffer), END, VAL, LEN)
#define             IStream_readBytesUntilPattern(STREAM, P, PLEN, VAL, LEN) Stream_readBytesUntilPatternAt(&((STREAM)->Buffer), 0, (P), (PLEN), (VAL), (LEN))
#endif // STREAM_READ_UNTIL
/* ------------------------------------ Transpose APIs ---------------------------------- */
#if STREAM_TRANSPOSE_AT
    #define         IStream_transposeAt(STREAM, OFF, LEN, TMP, CLEN, TP, A) Stream_transposeAt(&((STREAM)->Buffer), OFF, LEN, TMP, CLEN, T, A)
#endif
#if STREAM_TRANSPOSE
    #define         IStream_transpose(STREAM, LEN, BUF, CLEN, TP, ARGS)     Stream_transpose(&((STREAM)->Buffer), LEN, BUF, CLEN, TP, ARGS)
#endif

#ifdef __cplusplus
};
#endif

#endif // _INPUT_STREAM_H_
