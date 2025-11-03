/**
 * @file StreamConfig.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This file contains full configuration for Stream Library
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _STREAM_CONFIG_H_
#define _STREAM_CONFIG_H_

#include <stdint.h>

/* Check User Config file exists to include it or not */
#if STREAM_USER_CONFIG || __has_include("StreamConfigUser.h")
    #include "StreamConfigUser.h"
#endif

/************************************************************************/
/*                             StreamBuffer                             */
/************************************************************************/

// ----------------------------- General  -------------------------------
/**
 * @brief Enable stream value operations
 */
#ifndef STREAM_VALUE
    #define STREAM_VALUE                        1
#endif
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
#ifndef STREAM_CHAR
    #define STREAM_CHAR                         (1 && STREAM_VALUE)
#endif
/**
 * @brief if your platform support 8bit variables and you need it
 * you can enable this option
 */
#ifndef STREAM_UINT8
    #define STREAM_UINT8                        (1 && STREAM_VALUE)
#endif
/**
 * @brief if your platform support 16bit variables and you need it
 * you can enable this option
 */
#ifndef STREAM_UINT16
    #define STREAM_UINT16                       (1 && STREAM_VALUE)
#endif
/**
 * @brief if your platform support 32bit variables and you need it
 * you can enable this option
 */
#ifndef STREAM_UINT32
    #define STREAM_UINT32                       (1 && STREAM_VALUE)
#endif
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
#ifndef STREAM_UINT64
    #define STREAM_UINT64                       (1 && STREAM_VALUE)
#endif
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
#ifndef STREAM_FLOAT
    #define STREAM_FLOAT                        (1 && STREAM_VALUE)
#endif
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
#ifndef STREAM_DOUBLE
    #define STREAM_DOUBLE                       (1 && STREAM_VALUE)
#endif
/**
 * @brief check len parameter in read/write functions
 */
#ifndef STREAM_CHECK_ZERO_LEN
    #define STREAM_CHECK_ZERO_LEN               1
#endif
/**
 * @brief enable user argument in StreamBuffer
 */
#ifndef STREAM_ARGS
    #define STREAM_ARGS                         1
#endif
/**
 * @brief enable driver argument in StreamBuffer
 */
#ifndef STREAM_DRIVER_ARGS
    #define STREAM_DRIVER_ARGS                  1
#endif
/**
 * @brief If you want to use IStream/OStream you must enable this feature
 */
#ifndef STREAM_PENDING_BYTES
    #define STREAM_PENDING_BYTES                1
#endif
/**
 * @brief based on maximum size of buffer that you use for stream
 * you can change type of len variables
 * must be signed type
 */
#ifndef STREAM_LEN_TYPE
    typedef int16_t Stream_LenType;
#endif
// ---------------------------- Write APIs ------------------------------
/**
 * @brief Enable Write operations
 */
#ifndef STREAM_WRITE
    #define STREAM_WRITE                        1
#endif
/**
 * @brief enable write value functions
 */
#ifndef STREAM_WRITE_VALUE
    #define STREAM_WRITE_VALUE                  (1 && STREAM_WRITE && STREAM_VALUE)
#endif
/**
 * @brief enable write reverse functions
 */
#ifndef STREAM_WRITE_REVERSE
    #define STREAM_WRITE_REVERSE                (1 && STREAM_WRITE)
#endif
/**
 * @brief enable write padding functions
 */
#ifndef STREAM_WRITE_PADDING
    #define STREAM_WRITE_PADDING                (1 && STREAM_WRITE)
#endif
/**
 * @brief enable write array apis
 */
#ifndef STREAM_WRITE_ARRAY
    #define STREAM_WRITE_ARRAY                  (1 && STREAM_WRITE)
#endif
/**
 * @brief enable write stream apis
 */
#ifndef STREAM_WRITE_STREAM
    #define STREAM_WRITE_STREAM                 (1 && STREAM_WRITE)
#endif
/**
 * @brief enable flip write feature
 */
#ifndef STREAM_WRITE_FLIP
    #define STREAM_WRITE_FLIP                   (1 && STREAM_WRITE)
#endif
/**
 * @brief enable set limit for write functions
 */
#ifndef STREAM_WRITE_LIMIT
    #define STREAM_WRITE_LIMIT                  (1 && STREAM_WRITE)
#endif
/**
 * @brief enable write lock feature
 */
#ifndef STREAM_WRITE_LOCK
    #define STREAM_WRITE_LOCK                   (1 && STREAM_WRITE)
#endif
/**
 * @brief enable write lock feature for custom data types
 */
#ifndef STREAM_WRITE_LOCK_CUSTOM
    #define STREAM_WRITE_LOCK_CUSTOM            (1 && STREAM_WRITE_LOCK)
#endif

// ---------------------------- Read APIs -------------------------------
/**
 * @brief Enable Read operations
 */
#ifndef STREAM_READ
    #define STREAM_READ                         1
#endif
/**
 * @brief enable read value functions
 */
#ifndef STREAM_READ_VALUE
    #define STREAM_READ_VALUE                   (1 && STREAM_READ && STREAM_VALUE)
#endif
/**
 * @brief enable read value functions
 */
#ifndef STREAM_READ_VALUE_SAFE
    #define STREAM_READ_VALUE_SAFE              (1 && STREAM_READ &&  STREAM_VALUE)
#endif
/**
 * @brief enable read reverse functions
 */
#ifndef STREAM_READ_REVERSE
    #define STREAM_READ_REVERSE                 (1 && STREAM_READ)
#endif
/**
 * @brief enable read array apis
 */
#ifndef STREAM_READ_ARRAY
    #define STREAM_READ_ARRAY                   (1 && STREAM_READ)
#endif
/**
 * @brief enable read stream apis
 */
#ifndef STREAM_READ_STREAM
    #define STREAM_READ_STREAM                  (1 && STREAM_READ)
#endif
/**
 * @brief enable flip read feature
 */
#ifndef STREAM_READ_FLIP
    #define STREAM_READ_FLIP                    (1 && STREAM_READ)
#endif
/**
 * @brief enable set limit for read functions
 */
#ifndef STREAM_READ_LIMIT
    #define STREAM_READ_LIMIT                   (1 && STREAM_READ)
#endif
/**
 * @brief enable read lock feature
 */
#ifndef STREAM_READ_LOCK
    #define STREAM_READ_LOCK                    (1 && STREAM_READ)
#endif
/**
 * @brief enable read lock feature for custom data types
 */
#ifndef STREAM_READ_LOCK_CUSTOM
    #define STREAM_READ_LOCK_CUSTOM             (1 && STREAM_READ_LOCK)
#endif

// ----------------------------- Set APIs -------------------------------
/**
 * @brief enable setAt functions
 */
#ifndef STREAM_SET_AT
    #define STREAM_SET_AT                       (1 && STREAM_WRITE)
#endif
/**
 * @brief enable setBytesReverseAt functions
 */
#ifndef STREAM_SET_AT_BYTES_REVERSE
    #define STREAM_SET_AT_BYTES_REVERSE         (1 && STREAM_SET_AT)
#endif
/**
 * @brief enable setValueAt functions
 */
#ifndef STREAM_SET_AT_VALUE
    #define STREAM_SET_AT_VALUE                 (1 && STREAM_SET_AT && STREAM_VALUE)
#endif
/**
 * @brief enable setArrayAt functions
 */
#ifndef STREAM_SET_AT_ARRAY
    #define STREAM_SET_AT_ARRAY                 (1 && STREAM_SET_AT)
#endif
/**
 * @brief Enable set operations
 */
#ifndef STREAM_SET
    #define STREAM_SET                          (1 && STREAM_SET_AT)
#endif
/**
 * @brief enable setBytesReverse functions
 */
#ifndef STREAM_SET_BYTES_REVERSE
    #define STREAM_SET_BYTES_REVERSE            (1 && STREAM_SET)
#endif
/**
 * @brief enable setValue functions
 */
#ifndef STREAM_SET_VALUE
    #define STREAM_SET_VALUE                    (1 && STREAM_SET && STREAM_VALUE)
#endif
/**
 * @brief enable setArray functions
 */
#ifndef STREAM_SET_ARRAY
    #define STREAM_SET_ARRAY                    (1 && STREAM_SET_AT_ARRAY)
#endif

// ----------------------------- GetAt APIs -------------------------------
/**
 * @brief enable getAt functions
 */
#ifndef STREAM_GET_AT
    #define STREAM_GET_AT                       (1 && STREAM_READ)
#endif
/**
 * @brief enable getBytesReverseAt functions
 */
#ifndef STREAM_GET_AT_BYTES_REVERSE
    #define STREAM_GET_AT_BYTES_REVERSE         (1 && STREAM_GET_AT)
#endif
/**
 * @brief enable getValueAt functions
 */
#ifndef STREAM_GET_AT_VALUE
    #define STREAM_GET_AT_VALUE                 (1 && STREAM_GET_AT && STREAM_VALUE)
#endif
/**
 * @brief enable getValueSafeAt functions
 */
#ifndef STREAM_GET_AT_VALUE_SAFE
    #define STREAM_GET_AT_VALUE_SAFE            (1 && STREAM_GET_AT && STREAM_VALUE)
#endif
/**
 * @brief enable getAtArray functions
 */
#ifndef STREAM_GET_AT_ARRAY
    #define STREAM_GET_AT_ARRAY                 (1 && STREAM_GET_AT)
#endif

// ----------------------------- Get APIs -------------------------------
/**
 * @brief Enable Get operations
 */
#ifndef STREAM_GET
    #define STREAM_GET                          (1 && STREAM_GET_AT)
#endif
/**
 * @brief enable getBytesReverse functions
 */
#ifndef STREAM_GET_BYTES_REVERSE
    #define STREAM_GET_BYTES_REVERSE            (1 && STREAM_GET_AT_BYTES_REVERSE)
#endif
/**
 * @brief enable getValue functions
 */
#ifndef STREAM_GET_VALUE
    #define STREAM_GET_VALUE                    (1 && STREAM_GET_AT_VALUE)
#endif
/**
 * @brief enable getValueSafe functions
 */
#ifndef STREAM_GET_VALUE_SAFE
    #define STREAM_GET_VALUE_SAFE               (1 && STREAM_GET_AT_VALUE_SAFE)
#endif
/**
 * @brief enable getArray functions
 */
#ifndef STREAM_GET_ARRAY
    #define STREAM_GET_ARRAY                    (1 && STREAM_GET_AT_ARRAY)
#endif

// ----------------------------- Find APIs ------------------------------
/**
 * @brief enable findAt functions
 */
#ifndef STREAM_FIND_AT
    #define STREAM_FIND_AT                      (1 && STREAM_READ)
#endif
/**
 * @brief enable findValueAt functions
 */
#ifndef STREAM_FIND_AT_VALUE
    #define STREAM_FIND_AT_VALUE                (1 && STREAM_FIND_AT && STREAM_VALUE)
#endif

// ----------------------------- Find APIs ------------------------------
/**
 * @brief enable find functions
 */
#ifndef STREAM_FIND
    #define STREAM_FIND                         (1 && STREAM_FIND_AT)
#endif

// -------------------------- Read Until APIs ---------------------------
/**
 * @brief enable readBytesUntilAt functions
 */
#ifndef STREAM_READ_UNTIL_AT
    #define STREAM_READ_UNTIL_AT                (1 && STREAM_FIND_AT)
#endif
/**
 * @brief enable readBytesUntil functions
 */
#ifndef STREAM_READ_UNTIL
    #define STREAM_READ_UNTIL                   (1 && STREAM_READ_UNTIL_AT)
#endif

// -------------------------- Transpose APIs ----------------------------
/**
 * @brief enable transpose write functions
 */
#ifndef STREAM_TRANSPOSE_AT
    #define STREAM_TRANSPOSE_AT                 (1 && STREAM_GET_AT && STREAM_SET_AT)
#endif
/**
 * @brief enable transpose write functions
 */
#ifndef STREAM_TRANSPOSE
    #define STREAM_TRANSPOSE                    (1 && STREAM_TRANSPOSE_AT)
#endif


// -------------------------- ByteOrder APIs ----------------------------
/**
 * @brief you can enable ByteOrder option to have r/w operation
 * on what endian you need
 */
#ifndef STREAM_BYTE_ORDER
    #define STREAM_BYTE_ORDER                   (1 && STREAM_WRITE_REVERSE && STREAM_READ_REVERSE)
#endif
/**
 * @brief save system byte order in static variable for avoid calculate each time
 * run Stream_getSystemByteOrder function
 */
#ifndef STREAM_BYTE_ORDER_SYS_STATIC
    #define STREAM_BYTE_ORDER_SYS_STATIC        (1 && STREAM_BYTE_ORDER)
#endif

// --------------------------- Cursor APIs ------------------------------
/**
 * @brief enable cursor object for check how many bytes read or write
 */
#ifndef STREAM_CURSOR
    #define STREAM_CURSOR                       1
#endif

// ---------------------------- MemIO APIs ------------------------------
/* StreamBuffer Memory IO States */
#define STREAM_MEM_IO_DEFAULT                   0
#define STREAM_MEM_IO_CUSTOM                    1
#define STREAM_MEM_IO_DRIVER                    2
#define STREAM_MEM_IO_GLOBAL_DRIVER             3
/**
 * @brief This features help you to override memcpy, memrcpy, memset and etc with your custom functions
 */
#ifndef STREAM_MEM_IO
    #define STREAM_MEM_IO                       STREAM_MEM_IO_DEFAULT
#endif
/* Default IO functions */
#ifndef STREAM_MEM_COPY
    #define STREAM_MEM_COPY                     memcpy
#endif
#ifndef STREAM_MEM_COPY_REVERSE
    #define STREAM_MEM_COPY_REVERSE             memrcpy
#endif
#ifndef STREAM_MEM_SET
    #define STREAM_MEM_SET                      memset
#endif
#ifndef STREAM_MEM_REVERSE
    #define STREAM_MEM_REVERSE                  memreverse
#endif
/**
 * @brief Implement built in mem utils function for `memrcpy`, `memreverse
 */
#ifndef STREAM_MEM_IO_BUILT_IN
    #define STREAM_MEM_IO_BUILT_IN              (1 && (STREAM_MEM_IO == STREAM_MEM_IO_DEFAULT))
#endif

// ---------------------------- Mutex APIs ------------------------------
#define STREAM_MUTEX_NONE                       0
#define STREAM_MUTEX_CUSTOM                     1
#define STREAM_MUTEX_DRIVER                     2
#define STREAM_MUTEX_GLOBAL_DRIVER              3
/**
 * @brief If you want mutex for thread safety
 */
#ifndef STREAM_MUTEX
    #define STREAM_MUTEX                        STREAM_MUTEX_DRIVER
#endif

#if STREAM_MUTEX
    /**
     * @brief Mutex check result for every functions
     */
    #ifndef STREAM_MUTEX_CHECK_RESULT
        #define STREAM_MUTEX_CHECK_RESULT       0
    #endif
    /**
     * @brief Mutex result type
     */
    #ifndef STREAM_MUTEX_RESULT
        typedef int32_t Stream_MutexResult;
    #endif
    /**
     * @brief Stream Mutex object
     */
    #ifndef STREAM_MUTEX_TYPE
        typedef void* Stream_Mutex;
    #endif
    /**
     * @brief mutex function not exists
     */
    #ifndef STREAM_MUTEX_NO_Function
        #define STREAM_MUTEX_NO_Function        ((Stream_MutexResult) -1)
    #endif
#endif

/************************************************************************/
/*                             OutputStream                             */
/************************************************************************/

/**
 * @brief This lable shows StreamOut Library is enabled or not
 */
#ifndef OSTREAM
    #define OSTREAM                     (1 && STREAM_WRITE && STREAM_PENDING_BYTES)
#endif
/**
 * @brief enable checkTransmit function
 */
#ifndef OSTREAM_CHECK_TRANSMIT
    #define OSTREAM_CHECK_TRANSMIT      1
#endif
/**
 * @brief set default flush mode
 * Stream_FlushMode_Single:     flush only pending bytes before call flush function
 * Stream_FlushMode_Continue:   after flush complete if there is any pending bytes transmit pending bytes again
 */
#ifndef OSTREAM_FLUSH_MODE
    #define OSTREAM_FLUSH_MODE          Stream_FlushMode_Continue
#endif
/**
 * @brief enable flush complete callback
 */
#ifndef OSTREAM_FLUSH_CALLBACK
    #define OSTREAM_FLUSH_CALLBACK      1
#endif

/************************************************************************/
/*                              InputStream                             */
/************************************************************************/

/**
 * @brief This lable shows OStream Libraty is enabled or not
 */
#ifndef ISTREAM
    #define ISTREAM                     (1 && STREAM_READ && STREAM_PENDING_BYTES)
#endif
/**
 * @brief enable checkTransmit function
 */
#ifndef ISTREAM_CHECK_RECEIVE
    #define ISTREAM_CHECK_RECEIVE       1
#endif
/**
 * @brief Enable Receive Callback
 */
#ifndef ISTREAM_RECEIVE_CALLBACK
    #define ISTREAM_RECEIVE_CALLBACK    1
#endif
/**
 * @brief Enable Full Callback
 */
#ifndef ISTREAM_FULL_CALLBACK
    #define ISTREAM_FULL_CALLBACK       1
#endif

#endif // _STREAM_CONFIG_H_
