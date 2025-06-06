/**
 * @file StreamBufferMacro.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This file contains macros and definitions for stream buffer operations.
 * @version 0.1
 * @date 2025-06-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _STREAM_BUFFER_MACRO_H_
#define _STREAM_BUFFER_MACRO_H_

/**
 * @brief return actual type for detect sizeoof or other usage
 * 
 * @param TYPE
 */
#define STREAM_VALUE_TYPE(TYPE)       STREAM_VALUE_TYPE_(TYPE)
#define STREAM_VALUE_TYPE_(TYPE)      STREAM_VALUE_TYPE_ ##TYPE

#define STREAM_VALUE_TYPE_bytes       uint8_t
#define STREAM_VALUE_TYPE_char        char
#define STREAM_VALUE_TYPE_uint8       uint8_t
#define STREAM_VALUE_TYPE_uint16      uint16_t
#define STREAM_VALUE_TYPE_uint32      uint32_t
#define STREAM_VALUE_TYPE_uint64      uint64_t
#define STREAM_VALUE_TYPE_int8        int8_t
#define STREAM_VALUE_TYPE_int16       int16_t
#define STREAM_VALUE_TYPE_int32       int32_t
#define STREAM_VALUE_TYPE_int64       int64_t
#define STREAM_VALUE_TYPE_float       float
#define STREAM_VALUE_TYPE_double      double
#define STREAM_VALUE_TYPE_void        void

#define STREAM_VALUE_TYPE_Bytes       uint8_t
#define STREAM_VALUE_TYPE_Char        char
#define STREAM_VALUE_TYPE_UInt8       uint8_t
#define STREAM_VALUE_TYPE_UInt16      uint16_t
#define STREAM_VALUE_TYPE_UInt32      uint32_t
#define STREAM_VALUE_TYPE_UInt64      uint64_t
#define STREAM_VALUE_TYPE_Int8        int8_t
#define STREAM_VALUE_TYPE_Int16       int16_t
#define STREAM_VALUE_TYPE_Int32       int32_t
#define STREAM_VALUE_TYPE_Int64       int64_t
#define STREAM_VALUE_TYPE_Float       float
#define STREAM_VALUE_TYPE_Double      double
#define STREAM_VALUE_TYPE_Void        void


#endif 
