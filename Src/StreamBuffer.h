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
#define STREAM_VER_MINOR    9
#define STREAM_VER_FIX      3

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/
// ----------------------------- General  -------------------------------
/**
 * @brief Enable stream value operations
 */
#define STREAM_VALUE                        1
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
#define STREAM_CHAR                         (1 && STREAM_VALUE)
/**
 * @brief if your platform support 8bit variables and you need it
 * you can enable this option
 */
#define STREAM_UINT8                        (1 && STREAM_VALUE)
/**
 * @brief if your platform support 16bit variables and you need it
 * you can enable this option
 */
#define STREAM_UINT16                       (1 && STREAM_VALUE)
/**
 * @brief if your platform support 32bit variables and you need it
 * you can enable this option
 */
#define STREAM_UINT32                       (1 && STREAM_VALUE)
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
#define STREAM_UINT64                       (1 && STREAM_VALUE)
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
#define STREAM_FLOAT                        (1 && STREAM_VALUE)
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
#define STREAM_DOUBLE                       (1 && STREAM_VALUE)
/**
 * @brief check len parameter in read/write functions
 */
#define STREAM_CHECK_ZERO_LEN               1
/**
 * @brief enable user argument in StreamBuffer
 */
#define STREAM_ARGS                         1
/**
 * @brief If you want to use IStream/OStream you must enable this feature
 */
#define STREAM_PENDING_BYTES                1
/**
 * @brief based on maximum size of buffer that you use for stream
 * you can change type of len variables
 * must be signed type
 */
typedef int16_t Stream_LenType;
// ---------------------------- Write APIs ------------------------------
/**
 * @brief Enable Write operations
 */
#define STREAM_WRITE                        1
/**
 * @brief enable write value functions
 */
#define STREAM_WRITE_VALUE                  (1 && STREAM_WRITE && STREAM_VALUE)
/**
 * @brief enable write reverse functions
 */
#define STREAM_WRITE_REVERSE                (1 && STREAM_WRITE)
/**
 * @brief enable write padding functions
 */
#define STREAM_WRITE_PADDING                (1 && STREAM_WRITE)
/**
 * @brief enable write array apis
 */
#define STREAM_WRITE_ARRAY                  (1 && STREAM_WRITE)
/**
 * @brief enable write stream apis
 */
#define STREAM_WRITE_STREAM                 (1 && STREAM_WRITE)
/**
 * @brief enable flip write feature
 */
#define STREAM_WRITE_FLIP                   (1 && STREAM_WRITE)
/**
 * @brief enable set limit for write functions
 */
#define STREAM_WRITE_LIMIT                  (1 && STREAM_WRITE)
/**
 * @brief enable write lock feature
 */
#define STREAM_WRITE_LOCK                   (1 && STREAM_WRITE)
/**
 * @brief enable write lock feature for custom data types
 */
#define STREAM_WRITE_LOCK_CUSTOM            (1 && STREAM_WRITE_LOCK)

// ---------------------------- Read APIs -------------------------------
/**
 * @brief Enable Read operations
 */
#define STREAM_READ                         1
/**
 * @brief enable read value functions
 */
#define STREAM_READ_VALUE                   (1 && STREAM_READ && STREAM_VALUE)
/**
 * @brief enable read value functions
 */
#define STREAM_READ_VALUE_SAFE              (1 && STREAM_READ &&  STREAM_VALUE)
/**
 * @brief enable read reverse functions
 */
#define STREAM_READ_REVERSE                 (1 && STREAM_READ)
/**
 * @brief enable read array apis
 */
#define STREAM_READ_ARRAY                   (1 && STREAM_READ)
/**
 * @brief enable read stream apis
 */
#define STREAM_READ_STREAM                  (1 && STREAM_READ)
/**
 * @brief enable flip read feature
 */
#define STREAM_READ_FLIP                    (1 && STREAM_READ)
/**
 * @brief enable set limit for read functions
 */
#define STREAM_READ_LIMIT                   (1 && STREAM_READ)
/**
 * @brief enable read lock feature
 */
#define STREAM_READ_LOCK                    (1 && STREAM_READ)
/**
 * @brief enable read lock feature for custom data types
 */
#define STREAM_READ_LOCK_CUSTOM             (1 && STREAM_READ_LOCK)

// ----------------------------- Set APIs -------------------------------
/**
 * @brief enable setAt functions
 */
#define STREAM_SET_AT                       (1 && STREAM_WRITE)
/**
 * @brief enable setBytesReverseAt functions
 */
#define STREAM_SET_AT_BYTES_REVERSE         (1 && STREAM_SET_AT)
/**
 * @brief enable setValueAt functions
 */
#define STREAM_SET_AT_VALUE                 (1 && STREAM_SET_AT && STREAM_VALUE)
/**
 * @brief enable setArrayAt functions
 */
#define STREAM_SET_AT_ARRAY                 (1 && STREAM_SET_AT)
/**
 * @brief Enable set operations
 */
#define STREAM_SET                          (1 && STREAM_SET_AT)
/**
 * @brief enable setBytesReverse functions
 */
#define STREAM_SET_BYTES_REVERSE            (1 && STREAM_SET)
/**
 * @brief enable setValue functions
 */
#define STREAM_SET_VALUE                    (1 && STREAM_SET && STREAM_VALUE)
/**
 * @brief enable setArray functions
 */
#define STREAM_SET_ARRAY                    (1 && STREAM_SET_AT_ARRAY)

// ----------------------------- GetAt APIs -------------------------------
/**
 * @brief enable getAt functions
 */
#define STREAM_GET_AT                       (1 && STREAM_READ)
/**
 * @brief enable getBytesReverseAt functions
 */
#define STREAM_GET_AT_BYTES_REVERSE         (1 && STREAM_GET_AT)
/**
 * @brief enable getValueAt functions
 */
#define STREAM_GET_AT_VALUE                 (1 && STREAM_GET_AT && STREAM_VALUE)
/**
 * @brief enable getValueSafeAt functions
 */
#define STREAM_GET_AT_VALUE_SAFE            (1 && STREAM_GET_AT && STREAM_VALUE)
/**
 * @brief enable getAtArray functions
 */
#define STREAM_GET_AT_ARRAY                 (1 && STREAM_GET_AT)

// ----------------------------- Get APIs -------------------------------
/**
 * @brief Enable Get operations
 */
#define STREAM_GET                          (1 && STREAM_GET_AT)
/**
 * @brief enable getBytesReverse functions
 */
#define STREAM_GET_BYTES_REVERSE            (1 && STREAM_GET_AT_BYTES_REVERSE)
/**
 * @brief enable getValue functions
 */
#define STREAM_GET_VALUE                    (1 && STREAM_GET_AT_VALUE)
/**
 * @brief enable getValueSafe functions
 */
#define STREAM_GET_VALUE_SAFE               (1 && STREAM_GET_AT_VALUE_SAFE)
/**
 * @brief enable getArray functions
 */
#define STREAM_GET_ARRAY                    (1 && STREAM_GET_AT_ARRAY)

// ----------------------------- Find APIs ------------------------------
/**
 * @brief enable findAt functions
 */
#define STREAM_FIND_AT                      (1 && STREAM_READ)
/**
 * @brief enable findValueAt functions
 */
#define STREAM_FIND_AT_VALUE                (1 && STREAM_FIND_AT && STREAM_VALUE)

// ----------------------------- Find APIs ------------------------------
/**
 * @brief enable find functions
 */
#define STREAM_FIND                         (1 && STREAM_FIND_AT)

// -------------------------- Read Until APIs ---------------------------
/**
 * @brief enable readBytesUntilAt functions
 */
#define STREAM_READ_UNTIL_AT                (1 && STREAM_FIND_AT)
/**
 * @brief enable readBytesUntil functions
 */
#define STREAM_READ_UNTIL                   (1 && STREAM_READ_UNTIL_AT)

// -------------------------- Transpose APIs ----------------------------
/**
 * @brief enable transpose write functions
 */
#define STREAM_TRANSPOSE_AT                 (1 && STREAM_GET_AT && STREAM_SET_AT)
/**
 * @brief enable transpose write functions
 */
#define STREAM_TRANSPOSE                    (1 && STREAM_TRANSPOSE_AT)


// -------------------------- ByteOrder APIs ----------------------------
/**
 * @brief you can enable ByteOrder option to have r/w operation
 * on what endian you need
 */
#define STREAM_BYTE_ORDER                   (1 && STREAM_WRITE_REVERSE && STREAM_READ_REVERSE)
/**
 * @brief save system byte order in static variable for avoid calculate each time
 * run Stream_getSystemByteOrder function
 */
#define STREAM_BYTE_ORDER_SYS_STATIC        (1 && STREAM_BYTE_ORDER)

// --------------------------- Cursor APIs ------------------------------
/**
 * @brief enable cursor object for check how many bytes read or write
 */
#define STREAM_CURSOR                       1

// ---------------------------- MemIO APIs ------------------------------
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
 * @brief Implement built in mem utils function for `memrcpy`, `memreverse
 */
#define STREAM_MEM_IO_BUILT_IN              (1 && (STREAM_MEM_IO == STREAM_MEM_IO_DEFAULT))

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
 * @brief Allow you to implement custom transpose function for read/write
 * @param stream stream buffer
 * @param buf buffer that need to transpose
 * @param len length of buffer
 * @return Stream_Result result of transpose operation
 */
typedef Stream_Result (*Stream_TransposeFn)(void* args, uint8_t* buf, Stream_LenType len);
/**
 * @brief Custom memcpy for override default function
 * @param dest destination buffer
 * @param src source buffer
 * @param len length of buffer
 * @return Stream_LenType number of copied bytes
 */
typedef Stream_LenType (*Stream_MemCopyFn)(void* dest, const void* src, Stream_LenType len);
/**
 * @brief Custom memrcpy for override default function
 * @param dest destination buffer
 * @param src source buffer
 * @param len length of buffer
 * @return Stream_LenType number of copied bytes
 */
typedef Stream_LenType (*Stream_MemCopyReverseFn)(void* dest, const void* src, Stream_LenType len);
/**
 * @brief Custom memset for override default function
 * @param src source buffer
 * @param val value for set
 * @param len length of buffer
 * @return Stream_LenType number of set bytes
 */
typedef Stream_LenType (*Stream_MemSetFn)(void* src, uint8_t val, Stream_LenType len);
/**
 * @brief Custom memset for override default function
 * @param src source buffer
 * @param len length of buffer
 * @return Stream_LenType number of set bytes
 */
typedef Stream_LenType (*Stream_MemReverseFn)(void* src, Stream_LenType len);

typedef struct {
    Stream_MemCopyFn              copy;
    Stream_MemCopyReverseFn       copyReverse;
    Stream_MemSetFn               set;
    Stream_MemReverseFn           reverse;
} Stream_MemIO;
/**
 * @brief Stream Value container
 */
typedef union {
    void*       Raw;
#if STREAM_CHAR
    char        Char;
#endif
#if STREAM_UINT8
    uint8_t     UInt8;
    int8_t      Int8;
#endif
#if STREAM_UINT16
    uint16_t    UInt16;
    int16_t     Int16;
#endif
#if STREAM_UINT32
    uint32_t    UInt32;
    int32_t     Int32;
#endif
#if STREAM_UINT64
    uint64_t    UInt64;
    int64_t     Int64;
#endif
#if STREAM_FLOAT
    float       Float;
#endif
#if STREAM_DOUBLE
    double      Double;
#endif
} Stream_Value;
/**
 * @brief StreamBuffer struct
 * contains everything need for handle stream
 */
typedef struct {
#if STREAM_ARGS
    void*                   Args;
#endif
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
#if STREAM_PENDING_BYTES
    Stream_LenType          PendingBytes;           /**< hold pending bytes for receive or transmit */
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

// -------------------------- General APIs ----------------------------
void Stream_init(StreamBuffer* stream, uint8_t* buffer, Stream_LenType size);
void Stream_fromBuff(StreamBuffer* stream, uint8_t* buffer, Stream_LenType size, Stream_LenType len);
void Stream_deinit(StreamBuffer* stream);

#if STREAM_MEM_IO == STREAM_MEM_IO_CUSTOM
void Stream_setMemIO(
    StreamBuffer*               stream,
    Stream_MemCopyFn            copy,
    Stream_MemCopyReverseFn     copyReverse,
    Stream_MemSetFn             set,
    Stream_MemReverseFn         reverse
);
#elif STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
void Stream_setMemIO(StreamBuffer* stream, const Stream_MemIO* mem);
#elif STREAM_MEM_IO_BUILT_IN
void memrcpy(void* dest, const void* src, int len);
void memreverse(void* arr, int len);
#endif


#if STREAM_WRITE_LIMIT
    #define         Stream_space(STREAM)                                    Stream_spaceLimit((STREAM))
#else
    #define         Stream_space(STREAM)                                    Stream_spaceReal((STREAM))
#endif // STREAM_WRITE_LIMIT

#if STREAM_READ_LIMIT
    #define         Stream_available(STREAM)                                Stream_availableLimit((STREAM))
#else
    #define         Stream_available(STREAM)                                Stream_availableReal((STREAM))
#endif // STREAM_READ_LIMIT

Stream_LenType      Stream_availableReal(StreamBuffer* stream);
Stream_LenType      Stream_spaceReal(StreamBuffer* stream);
uint8_t             Stream_isEmpty(StreamBuffer* stream);
uint8_t             Stream_isFull(StreamBuffer* stream);

Stream_LenType      Stream_directAvailable(StreamBuffer* stream);
Stream_LenType      Stream_directSpace(StreamBuffer* stream);

Stream_LenType      Stream_directAvailableAt(StreamBuffer* stream, Stream_LenType index);
Stream_LenType      Stream_directSpaceAt(StreamBuffer* stream, Stream_LenType index);

uint8_t*            Stream_getWritePtr(StreamBuffer* stream);
uint8_t*            Stream_getReadPtr(StreamBuffer* stream);
uint8_t*            Stream_getWritePtrAt(StreamBuffer* stream, Stream_LenType index);
uint8_t*            Stream_getReadPtrAt(StreamBuffer* stream, Stream_LenType index);

void                Stream_reset(StreamBuffer* stream);
void                Stream_resetIO(StreamBuffer* stream);
void                Stream_clear(StreamBuffer* stream);

void                Stream_setFlushMode(StreamBuffer* stream, Stream_FlushMode mode);

uint8_t             Stream_inReceive(StreamBuffer* stream);
uint8_t             Stream_inTransmit(StreamBuffer* stream);

uint8_t*            Stream_getBuffer(StreamBuffer* stream);

void                Stream_setBuffer(StreamBuffer* stream, uint8_t* data, Stream_LenType size);
Stream_LenType      Stream_getBufferSize(StreamBuffer* stream);

Stream_LenType      Stream_getWritePos(StreamBuffer* stream);
Stream_LenType      Stream_getReadPos(StreamBuffer* stream);

Stream_Result       Stream_moveWritePos(StreamBuffer* stream, Stream_LenType steps);
Stream_Result       Stream_moveReadPos(StreamBuffer* stream, Stream_LenType steps);

#if STREAM_WRITE_FLIP
    void            Stream_flipWrite(StreamBuffer* stream, Stream_LenType len);
#endif // STREAM_WRITE_FLIP
#if STREAM_READ_FLIP
    void            Stream_flipRead(StreamBuffer* stream, Stream_LenType len);
#endif // STREAM_READ_FLIP

#if STREAM_BYTE_ORDER
    ByteOrder       Stream_getSystemByteOrder(void);
    void            Stream_setByteOrder(StreamBuffer* stream, ByteOrder order);
    ByteOrder       Stream_getByteOrder(StreamBuffer* stream);
#endif // STREAM_BYTE_ORDER

#if STREAM_WRITE_LIMIT
    void            Stream_setWriteLimit(StreamBuffer* stream, Stream_LenType len);
    uint8_t         Stream_isWriteLimited(StreamBuffer* stream);
    Stream_LenType  Stream_spaceLimit(StreamBuffer* stream);
    Stream_LenType  Stream_getWriteLimit(StreamBuffer* stream);
#endif // STREAM_WRITE_LIMIT

#if STREAM_READ_LIMIT
    void            Stream_setReadLimit(StreamBuffer* stream, Stream_LenType len);
    uint8_t         Stream_isReadLimited(StreamBuffer* stream);
    Stream_LenType  Stream_availableLimit(StreamBuffer* stream);
    Stream_LenType  Stream_getReadLimit(StreamBuffer* stream);
#endif // STREAM_READ_LIMIT

#if STREAM_CURSOR
    void            Stream_getCursor(StreamBuffer* stream, Stream_Cursor* cursor);
    Stream_LenType  Stream_getReadLen(StreamBuffer* stream, Stream_Cursor* cursor);
    Stream_LenType  Stream_getWriteLen(StreamBuffer* stream, Stream_Cursor* cursor);
#endif // STREAM_CURSOR

#if STREAM_WRITE_LOCK
    Stream_Result   Stream_lockWrite(StreamBuffer* stream, StreamBuffer* lock, Stream_LenType len);
    void            Stream_unlockWrite(StreamBuffer* stream, StreamBuffer* lock);
    void            Stream_unlockWriteIgnore(StreamBuffer* stream);
    Stream_LenType  Stream_lockWriteLen(StreamBuffer* stream, StreamBuffer* lock);
#if STREAM_WRITE_LOCK_CUSTOM
    Stream_Result   Stream_lockWriteCustom(void* stream, void* lock, Stream_LenType len, Stream_LenType datatypeLen);
#endif // STREAM_WRITE_LOCK_CUSTOM
#endif // STREAM_WRITE_LOCK

#if STREAM_READ_LOCK
    Stream_Result   Stream_lockRead(StreamBuffer* stream, StreamBuffer* lock, Stream_LenType len);
    void            Stream_unlockRead(StreamBuffer* stream, StreamBuffer* lock);
    void            Stream_unlockReadIgnore(StreamBuffer* stream);
    Stream_LenType  Stream_lockReadLen(StreamBuffer* stream, StreamBuffer* lock);
#if STREAM_READ_LOCK_CUSTOM
    Stream_Result   Stream_lockReadCustom(void* stream, void* lock, Stream_LenType len, Stream_LenType datatypeLen);
#endif // STREAM_READ_LOCK_CUSTOM
#endif // STREAM_READ_LOCK

#if STREAM_ARGS
    void            Stream_setArgs(StreamBuffer* stream, void* args);
    void*           Stream_getArgs(StreamBuffer* stream);
#endif // STREAM_ARGS

#if STREAM_PENDING_BYTES
    void            Stream_setPendingBytes(StreamBuffer* stream, Stream_LenType len);
    Stream_LenType  Stream_getPendingBytes(StreamBuffer* stream);
#endif

/* ------------------------------------ General Write APIs ---------------------------------- */
#if STREAM_WRITE
Stream_Result       Stream_writeBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
#if STREAM_WRITE_REVERSE
    Stream_Result   Stream_writeBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
    Stream_Result   Stream_write(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
#else
    #define         Stream_write(STREAM, VAL, LEN)                          Stream_writeBytes((STREAM), (VAL), (LEN))
#endif
#if STREAM_WRITE_STREAM
    Stream_Result   Stream_writeStream(StreamBuffer* out, StreamBuffer* in, Stream_LenType len);
#endif
#if STREAM_WRITE_PADDING
    Stream_Result   Stream_writePadding(StreamBuffer* stream, uint8_t val, Stream_LenType len);
#endif
#endif // STREAM_WRITE
/* ------------------------------------ Write Value APIs ---------------------------------- */
#if STREAM_WRITE_VALUE
#define             Stream_writeStr(STREAM, VAL)                            Stream_writeBytes((STREAM), (uint8_t*) VAL, strlen(VAL))
#define             Stream_writeValue(STREAM, VAL, LEN)                     Stream_write((STREAM), (uint8_t*) &(Stream_Value) { VAL }, LEN)
#if STREAM_CHAR
    #define         Stream_writeChar(STREAM, VAL)                           Stream_writeBytes((STREAM), (uint8_t*) &(char) { VAL }, sizeof(char))
#endif
#if STREAM_UINT8
    #define         Stream_writeUInt8(STREAM, VAL)                          Stream_writeBytes((STREAM), (uint8_t*) &(uint8_t) { VAL }, sizeof(uint8_t))
    #define         Stream_writeInt8(STREAM, VAL)                           Stream_writeBytes((STREAM), (uint8_t*) &(int8_t) { VAL }, sizeof(int8_t))
#endif
#if STREAM_UINT16
    #define         Stream_writeUInt16(STREAM, VAL)                         Stream_write((STREAM), (uint8_t*) &(uint16_t) { VAL }, sizeof(uint16_t))
    #define         Stream_writeInt16(STREAM, VAL)                          Stream_write((STREAM), (uint8_t*) &(int16_t) { VAL }, sizeof(int16_t))
#endif
#if STREAM_UINT32
    #define         Stream_writeUInt32(STREAM, VAL)                         Stream_write((STREAM), (uint8_t*) &(uint32_t) { VAL }, sizeof(uint32_t))
    #define         Stream_writeInt32(STREAM, VAL)                          Stream_write((STREAM), (uint8_t*) &(int32_t) { VAL }, sizeof(int32_t))
#endif
#if STREAM_UINT64
    #define         Stream_writeUInt64(STREAM, VAL)                         Stream_write((STREAM), (uint8_t*) &(uint64_t) { VAL }, sizeof(uint64_t))
    #define         Stream_writeInt64(STREAM, VAL)                          Stream_write((STREAM), (uint8_t*) &(int64_t) { VAL }, sizeof(int64_t))
#endif
#if STREAM_FLOAT
    #define         Stream_writeFloat(STREAM, VAL)                          Stream_write((STREAM), (uint8_t*) &(float) { VAL }, sizeof(float))
#endif
#if STREAM_DOUBLE
    #define         Stream_writeDouble(STREAM, VAL)                         Stream_write((STREAM), (uint8_t*) &(double) { VAL }, sizeof(double))
#endif
#endif // STREAM_WRITE_VALUE
/* ------------------------------------ Write Value Array APIs ---------------------------------- */
#if STREAM_WRITE_ARRAY
Stream_Result       Stream_writeArray(StreamBuffer* stream, void* val, Stream_LenType itemLen, Stream_LenType len);
#if STREAM_WRITE_VALUE
    #define         Stream_writeValueArray(STREAM, VAL, LEN)                Stream_writeArray((STREAM), (VAL), sizeof(Stream_Value), LEN)
#if STREAM_CHAR
    #define         Stream_writeCharArray(STREAM, VAL, LEN)                 Stream_writeBytes((STREAM), (uint8_t*) (VAL), LEN)
#endif
#if STREAM_UINT8
    #define         Stream_writeUInt8Array(STREAM, VAL, LEN)                Stream_writeBytes((STREAM), (uint8_t*) (VAL), LEN)
    #define         Stream_writeInt8Array(STREAM, VAL, LEN)                 Stream_writeBytes((STREAM), (uint8_t*) (VAL), LEN)
#endif
#if STREAM_UINT16
    #define         Stream_writeUInt16Array(STREAM, VAL, LEN)               Stream_writeArray((STREAM), (VAL), sizeof(uint16_t), LEN)
    #define         Stream_writeInt16Array(STREAM, VAL, LEN)                Stream_writeArray((STREAM), (VAL), sizeof(int16_t), LEN)
#endif
#if STREAM_UINT32
    #define         Stream_writeUInt32Array(STREAM, VAL, LEN)               Stream_writeArray((STREAM), (VAL), sizeof(uint32_t), LEN)
    #define         Stream_writeInt32Array(STREAM, VAL, LEN)                Stream_writeArray((STREAM), (VAL), sizeof(int32_t), LEN)
#endif
#if STREAM_UINT64
    #define         Stream_writeUInt64Array(STREAM, VAL, LEN)               Stream_writeArray((STREAM), (VAL), sizeof(uint64_t), LEN)
    #define         Stream_writeInt64Array(STREAM, VAL, LEN)                Stream_writeArray((STREAM), (VAL), sizeof(int64_t), LEN)
#endif
#if STREAM_FLOAT
    #define         Stream_writeFloatArray(STREAM, VAL, LEN)                Stream_writeArray((STREAM), (VAL), sizeof(float), LEN)
#endif
#if STREAM_DOUBLE
    #define         Stream_writeDoubleArray(STREAM, VAL, LEN)               Stream_writeArray((STREAM), (VAL), sizeof(double), LEN)
#endif
#endif // STREAM_WRITE_VALUE
#endif // STREAM_WRITE_ARRAY
/* --------------------------------------- Set API------------------------------------------ */
#if STREAM_SET_AT
Stream_Result       Stream_setBytesAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
#if STREAM_SET_AT_BYTES_REVERSE
    Stream_Result   Stream_setBytesReverseAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
    Stream_Result   Stream_setAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
#else
    #define         Stream_getAt(STREAM, IDX, VAL, LEN)                     Stream_setBytesAt((STREAM), (IDX), (VAL), (LEN))
#endif
#endif // STREAM_SET_AT
/* ------------------------------------ SetAt Value APIs ---------------------------------- */
#if STREAM_SET_AT_VALUE
#define             Stream_setValueAt(STREAM, IDX, VAL)                     Stream_setAt((STREAM), (IDX), (uint8_t*) &(Stream_Value) { VAL }, sizeof(Stream_Value))
#if STREAM_CHAR
    #define         Stream_setCharAt(STREAM, IDX, VAL)                      Stream_setBytesAt((STREAM), (IDX), (uint8_t*) &(char) { VAL }, sizeof(char))
#endif
#if STREAM_UINT8
    #define         Stream_setUInt8At(STREAM, IDX, VAL)                     Stream_setBytesAt((STREAM), (IDX), (uint8_t*) &(uint8_t) { VAL }, sizeof(uint8_t))
    #define         Stream_setInt8At(STREAM, IDX, VAL)                      Stream_setBytesAt((STREAM), (IDX), (uint8_t*) &(int8_t) { VAL }, sizeof(int8_t))
#endif
#if STREAM_UINT16
    #define         Stream_setUInt16At(STREAM, IDX, VAL)                    Stream_setAt((STREAM), (IDX), (uint8_t*) &(uint16_t) { VAL }, sizeof(uint16_t))
    #define         Stream_setInt16At(STREAM, IDX, VAL)                     Stream_setAt((STREAM), (IDX), (uint8_t*) &(int16_t) { VAL }, sizeof(int16_t))
#endif
#if STREAM_UINT32
    #define         Stream_setUInt32At(STREAM, IDX, VAL)                    Stream_setAt((STREAM), (IDX), (uint8_t*) &(uint32_t) { VAL }, sizeof(uint32_t))
    #define         Stream_setInt32At(STREAM, IDX, VAL)                     Stream_setAt((STREAM), (IDX), (uint8_t*) &(int32_t) { VAL }, sizeof(int32_t))
#endif
#if STREAM_UINT64
    #define         Stream_setUInt64At(STREAM, IDX, VAL)                    Stream_setAt((STREAM), (IDX), (uint8_t*) &(uint64_t) { VAL }, sizeof(uint64_t))
    #define         Stream_setInt64At(STREAM, IDX, VAL)                     Stream_setAt((STREAM), (IDX), (uint8_t*) &(int64_t) { VAL }, sizeof(int64_t))
#endif
#if STREAM_FLOAT
    #define         Stream_setFloatAt(STREAM, IDX, VAL)                     Stream_setAt((STREAM), (IDX), (uint8_t*) &(float) { VAL }, sizeof(float))
#endif
#if STREAM_DOUBLE
    #define         Stream_setDoubleAt(STREAM, IDX, VAL)                    Stream_setAt((STREAM), (IDX), (uint8_t*) &(double) { VAL }, sizeof(double))
#endif
#endif // STREAM_SET_AT_VALUE
/* ------------------------------------ SetAt Value Array APIs ---------------------------------- */
#if STREAM_SET_AT_ARRAY
Stream_Result       Stream_setArrayAt(StreamBuffer* stream, Stream_LenType index, void* val, Stream_LenType itemLen, Stream_LenType len);
#if STREAM_SET_AT_VALUE
    #define         Stream_setValueArrayAt(STREAM, IDX, VAL, LEN)           Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(Stream_Value), (LEN))
#if STREAM_CHAR
    #define         Stream_setCharArrayAt(STREAM, IDX, VAL, LEN)            Stream_setBytesAt((STREAM), (IDX), (uint8_t*) (VAL), (LEN))
#endif
#if STREAM_UINT8
    #define         Stream_setUInt8ArrayAt(STREAM, IDX, VAL, LEN)           Stream_setBytesAt((STREAM), (IDX), (uint8_t*) (VAL), (LEN))
    #define         Stream_setInt8ArrayAt(STREAM, IDX, VAL, LEN)            Stream_setBytesAt((STREAM), (IDX), (uint8_t*) (VAL), (LEN))
#endif
#if STREAM_UINT16
    #define         Stream_setUInt16ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(uint16_t), (LEN))
    #define         Stream_setInt16ArrayAt(STREAM, IDX, VAL, LEN)           Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(int16_t), (LEN))
#endif
#if STREAM_UINT32
    #define         Stream_setUInt32ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(uint32_t), (LEN))
    #define         Stream_setInt32ArrayAt(STREAM, IDX, VAL, LEN)           Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(int32_t), (LEN))
#endif
#if STREAM_UINT64
    #define         Stream_setUInt64ArrayAt(STREAM, IDX, VAL, LEN)          Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(uint64_t), (LEN))
    #define         Stream_setInt64ArrayAt(STREAM, IDX, VAL, LEN)           Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(int64_t), (LEN))
#endif
#if STREAM_FLOAT
    #define         Stream_setFloatArrayAt(STREAM, IDX, VAL, LEN)           Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(float), (LEN))
#endif
#if STREAM_DOUBLE
    #define         Stream_setDoubleArrayAt(STREAM, IDX, VAL, LEN)          Stream_setArrayAt((STREAM), (IDX), (VAL), sizeof(double), (LEN))
#endif
#endif // STREAM_SET_AT_VALUE
#endif // STREAM_SET_AT_ARRAY
/* ------------------------------------ General Set APIs ---------------------------------- */
#if STREAM_SET_VALUE
#define             Stream_setBytes(STREAM, VAL, LEN)                       Stream_setBytesAt((STREAM), 0, (VAL), (LEN))
#define             Stream_setBytes(STREAM, VAL, LEN)                       Stream_setBytesAt((STREAM), 0, (VAL), (LEN))
#if STREAM_SET_BYTES_REVERSE
    #define         Stream_setBytesReverse(STREAM, VAL, LEN)                Stream_setBytesReverseAt((STREAM), 0, (VAL), (LEN))
    #define         Stream_set(STREAM, VAL, LEN)                            Stream_setAt((STREAM), 0, (VAL), (LEN))
#else
    #define         Stream_set(STREAM, VAL, LEN)                            Stream_setBytesAt((STREAM), 0, (VAL), (LEN))
#endif
#endif // STREAM_SET_VALUE
/* ------------------------------------ Set Value Safe APIs ---------------------------------- */
#if STREAM_SET_VALUE
    #define         Stream_setValue(STREAM, LEN, IDX)                       Stream_setValueAt((STREAM), (LEN), (IDX))
#if STREAM_CHAR
    #define         Stream_setChar(STREAM, VAL)                             Stream_setCharAt((STREAM), 0, VAL)
#endif
#if STREAM_UINT8
    #define         Stream_setUInt8(STREAM, VAL)                            Stream_setUInt8At((STREAM), 0, VAL)
    #define         Stream_setInt8(STREAM, VAL)                             Stream_setInt8At((STREAM), 0, VAL)
#endif
#if STREAM_UINT16
    #define         Stream_setUInt16(STREAM, VAL)                           Stream_setUInt16At((STREAM), 0, VAL)
    #define         Stream_setInt16(STREAM, VAL)                            Stream_setInt16At((STREAM), 0, VAL)
#endif
#if STREAM_UINT32
    #define         Stream_setUInt32(STREAM, VAL)                           Stream_setUInt32At((STREAM), 0, VAL)
    #define         Stream_setInt32(STREAM, VAL)                            Stream_setInt32At((STREAM), 0, VAL)
#endif
#if STREAM_UINT64
    #define         Stream_setUInt64(STREAM, VAL)                           Stream_setUInt64At((STREAM), 0, VAL)
    #define         Stream_setInt64(STREAM, VAL)                            Stream_setInt64At((STREAM), 0, VAL)
#endif
#if STREAM_FLOAT
    #define         Stream_setFloat(STREAM, VAL)                            Stream_setFloatAt((STREAM), 0, VAL)
#endif
#if STREAM_DOUBLE
    #define         Stream_setDouble(STREAM, VAL)                           Stream_setDoubleAt((STREAM), 0, VAL)
#endif
#endif // STREAM_SET_VALUE
/* ------------------------------------ Set Value Array APIs ---------------------------------- */
#if STREAM_SET_AT_ARRAY
#define             Stream_setArray(STREAM, VAL, ILEN, LEN)                 Stream_setArrayAt((STREAM), 0, (LEN), (ILEN), (LEN))
#if STREAM_SET_AT_VALUE
    #define         Stream_setValueArray(STREAM, IDX, VAL, LEN)             Stream_setArrayAt((STREAM), 0, (VAL), sizeof(Stream_Value), (LEN))
#if STREAM_CHAR
    #define         Stream_setCharArray(STREAM, IDX, VAL, LEN)              Stream_setBytesAt((STREAM), 0, (uint8_t*) (VAL), (LEN))
#endif
#if STREAM_UINT8
    #define         Stream_setUInt8Array(STREAM, IDX, VAL, LEN)             Stream_setBytesAt((STREAM), 0, (uint8_t*) (VAL), (LEN))
    #define         Stream_setInt8Array(STREAM, IDX, VAL, LEN)              Stream_setBytesAt((STREAM), 0, (uint8_t*) (VAL), (LEN))
#endif
#if STREAM_UINT16
    #define         Stream_setUInt16Array(STREAM, IDX, VAL, LEN)            Stream_setArrayAt((STREAM), 0, (VAL), sizeof(uint16_t), (LEN))
    #define         Stream_setInt16Array(STREAM, IDX, VAL, LEN)             Stream_setArrayAt((STREAM), 0, (VAL), sizeof(int16_t), (LEN))
#endif
#if STREAM_UINT32
    #define         Stream_setUInt32Array(STREAM, IDX, VAL, LEN)            Stream_setArrayAt((STREAM), 0, (VAL), sizeof(uint32_t), (LEN))
    #define         Stream_setInt32Array(STREAM, IDX, VAL, LEN)             Stream_setArrayAt((STREAM), 0, (VAL), sizeof(int32_t), (LEN))
#endif
#if STREAM_UINT64
    #define         Stream_setUInt64Array(STREAM, IDX, VAL, LEN)            Stream_setArrayAt((STREAM), 0, (VAL), sizeof(uint64_t), (LEN))
    #define         Stream_setInt64Array(STREAM, IDX, VAL, LEN)             Stream_setArrayAt((STREAM), 0, (VAL), sizeof(int64_t), (LEN))
#endif
#if STREAM_FLOAT
    #define         Stream_setFloatArray(STREAM, IDX, VAL, LEN)             Stream_setArrayAt((STREAM), 0, (VAL), sizeof(float), (LEN))
#endif
#if STREAM_DOUBLE
    #define         Stream_setDoubleArray(STREAM, IDX, VAL, LEN)            Stream_setArrayAt((STREAM), 0, (VAL), sizeof(double), (LEN))
#endif
#endif // STREAM_SET_VALUE
#endif // STREAM_SET_ARRAY
/* ------------------------------------ General Read APIs ---------------------------------- */
#if STREAM_READ
Stream_Result       Stream_readBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
#if STREAM_READ_REVERSE
    Stream_Result   Stream_readBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
    Stream_Result   Stream_read(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
#else
    #define         Stream_read(STREAM, VAL, LEN)                           Stream_readBytes((STREAM), (VAL), (LEN))
#endif
#if STREAM_READ_STREAM
    Stream_Result   Stream_readStream(StreamBuffer* in, StreamBuffer* out, Stream_LenType len);
#endif
#endif // STREAM_READ
/* ------------------------------------ Read Value APIs ---------------------------------- */
#if STREAM_READ_VALUE
Stream_Value        Stream_readValue(StreamBuffer* stream, Stream_LenType len);
#if STREAM_CHAR
    #define         Stream_readChar(STREAM)                                 Stream_readValue((STREAM), sizeof(char)).Char
#endif
#if STREAM_UINT8
    #define         Stream_readUInt8(STREAM)                                Stream_readValue((STREAM), sizeof(uint8_t)).UInt8
    #define         Stream_readInt8(STREAM)                                 Stream_readValue((STREAM), sizeof(int8_t)).Int8
#endif
#if STREAM_UINT16
    #define         Stream_readUInt16(STREAM)                               Stream_readValue((STREAM), sizeof(uint16_t)).UInt16
    #define         Stream_readInt16(STREAM)                                Stream_readValue((STREAM), sizeof(int16_t)).Int16
#endif
#if STREAM_UINT32
    #define         Stream_readUInt32(STREAM)                               Stream_readValue((STREAM), sizeof(uint32_t)).UInt32
    #define         Stream_readInt32(STREAM)                                Stream_readValue((STREAM), sizeof(int32_t)).Int32
#endif
#if STREAM_UINT64
    #define         Stream_readUInt64(STREAM)                               Stream_readValue((STREAM), sizeof(uint64_t)).UInt64
    #define         Stream_readInt64(STREAM)                                Stream_readValue((STREAM), sizeof(int64_t)).Int64
#endif
#if STREAM_FLOAT
    #define         Stream_readFloat(STREAM)                                Stream_readValue((STREAM), sizeof(float)).Float
#endif
#if STREAM_DOUBLE
    #define         Stream_readDouble(STREAM)                               Stream_readValue((STREAM), sizeof(double)).Double
#endif
#endif // STREAM_READ_VALUE
/* ------------------------------------ Read Value Safe APIs ---------------------------------- */
#if STREAM_READ_VALUE_SAFE
    #define         Stream_readValueSafe(STREAM, VAL, LEN)                  Stream_readValue((STREAM), (uint8_t*) &VAL, LEN)
#if STREAM_CHAR
    #define         Stream_readCharSafe(STREAM, VAL)                        Stream_readBytes((STREAM), (uint8_t*) &VAL, sizeof(char))
#endif
#if STREAM_UINT8
    #define         Stream_readUInt8Safe(STREAM, VAL)                       Stream_readBytes((STREAM), (uint8_t*) &VAL, sizeof(uint8_t))
    #define         Stream_readInt8Safe(STREAM, VAL)                        Stream_readBytes((STREAM), (uint8_t*) &VAL, sizeof(int8_t))
#endif
#if STREAM_UINT16
    #define         Stream_readUInt16Safe(STREAM, VAL)                      Stream_read((STREAM), (uint8_t*) &VAL, sizeof(uint16_t))
    #define         Stream_readInt16Safe(STREAM, VAL)                       Stream_read((STREAM), (uint8_t*) &VAL, sizeof(int16_t))
#endif
#if STREAM_UINT32
    #define         Stream_readUInt32Safe(STREAM, VAL)                      Stream_read((STREAM), (uint8_t*) &VAL, sizeof(uint32_t))
    #define         Stream_readInt32Safe(STREAM, VAL)                       Stream_read((STREAM), (uint8_t*) &VAL, sizeof(int32_t))
#endif
#if STREAM_UINT64
    #define         Stream_readUInt64Safe(STREAM, VAL)                      Stream_read((STREAM), (uint8_t*) &VAL, sizeof(uint64_t))
    #define         Stream_readInt64Safe(STREAM, VAL)                       Stream_read((STREAM), (uint8_t*) &VAL, sizeof(int64_t))
#endif
#if STREAM_FLOAT
    #define         Stream_readFloatSafe(STREAM, VAL)                       Stream_read((STREAM), (uint8_t*) &VAL, sizeof(float))
#endif
#if STREAM_DOUBLE
    #define         Stream_readDoubleSafe(STREAM, VAL)                      Stream_read((STREAM), (uint8_t*) &VAL, sizeof(double))
#endif
#endif // STREAM_READ_VALUE_SAFE
/* ------------------------------------ Read Value Array APIs ---------------------------------- */
#if STREAM_READ_ARRAY
Stream_Result       Stream_readArray(StreamBuffer* stream, void* val, Stream_LenType itemLen, Stream_LenType len);
#if STREAM_READ_VALUE
    #define         Stream_readValueArray(STREAM, VAL, LEN)                 Stream_readArray((STREAM), VAL, sizeof(Stream_Value), LEN)
#if STREAM_CHAR
    #define         Stream_readCharArray(STREAM, VAL, LEN)                  Stream_readBytes((STREAM), (uint8_t*) VAL, LEN)
#endif
#if STREAM_UINT8
    #define         Stream_readUInt8Array(STREAM, VAL, LEN)                 Stream_readBytes((STREAM), (uint8_t*) VAL, LEN)
    #define         Stream_readInt8Array(STREAM, VAL, LEN)                  Stream_readBytes((STREAM), (uint8_t*) VAL, LEN)
#endif
#if STREAM_UINT16
    #define         Stream_readUInt16Array(STREAM, VAL, LEN)                Stream_readArray((STREAM), VAL, sizeof(uint16_t), LEN)
    #define         Stream_readInt16Array(STREAM, VAL, LEN)                 Stream_readArray((STREAM), VAL, sizeof(int16_t), LEN)
#endif
#if STREAM_UINT32
    #define         Stream_readUInt32Array(STREAM, VAL, LEN)                Stream_readArray((STREAM), VAL, sizeof(uint32_t), LEN)
    #define         Stream_readInt32Array(STREAM, VAL, LEN)                 Stream_readArray((STREAM), VAL, sizeof(int32_t), LEN)
#endif
#if STREAM_UINT64
    #define         Stream_readUInt64Array(STREAM, VAL, LEN)                Stream_readArray((STREAM), VAL, sizeof(uint64_t), LEN)
    #define         Stream_readInt64Array(STREAM, VAL, LEN)                 Stream_readArray((STREAM), VAL, sizeof(int64_t), LEN)
#endif
#if STREAM_DOUBLE
    #define         Stream_readDoubleArray(STREAM, VAL, LEN)                Stream_readArray((STREAM), VAL, sizeof(double), LEN)
#endif
#if STREAM_FLOAT
    #define         Stream_readFloatArray(STREAM, VAL, LEN)                 Stream_readArray((STREAM), VAL, sizeof(float), LEN)
#endif
#endif // STREAM_READ_VALUE
#endif // STREAM_READ_ARRAY
/* ------------------------------------ General GetAt APIs ---------------------------------- */
#if STREAM_GET_AT
Stream_Result       Stream_getBytesAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
#if STREAM_GET_AT_BYTES_REVERSE
    Stream_Result   Stream_getBytesReverseAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
    Stream_Result   Stream_getAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
#else
    #define         Stream_getAt(STREAM, IDX, VAL, LEN)                     Stream_getBytesAt((STREAM), (IDX), (VAL), (LEN))
#endif
#endif // STREAM_GET_AT
/* ------------------------------------ GetAt Value APIs ---------------------------------- */
#if STREAM_GET_AT_VALUE
Stream_Value        Stream_getValueAt(StreamBuffer* stream, Stream_LenType index, Stream_LenType len);
#if STREAM_CHAR
    #define         Stream_getCharAt(STREAM, IDX)                           Stream_getValueAt((STREAM), (IDX), sizeof(char)).Char
#endif
#if STREAM_UINT8
    #define         Stream_getUInt8At(STREAM, IDX)                          Stream_getValueAt((STREAM), (IDX), sizeof(uint8_t)).UInt8
    #define         Stream_getInt8At(STREAM, IDX)                           Stream_getValueAt((STREAM), (IDX), sizeof(int8_t)).Int8
#endif
#if STREAM_UINT16
    #define         Stream_getUInt16At(STREAM, IDX)                         Stream_getValueAt((STREAM), (IDX), sizeof(uint16_t)).UInt16
    #define         Stream_getInt16At(STREAM, IDX)                          Stream_getValueAt((STREAM), (IDX), sizeof(int16_t)).Int16
#endif
#if STREAM_UINT32
    #define         Stream_getUInt32At(STREAM, IDX)                         Stream_getValueAt((STREAM), (IDX), sizeof(uint32_t)).UInt32
    #define         Stream_getInt32At(STREAM, IDX)                          Stream_getValueAt((STREAM), (IDX), sizeof(int32_t)).Int32
#endif
#if STREAM_UINT64
    #define         Stream_getUInt64At(STREAM, IDX)                         Stream_getValueAt((STREAM), (IDX), sizeof(uint64_t)).UInt64
    #define         Stream_getInt64At(STREAM, IDX)                          Stream_getValueAt((STREAM), (IDX), sizeof(int64_t)).Int64
#endif
#if STREAM_FLOAT
    #define         Stream_getFloatAt(STREAM, IDX)                          Stream_getValueAt((STREAM), (IDX), sizeof(float)).Float
#endif
#if STREAM_DOUBLE
    #define         Stream_getDoubleAt(STREAM, IDX)                         Stream_getValueAt((STREAM), (IDX), sizeof(double)).Double 
#endif
#endif // STREAM_GET_AT_VALUE
/* ------------------------------------ GetAt Value Safe APIs ---------------------------------- */
#if STREAM_GET_AT_VALUE_SAFE
    #define         Stream_getValueAtSafe(STREAM, IDX, VAL)                 Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#if STREAM_CHAR
    #define         Stream_getCharAtSafe(STREAM, IDX, VAL)                  Stream_getBytesAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#if STREAM_UINT8
    #define         Stream_getUInt8AtSafe(STREAM, IDX, VAL)                 Stream_getBytesAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
    #define         Stream_getInt8AtSafe(STREAM, IDX, VAL)                  Stream_getBytesAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#if STREAM_UINT16
    #define         Stream_getUInt16AtSafe(STREAM, IDX, VAL)                Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
    #define         Stream_getInt16AtSafe(STREAM, IDX, VAL)                 Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#if STREAM_UINT32
    #define         Stream_getUInt32AtSafe(STREAM, IDX, VAL)                Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
    #define         Stream_getInt32AtSafe(STREAM, IDX, VAL)                 Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#if STREAM_UINT64
    #define         Stream_getUInt64AtSafe(STREAM, IDX, VAL)                Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
    #define         Stream_getInt64AtSafe(STREAM, IDX, VAL)                 Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#if STREAM_FLOAT
    #define         Stream_getFloatAtSafe(STREAM, IDX, VAL)                 Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#if STREAM_DOUBLE
    #define         Stream_getDoubleAtSafe(STREAM, IDX, VAL)                Stream_getAt((STREAM), (IDX), (uint8_t*) &VAL, sizeof(VAL))
#endif
#endif // STREAM_GET_AT_VALUE_SAFE
/* ------------------------------------ GetAt Value Array APIs ---------------------------------- */
#if STREAM_GET_AT_ARRAY
Stream_Result       Stream_getArrayAt(StreamBuffer* stream, Stream_LenType index, void* val, Stream_LenType itemLen, Stream_LenType len);
#if STREAM_GET_AT_VALUE
    #define         Stream_getValueArrayAt(STREAM, IDX, VAL, LEN)           Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(Stream_Value), (LEN))
#if STREAM_CHAR
    #define         Stream_getCharArrayAt(STREAM, IDX, VAL, LEN)            Stream_getBytesAt((STREAM), (IDX), (uint8_t*) (VAL), (LEN))
#endif
#if STREAM_UINT8
    #define         Stream_getUInt8ArrayAt(STREAM, IDX, VAL, LEN)           Stream_getBytesAt((STREAM), (IDX), (uint8_t*) (VAL), (LEN))
    #define         Stream_getInt8ArrayAt(STREAM, IDX, VAL, LEN)            Stream_getBytesAt((STREAM), (IDX), (uint8_t*) (VAL), (LEN))
#endif
#if STREAM_UINT16
    #define         Stream_getUInt16ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(uint16_t), (LEN))
    #define         Stream_getInt16ArrayAt(STREAM, IDX, VAL, LEN)           Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(int16_t), (LEN))
#endif
#if STREAM_UINT32
    #define         Stream_getUInt32ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(uint32_t), (LEN))
    #define         Stream_getInt32ArrayAt(STREAM, IDX, VAL, LEN)           Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(int32_t), (LEN))
#endif
#if STREAM_UINT64
    #define         Stream_getUInt64ArrayAt(STREAM, IDX, VAL, LEN)          Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(uint64_t), (LEN))
    #define         Stream_getInt64ArrayAt(STREAM, IDX, VAL, LEN)           Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(int64_t), (LEN))
#endif
#if STREAM_FLOAT
    #define         Stream_getFloatArrayAt(STREAM, IDX, VAL, LEN)           Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(float), (LEN))
#endif
#if STREAM_DOUBLE
    #define         Stream_getDoubleArrayAt(STREAM, IDX, VAL, LEN)          Stream_getArrayAt((STREAM), (IDX), (VAL), sizeof(double), (LEN))
#endif
#endif // STREAM_GET_AT_VALUE
#endif // STREAM_GET_AT_ARRAY
/* ------------------------------------ General Get APIs ---------------------------------- */
#if STREAM_GET
#define             Stream_getBytes(STREAM, VAL, LEN)                       Stream_getBytesAt((STREAM), 0, (VAL), (LEN))
#if STREAM_GET_BYTES_REVERSE
    #define         Stream_getBytesReverse(STREAM, VAL, LEN)                Stream_getBytesReverseAt((STREAM), 0, (VAL), (LEN))
    #define         Stream_get(STREAM, VAL, LEN)                            Stream_getAt((STREAM), 0, (VAL), (LEN))
#else
    #define         Stream_get(STREAM, VAL, LEN)                            Stream_getBytesAt((STREAM), 0, (VAL), (LEN))
#endif
#endif // STREAM_GET
/* ------------------------------------ Get Value APIs ---------------------------------- */
#if STREAM_GET_VALUE
    #define         Stream_getValue(STREAM)                                 Stream_getValueAt((STREAM), 0)
#if STREAM_CHAR
    #define         Stream_getChar(STREAM)                                  Stream_getCharAt((STREAM), 0)
#endif
#if STREAM_UINT8
    #define         Stream_getUInt8(STREAM)                                 Stream_getUInt8At((STREAM), 0)
    #define         Stream_getInt8(STREAM)                                  Stream_getInt8At((STREAM), 0)
#endif
#if STREAM_UINT16
    #define         Stream_getUInt16(STREAM)                                Stream_getUInt16At((STREAM), 0)
    #define         Stream_getInt16(STREAM)                                 Stream_getInt16At((STREAM), 0)
#endif
#if STREAM_UINT32
    #define         Stream_getUInt32(STREAM)                                Stream_getUInt32At((STREAM), 0)
    #define         Stream_getInt32(STREAM)                                 Stream_getInt32At((STREAM), 0)
#endif
#if STREAM_UINT64
    #define         Stream_getUInt64(STREAM)                                Stream_getUInt64At((STREAM), 0)
    #define         Stream_getInt64(STREAM)                                 Stream_getInt64At((STREAM), 0)
#endif
#if STREAM_FLOAT
    #define         Stream_getFloat(STREAM)                                 Stream_getFloatAt((STREAM), 0)
#endif
#if STREAM_DOUBLE
    #define         Stream_getDouble(STREAM)                                Stream_getDoubleAt((STREAM), 0)
#endif
#endif // STREAM_GET_VALUE
/* ------------------------------------ Get Value Safe APIs ---------------------------------- */
#if STREAM_GET_VALUE_SAFE
    #define         Stream_getValueSafe(STREAM, VAL, LEN)                   Stream_getValueAtSafe((STREAM), 0, (VAL), (LEN))
#if STREAM_CHAR
    #define         Stream_getCharSafe(STREAM, VAL)                         Stream_getCharAtSafe((STREAM), 0, (VAL))
#endif
#if STREAM_UINT8
    #define         Stream_getUInt8Safe(STREAM, VAL)                        Stream_getUInt8AtSafe((STREAM), 0, (VAL))
    #define         Stream_getInt8Safe(STREAM, VAL)                         Stream_getInt8AtSafe((STREAM), 0, (VAL))
#endif
#if STREAM_UINT16
    #define         Stream_getUInt16Safe(STREAM, VAL)                       Stream_getUInt16AtSafe((STREAM), 0, (VAL))
    #define         Stream_getInt16Safe(STREAM, VAL)                        Stream_getInt16AtSafe((STREAM), 0, (VAL))
#endif
#if STREAM_UINT32
    #define         Stream_getUInt32Safe(STREAM, VAL)                       Stream_getUInt32AtSafe((STREAM), 0, (VAL))
    #define         Stream_getInt32Safe(STREAM, VAL)                        Stream_getInt32AtSafe((STREAM), 0, (VAL))
#endif
#if STREAM_UINT64
    #define         Stream_getUInt64Safe(STREAM, VAL)                       Stream_getUInt64AtSafe((STREAM), 0, VAL)
    #define         Stream_getInt64Safe(STREAM, VAL)                        Stream_getInt64AtSafe((STREAM), 0, VAL)
#endif
#if STREAM_FLOAT
    #define         Stream_getFloatSafe(STREAM, VAL)                        Stream_getFloatAtSafe((STREAM), 0, (VAL))
#endif
#if STREAM_DOUBLE
    #define         Stream_getDoubleSafe(STREAM, VAL)                       Stream_getDoubleAtSafe((STREAM), 0, VAL)
#endif
#endif // STREAM_GET_VALUE_SAFE
/* ------------------------------------ Get Value Array APIs ---------------------------------- */
#if STREAM_GET_ARRAY
#define             Stream_getArray(STREAM, VAL, ITEM_LEN, LEN)             Stream_getArrayAt((STREAM), 0, (VAL), (ITEM_LEN), (LEN))
#if STREAM_GET_VALUE
    #define         Stream_getValueArray(STREAM, VAL, LEN)                  Stream_getValueArrayAt((STREAM), 0, (VAL), (LEN))
#if STREAM_CHAR
    #define         Stream_getCharArray(STREAM, VAL, LEN)                   Stream_getCharArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#if STREAM_UINT8
    #define         Stream_getUInt8Array(STREAM, VAL, LEN)                  Stream_getUInt8ArrayAt((STREAM), 0, (VAL), (LEN))
    #define         Stream_getInt8Array(STREAM, VAL, LEN)                   Stream_getInt8ArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#if STREAM_UINT16
    #define         Stream_getUInt16Array(STREAM, VAL, LEN)                 Stream_getUInt16ArrayAt((STREAM), 0, (VAL), (LEN))
    #define         Stream_getInt16Array(STREAM, VAL, LEN)                  Stream_getInt16ArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#if STREAM_UINT32
    #define         Stream_getUInt32Array(STREAM, VAL, LEN)                 Stream_getUInt32ArrayAt((STREAM), 0, (VAL), (LEN))
    #define         Stream_getInt32Array(STREAM, VAL, LEN)                  Stream_getInt32ArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#if STREAM_UINT64
    #define         Stream_getUInt64Array(STREAM, VAL, LEN)                 Stream_getUInt64ArrayAt((STREAM), 0, (VAL), (LEN))
    #define         Stream_getInt64Array(STREAM, VAL, LEN)                  Stream_getInt64ArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#if STREAM_FLOAT
    #define         Stream_getFloatArray(STREAM, VAL, LEN)                  Stream_getFloatArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#if STREAM_DOUBLE
    #define         Stream_getDoubleArray(STREAM, VAL, LEN)                 Stream_getDoubleArrayAt((STREAM), 0, (VAL), (LEN))
#endif
#endif // STREAM_GET_VALUE
#endif // STREAM_GET
/* ------------------------------------ General Find At APIs ---------------------------------- */
#if STREAM_FIND_AT
Stream_LenType      Stream_findByteAt(StreamBuffer* stream, Stream_LenType offset, uint8_t val);
Stream_LenType      Stream_findPatternAt(StreamBuffer* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen);
#if STREAM_FIND_AT_VALUE
Stream_LenType      Stream_findValueAt(StreamBuffer* stream, Stream_LenType offset, Stream_Value val, Stream_LenType len);
#if STREAM_CHAR
    #define         Stream_findCharAt(STREAM, IDX, VAL)                     Stream_findByteAt((STREAM), IDX, (VAL))
#endif
#if STREAM_UINT8
    #define         Stream_findUInt8At(STREAM, IDX, VAL)                    Stream_findByteAt((STREAM), IDX, (VAL))
    #define         Stream_findInt8At(STREAM, IDX, VAL)                     Stream_findByteAt((STREAM), IDX, (VAL))
#endif
#if STREAM_UINT16
    #define         Stream_findUInt16At(STREAM, IDX, VAL)                   Stream_findValueAt((STREAM), IDX, (Stream_Value) { .UInt16 = (VAL) }, sizeof(uint16_t))
    #define         Stream_findInt16At(STREAM, IDX, VAL)                    Stream_findValueAt((STREAM), IDX, (Stream_Value) { .Int16  = (VAL) }, sizeof(int16_t))
#endif
#if STREAM_UINT32
    #define         Stream_findUInt32At(STREAM, IDX, VAL)                   Stream_findValueAt((STREAM), IDX, (Stream_Value) { .UInt32 = (VAL) }, sizeof(uint32_t))
    #define         Stream_findInt32At(STREAM, IDX, VAL)                    Stream_findValueAt((STREAM), IDX, (Stream_Value) { .Int32  = (VAL) }, sizeof(int32_t))
#endif
#if STREAM_UINT64
    #define         Stream_findUInt64At(STREAM, IDX, VAL)                   Stream_findValueAt((STREAM), IDX, (Stream_Value) { .UInt64 = (VAL) }, sizeof(uint64_t))
    #define         Stream_findInt64At(STREAM, IDX, VAL)                    Stream_findValueAt((STREAM), IDX, (Stream_Value) { .Int64  = (VAL) }, sizeof(int64_t))
#endif
#if STREAM_FLOAT
    #define         Stream_findFloatAt(STREAM, IDX, VAL)                    Stream_findValueAt((STREAM), IDX, (Stream_Value) { .Float  = (VAL) }, sizeof(float))
#endif
#if STREAM_DOUBLE
    #define         Stream_findDoubleAt(STREAM, IDX, VAL)                   Stream_findValueAt((STREAM), IDX, (Stream_Value) { .Double = (VAL) }, sizeof(double))
#endif
#endif // STREAM_FIND_AT_VALUE
#endif // STREAM_FIND_AT
/* ------------------------------------ General Find APIs ---------------------------------- */
#if STREAM_FIND
#define             Stream_findByte(STREAM, VAL)                            Stream_findByteAt((STREAM), 0, (VAL))
#define             Stream_findPattern(STREAM, PAT, PAT_LEN)                Stream_findPatternAt((STREAM), 0, (PAT), (PAT_LEN))
#if STREAM_FIND_AT_VALUE
    #define         Stream_findValue(STREAM, VAL, LEN)                      Stream_findValueAt((STREAM), 0, (VAL), (LEN))
#if STREAM_CHAR
    #define         Stream_findChar(STREAM, VAL)                            Stream_findCharAt((STREAM), 0, (VAL))
#endif
#if STREAM_UINT8
    #define         Stream_findUInt8(STREAM, VAL)                           Stream_findUInt8At((STREAM), 0, (VAL))
    #define         Stream_findInt8(STREAM, VAL)                            Stream_findInt8At((STREAM), 0, (VAL))
#endif
#if STREAM_UINT16
    #define         Stream_findUInt16(STREAM, VAL)                          Stream_findUInt16At((STREAM), 0, (VAL))
    #define         Stream_findInt16(STREAM, VAL)                           Stream_findInt16At((STREAM), 0, (VAL))
#endif
#if STREAM_UINT32
    #define         Stream_findUInt32(STREAM, VAL)                          Stream_findUInt32At((STREAM), 0, (VAL))
    #define         Stream_findInt32(STREAM, VAL)                           Stream_findInt32At((STREAM), 0, (VAL))
#endif
#if STREAM_UINT64
    #define         Stream_findUInt64(STREAM, VAL)                          Stream_findUInt64At((STREAM), 0, (VAL))
    #define         Stream_findInt64(STREAM, VAL)                           Stream_findInt64At((STREAM), 0, (VAL))
#endif
#if STREAM_FLOAT
    #define         Stream_findFloat(STREAM, VAL)                           Stream_findFloatAt((STREAM), 0, (VAL))
#endif
#if STREAM_DOUBLE
    #define         Stream_findDouble(STREAM, VAL)                          Stream_findDoubleAt((STREAM), 0, (VAL))
#endif
#endif // STREAM_FIND_AT_VALUE
#endif // STREAM_FIND_AT
/* ------------------------------------ Read Bytes Until At APIs ---------------------------------- */
#if STREAM_READ_UNTIL_AT
Stream_LenType      Stream_readBytesUntilAt(StreamBuffer* stream, Stream_LenType offset, uint8_t end, uint8_t* val, Stream_LenType len);
Stream_LenType      Stream_readBytesUntilPatternAt(StreamBuffer* stream,  Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len);
#endif // STREAM_READ_UNTIL_AT
/* ------------------------------------ Read Bytes Until APIs ---------------------------------- */
#if STREAM_READ_UNTIL
#define             Stream_readBytesUntil(STREAM, END, VAL, LEN)            Stream_readBytesUntilAt((STREAM), 0, (END), (VAL), (LEN))
#define             Stream_readBytesUntilPattern(STREAM, P, PLEN, VAL, LEN) Stream_readBytesUntilPatternAt((STREAM), 0, (P), (PLEN), (VAL), (LEN))
#endif // STREAM_READ_UNTIL
/* ------------------------------------ Transpose APIs ---------------------------------- */
#if STREAM_TRANSPOSE_AT
    Stream_Result   Stream_transposeAt(StreamBuffer* stream, Stream_LenType offset, Stream_LenType len, uint8_t* tmpBuf, Stream_LenType chunkLen, Stream_TransposeFn transpose, void* args);
#endif
#if STREAM_TRANSPOSE
    #define         Stream_transpose(STREAM, LEN, BUF, CLEN, TP, ARGS)      Stream_transposeAt((STREAM), 0, (LEN), (BUF), (CLEN), (TP), (ARGS))
#endif
/* ------------------------------------ Compare APIs ---------------------------------- */
int8_t              Stream_compareAt(StreamBuffer* stream, Stream_LenType index, const uint8_t* val, Stream_LenType len);
#define             Stream_compare(STREAM, VAL, LEN)                        Stream_compareAt((STREAM), 0, (VAL), (LEN))

#ifdef __cplusplus
};
#endif

#endif /* _STREAM_H_ */
