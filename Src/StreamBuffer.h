#ifndef _STREAM_H_
#define _STREAM_H_

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/

/**
 * @brief you can enable ByteOrder option to have r/w operation
 * on what endian you need
 */
#define STREAM_BYTE_ORDER               1
/**
 * @brief if your platform support 64bit variables and you need it
 * you can enable this option
 */
#define STREAM_UINT64                   1
/**
 * @brief if you need r/w double variables and your platform support
 * you can enable this option
 */
#define STREAM_DOUBLE                   1
/**
 * @brief based on maximum size of buffer that you use for stream
 * you can change type of len variables
 */
typedef int16_t Stream_LenType;

/************************************************************************/

/**
 * @brief you can choose what ByteOrder can use for r/w operations
 */
typedef enum {
    ByteOrder_LittleEndian  = 0,
    ByteOrder_BigEndian     = 1,
    ByteOrder_Reserved      = 0xFF,
} ByteOrder;
/**
 * @brief result of Stream functions
 */
typedef enum {
    Stream_Ok               = 0,    /**< everythings is OK */
    Stream_NoSpace          = 1,    /**< there is no space for write operation */
    Stream_NoAvailable      = 2,    /**< there no available bytes for read operation */
    Stream_BufferFull       = 3,    /**< buffer full*/
    Stream_NoReceiveFn      = 4,    /**< no receive function set for IStream */
    Stream_NoTransmitFn     = 5,    /**< no transmit function set for OStream */
    Stream_NoReceive		= 6,    /**< stream is not in receive mode */
    Stream_NoTransmit		= 7,    /**< stream is not in transmit mode */
} Stream_Result;
/**
 * @brief Stream struct
 * contains everything need for handle stream
 */
typedef struct {
    uint8_t*                Data;
    Stream_LenType          Size;
    Stream_LenType          WPos;
    Stream_LenType          RPos;
    uint8_t                 Overflow    : 1;
    uint8_t                 InReceive   : 1;
    uint8_t                 InTransmit  : 1;
#if STREAM_BYTE_ORDER
    uint8_t                 Order       : 1;
    uint8_t                 OrderFn     : 1;
    uint8_t                 Reserved    : 3;
#else
    uint8_t                 Reserved    : 5;
#endif
} Stream;


void Stream_init(Stream* stream, uint8_t* buffer, Stream_LenType size);
void Stream_deinit(Stream* stream);

/*************** General APIs *************/
Stream_LenType Stream_available(Stream* stream);
Stream_LenType Stream_space(Stream* stream);
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
#endif

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

int8_t Stream_compareAt(Stream* stream, Stream_LenType index, const uint8_t* val, Stream_LenType len);
Stream_LenType Stream_findByte(Stream* stream, uint8_t val);
Stream_LenType Stream_findByteAt(Stream* stream, Stream_LenType offset, uint8_t val);
Stream_LenType Stream_findPattern(Stream* stream, const uint8_t* pat, Stream_LenType patLen);
Stream_LenType Stream_findPatternAt(Stream* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen);
Stream_LenType Stream_readBytesUntil(Stream* stream, uint8_t end, uint8_t* val, Stream_LenType len);
Stream_LenType Stream_readBytesUntilPattern(Stream* stream, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len);

#if __cplusplus
};
#endif

#endif /* _STREAM_H_ */
