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
#if STREAM_GET_AT_FUNCTIONS
static const Stream_GetBytesFn getBytesAt[2] = {
    Stream_getBytesAt,
    Stream_getBytesReverseAt,
};
#endif

    #define __writeBytes(STREAM, VAL, LEN)          writeBytes[stream->OrderFn]((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)           readBytes[stream->OrderFn]((STREAM), (VAL), (LEN))
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   getBytesAt[stream->OrderFn]((STREAM), (INDEX), (VAL), (LEN))
#else
    #define __writeBytes(STREAM, VAL, LEN)          Stream_writeBytes((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)           Stream_readBytes((STREAM), (VAL), (LEN))
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   Stream_getBytesAt((STREAM), (INDEX), (VAL), (LEN))
#endif // STREAM_BYTE_ORDER
/* private function */
static void memrcpy(void* dest, const void* src, int len);

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
#if STREAM_WRITE_LIMIT
    stream->WriteLimit = STREAM_NO_LIMIT;
#endif // STREAM_WRITE_LIMIT
#if STREAM_READ_LIMIT
    stream->ReadLimit = STREAM_NO_LIMIT;
#endif // STREAM_READ_LIMIT
#if STREAM_WRITE_LOCK
    stream->WriteLocked = 0;
#endif // STREAM_WRITE_LOCK
#if STREAM_READ_LOCK
    stream->ReadLocked = 0;
#endif // STREAM_READ_LOCK
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
Stream_LenType Stream_availableReal(Stream* stream) {
    return stream->Overflow ? stream->WPos + (stream->Size - stream->RPos) :
                                stream->WPos - stream->RPos;
}
/**
 * @brief return buffer space for write bytes
 *
 * @param stream
 * @return Stream_LenType space for write
 */
Stream_LenType Stream_spaceReal(Stream* stream) {
    return stream->Overflow ? stream->RPos - stream->WPos :
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
 * @brief reset stream
 *
 * @param stream
 */
void Stream_reset(Stream* stream) {
    stream->RPos = 0;
    stream->WPos = 0;
    stream->Overflow = 0;
}
/**
 * @brief reset stream and ignore receive and transmit operations
 *
 * @param stream
 */
void Stream_resetIO(Stream* stream) {
    stream->RPos = 0;
    stream->WPos = 0;
    stream->Overflow = 0;
    stream->InReceive = 0;
    stream->InTransmit = 0;
}
/**
 * @brief clear buffer and reset stream
 *
 * @param stream
 */
void Stream_clear(Stream* stream) {
    Stream_reset(stream);
    memset(stream->Data, 0, stream->Size);
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
    Stream_LenType len = Stream_availableReal(stream);
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
    Stream_LenType len = Stream_spaceReal(stream);
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
/**
 * @brief flip space size for write
 *
 * @param stream
 * @param len
 */
void Stream_flipWrite(Stream* stream, Stream_LenType len) {
    stream->RPos = stream->WPos + len;
    if (stream->RPos >= stream->Size) {
        stream->RPos %= stream->Size;
        stream->Overflow = 0;
    }
    else {
        stream->Overflow = 1;
    }
}
/**
 * @brief flip available size for read
 *
 * @param stream
 * @param len
 */
void Stream_flipRead(Stream* stream, Stream_LenType len) {
    stream->WPos = stream->RPos + len;
    if (stream->WPos >= stream->Size) {
        stream->WPos %= stream->Size;
        stream->Overflow = 1;
    }
    else {
        stream->Overflow = 0;
    }
}
#if STREAM_BYTE_ORDER

/**
 * @brief determine platform byte order
 *
 * @return ByteOrder
 */
ByteOrder Stream_getSystemByteOrder(void) {
#if STREAM_BYTE_ORDER_SYS_STATIC
    static ByteOrder sysByteOrder = ByteOrder_Reserved;
    if (sysByteOrder == ByteOrder_Reserved) {
        const uint8_t arr[2] = {0xAA, 0xBB};
        const uint16_t val = 0xAABB;
        sysByteOrder = (ByteOrder) (memcmp(arr, (uint8_t*) &val, sizeof(val)) == 0);
    }
    return sysByteOrder;
#else
    const uint8_t arr[2] = {0xAA, 0xBB};
    const uint16_t val = 0xAABB;
    return (ByteOrder) (memcmp(arr, (uint8_t*) &val, sizeof(val)) == 0);
#endif
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
#if STREAM_WRITE_LIMIT
/**
 * @brief set limit for write operations, you can't set limit greater than space
 *
 * @param stream
 * @param len
 */
void       Stream_setWriteLimit(Stream* stream, Stream_LenType len) {
    Stream_LenType space = Stream_spaceReal(stream);
    if (space < len) {
        len = space;
    }
    stream->WriteLimit = len;
}
/**
 * @brief return write operation is limited or not
 *
 * @param stream
 * @return uint8_t true means limited
 */
uint8_t    Stream_isWriteLimited(Stream* stream) {
    return stream->WriteLimit >= 0;
}
/**
 * @brief return space available for write bytes respect to write limit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_spaceLimit(Stream* stream) {
    return stream->WriteLimit >= 0 ? stream->WriteLimit : Stream_spaceReal(stream);
}
Stream_LenType Stream_getWriteLimit(Stream* stream) {
    return stream->WriteLimit;
}
#endif // STREAM_WRITE_LIMIT
#if STREAM_READ_LIMIT
/**
 * @brief set limit for read operations, you can't set limit greater than available
 *
 * @param stream
 * @param len
 */
void Stream_setReadLimit(Stream* stream, Stream_LenType len) {
    Stream_LenType available = Stream_availableReal(stream);
    if (available < len) {
        len = available;
    }
    stream->ReadLimit = len;
}
/**
 * @brief return read operations is limited or not
 *
 * @param stream
 * @return uint8_t true means it's limited
 */
uint8_t Stream_isReadLimited(Stream* stream) {
    return stream->ReadLimit >= 0;
}
/**
 * @brief retruna available bytes for read respect to read limit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_availableLimit(Stream* stream) {
    return stream->ReadLimit >= 0 ? stream->ReadLimit : Stream_availableReal(stream);
}
/**
 * @brief return read limit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_getReadLimit(Stream* stream) {
    return stream->ReadLimit;
}
#endif // STREAM_READ_LIMIT
#if STREAM_CURSOR
/**
 * @brief fill cursor object based on given stream
 *
 * @param stream
 * @param cursor
 */
void Stream_getCursor(Stream* stream, Stream_Cursor* cursor) {
    cursor->WPos = stream->WPos;
    cursor->RPos = stream->RPos;
}
/**
 * @brief return read len from cursor pos
 *
 * @param stream
 * @param cursor
 * @return Stream_LenType
 */
Stream_LenType Stream_getReadLen(Stream* stream, Stream_Cursor* cursor) {
    return cursor->RPos >= stream->RPos ? cursor->RPos - stream->RPos :
                                            (stream->Size - cursor->RPos) + stream->RPos;
}
/**
 * @brief return write len from cursor pos
 *
 * @param stream
 * @param cursor
 * @return Stream_LenType
 */
Stream_LenType Stream_getWriteLen(Stream* stream, Stream_Cursor* cursor) {
    return cursor->WPos >= stream->WPos ? cursor->WPos - stream->WPos :
                                            (stream->Size - cursor->WPos) + stream->WPos;
}
#endif // STREAM_CURSOR
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
#if STREAM_WRITE_LIMIT
    if (Stream_isWriteLimited(stream)) {
        stream->WriteLimit -= len;
    }
#endif

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
#if STREAM_WRITE_LIMIT
    if (Stream_isWriteLimited(stream)) {
        stream->WriteLimit -= len;
    }
#endif

    if (stream->WPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->WPos;
        len -= tmpLen;
        memrcpy(&stream->Data[stream->WPos], val + len, tmpLen);
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
#if STREAM_WRITE_LIMIT
    if (Stream_isWriteLimited(out)) {
        out->WriteLimit -= len;
    }
#endif

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
/**
 * @brief this function can use for write value multiple time into stream, can use for padding
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writePadding(Stream* stream, uint8_t val, Stream_LenType len) {
    if (Stream_space(stream) < len) {
        return Stream_NoSpace;
    }
#if STREAM_WRITE_LIMIT
    if (Stream_isWriteLimited(stream)) {
        stream->WriteLimit -= len;
    }
#endif

    if (stream->WPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->WPos;
        len -= tmpLen;
        memset(&stream->Data[stream->WPos], val, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = (stream->WPos + tmpLen) % stream->Size;
        stream->Overflow = 1;
    }
    if (len > 0) {
        memset(&stream->Data[stream->WPos], val, len);
        // move WPos
        stream->WPos = (stream->WPos + len) % stream->Size;
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
#if STREAM_READ_LIMIT
    if (Stream_isReadLimited(stream)) {
        stream->ReadLimit -= len;
    }
#endif

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
#if STREAM_READ_LIMIT
    if (Stream_isReadLimited(stream)) {
        stream->ReadLimit -= len;
    }
#endif

    if (stream->RPos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->RPos;
        len -= tmpLen;
        memrcpy(val + len, &stream->Data[stream->RPos], tmpLen);
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
#if STREAM_READ_LIMIT
    if (Stream_isReadLimited(in)) {
        in->ReadLimit -= len;
    }
#endif

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
    char val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
uint8_t  Stream_readUInt8(Stream* stream) {
    uint8_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int8_t   Stream_readInt8(Stream* stream) {
    int8_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
uint16_t Stream_readUInt16(Stream* stream) {
    uint16_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int16_t  Stream_readInt16(Stream* stream) {
    int16_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
uint32_t Stream_readUInt32(Stream* stream) {
    uint32_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int32_t Stream_readInt32(Stream* stream) {
    int32_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
float Stream_readFloat(Stream* stream) {
    float val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
#if STREAM_UINT64
uint64_t Stream_readUInt64(Stream* stream) {
    uint64_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int64_t Stream_readInt64(Stream* stream) {
    int64_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_readDouble(Stream* stream) {
    double val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_DOUBLE

#if STREAM_GET_AT_FUNCTIONS && STREAM_GET_FUNCTIONS

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
/**
 * @brief get array of characters without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getCharArray(Stream* stream, char* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getCharAt(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint8_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt8Array(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getUInt8At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int8_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt8Array(Stream* stream, int8_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getInt8At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint16_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt16Array(Stream* stream, uint16_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getUInt16At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int16_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt16Array(Stream* stream, int16_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getInt16At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint32_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt32Array(Stream* stream, uint32_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getUInt32At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int32_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt32Array(Stream* stream, int32_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getInt32At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of float without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getFloatArray(Stream* stream, float* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getFloatAt(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#if STREAM_UINT64
/**
 * @brief get array of uint64_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt64Array(Stream* stream, uint64_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getUInt64At(stream, index);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int64_t without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt64Array(Stream* stream, int64_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getInt64At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
/**
 * @brief get array of double without change the position of stream
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getDoubleArray(Stream* stream, double* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        Stream_LenType index = 0;
        while (len-- > 0) {
            *val++ = Stream_getDoubleAt(stream, index);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#endif // STREAM_DOUBLE

#endif // STREAM_GET_FUNCTIONS

#if STREAM_GET_AT_FUNCTIONS

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
char Stream_getCharAt(Stream* stream, Stream_LenType index) {
    char val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
uint8_t Stream_getUInt8At(Stream* stream, Stream_LenType index) {
    uint8_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int8_t Stream_getInt8At(Stream* stream, Stream_LenType index) {
    int8_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
uint16_t Stream_getUInt16At(Stream* stream, Stream_LenType index) {
    uint16_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int16_t Stream_getInt16At(Stream* stream, Stream_LenType index) {
    int16_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
uint32_t Stream_getUInt32At(Stream* stream, Stream_LenType index) {
    uint32_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int32_t Stream_getInt32At(Stream* stream, Stream_LenType index) {
    int32_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
float Stream_getFloatAt(Stream* stream, Stream_LenType index) {
    float val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
#if STREAM_UINT64
uint64_t Stream_getUInt64At(Stream* stream, Stream_LenType index) {
    uint64_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int64_t  Stream_getInt64At(Stream* stream, Stream_LenType index) {
    int64_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_getDoubleAt(Stream* stream, Stream_LenType index) {
    double val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_DOUBLE

/**
 * @brief get array of string without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getCharArrayAt(Stream* stream, Stream_LenType index, char* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getCharAt(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint8_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt8ArrayAt(Stream* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getUInt8At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int8_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt8ArrayAt(Stream* stream, Stream_LenType index, int8_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getInt8At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint16_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt16ArrayAt(Stream* stream, Stream_LenType index, uint16_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getUInt16At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int16_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt16ArrayAt(Stream* stream, Stream_LenType index, int16_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getInt16At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint32_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt32ArrayAt(Stream* stream, Stream_LenType index, uint32_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getUInt32At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int32_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt32ArrayAt(Stream* stream, Stream_LenType index, int32_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getInt32At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of uint64_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getFloatArrayAt(Stream* stream, Stream_LenType index, float* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getFloatAt(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#if STREAM_UINT64
/**
 * @brief get array of uint64_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getUInt64ArrayAt(Stream* stream, Stream_LenType index, uint64_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getUInt64At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief get array of int64_t without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getInt64ArrayAt(Stream* stream, Stream_LenType index, int64_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getInt64At(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
/**
 * @brief get array of double without change the position of stream at given index
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_getDoubleArrayAt(Stream* stream, Stream_LenType index, double* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_getDoubleAt(stream, index++);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#endif // STREAM_DOUBLE

#endif // STREAM_GET_AT_FUNCTIONS

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

    if (Stream_available(stream) == 0) {
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
/**
 * @brief write array of characters
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeCharArray(Stream* stream, char* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeChar(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of uint8_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeUInt8Array(Stream* stream, uint8_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeUInt8(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of int8_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeInt8Array(Stream* stream, int8_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeInt8(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of uint16_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeUInt16Array(Stream* stream, uint16_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeUInt16(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of int16_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeInt16Array(Stream* stream, int16_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeInt16(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of uint32_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeUInt32Array(Stream* stream, uint32_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeUInt32(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of int32_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeInt32Array(Stream* stream, int32_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeInt32(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of float
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeFloatArray(Stream* stream, float* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeFloat(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
#if STREAM_UINT64
/**
 * @brief write array of uint64_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeUInt64Array(Stream* stream, uint64_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeUInt64(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
/**
 * @brief write array of int64_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeInt64Array(Stream* stream, int64_t* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeInt64(stream, *val++)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
/**
 * @brief write array of double
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeDoubleArray(Stream* stream, double val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeDouble(stream, val)) != Stream_Ok) {
            return result;
        }
    }
    return result;
}
#endif // STREAM_DOUBLE
/**
 * @brief read array of characters
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readCharArray(Stream* stream, char* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readChar(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of uint8_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readUInt8Array(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readUInt8(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of int8_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readInt8Array(Stream* stream, int8_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readInt8(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of uint16_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readUInt16Array(Stream* stream, uint16_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readUInt16(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of int16_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readInt16Array(Stream* stream, int16_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readInt16(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of uint32_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readUInt32Array(Stream* stream, uint32_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readUInt32(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of int32_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readInt32Array(Stream* stream, int32_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readInt32(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of float
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readFloatArray(Stream* stream, float* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readFloat(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#if STREAM_UINT64
/**
 * @brief read array of uint64_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readUInt64Array(Stream* stream, uint64_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readUInt64(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief read array of int64_t
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readInt64Array(Stream* stream, int64_t* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readInt64(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
/**
 * @brief read array of double
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_readDoubleArray(Stream* stream, double* val, Stream_LenType len) {
    if (Stream_available(stream) >= len) {
        while (len-- > 0) {
            *val++ = Stream_readDouble(stream);
        }
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
#endif // STREAM_DOUBLE
#if STREAM_WRITE_LOCK
/**
 * @brief lock the stream for writing
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result Stream_lockWrite(Stream* stream, Stream* lock, Stream_LenType len) {
    Stream_LenType space = Stream_space(stream);
    if (space >= len && !stream->WriteLocked) {
        memcpy(lock, stream, sizeof(Stream));
        Stream_flipWrite(lock, len);
        stream->WriteLocked = 1;
        return Stream_Ok;
    }
    else {
        return Stream_NoSpace;
    }
}
/**
 * @brief unlock the stream for writing
 *
 * @param stream
 * @return Stream_Result
 */
void Stream_unlockWrite(Stream* stream, Stream* lock) {
    if (stream->WriteLocked) {
        if (stream->WPos != lock->WPos) {
            // some data wrote
            if (stream->WPos < lock->WPos) {
                Stream_moveWritePos(stream, lock->WPos - stream->WPos);
            }
            else {
                Stream_moveWritePos(stream, (stream->Size - stream->WPos) + lock->WPos);
            }
        }
        else if (stream->RPos == lock->RPos && 
            stream->Overflow == 0 &&
            lock->Overflow) {
            stream->Overflow = 1;
        }
        stream->WriteLocked = 0;
    }
}
/**
 * @brief unlock stream for write with ignore changes
 * 
 * @param stream 
 */
void Stream_unlockWriteIgnore(Stream* stream) {
    if (stream->WriteLocked) {
        stream->WriteLocked = 0;
    }
}
#endif // STREAM_WRITE_LOCK

#if STREAM_READ_LOCK
/**
 * @brief lock the stream for reading
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result Stream_lockRead(Stream* stream, Stream* lock, Stream_LenType len) {
    Stream_LenType available = Stream_available(stream);
    if (available >= len && !stream->ReadLocked) {
        memcpy(lock, stream, sizeof(Stream));
        Stream_flipRead(lock, len);
        stream->ReadLocked = 1;
        return Stream_Ok;
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief unlock the stream for reading
 *
 * @param stream
 * @return Stream_Result
 */
void Stream_unlockRead(Stream* stream, Stream* lock) {
    if (stream->ReadLocked) {
        if (stream->RPos != lock->RPos) {
            // some data read
            if (stream->RPos < lock->RPos) {
                Stream_moveReadPos(stream, lock->RPos - stream->RPos);
            }
            else {
                Stream_moveReadPos(stream, (stream->Size - stream->RPos) + lock->RPos);
            }
        }
        else if (stream->WPos == lock->WPos && 
            stream->Overflow != 0 &&
            !lock->Overflow) {
            stream->Overflow = 0;
        }
        stream->ReadLocked = 0;
    }
}
/**
 * @brief unlock stream for read with ignore changes
 * 
 * @param stream 
 */
void Stream_unlockReadIgnore(Stream* stream) {
    if (stream->ReadLocked) {
        stream->ReadLocked = 0;
    }
}
#endif // STREAM_READ_LOCK
// TODO: need to implement with more performance
static void memrcpy(void* dest, const void* src, int len) {
    uint8_t* pDest = (uint8_t*) dest;
    const uint8_t* pSrc = (const uint8_t*) src + len - 1;

    while (len-- > 0) {
        *pDest++ = *pSrc--;
    }
}

