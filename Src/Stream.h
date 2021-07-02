#ifndef _STREAM_H_
#define _STREAM_H_

#include <stdint.h>

/************************************************************************/
/*                            Configuration                             */
/************************************************************************/

#define STREAM_BYTE_ORDER               1

#define STREAM_UINT64                   1

#define STREAM_DOUBLE                   1

#define STREAM_RESIZABLE_BUFFER         1

typedef uint16_t Stream_LenType;







/************************************************************************/

typedef enum {
    ByteOrder_LittleEndian  = 0,
    ByteOrder_BigEndian     = 1,
    ByteOrder_Reserved      = 0xFF,
} ByteOrder;

typedef enum {
    Stream_Ok               = 0,
    Stream_NoSpace          = 1,
    Stream_NoAvailable      = 2,
    Stream_BufferFull       = 3,
} Stream_Result;

typedef struct {
    uint8_t*                Data;
    Stream_LenType          Size;
    Stream_LenType          WPos;
    Stream_LenType          RPos;
#if STREAM_BYTE_ORDER
    ByteOrder               Order;
#endif
    uint8_t                 Overflow    : 1;
    uint8_t                 Reserved    : 7;
} Stream;


void Stream_init(Stream* stream, uint8_t* buffer, Stream_LenType size);
void Stream_deinit(Stream* stream);

/*************** General APIs *************/
Stream_LenType Stream_available(Stream* stream);
Stream_LenType Stream_space(Stream* stream);
uint8_t Stream_isEmpty(Stream* stream);
uint8_t Stream_isFull(Stream* stream);

void Stream_clear(Stream* stream);

uint8_t* Stream_getBuffer(Stream* stream);

Stream_LenType Stream_getBufferSize(Stream* stream);
//Stream_LenType Stream_getWritePos(Stream* stream);
//Stream_LenType Stream_getReadPos(Stream* stream);

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
Stream_LenType Stream_readBytes(Stream* stream, uint8_t* val, Stream_LenType len);
Stream_LenType Stream_readBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len);
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


#endif /* _STREAM_H_ */
