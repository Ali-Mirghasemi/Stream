#include "StreamBuffer.h"
#include <string.h>

#if STREAM_BYTE_ORDER
/* private typedef */
typedef Stream_Result (*Stream_WriteBytesFn)(Stream* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_ReadBytesFn)(Stream* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_GetBytesFn)(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
/* private variables */
static const Stream_WriteBytesFn writeBytes[2] = {
    Stream_writeBytes,
    Stream_writeBytesReverse,
};
static const Stream_ReadBytesFn readBytes[2] = {
    Stream_readBytes,
    Stream_readBytesReverse,
};
static const Stream_GetBytesFn getBytesAt[2] = {
    Stream_getBytesAt,
    Stream_getBytesReverseAt,
};

    #define __writeBytes(STREAM, VAL, LEN)          writeBytes[stream->OrderFn]((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)           readBytes[stream->OrderFn]((STREAM), (VAL), (LEN))
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   getBytesAt[stream->OrderFn]((STREAM), (INDEX), (VAL), (LEN))
#else
    #define __writeBytes(STREAM, VAL, LEN)          Stream_writeBytes((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)           Stream_readBytes((STREAM), (VAL), (LEN))
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   Stream_getBytesAt((STREAM), (INDEX), (VAL), (LEN))
#endif
/* private function */
void memrcpy(void* dest, const void* src, int len);

/**
 * @brief initialize stream
 *
 * @param stream address of stream struct
 * @param buffer address of byte buffer
 * @param size size of buffer
 */
void Stream_init(Stream* stream, uint8_t* buffer, Stream_LenType size) {
    stream->Data = buffer;
    stream->Size = size;
    stream->Overflow = 0;
    stream->InReceive = 0;
    stream->InTransmit = 0;
#if STREAM_BYTE_ORDER
    stream->Order = Stream_getSystemByteOrder();
    stream->OrderFn = 0;
#endif // STREAM_BYTE_ORDER
    stream->RPos = 0;
    stream->WPos = 0;
}
/**
 * @brief reset stream struct into default values
 *
 * @param stream address of stream
 */
void Stream_deinit(Stream* stream) {
    memset(stream, 0, sizeof(Stream));
}

/**
 * @brief return available bytes to read
 *
 * @param stream
 * @return Stream_LenType available bytes
 */
Stream_LenType Stream_available(Stream* stream) {
    return stream->Overflow ? stream->WPos + (stream->Size - stream->RPos) :
                                stream->WPos - stream->RPos;
}
/**
 * @brief return buffer space for write bytes
 *
 * @param stream
 * @return Stream_LenType space for write
 */
Stream_LenType Stream_space(Stream* stream) {
    return stream->Overflow ? stream->RPos - stream->WPos:
                                stream->RPos + (stream->Size - stream->WPos);
}
/**
 * @brief check stream is empty
 * no bytes available to read
 * @param stream
 * @return uint8_t 0 -> Not Empty, 1-> it's empty
 */
uint8_t Stream_isEmpty(Stream* stream) {
    return stream->RPos == stream->WPos && !stream->Overflow;
}
/**
 * @brief check stream it's full
 * no bytes can write
 *
 * @param stream
 * @return uint8_t 0 -> Not Full, 1-> it's Full
 */
uint8_t Stream_isFull(Stream* stream) {
    return stream->RPos == stream->WPos && stream->Overflow;
}
/**
 * @brief reset stream and ignore receive and transmit operations
 *
 * @param stream
 */
void Stream_clear(Stream* stream) {
    stream->RPos = 0;
    stream->WPos = 0;
    stream->Overflow = 0;
    stream->InReceive = 0;
    stream->InTransmit = 0;
}
/**
 * @brief return Write Pos
 *
 * @param stream
 * @return Stream_LenType it's between 0 ~ Size
 */
Stream_LenType Stream_getWritePos(Stream* stream) {
    return stream->WPos;
}
/**
 * @brief return Read Pos
 *
 * @param stream
 * @return Stream_LenType it's between 0 ~ Size
 */
Stream_LenType Stream_getReadPos(Stream* stream) {
    return stream->RPos;
}
/**
 * @brief return number of bytes that it's in row in the ram
 * use for transmit function in OStream
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_directAvailable(Stream* stream) {
    return stream->Overflow ? stream->Size - stream->RPos :
                                stream->WPos - stream->RPos;
}
/**
 * @brief return number of bytes it's in row in the ram
 * use for receive function in IStream
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_directSpace(Stream* stream) {
    return stream->Overflow ? stream->RPos - stream->WPos :
                                stream->Size - stream->WPos;
}
/**
 * @brief return number of bytes that it's in row in the ram
 * use for transmit function in OStream
 * offset is RPos
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_directAvailableAt(Stream* stream, Stream_LenType index) {
    Stream_LenType len = Stream_available(stream);
    Stream_LenType dirLen = Stream_directAvailable(stream);
    if (len == dirLen) {
        return len - index;
    }
    else {
        return dirLen > index ? dirLen - index :
                                stream->WPos - (index - dirLen);
    }
}
/**
 * @brief return number of bytes it's in row in the ram
 * use for receive function in IStream
 * offset is WPos
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_directSpaceAt(Stream* stream, Stream_LenType index) {
    Stream_LenType len = Stream_space(stream);
    Stream_LenType dirLen = Stream_directSpace(stream);
    if (len == dirLen) {
        return len - index;
    }
    else {
        return dirLen > index ? dirLen - index :
                                stream->RPos - (index - dirLen);
    }
}
/**
 * @brief get ptr to start of WPos in ram
 *
 * @param stream
 * @return uint8_t*
 */
uint8_t* Stream_getWritePtr(Stream* stream) {
    return &stream->Data[stream->WPos];
}
/**
 * @brief get ptr to start of RPos in ram
 *
 * @param stream
 * @return uint8_t*
 */
uint8_t* Stream_getReadPtr(Stream* stream) {
    return &stream->Data[stream->RPos];
}
uint8_t* Stream_getWritePtrAt(Stream* stream, Stream_LenType index) {
    index += stream->WPos;

    if (index >= stream->Size) {
        index %= stream->Size;
    }

    return &stream->Data[index];
}
uint8_t* Stream_getReadPtrAt(Stream* stream, Stream_LenType index) {
    index += stream->RPos;

    if (index >= stream->Size) {
        index %= stream->Size;
    }

    return &stream->Data[index];
}
/**
 * @brief set buffer for stream and reset stream
 *
 * @param stream
 */
void Stream_setBuffer(Stream* stream, uint8_t* data, Stream_LenType size) {
    Stream_init(stream, data, size);
}
/**
 * @brief return byte buffer
 *
 * @param stream
 * @return uint8_t*
 */
uint8_t* Stream_getBuffer(Stream* stream) {
    return stream->Data;
}
/**
 * @brief return size of stream buffer
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_getBufferSize(Stream* stream) {
    return stream->Size;
}
/**
 * @brief can use for ignore bytes, and move WPos index
 *
 * @param stream
 * @param steps
 * @return Stream_Result
 */
Stream_Result Stream_moveWritePos(Stream* stream, Stream_LenType steps) {
    if (Stream_space(stream) < steps) {
        return Stream_NoSpace;
    }

    stream->WPos += steps;
    if (stream->WPos >= stream->Size) {
        stream->WPos %= stream->Size;
        stream->Overflow = 1;
    }

    return Stream_Ok;
}
/**
 * @brief can use for ignore bytes, and move RPos index
 *
 * @param stream
 * @param steps
 * @return Stream_Result
 */
Stream_Result Stream_moveReadPos(Stream* stream, Stream_LenType steps) {
    if (Stream_available(stream) < steps) {
        return Stream_NoAvailable;
    }

    stream->RPos += steps;
    if (stream->RPos >= stream->Size) {
        stream->RPos %= stream->Size;
        stream->Overflow = 0;
    }

    return Stream_Ok;
}

#if STREAM_BYTE_ORDER

/**
 * @brief determine platform byte order
 *
 * @return ByteOrder
 */
ByteOrder Stream_getSystemByteOrder(void) {
    const uint8_t arr[2] = {0xAA, 0xBB};
    const uint16_t val = 0xAABB;
    return (ByteOrder) (memcmp(arr, (uint8_t*) &val, sizeof(val)) == 0);
}
/**
 * @brief set stream r/w byte order
 *
 * @param stream
 * @param order
 */
void       Stream_setByteOrder(Stream* stream, ByteOrder order) {
    ByteOrder osOrder = Stream_getSystemByteOrder();
    stream->Order = order;
    stream->OrderFn = osOrder != order;
}
/**
 * @brief return stream byte order
 *
 * @param stream
 * @return ByteOrder
 */
ByteOrder  Stream_getByteOrder(Stream* stream) {
    return (ByteOrder) stream->Order;
}
#endif

/**
 * @brief write byte array into stream
 *
 * @param stream
 * @param val byte array
 * @param len len of array
 * @return Stream_Result
 */
Stream_Result Stream_writeBytes(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_space(stream) < len) {
        return Stream_NoSpace;
    }

    if (stream->WPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->WPos;
        len -= tmpLen;
        memcpy(&stream->Data[stream->WPos], val, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = (stream->WPos + tmpLen) % stream->Size;
        stream->Overflow = 1;
    }
    if (len > 0) {
        memcpy(&stream->Data[stream->WPos], val, len);
        // move WPos
        stream->WPos = (stream->WPos + len) % stream->Size;
    }

    return Stream_Ok;
}
/**
 * @brief write array into stream in reverse order
 * 
 * @param stream 
 * @param val 
 * @param len 
 * @return Stream_Result 
 */
Stream_Result Stream_writeBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_space(stream) < len) {
        return Stream_NoSpace;
    }

    if (stream->WPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->WPos;
        len -= tmpLen;
        memrcpy(&stream->Data[stream->WPos], val + len, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = (stream->WPos + tmpLen) % stream->Size;
        stream->Overflow = 1;
    }
    if (len > 0) {
        memrcpy(&stream->Data[stream->WPos], val, len);
        // move WPos
        stream->WPos = (stream->WPos + len) % stream->Size;
    }

    return Stream_Ok;
}
/**
 * @brief directly read from a stream and write to another
 * 
 * @param stream 
 * @param in 
 * @param len 
 * @return Stream_Result 
 */
Stream_Result Stream_writeStream(Stream* out, Stream* in, Stream_LenType len) {
    // check available space for write
    if (Stream_space(out) < len) {
        return Stream_NoSpace;
    }
    // check available bytes for read
    if (Stream_available(in) < len) {
        return Stream_NoAvailable;
    }

    if (out->WPos + len >= out->Size) {
        Stream_LenType tmpLen;
        tmpLen = out->Size - out->WPos;
        len -= tmpLen;
        Stream_readBytes(in, &out->Data[out->WPos], tmpLen);
        // move WPos
        out->WPos = (out->WPos + tmpLen) % out->Size;
        out->Overflow = 1;
    }
    if (len > 0) {
        Stream_readBytes(in, &out->Data[out->WPos], len);
        // move WPos
        out->WPos = (out->WPos + len) % out->Size;
    }

    return Stream_Ok;

}
Stream_Result Stream_writeChar(Stream* stream, char val) {
    return Stream_writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeUInt8(Stream* stream, uint8_t val) {
    return Stream_writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt8(Stream* stream, int8_t val) {
    return Stream_writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeUInt16(Stream* stream, uint16_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt16(Stream* stream, int16_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeUInt32(Stream* stream, uint32_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt32(Stream* stream, int32_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeFloat(Stream* stream, float val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
#if STREAM_UINT64
Stream_Result Stream_writeUInt64(Stream* stream, uint64_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt64(Stream* stream, int64_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
Stream_Result Stream_writeDouble(Stream* stream, double val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
#endif // STREAM_DOUBLE

/**
 * @brief read one byte from stream if available return value between 0~255, else return -1
 *
 * @param stream
 * @return int16_t
 */
int16_t  Stream_read(Stream* stream) {
    if (Stream_available(stream)) {
        return (int16_t) Stream_readUInt8(stream);
    }
    else {
        return -1;
    }
}
/**
 * @brief read bytes form stream, if possible
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readBytes(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) < len) {
        return Stream_NoAvailable;
    }

    if (stream->RPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->RPos;
        len -= tmpLen;
        memcpy(val, &stream->Data[stream->RPos], tmpLen);
        val += tmpLen;
        // move RPos
        stream->RPos = (stream->RPos + tmpLen) % stream->Size;
        stream->Overflow = 0;
    }
    if (len > 0) {
        memcpy(val, &stream->Data[stream->RPos], len);
        // move RPos
        stream->RPos = (stream->RPos + len) % stream->Size;
    }

    return Stream_Ok;
}
Stream_Result Stream_readBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) < len) {
        return Stream_NoAvailable;
    }

    if (stream->RPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->RPos;
        len -= tmpLen;
        memrcpy(val, &stream->Data[stream->RPos + len], tmpLen);
        val += tmpLen;
        // move RPos
        stream->RPos = (stream->RPos + tmpLen) % stream->Size;
        stream->Overflow = 0;
    }
    if (len > 0) {
        memrcpy(val, &stream->Data[stream->RPos], len);
        // move RPos
        stream->RPos = (stream->RPos + len) % stream->Size;
    }

    return Stream_Ok;
}
Stream_Result Stream_readStream(Stream* in, Stream* out, Stream_LenType len) {
    if (Stream_available(in) < len) {
        return Stream_NoAvailable;
    }
    if (Stream_space(out) < len) {
        return Stream_NoSpace;
    }

    if (in->RPos + len >= in->Size) {
        Stream_LenType tmpLen;

        tmpLen = in->Size - in->RPos;
        len -= tmpLen;
        Stream_writeBytes(out, &in->Data[in->RPos], tmpLen);
        // move RPos
        in->RPos = (in->RPos + tmpLen) % in->Size;
        in->Overflow = 0;
    }
    if (len > 0) {
        Stream_writeBytes(out, &in->Data[in->RPos], len);
        // move RPos
        in->RPos = (in->RPos + len) % in->Size;
    }

    return Stream_Ok;
}
char     Stream_readChar(Stream* stream) {
    char val;
    if (Stream_readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
uint8_t  Stream_readUInt8(Stream* stream) {
    uint8_t val;
    if (Stream_readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int8_t   Stream_readInt8(Stream* stream) {
    int8_t val;
    if (Stream_readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
uint16_t Stream_readUInt16(Stream* stream) {
    uint16_t val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int16_t  Stream_readInt16(Stream* stream) {
    int16_t val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
uint32_t Stream_readUInt32(Stream* stream) {
    uint32_t val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int32_t  Stream_readInt32(Stream* stream) {
    int32_t val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
float    Stream_readFloat(Stream* stream) {
    float val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
#if STREAM_UINT64
uint64_t Stream_readUInt64(Stream* stream) {
    uint64_t val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int64_t  Stream_readInt64(Stream* stream) {
    int64_t val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_readDouble(Stream* stream) {
    double val;
    if (__readBytes(stream, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
#endif // STREAM_DOUBLE

Stream_Result Stream_getBytes(Stream* stream, uint8_t* val, Stream_LenType len) {
    return Stream_getBytesAt(stream, stream->RPos, val, len);
}
Stream_Result Stream_getBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len) {
    return Stream_getBytesAt(stream, stream->RPos, val, len);
}
char     Stream_getChar(Stream* stream) {
    return Stream_getCharAt(stream, stream->RPos);
}
uint8_t  Stream_getUInt8(Stream* stream) {
    return Stream_getUInt8At(stream, stream->RPos);
}
int8_t   Stream_getInt8(Stream* stream) {
    return Stream_getInt8At(stream, stream->RPos);
}
uint16_t Stream_getUInt16(Stream* stream) {
    return Stream_getUInt16At(stream, stream->RPos);
}
int16_t  Stream_getInt16(Stream* stream) {
    return Stream_getInt16At(stream, stream->RPos);
}
uint32_t Stream_getUInt32(Stream* stream) {
    return Stream_getUInt32At(stream, stream->RPos);
}
int32_t  Stream_getInt32(Stream* stream) {
    return Stream_getInt32At(stream, stream->RPos);
}
float    Stream_getFloat(Stream* stream) {
    return Stream_getFloatAt(stream, stream->RPos);
}
#if STREAM_UINT64
uint64_t Stream_getUInt64(Stream* stream) {
    return Stream_getUInt64At(stream, stream->RPos);
}
int64_t  Stream_getInt64(Stream* stream) {
    return Stream_getInt64At(stream, stream->RPos);
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_getDouble(Stream* stream) {
    return Stream_getDoubleAt(stream, stream->RPos);
}
#endif // STREAM_DOUBLE

Stream_Result Stream_getBytesAt(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) - index < len) {
        return Stream_NoAvailable;
    }

    index = (stream->RPos + index) % stream->Size;

    if (index + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - index;
        len -= tmpLen;
        memcpy(val, &stream->Data[index], tmpLen);
        val += tmpLen;
        index = (index + tmpLen) % stream->Size;
    }

    memcpy(val, &stream->Data[index], len);

    return Stream_Ok;
}
Stream_Result Stream_getBytesReverseAt(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) - index < len) {
        return Stream_NoAvailable;
    }

    index = (stream->RPos + index) % stream->Size;

    if (index + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - index;
        len -= tmpLen;
        memrcpy(val, &stream->Data[index + len], tmpLen);
        val += tmpLen;
        index = (index + tmpLen) % stream->Size;
    }

    memrcpy(val, &stream->Data[index], len);

    return Stream_Ok;
}
char     Stream_getCharAt(Stream* stream, Stream_LenType index) {
    char val;
    if (Stream_getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
uint8_t  Stream_getUInt8At(Stream* stream, Stream_LenType index) {
    uint8_t val;
    if (Stream_getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int8_t   Stream_getInt8At(Stream* stream, Stream_LenType index) {
    int8_t val;
    if (Stream_getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
uint16_t Stream_getUInt16At(Stream* stream, Stream_LenType index) {
    uint16_t val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int16_t  Stream_getInt16At(Stream* stream, Stream_LenType index) {
    int16_t val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
uint32_t Stream_getUInt32At(Stream* stream, Stream_LenType index) {
    uint32_t val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int32_t  Stream_getInt32At(Stream* stream, Stream_LenType index) {
    int32_t val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
float    Stream_getFloatAt(Stream* stream, Stream_LenType index) {
    float val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
#if STREAM_UINT64
uint64_t Stream_getUInt64At(Stream* stream, Stream_LenType index) {
    uint64_t val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
int64_t  Stream_getInt64At(Stream* stream, Stream_LenType index) {
    int64_t val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_getDoubleAt(Stream* stream, Stream_LenType index) {
    double val;
    if (__getBytesAt(stream, index, (uint8_t*) &val, sizeof(val)) != Stream_Ok) {
        return 0;
    }
    return val;
}
#endif // STREAM_DOUBLE
/**
 * @brief compare a given bytes at index with available bytes in stream
 *
 * @param stream
 * @param index
 * @param val
 * @param len
 * @return int8_t
 */
int8_t Stream_compareAt(Stream* stream, Stream_LenType index, const uint8_t* val, Stream_LenType len) {
    int8_t result;
    Stream_LenType tmpLen;

    if (Stream_available(stream) - index < len) {
        return -2;
    }

    tmpLen = Stream_directAvailableAt(stream, index);
    if (tmpLen < len) {
        if ((result = (int8_t) memcmp(Stream_getReadPtrAt(stream, index), val, tmpLen)) != 0) {
            return result;
        }

        index += tmpLen;
        val += tmpLen;
        len -= tmpLen;
    }

    return (int8_t) memcmp(Stream_getReadPtrAt(stream, index), val, len);
}
Stream_LenType Stream_findByte(Stream* stream, uint8_t val) {
    Stream_LenType tmpLen = 0;
    uint8_t* pStart = &stream->Data[stream->RPos];
    uint8_t* pEnd;

    if (Stream_isEmpty(stream)) {
        return -1;
    }

    pEnd = memchr(pStart, val, Stream_directAvailable(stream));
    if (!pEnd && stream->Overflow) {
        tmpLen = stream->Size - stream->RPos;
        pStart = stream->Data;
        pEnd = memchr(pStart, val, stream->WPos);
    }

    return pEnd != NULL ? (Stream_LenType)(pEnd - pStart) + tmpLen : -1;
}
Stream_LenType Stream_findByteAt(Stream* stream, Stream_LenType offset, uint8_t val) {
    Stream_LenType tmpLen = 0;
    uint8_t* pStart = Stream_getReadPtrAt(stream, offset);
    uint8_t* pEnd;

    if (Stream_available(stream) < offset) {
        return -1;
    }

    tmpLen = Stream_directAvailableAt(stream, offset);
    pEnd = memchr(pStart, val, tmpLen);
    if (!pEnd && (tmpLen + offset) < Stream_available(stream)) {
        pStart = stream->Data;
        pEnd = memchr(pStart, val, stream->WPos);
    }

    return pEnd != NULL ? (Stream_LenType)(pEnd - pStart) + offset : -1;
}
Stream_LenType Stream_findPattern(Stream* stream, const uint8_t* pat, Stream_LenType patLen) {
    Stream_LenType index = 0;

    if (Stream_available(stream) < patLen) {
        return -1;
    }

    while ((index = Stream_findByteAt(stream, index, *pat)) != -1) {
        if (Stream_compareAt(stream, index, pat, patLen) == 0) {
            break;
        }
        index++;
    }

    return index;
}
Stream_LenType Stream_findPatternAt(Stream* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen)  {
    if (Stream_available(stream) < patLen) {
        return -1;
    }

    while ((offset = Stream_findByteAt(stream, offset, *pat)) != -1) {
        if (Stream_compareAt(stream, offset, pat, patLen) == 0) {
            break;
        }
        offset++;
    }

    return offset;
}
Stream_LenType Stream_readBytesUntil(Stream* stream, uint8_t end, uint8_t* val, Stream_LenType len) {
    Stream_LenType tmpLen;
    // find end byte
    if ((tmpLen = Stream_findByte(stream, end)) >= 0) {
        tmpLen++;

        if (len < tmpLen) {
            tmpLen = len;
        }

        if (Stream_readBytes(stream, val, tmpLen) == Stream_Ok) {
            return tmpLen;
        }
    }

    return 0;
}
Stream_LenType Stream_readBytesUntilPattern(Stream* stream, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len) {
    Stream_LenType tmpLen;
    // find end byte
    if ((tmpLen = Stream_findPattern(stream, pat, patLen)) >= 0) {
        tmpLen += patLen;

        if (len < tmpLen) {
            tmpLen = len;
        }

        if (Stream_readBytes(stream, val, tmpLen) == Stream_Ok) {
            return tmpLen;
        }
    }

    return 0;
}

// TODO: need to implement with more performance
void memrcpy(void* dest, const void* src, int len) {
    uint8_t* pDest = (uint8_t*) dest;
    const uint8_t* pSrc = (const uint8_t*) src + len - 1;

    while (len-- > 0) {
        *pDest++ = *pSrc--;
    }
}

