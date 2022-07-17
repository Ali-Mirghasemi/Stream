/**
 * @file StreamBuffer.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief this library implement stream buffer with read & write operations
 * @version 0.3
 * @date 2021-09-01
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef _STREAM_H_
#define _STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define STREAM_VER_MAJOR    0
#define STREAM_VER_MINOR    3
#define STREAM_VER_FIX      0

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/

/**
 * @brief you can enable ByteOrder option to have r/w operation
 * on what endian you need
 */
#define STREAM_BYTE_ORDER                   1
#if STREAM_BYTE_ORDER
    /**
     * @brief save system byte order in static variable for avoid calculate each time
     * run Stream_getSystemByteOrder function
     */
    #define STREAM_BYTE_ORDER_SYS_STATIC    1
#endif
/**
 * @brief enable set limit for write functions
 */
#define STREAM_WRITE_LIMIT                  1
/**
 * @brief enable set limit for read operations
 */
#define STREAM_READ_LIMIT                   1
/**
 * @brief enable cursor object for check how many bytes read or write
 */
#define STREAM_CURSOR                       0
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
#define STREAM_UINT64                       1
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
#define STREAM_DOUBLE                       1
/**
 * @brief based on maximum size of buffer that you use for stream
 * you can change type of len variables
 * must be signed type
 */
typedef int16_t Stream_LenType;

/************************************************************************/

#define __STREAM_VER_STR(major, minor, fix)     #major "." #minor "." #fix
#define _STREAM_VER_STR(major, minor, fix)      __STREAM_VER_STR(major, minor, fix)
/**
 * @brief show stream version in string format
 */
#define STREAM_VER_STR                          _STREAM_VER_STR(STREAM_VER_MAJOR, STREAM_VER_MINOR, STREAM_VER_FIX)
/**
 * @brief show stream version in integer format, ex: 0.2.0 -> 200
 */
#define STREAM_VER                              ((STREAM_VER_MAJOR * 10000UL) + (STREAM_VER_MINOR * 100UL) + (STREAM_VER_FIX))

/**
 * @brief use for disable limit
 */
#define STREAM_NO_LIMIT                     -1

/**
 * @brief default value for read function if cause error
 */
#define STREAM_READ_DEFAULT_VALUE           0

/**
 * @brief you can choose what ByteOrder can use for r/w operations
 */
typedef enum {
    ByteOrder_LittleEndian  = 0,    /**< little endian */
    ByteOrder_BigEndian     = 1,    /**< big endian */
    ByteOrder_Reserved      = 0x0F,
} ByteOrder;
/**
 * @brief result of Stream functions
 */
typedef enum {
    Stream_Ok               = 0,    /**< everything is OK */
    Stream_NoSpace          = 1,    /**< there is no space for write operation */
    Stream_NoAvailable      = 2,    /**< there no available bytes for read operation */
    Stream_BufferFull       = 3,    /**< buffer full*/
    Stream_NoReceiveFn      = 4,    /**< no receive function set for IStream */
    Stream_NoTransmitFn     = 5,    /**< no transmit function set for OStream */
    Stream_NoReceive		= 6,    /**< stream is not in receive mode */
    Stream_NoTransmit		= 7,    /**< stream is not in transmit mode */
    Stream_InReceive		= 8,    /**< stream is in receive mode */
    Stream_InTransmit		= 9,    /**< stream is in transmit mode */
} Stream_Result;
/**
 * @brief Stream struct
 * contains everything need for handle stream
 */
typedef struct {
    uint8_t*                Data;                   /**< pointer to buffer */
    Stream_LenType          Size;                   /**< size of buffer */
    Stream_LenType          WPos;                   /**< write position */
    Stream_LenType          RPos;                   /**< read position */
#if STREAM_WRITE_LIMIT
    Stream_LenType          WriteLimit;             /**< limit for write operation */
#endif // STREAM_WRITE_LIMIT
#if STREAM_READ_LIMIT
    Stream_LenType          ReadLimit;              /**< limit for read operation */
#endif
    uint8_t                 Overflow    : 1;        /**< overflow flag */
    uint8_t                 InReceive   : 1;        /**< stream is in receive mode */
    uint8_t                 InTransmit  : 1;        /**< stream is in transmit mode */
#if STREAM_BYTE_ORDER
    uint8_t                 Order       : 1;        /**< byte order */
    uint8_t                 OrderFn     : 1;        /**< byte order function */
    uint8_t                 Reserved    : 3;        /**< reserved */
#else
    uint8_t                 Reserved    : 5;        /**< reserved */
#endif
} Stream;
/**
 * @brief hold properties of cursor over stream
 */
typedef struct {
    Stream_LenType          WPos;
    Stream_LenType          RPos;
} Stream_Cursor;

void Stream_init(Stream* stream, uint8_t* buffer, Stream_LenType size);
void Stream_deinit(Stream* stream);

/*************** General APIs *************/

#if STREAM_WRITE_LIMIT
    #define Stream_space(STREAM)                Stream_spaceLimit((STREAM))
#else
    #define Stream_space(STREAM)                Stream_spaceReal((STREAM))
#endif // STREAM_WRITE_LIMIT

#if STREAM_READ_LIMIT
    #define Stream_available(STREAM)            Stream_availableLimit((STREAM))
#else
    #define Stream_available(STREAM)            Stream_availableReal((STREAM))
#endif // STREAM_READ_LIMIT

Stream_LenType Stream_availableReal(Stream* stream);
Stream_LenType Stream_spaceReal(Stream* stream);
uint8_t Stream_isEmpty(Stream* stream);
uint8_t Stream_isFull(Stream* stream);

Stream_LenType Stream_directAvailable(Stream* stream);
Stream_LenType Stream_directSpace(Stream* stream);

Stream_LenType Stream_directAvailableAt(Stream* stream, Stream_LenType index);
Stream_LenType Stream_directSpaceAt(Stream* stream, Stream_LenType index);

uint8_t* Stream_getWritePtr(Stream* stream);
uint8_t* Stream_getReadPtr(Stream* stream);
uint8_t* Stream_getWritePtrAt(Stream* stream, Stream_LenType index);
uint8_t* Stream_getReadPtrAt(Stream* stream, Stream_LenType index);

void Stream_reset(Stream* stream);
void Stream_resetIO(Stream* stream);
void Stream_clear(Stream* stream);

uint8_t* Stream_getBuffer(Stream* stream);

void Stream_setBuffer(Stream* stream, uint8_t* data, Stream_LenType size);
Stream_LenType Stream_getBufferSize(Stream* stream);

Stream_LenType Stream_getWritePos(Stream* stream);
Stream_LenType Stream_getReadPos(Stream* stream);

Stream_Result Stream_moveWritePos(Stream* stream, Stream_LenType steps);
Stream_Result Stream_moveReadPos(Stream* stream, Stream_LenType steps);

#if STREAM_BYTE_ORDER
    ByteOrder  Stream_getSystemByteOrder(void);
    void       Stream_setByteOrder(Stream* stream, ByteOrder order);
    ByteOrder  Stream_getByteOrder(Stream* stream);
#endif // STREAM_BYTE_ORDER

#if STREAM_WRITE_LIMIT
    void       Stream_setWriteLimit(Stream* stream, Stream_LenType len);
    uint8_t    Stream_isWriteLimited(Stream* stream);
    Stream_LenType Stream_spaceLimit(Stream* stream);
    Stream_LenType Stream_getWriteLimit(Stream* stream);
#endif // STREAM_WRITE_LIMIT

#if STREAM_READ_LIMIT
    void       Stream_setReadLimit(Stream* stream, Stream_LenType len);
    uint8_t    Stream_isReadLimited(Stream* stream);
    Stream_LenType Stream_availableLimit(Stream* stream);
    Stream_LenType Stream_getReadLimit(Stream* stream);
#endif // STREAM_READ_LIMIT

#if STREAM_CURSOR
    void       Stream_getCursor(Stream* stream, Stream_Cursor* cursor);
    Stream_LenType Stream_getReadLen(Stream* stream, Stream_Cursor* cursor);
    Stream_LenType Stream_getWriteLen(Stream* stream, Stream_Cursor* cursor);
#endif // STREAM_CURSOR

/**************** Write APIs **************/
Stream_Result Stream_writeBytes(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_writeBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_writeChar(Stream* stream, char val);
Stream_Result Stream_writeUInt8(Stream* stream, uint8_t val);
Stream_Result Stream_writeInt8(Stream* stream, int8_t val);
Stream_Result Stream_writeUInt16(Stream* stream, uint16_t val);
Stream_Result Stream_writeInt16(Stream* stream, int16_t val);
Stream_Result Stream_writeUInt32(Stream* stream, uint32_t val);
Stream_Result Stream_writeInt32(Stream* stream, int32_t val);
Stream_Result Stream_writeFloat(Stream* stream, float val);
#if STREAM_UINT64
    Stream_Result Stream_writeUInt64(Stream* stream, uint64_t val);
    Stream_Result Stream_writeInt64(Stream* stream, int64_t val);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_writeDouble(Stream* stream, double val);
#endif // STREAM_DOUBLE

Stream_Result Stream_writeCharArray(Stream* stream, char* val, Stream_LenType len);
Stream_Result Stream_writeUInt8Array(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_writeInt8Array(Stream* stream, int8_t* val, Stream_LenType len);
Stream_Result Stream_writeUInt16Array(Stream* stream, uint16_t* val, Stream_LenType len);
Stream_Result Stream_writeInt16Array(Stream* stream, int16_t* val, Stream_LenType len);
Stream_Result Stream_writeUInt32Array(Stream* stream, uint32_t* val, Stream_LenType len);
Stream_Result Stream_writeInt32Array(Stream* stream, int32_t* val, Stream_LenType len);
Stream_Result Stream_writeFloatArray(Stream* stream, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_writeUInt64Array(Stream* stream, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_writeInt64Array(Stream* stream, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_writeDoubleArray(Stream* stream, double val, Stream_LenType len);
#endif // STREAM_DOUBLE

Stream_Result Stream_writeStream(Stream* out, Stream* in, Stream_LenType len);

Stream_Result Stream_writePadding(Stream* stream, uint8_t val, Stream_LenType len);

/**************** Read APIs **************/
int16_t  Stream_read(Stream* stream);
Stream_Result Stream_readBytes(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_readBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len);
char     Stream_readChar(Stream* stream);
uint8_t  Stream_readUInt8(Stream* stream);
int8_t   Stream_readInt8(Stream* stream);
uint16_t Stream_readUInt16(Stream* stream);
int16_t  Stream_readInt16(Stream* stream);
uint32_t Stream_readUInt32(Stream* stream);
int32_t  Stream_readInt32(Stream* stream);
float    Stream_readFloat(Stream* stream);
#if STREAM_UINT64
    uint64_t Stream_readUInt64(Stream* stream);
    int64_t  Stream_readInt64(Stream* stream);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    double   Stream_readDouble(Stream* stream);
#endif // STREAM_DOUBLE

Stream_Result Stream_readCharArray(Stream* stream, char* val, Stream_LenType len);
Stream_Result Stream_readUInt8Array(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_readInt8Array(Stream* stream, int8_t* val, Stream_LenType len);
Stream_Result Stream_readUInt16Array(Stream* stream, uint16_t* val, Stream_LenType len);
Stream_Result Stream_readInt16Array(Stream* stream, int16_t* val, Stream_LenType len);
Stream_Result Stream_readUInt32Array(Stream* stream, uint32_t* val, Stream_LenType len);
Stream_Result Stream_readInt32Array(Stream* stream, int32_t* val, Stream_LenType len);
Stream_Result Stream_readFloatArray(Stream* stream, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_readUInt64Array(Stream* stream, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_readInt64Array(Stream* stream, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_readDoubleArray(Stream* stream, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE

Stream_Result Stream_readStream(Stream* in, Stream* out, Stream_LenType len);

Stream_Result Stream_getBytes(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len);
char     Stream_getChar(Stream* stream);
uint8_t  Stream_getUInt8(Stream* stream);
int8_t   Stream_getInt8(Stream* stream);
uint16_t Stream_getUInt16(Stream* stream);
int16_t  Stream_getInt16(Stream* stream);
uint32_t Stream_getUInt32(Stream* stream);
int32_t  Stream_getInt32(Stream* stream);
float    Stream_getFloat(Stream* stream);
#if STREAM_UINT64
    uint64_t Stream_getUInt64(Stream* stream);
    int64_t  Stream_getInt64(Stream* stream);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    double   Stream_getDouble(Stream* stream);
#endif // STREAM_DOUBLE

Stream_Result Stream_getCharArray(Stream* stream, char* val, Stream_LenType len);
Stream_Result Stream_getUInt8Array(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getInt8Array(Stream* stream, int8_t* val, Stream_LenType len);
Stream_Result Stream_getUInt16Array(Stream* stream, uint16_t* val, Stream_LenType len);
Stream_Result Stream_getInt16Array(Stream* stream, int16_t* val, Stream_LenType len);
Stream_Result Stream_getUInt32Array(Stream* stream, uint32_t* val, Stream_LenType len);
Stream_Result Stream_getInt32Array(Stream* stream, int32_t* val, Stream_LenType len);
Stream_Result Stream_getFloatArray(Stream* stream, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_getUInt64Array(Stream* stream, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_getInt64Array(Stream* stream, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_getDoubleArray(Stream* stream, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE

Stream_Result Stream_getBytesAt(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getBytesReverseAt(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
char     Stream_getCharAt(Stream* stream, Stream_LenType index);
uint8_t  Stream_getUInt8At(Stream* stream, Stream_LenType index);
int8_t   Stream_getInt8At(Stream* stream, Stream_LenType index);
uint16_t Stream_getUInt16At(Stream* stream, Stream_LenType index);
int16_t  Stream_getInt16At(Stream* stream, Stream_LenType index);
uint32_t Stream_getUInt32At(Stream* stream, Stream_LenType index);
int32_t  Stream_getInt32At(Stream* stream, Stream_LenType index);
float    Stream_getFloatAt(Stream* stream, Stream_LenType index);
#if STREAM_UINT64
    uint64_t Stream_getUInt64At(Stream* stream, Stream_LenType index);
    int64_t  Stream_getInt64At(Stream* stream, Stream_LenType index);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    double   Stream_getDoubleAt(Stream* stream, Stream_LenType index);
#endif // STREAM_DOUBLE

Stream_Result Stream_getCharArrayAt(Stream* stream, Stream_LenType index, char* val, Stream_LenType len);
Stream_Result Stream_getUInt8ArrayAt(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getInt8ArrayAt(Stream* stream, Stream_LenType index, int8_t* val, Stream_LenType len);
Stream_Result Stream_getUInt16ArrayAt(Stream* stream, Stream_LenType index, uint16_t* val, Stream_LenType len);
Stream_Result Stream_getInt16ArrayAt(Stream* stream, Stream_LenType index, int16_t* val, Stream_LenType len);
Stream_Result Stream_getUInt32ArrayAt(Stream* stream, Stream_LenType index, uint32_t* val, Stream_LenType len);
Stream_Result Stream_getInt32ArrayAt(Stream* stream, Stream_LenType index, int32_t* val, Stream_LenType len);
Stream_Result Stream_getFloatArrayAt(Stream* stream, Stream_LenType index, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_getUInt64ArrayAt(Stream* stream, Stream_LenType index, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_getInt64ArrayAt(Stream* stream, Stream_LenType index, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_getDoubleArrayAt(Stream* stream, Stream_LenType index, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE


int8_t Stream_compareAt(Stream* stream, Stream_LenType index, const uint8_t* val, Stream_LenType len);
Stream_LenType Stream_findByte(Stream* stream, uint8_t val);
Stream_LenType Stream_findByteAt(Stream* stream, Stream_LenType offset, uint8_t val);
Stream_LenType Stream_findPattern(Stream* stream, const uint8_t* pat, Stream_LenType patLen);
Stream_LenType Stream_findPatternAt(Stream* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen);
Stream_LenType Stream_readBytesUntil(Stream* stream, uint8_t end, uint8_t* val, Stream_LenType len);
Stream_LenType Stream_readBytesUntilPattern(Stream* stream, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len);

#ifdef __cplusplus
};
#endif

#endif /* _STREAM_H_ */
