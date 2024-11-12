/**
 * @file StreamBuffer.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief this library implement stream buffer with read & write operations
 * @version 0.5
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
#define STREAM_VER_MINOR    6
#define STREAM_VER_FIX      1

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
 * @brief enable write lock feature
 */
#define STREAM_WRITE_LOCK                   1
/**
 * @brief enable read lock feature
 */
#define STREAM_READ_LOCK                    1
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
 * @brief enable get functions
 */
#define STREAM_GET_FUNCTIONS                1
/**
 * @brief enable getAt functions
 */
#define STREAM_GET_AT_FUNCTIONS             1
/**
 * @brief enable find functions
 */
#define STREAM_FIND_FUNCTIONS               1
/**
 * @brief enable findAt functions
 */
#define STREAM_FIND_AT_FUNCTIONS            1
/**
 * @brief check len parameter in read/write functions
 */
#define STREAM_CHECK_ZERO_LEN               1
/* StreamBuffer Memory IO States */
#define STREAM_MEM_IO_DEFAULT               0
#define STREAM_MEM_IO_CUSTOM                1
#define STREAM_MEM_IO_DRIVER                2
/**
 * @brief This features help you to override memcpy, memrcpy, memset and etc with your custom functions
 */
#define STREAM_MEM_IO                       STREAM_MEM_IO_DEFAULT
/* Default IO functions */
#define STREAM_MEM_COPY                     memcpy
#define STREAM_MEM_COPY_REVERSE             memrcpy
#define STREAM_MEM_SET                      memset
#define STREAM_MEM_REVERSE                  memreverse
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
 * @brief result of StreamBuffer functions
 */
typedef enum {
    Stream_Ok               = 0,        /**< everything is OK */
    Stream_NoSpace          = 1,        /**< there is no space for write operation */
    Stream_NoAvailable      = 2,        /**< there no available bytes for read operation */
    Stream_BufferFull       = 3,        /**< buffer full*/
    Stream_NoReceiveFn      = 4,        /**< no receive function set for IStream */
    Stream_NoTransmitFn     = 5,        /**< no transmit function set for OStream */
    Stream_NoReceive        = 6,        /**< stream is not in receive mode */
    Stream_NoTransmit       = 7,        /**< stream is not in transmit mode */
    Stream_InReceive        = 8,        /**< stream is in receive mode */
    Stream_InTransmit       = 9,        /**< stream is in transmit mode */
    Stream_ZeroLen          = 10,       /**< len parameter is zero */
    Stream_ReceiveFailed    = 11,       /**< failed in receive */
    Stream_TransmitFailed   = 12,       /**< failed in transmit */
    Stream_CustomError      = 0x80,     /**< can be used for custom errors */
} Stream_Result;
/**
 * @brief describe flush mode, use in OStream
 */
typedef enum {
    Stream_FlushMode_Single     = 0,    /**< flush only pending bytes before call flush function */
    Stream_FlushMode_Continue   = 1,    /**< after flush complete if there is any pending bytes transmit pending bytes again */
} Stream_FlushMode;
/**
 * @brief Custom memcpy for override default function
 */
typedef Stream_LenType (*Stream_MemCopyFn)(void* dest, const void* src, Stream_LenType len);
/**
 * @brief Custom memrcpy for override default function
 */
typedef Stream_LenType (*Stream_MemCopyReverseFn)(void* dest, const void* src, Stream_LenType len);
/**
 * @brief Custom memset for override default function
 */
typedef Stream_LenType (*Stream_MemSetFn)(void* src, uint8_t val, Stream_LenType len);
/**
 * @brief Custom memset for override default function
 */
typedef Stream_LenType (*Stream_MemReverseFn)(void* src, Stream_LenType len);

typedef struct {
    Stream_MemCopyFn              copy;
    Stream_MemCopyReverseFn       copyReverse;
    Stream_MemSetFn               set;
    Stream_MemReverseFn           reverse;
} Stream_MemIO;

/**
 * @brief StreamBuffer struct
 * contains everything need for handle stream
 */
typedef struct {
#if   STREAM_MEM_IO == STREAM_MEM_IO_CUSTOM
    Stream_MemIO            Mem;                    /**< Custom io functions for interact with memory */
#elif STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
    const Stream_MemIO*     Mem;                    /**< Custom io functions for interact with memory in driver mode */
#endif
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
    uint8_t                 Overflow        : 1;    /**< overflow flag */
    uint8_t                 InReceive       : 1;    /**< stream is in receive mode */
    uint8_t                 InTransmit      : 1;    /**< stream is in transmit mode */
    uint8_t                 Order           : 1;    /**< byte order */
    uint8_t                 OrderFn         : 1;    /**< byte order function */
    uint8_t                 WriteLocked     : 1;    /**< stream write locked */
    uint8_t                 ReadLocked      : 1;    /**< stream write locked */
    uint8_t                 FlushMode       : 1;    /**< flush mode */
} StreamBuffer;
/**
 * @brief hold properties of cursor over stream
 */
typedef struct {
    Stream_LenType          WPos;
    Stream_LenType          RPos;
} Stream_Cursor;

void Stream_init(StreamBuffer* stream, uint8_t* buffer, Stream_LenType size);
void Stream_fromBuff(StreamBuffer* stream, uint8_t* buffer, Stream_LenType size, Stream_LenType len);
void Stream_deinit(StreamBuffer* stream);

#if STREAM_MEM_IO == STREAM_MEM_IO_CUSTOM
void Stream_setMemIO(
    StreamBuffer*                 stream,
    Stream_MemCopyFn          copy,
    Stream_MemCopyReverseFn   copyReverse,
    Stream_MemSetFn           set,
    Stream_MemReverseFn       reverse
);
#elif STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
void Stream_setMemIO(StreamBuffer* stream, const Stream_MemIO* mem);
#endif

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

Stream_LenType Stream_availableReal(StreamBuffer* stream);
Stream_LenType Stream_spaceReal(StreamBuffer* stream);
uint8_t Stream_isEmpty(StreamBuffer* stream);
uint8_t Stream_isFull(StreamBuffer* stream);

Stream_LenType Stream_directAvailable(StreamBuffer* stream);
Stream_LenType Stream_directSpace(StreamBuffer* stream);

Stream_LenType Stream_directAvailableAt(StreamBuffer* stream, Stream_LenType index);
Stream_LenType Stream_directSpaceAt(StreamBuffer* stream, Stream_LenType index);

uint8_t* Stream_getWritePtr(StreamBuffer* stream);
uint8_t* Stream_getReadPtr(StreamBuffer* stream);
uint8_t* Stream_getWritePtrAt(StreamBuffer* stream, Stream_LenType index);
uint8_t* Stream_getReadPtrAt(StreamBuffer* stream, Stream_LenType index);

void Stream_reset(StreamBuffer* stream);
void Stream_resetIO(StreamBuffer* stream);
void Stream_clear(StreamBuffer* stream);

void Stream_setFlushMode(StreamBuffer* stream, Stream_FlushMode mode);

uint8_t Stream_inReceive(StreamBuffer* stream);
uint8_t Stream_inTransmit(StreamBuffer* stream);

uint8_t* Stream_getBuffer(StreamBuffer* stream);

void Stream_setBuffer(StreamBuffer* stream, uint8_t* data, Stream_LenType size);
Stream_LenType Stream_getBufferSize(StreamBuffer* stream);

Stream_LenType Stream_getWritePos(StreamBuffer* stream);
Stream_LenType Stream_getReadPos(StreamBuffer* stream);

Stream_Result Stream_moveWritePos(StreamBuffer* stream, Stream_LenType steps);
Stream_Result Stream_moveReadPos(StreamBuffer* stream, Stream_LenType steps);

void Stream_flipWrite(StreamBuffer* stream, Stream_LenType len);
void Stream_flipRead(StreamBuffer* stream, Stream_LenType len);

#if STREAM_BYTE_ORDER
    ByteOrder  Stream_getSystemByteOrder(void);
    void       Stream_setByteOrder(StreamBuffer* stream, ByteOrder order);
    ByteOrder  Stream_getByteOrder(StreamBuffer* stream);
#endif // STREAM_BYTE_ORDER

#if STREAM_WRITE_LIMIT
    void       Stream_setWriteLimit(StreamBuffer* stream, Stream_LenType len);
    uint8_t    Stream_isWriteLimited(StreamBuffer* stream);
    Stream_LenType Stream_spaceLimit(StreamBuffer* stream);
    Stream_LenType Stream_getWriteLimit(StreamBuffer* stream);
#endif // STREAM_WRITE_LIMIT

#if STREAM_READ_LIMIT
    void       Stream_setReadLimit(StreamBuffer* stream, Stream_LenType len);
    uint8_t    Stream_isReadLimited(StreamBuffer* stream);
    Stream_LenType Stream_availableLimit(StreamBuffer* stream);
    Stream_LenType Stream_getReadLimit(StreamBuffer* stream);
#endif // STREAM_READ_LIMIT

#if STREAM_CURSOR
    void       Stream_getCursor(StreamBuffer* stream, Stream_Cursor* cursor);
    Stream_LenType Stream_getReadLen(StreamBuffer* stream, Stream_Cursor* cursor);
    Stream_LenType Stream_getWriteLen(StreamBuffer* stream, Stream_Cursor* cursor);
#endif // STREAM_CURSOR

#if STREAM_WRITE_LOCK
    Stream_Result   Stream_lockWrite(StreamBuffer* stream, StreamBuffer* lock, Stream_LenType len);
    void            Stream_unlockWrite(StreamBuffer* stream, StreamBuffer* lock);
    void            Stream_unlockWriteIgnore(StreamBuffer* stream);
    Stream_LenType  Stream_lockWriteLen(StreamBuffer* stream, StreamBuffer* lock);
#endif // STREAM_WRITE_LOCK

#if STREAM_READ_LOCK
    Stream_Result   Stream_lockRead(StreamBuffer* stream, StreamBuffer* lock, Stream_LenType len);
    void            Stream_unlockRead(StreamBuffer* stream, StreamBuffer* lock);
    void            Stream_unlockReadIgnore(StreamBuffer* stream);
    Stream_LenType  Stream_lockReadLen(StreamBuffer* stream, StreamBuffer* lock);
#endif // STREAM_READ_LOCK

/**************** Write APIs **************/
Stream_Result Stream_writeBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_writeBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_writeChar(StreamBuffer* stream, char val);
Stream_Result Stream_writeStr(StreamBuffer* stream, const char* val);
Stream_Result Stream_writeUInt8(StreamBuffer* stream, uint8_t val);
Stream_Result Stream_writeInt8(StreamBuffer* stream, int8_t val);
Stream_Result Stream_writeUInt16(StreamBuffer* stream, uint16_t val);
Stream_Result Stream_writeInt16(StreamBuffer* stream, int16_t val);
Stream_Result Stream_writeUInt32(StreamBuffer* stream, uint32_t val);
Stream_Result Stream_writeInt32(StreamBuffer* stream, int32_t val);
Stream_Result Stream_writeFloat(StreamBuffer* stream, float val);
#if STREAM_UINT64
    Stream_Result Stream_writeUInt64(StreamBuffer* stream, uint64_t val);
    Stream_Result Stream_writeInt64(StreamBuffer* stream, int64_t val);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_writeDouble(StreamBuffer* stream, double val);
#endif // STREAM_DOUBLE

Stream_Result Stream_writeCharArray(StreamBuffer* stream, char* val, Stream_LenType len);
Stream_Result Stream_writeUInt8Array(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_writeInt8Array(StreamBuffer* stream, int8_t* val, Stream_LenType len);
Stream_Result Stream_writeUInt16Array(StreamBuffer* stream, uint16_t* val, Stream_LenType len);
Stream_Result Stream_writeInt16Array(StreamBuffer* stream, int16_t* val, Stream_LenType len);
Stream_Result Stream_writeUInt32Array(StreamBuffer* stream, uint32_t* val, Stream_LenType len);
Stream_Result Stream_writeInt32Array(StreamBuffer* stream, int32_t* val, Stream_LenType len);
Stream_Result Stream_writeFloatArray(StreamBuffer* stream, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_writeUInt64Array(StreamBuffer* stream, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_writeInt64Array(StreamBuffer* stream, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_writeDoubleArray(StreamBuffer* stream, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE

Stream_Result Stream_writeStream(StreamBuffer* out, StreamBuffer* in, Stream_LenType len);

Stream_Result Stream_writePadding(StreamBuffer* stream, uint8_t val, Stream_LenType len);

/**************** Read APIs **************/
int16_t  Stream_read(StreamBuffer* stream);
Stream_Result Stream_readBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_readBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
char     Stream_readChar(StreamBuffer* stream);
uint8_t  Stream_readUInt8(StreamBuffer* stream);
int8_t   Stream_readInt8(StreamBuffer* stream);
uint16_t Stream_readUInt16(StreamBuffer* stream);
int16_t  Stream_readInt16(StreamBuffer* stream);
uint32_t Stream_readUInt32(StreamBuffer* stream);
int32_t  Stream_readInt32(StreamBuffer* stream);
float    Stream_readFloat(StreamBuffer* stream);
#if STREAM_UINT64
    uint64_t Stream_readUInt64(StreamBuffer* stream);
    int64_t  Stream_readInt64(StreamBuffer* stream);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    double   Stream_readDouble(StreamBuffer* stream);
#endif // STREAM_DOUBLE

Stream_Result Stream_readCharArray(StreamBuffer* stream, char* val, Stream_LenType len);
Stream_Result Stream_readUInt8Array(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_readInt8Array(StreamBuffer* stream, int8_t* val, Stream_LenType len);
Stream_Result Stream_readUInt16Array(StreamBuffer* stream, uint16_t* val, Stream_LenType len);
Stream_Result Stream_readInt16Array(StreamBuffer* stream, int16_t* val, Stream_LenType len);
Stream_Result Stream_readUInt32Array(StreamBuffer* stream, uint32_t* val, Stream_LenType len);
Stream_Result Stream_readInt32Array(StreamBuffer* stream, int32_t* val, Stream_LenType len);
Stream_Result Stream_readFloatArray(StreamBuffer* stream, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_readUInt64Array(StreamBuffer* stream, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_readInt64Array(StreamBuffer* stream, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_readDoubleArray(StreamBuffer* stream, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE

Stream_Result Stream_readStream(StreamBuffer* in, StreamBuffer* out, Stream_LenType len);

#if STREAM_GET_AT_FUNCTIONS && STREAM_GET_FUNCTIONS
Stream_Result Stream_getBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
char     Stream_getChar(StreamBuffer* stream);
uint8_t  Stream_getUInt8(StreamBuffer* stream);
int8_t   Stream_getInt8(StreamBuffer* stream);
uint16_t Stream_getUInt16(StreamBuffer* stream);
int16_t  Stream_getInt16(StreamBuffer* stream);
uint32_t Stream_getUInt32(StreamBuffer* stream);
int32_t  Stream_getInt32(StreamBuffer* stream);
float    Stream_getFloat(StreamBuffer* stream);
#if STREAM_UINT64
    uint64_t Stream_getUInt64(StreamBuffer* stream);
    int64_t  Stream_getInt64(StreamBuffer* stream);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    double   Stream_getDouble(StreamBuffer* stream);
#endif // STREAM_DOUBLE

Stream_Result Stream_getCharArray(StreamBuffer* stream, char* val, Stream_LenType len);
Stream_Result Stream_getUInt8Array(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getInt8Array(StreamBuffer* stream, int8_t* val, Stream_LenType len);
Stream_Result Stream_getUInt16Array(StreamBuffer* stream, uint16_t* val, Stream_LenType len);
Stream_Result Stream_getInt16Array(StreamBuffer* stream, int16_t* val, Stream_LenType len);
Stream_Result Stream_getUInt32Array(StreamBuffer* stream, uint32_t* val, Stream_LenType len);
Stream_Result Stream_getInt32Array(StreamBuffer* stream, int32_t* val, Stream_LenType len);
Stream_Result Stream_getFloatArray(StreamBuffer* stream, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_getUInt64Array(StreamBuffer* stream, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_getInt64Array(StreamBuffer* stream, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_getDoubleArray(StreamBuffer* stream, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE

#endif // STREAM_GET_FUNCTIONS

#if STREAM_GET_AT_FUNCTIONS

Stream_Result Stream_getBytesAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getBytesReverseAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
char     Stream_getCharAt(StreamBuffer* stream, Stream_LenType index);
uint8_t  Stream_getUInt8At(StreamBuffer* stream, Stream_LenType index);
int8_t   Stream_getInt8At(StreamBuffer* stream, Stream_LenType index);
uint16_t Stream_getUInt16At(StreamBuffer* stream, Stream_LenType index);
int16_t  Stream_getInt16At(StreamBuffer* stream, Stream_LenType index);
uint32_t Stream_getUInt32At(StreamBuffer* stream, Stream_LenType index);
int32_t  Stream_getInt32At(StreamBuffer* stream, Stream_LenType index);
float    Stream_getFloatAt(StreamBuffer* stream, Stream_LenType index);
#if STREAM_UINT64
    uint64_t Stream_getUInt64At(StreamBuffer* stream, Stream_LenType index);
    int64_t  Stream_getInt64At(StreamBuffer* stream, Stream_LenType index);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    double   Stream_getDoubleAt(StreamBuffer* stream, Stream_LenType index);
#endif // STREAM_DOUBLE

Stream_Result Stream_getCharArrayAt(StreamBuffer* stream, Stream_LenType index, char* val, Stream_LenType len);
Stream_Result Stream_getUInt8ArrayAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
Stream_Result Stream_getInt8ArrayAt(StreamBuffer* stream, Stream_LenType index, int8_t* val, Stream_LenType len);
Stream_Result Stream_getUInt16ArrayAt(StreamBuffer* stream, Stream_LenType index, uint16_t* val, Stream_LenType len);
Stream_Result Stream_getInt16ArrayAt(StreamBuffer* stream, Stream_LenType index, int16_t* val, Stream_LenType len);
Stream_Result Stream_getUInt32ArrayAt(StreamBuffer* stream, Stream_LenType index, uint32_t* val, Stream_LenType len);
Stream_Result Stream_getInt32ArrayAt(StreamBuffer* stream, Stream_LenType index, int32_t* val, Stream_LenType len);
Stream_Result Stream_getFloatArrayAt(StreamBuffer* stream, Stream_LenType index, float* val, Stream_LenType len);
#if STREAM_UINT64
    Stream_Result Stream_getUInt64ArrayAt(StreamBuffer* stream, Stream_LenType index, uint64_t* val, Stream_LenType len);
    Stream_Result Stream_getInt64ArrayAt(StreamBuffer* stream, Stream_LenType index, int64_t* val, Stream_LenType len);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    Stream_Result Stream_getDoubleArrayAt(StreamBuffer* stream, Stream_LenType index, double* val, Stream_LenType len);
#endif // STREAM_DOUBLE

#endif // STREAM_GET_AT_FUNCTION

#if STREAM_FIND_FUNCTIONS
    Stream_LenType Stream_findByte(StreamBuffer* stream, uint8_t val);
    Stream_LenType Stream_findByteAt(StreamBuffer* stream, Stream_LenType offset, uint8_t val);
    Stream_LenType Stream_findPattern(StreamBuffer* stream, const uint8_t* pat, Stream_LenType patLen);
    Stream_LenType Stream_findPatternAt(StreamBuffer* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen);
    Stream_LenType Stream_findUInt8(StreamBuffer* stream, uint8_t val);
    Stream_LenType Stream_findInt8(StreamBuffer* stream, int8_t val);
    Stream_LenType Stream_findUInt16(StreamBuffer* stream, uint16_t val);
    Stream_LenType Stream_findInt16(StreamBuffer* stream, int16_t val);
    Stream_LenType Stream_findUInt32(StreamBuffer* stream, uint32_t val);
    Stream_LenType Stream_findInt32(StreamBuffer* stream, int32_t val);
#if STREAM_UINT64
    Stream_LenType Stream_findUInt64(StreamBuffer* stream, uint64_t val);
    Stream_LenType Stream_findInt64(StreamBuffer* stream, int64_t val);
#endif
    Stream_LenType Stream_findFloat(StreamBuffer* stream, float val);
#if STREAM_DOUBLE
    Stream_LenType Stream_findDouble(StreamBuffer* stream, double val);
#endif

#if STREAM_FIND_AT_FUNCTIONS
    Stream_LenType Stream_findUInt8At(StreamBuffer* stream, Stream_LenType offset, uint8_t val);
    Stream_LenType Stream_findInt8At(StreamBuffer* stream, Stream_LenType offset, int8_t val);
    Stream_LenType Stream_findUInt16At(StreamBuffer* stream, Stream_LenType offset, uint16_t val);
    Stream_LenType Stream_findInt16At(StreamBuffer* stream, Stream_LenType offset, int16_t val);
    Stream_LenType Stream_findUInt32At(StreamBuffer* stream, Stream_LenType offset, uint32_t val);
    Stream_LenType Stream_findInt32At(StreamBuffer* stream, Stream_LenType offset, int32_t val);
#if STREAM_UINT64
    Stream_LenType Stream_findUInt64At(StreamBuffer* stream, Stream_LenType offset, uint64_t val);
    Stream_LenType Stream_findInt64At(StreamBuffer* stream, Stream_LenType offset, int64_t val);
#endif
    Stream_LenType Stream_findFloatAt(StreamBuffer* stream, Stream_LenType offset, float val);
#if STREAM_DOUBLE
    Stream_LenType Stream_findDoubleAt(StreamBuffer* stream, Stream_LenType offset, double val);
#endif
#endif // STREAM_FIND_AT_FUNCTIONS

    Stream_LenType Stream_readBytesUntil(StreamBuffer* stream, uint8_t end, uint8_t* val, Stream_LenType len);
    Stream_LenType Stream_readBytesUntilPattern(StreamBuffer* stream, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len);
    Stream_LenType Stream_readBytesUntilPatternAt(StreamBuffer* stream,  Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len);
#endif // STREAM_FIND_FUNCTIONS

int8_t Stream_compare(StreamBuffer* stream, const uint8_t* val, Stream_LenType len);
int8_t Stream_compareAt(StreamBuffer* stream, Stream_LenType index, const uint8_t* val, Stream_LenType len);

#ifdef __cplusplus
};
#endif

#endif /* _STREAM_H_ */
