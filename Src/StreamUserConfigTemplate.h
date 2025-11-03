/**
 * @file StreamUserConfig.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gamil.com)
 * @brief This is configuration template for users to use
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _STREAM_USER_CONFIG_H_
#define _STREAM_USER_CONFIG_H_


/************************************************************************/
/*                             StreamBuffer                             */
/************************************************************************/

// ----------------------------- General  -------------------------------
/**
 * @brief Enable stream value operations
 */
//#define STREAM_VALUE                        1
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
//#define STREAM_CHAR                         (1 && STREAM_VALUE)
/**
 * @brief if your platform support 8bit variables and you need it
 * you can enable this option
 */
//#define STREAM_UINT8                        (1 && STREAM_VALUE)
/**
 * @brief if your platform support 16bit variables and you need it
 * you can enable this option
 */
//#define STREAM_UINT16                       (1 && STREAM_VALUE)
/**
 * @brief if your platform support 32bit variables and you need it
 * you can enable this option
 */
//#define STREAM_UINT32                       (1 && STREAM_VALUE)
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
//#define STREAM_UINT64                       (1 && STREAM_VALUE)
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
//#define STREAM_FLOAT                        (1 && STREAM_VALUE)
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
//#define STREAM_DOUBLE                       (1 && STREAM_VALUE)
/**
 * @brief check len parameter in read/write functions
 */
//#define STREAM_CHECK_ZERO_LEN               1
/**
 * @brief enable user argument in StreamBuffer
 */
//#define STREAM_ARGS                         1
/**
 * @brief enable driver argument in StreamBuffer
 */
//#define STREAM_DRIVER_ARGS                  1
/**
 * @brief If you want to use IStream/OStream you must enable this feature
 */
//#define STREAM_PENDING_BYTES                1
/**
 * @brief based on maximum size of buffer that you use for stream
 * you can change type of len variables
 * must be signed type
 */
//#define STREAM_LEN_TYPE
//typedef int16_t Stream_LenType;
// ---------------------------- Write APIs ------------------------------
/**
 * @brief Enable Write operations
 */
//#define STREAM_WRITE                        1
/**
 * @brief enable write value functions
 */
//#define STREAM_WRITE_VALUE                  (1 && STREAM_WRITE && STREAM_VALUE)
/**
 * @brief enable write reverse functions
 */
//#define STREAM_WRITE_REVERSE                (1 && STREAM_WRITE)
/**
 * @brief enable write padding functions
 */
//#define STREAM_WRITE_PADDING                (1 && STREAM_WRITE)
/**
 * @brief enable write array apis
 */
//#define STREAM_WRITE_ARRAY                  (1 && STREAM_WRITE)
/**
 * @brief enable write stream apis
 */
//#define STREAM_WRITE_STREAM                 (1 && STREAM_WRITE)
/**
 * @brief enable flip write feature
 */
//#define STREAM_WRITE_FLIP                   (1 && STREAM_WRITE)
/**
 * @brief enable set limit for write functions
 */
//#define STREAM_WRITE_LIMIT                  (1 && STREAM_WRITE)
/**
 * @brief enable write lock feature
 */
//#define STREAM_WRITE_LOCK                   (1 && STREAM_WRITE)
/**
 * @brief enable write lock feature for custom data types
 */
//#define STREAM_WRITE_LOCK_CUSTOM            (1 && STREAM_WRITE_LOCK)

// ---------------------------- Read APIs -------------------------------
/**
 * @brief Enable Read operations
 */
//#define STREAM_READ                         1
/**
 * @brief enable read value functions
 */
//#define STREAM_READ_VALUE                   (1 && STREAM_READ && STREAM_VALUE)
/**
 * @brief enable read value functions
 */
//#define STREAM_READ_VALUE_SAFE              (1 && STREAM_READ &&  STREAM_VALUE)
/**
 * @brief enable read reverse functions
 */
//#define STREAM_READ_REVERSE                 (1 && STREAM_READ)
/**
 * @brief enable read array apis
 */
//#define STREAM_READ_ARRAY                   (1 && STREAM_READ)
/**
 * @brief enable read stream apis
 */
//#define STREAM_READ_STREAM                  (1 && STREAM_READ)
/**
 * @brief enable flip read feature
 */
//#define STREAM_READ_FLIP                    (1 && STREAM_READ)
/**
 * @brief enable set limit for read functions
 */
//#define STREAM_READ_LIMIT                   (1 && STREAM_READ)
/**
 * @brief enable read lock feature
 */
//#define STREAM_READ_LOCK                    (1 && STREAM_READ)
/**
 * @brief enable read lock feature for custom data types
 */
//#define STREAM_READ_LOCK_CUSTOM             (1 && STREAM_READ_LOCK)

// ----------------------------- Set APIs -------------------------------
/**
 * @brief enable setAt functions
 */
//#define STREAM_SET_AT                       (1 && STREAM_WRITE)
/**
 * @brief enable setBytesReverseAt functions
 */
//#define STREAM_SET_AT_BYTES_REVERSE         (1 && STREAM_SET_AT)
/**
 * @brief enable setValueAt functions
 */
//#define STREAM_SET_AT_VALUE                 (1 && STREAM_SET_AT && STREAM_VALUE)
/**
 * @brief enable setArrayAt functions
 */
//#define STREAM_SET_AT_ARRAY                 (1 && STREAM_SET_AT)
/**
 * @brief Enable set operations
 */
//#define STREAM_SET                          (1 && STREAM_SET_AT)
/**
 * @brief enable setBytesReverse functions
 */
//#define STREAM_SET_BYTES_REVERSE            (1 && STREAM_SET)
/**
 * @brief enable setValue functions
 */
//#define STREAM_SET_VALUE                    (1 && STREAM_SET && STREAM_VALUE)
/**
 * @brief enable setArray functions
 */
//#define STREAM_SET_ARRAY                    (1 && STREAM_SET_AT_ARRAY)

// ----------------------------- GetAt APIs -------------------------------
/**
 * @brief enable getAt functions
 */
//#define STREAM_GET_AT                       (1 && STREAM_READ)
/**
 * @brief enable getBytesReverseAt functions
 */
//#define STREAM_GET_AT_BYTES_REVERSE         (1 && STREAM_GET_AT)
/**
 * @brief enable getValueAt functions
 */
//#define STREAM_GET_AT_VALUE                 (1 && STREAM_GET_AT && STREAM_VALUE)
/**
 * @brief enable getValueSafeAt functions
 */
//#define STREAM_GET_AT_VALUE_SAFE            (1 && STREAM_GET_AT && STREAM_VALUE)
/**
 * @brief enable getAtArray functions
 */
//#define STREAM_GET_AT_ARRAY                 (1 && STREAM_GET_AT)

// ----------------------------- Get APIs -------------------------------
/**
 * @brief Enable Get operations
 */
//#define STREAM_GET                          (1 && STREAM_GET_AT)
/**
 * @brief enable getBytesReverse functions
 */
//#define STREAM_GET_BYTES_REVERSE            (1 && STREAM_GET_AT_BYTES_REVERSE)
/**
 * @brief enable getValue functions
 */
//#define STREAM_GET_VALUE                    (1 && STREAM_GET_AT_VALUE)
/**
 * @brief enable getValueSafe functions
 */
//#define STREAM_GET_VALUE_SAFE               (1 && STREAM_GET_AT_VALUE_SAFE)
/**
 * @brief enable getArray functions
 */
//#define STREAM_GET_ARRAY                    (1 && STREAM_GET_AT_ARRAY)

// ----------------------------- Find APIs ------------------------------
/**
 * @brief enable findAt functions
 */
//#define STREAM_FIND_AT                      (1 && STREAM_READ)
/**
 * @brief enable findValueAt functions
 */
//#define STREAM_FIND_AT_VALUE                (1 && STREAM_FIND_AT && STREAM_VALUE)

// ----------------------------- Find APIs ------------------------------
/**
 * @brief enable find functions
 */
//#define STREAM_FIND                         (1 && STREAM_FIND_AT)

// -------------------------- Read Until APIs ---------------------------
/**
 * @brief enable readBytesUntilAt functions
 */
//#define STREAM_READ_UNTIL_AT                (1 && STREAM_FIND_AT)
/**
 * @brief enable readBytesUntil functions
 */
//#define STREAM_READ_UNTIL                   (1 && STREAM_READ_UNTIL_AT)

// -------------------------- Transpose APIs ----------------------------
/**
 * @brief enable transpose write functions
 */
//#define STREAM_TRANSPOSE_AT                 (1 && STREAM_GET_AT && STREAM_SET_AT)
/**
 * @brief enable transpose write functions
 */
//#define STREAM_TRANSPOSE                    (1 && STREAM_TRANSPOSE_AT)


// -------------------------- ByteOrder APIs ----------------------------
/**
 * @brief you can enable ByteOrder option to have r/w operation
 * on what endian you need
 */
//#define STREAM_BYTE_ORDER                   (1 && STREAM_WRITE_REVERSE && STREAM_READ_REVERSE)
/**
 * @brief save system byte order in static variable for avoid calculate each time
 * run Stream_getSystemByteOrder function
 */
//#define STREAM_BYTE_ORDER_SYS_STATIC        (1 && STREAM_BYTE_ORDER)

// --------------------------- Cursor APIs ------------------------------
/**
 * @brief enable cursor object for check how many bytes read or write
 */
//#define STREAM_CURSOR                       1

// ---------------------------- MemIO APIs ------------------------------
/**
 * @brief This features help you to override memcpy, memrcpy, memset and etc with your custom functions
 */
//#define STREAM_MEM_IO                       STREAM_MEM_IO_DEFAULT
/* Default IO functions */
//#define STREAM_MEM_COPY                     memcpy
//#define STREAM_MEM_COPY_REVERSE             memrcpy
//#define STREAM_MEM_SET                      memset
//#define STREAM_MEM_REVERSE                  memreverse
/**
 * @brief Implement built in mem utils function for `memrcpy`, `memreverse
 */
//#define STREAM_MEM_IO_BUILT_IN              (1 && (STREAM_MEM_IO == STREAM_MEM_IO_DEFAULT))

// ---------------------------- Mutex APIs ------------------------------
/**
 * @brief If you want mutex for thread safety
 */
//#define STREAM_MUTEX                        STREAM_MUTEX_DRIVER
/**
 * @brief Mutex check result for every functions
 */
//#define STREAM_MUTEX_CHECK_RESULT       0
/**
 * @brief Mutex result type
 */
//#define STREAM_MUTEX_RESULT
//typedef int32_t Stream_MutexResult;
/**
 * @brief Stream Mutex object
 */
//#define STREAM_MUTEX_TYPE
//typedef void* Stream_Mutex;
/**
 * @brief mutex function not exists
 */
//#define STREAM_MUTEX_NO_Function        ((Stream_MutexResult) -1)

/************************************************************************/
/*                             OutputStream                             */
/************************************************************************/

/**
 * @brief This lable shows StreamOut Library is enabled or not
 */
//#define OSTREAM                     (1 && STREAM_WRITE && STREAM_PENDING_BYTES)
/**
 * @brief enable checkTransmit function
 */
//#define OSTREAM_CHECK_TRANSMIT      1
/**
 * @brief set default flush mode
 * Stream_FlushMode_Single:     flush only pending bytes before call flush function
 * Stream_FlushMode_Continue:   after flush complete if there is any pending bytes transmit pending bytes again
 */
//#define OSTREAM_FLUSH_MODE          Stream_FlushMode_Continue
/**
 * @brief enable flush complete callback
 */
//#define OSTREAM_FLUSH_CALLBACK      1

/************************************************************************/
/*                              InputStream                             */
/************************************************************************/

/**
 * @brief This lable shows OStream Libraty is enabled or not
 */
//#define ISTREAM                     (1 && STREAM_READ && STREAM_PENDING_BYTES)
/**
 * @brief enable checkTransmit function
 */
//#define ISTREAM_CHECK_RECEIVE       1
/**
 * @brief Enable Receive Callback
 */
//#define ISTREAM_RECEIVE_CALLBACK    1
/**
 * @brief Enable Full Callback
 */
//#define ISTREAM_FULL_CALLBACK       1

#endif // _STREAM_USER_CONFIG_H_
