#include "Stream.h"
#include <string.h>

#if STREAM_BYTE_ORDER
/* private typedef */
typedef Stream_Result (*Stream_writeBytesFn)(Stream* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_readBytesFn)(Stream* stream, uint8_t* val, Stream_LenType len);
/* private variables */
static const Stream_writeBytesFn writeBytes[2] = {
    (Stream_writeBytesFn) Stream_writeBytes,
    (Stream_writeBytesFn) Stream_writeBytesReverse,
};
static const Stream_readBytesFn readBytes[2] = {
    (Stream_writeBytesFn) Stream_readBytes,
    (Stream_writeBytesFn) Stream_readBytesReverse,
};

    #define __writeBytes(STREAM, VAL, LEN)      writeBytes[stream->Order]((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)       readBytes[stream->Order]((STREAM), (VAL), (LEN))
#else
    #define __writeBytes(STREAM, VAL, LEN)      Stream_writeBytes((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)       Stream_readBytes((STREAM), (VAL), (LEN))
#endif
/* private function */
void memrcpy(void* dest, const void* src, int len);


void Stream_init(Stream* stream, uint8_t* buffer, Stream_LenType size) {
    stream->Data = buffer;
    stream->Size = size;
#if STREAM_BYTE_ORDER
    stream->Order = Stream_getSystemByteOrder();
#endif // STREAM_BYTE_ORDER
    stream->RPos = 0;
    stream->WPos = 0;
}
void Stream_deinit(Stream* stream) {
    memset(stream, 0, sizeof(Stream));
}

/*************** General APIs *************/
Stream_LenType Stream_available(Stream* stream) {
    if (stream->WPos > stream->RPos) {
        return stream->WPos - stream->RPos;
    }
    else if (stream->WPos < stream->RPos) {
        return stream->WPos + (stream->Size - stream->RPos);
    }
    else {
        return stream->Overflow ? stream->Size : 0;
    }
}
Stream_LenType Stream_space(Stream* stream) {
    return stream->Size - Stream_available(stream);
}
uint8_t Stream_isEmpty(Stream* stream) {
    return Stream_available(stream) == 0;
}
uint8_t Stream_isFull(Stream* stream) {
    return Stream_available(stream) == stream->Size;
}

void Stream_clear(Stream* stream) {
    stream->RPos = 0;
    stream->WPos = 0;
}

uint8_t* Stream_getBuffer(Stream* stream) {
    return stream->Data;
}

Stream_LenType Stream_getBufferSize(Stream* stream) {
    return stream->Size;
}

#if STREAM_BYTE_ORDER
ByteOrder  Stream_getSystemByteOrder(void) {
    const uint8_t arr[2] = {0xAA, 0xBB};
    const uint16_t val = 0xAABB;
    return (ByteOrder) (memcmp(arr, (uint8_t*) &val, sizeof(val)) == 0);
}
void       Stream_setByteOrder(Stream* stream, ByteOrder order) {
    stream->Order = order;
}
ByteOrder  Stream_getByteOrder(Stream* stream) {
    return stream->Order;
}
#endif

/**************** Write APIs **************/
Stream_Result Stream_writeBytes(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_space(stream) < len) {
        return Stream_NoSpace;
    }

    if (stream->WPos + len > stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->WPos;
        len -= tmpLen;
        memcpy(&stream->Data[stream->WPos], val, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = (stream->WPos + tmpLen) % stream->Size;
        stream->Overflow = 1;
    }

    memcpy(&stream->Data[stream->WPos], val, len);
    // move WPos
    stream->WPos = (stream->WPos + len) % stream->Size;

    return Stream_Ok;
}
Stream_Result Stream_writeBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_space(stream) < len) {
        return Stream_NoSpace;
    }

    if (stream->WPos + len > stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->WPos;
        len -= tmpLen;
        memrcpy(&stream->Data[stream->WPos], val + len, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = (stream->WPos + tmpLen) % stream->Size;
        stream->Overflow = 1;
    }

    memrcpy(&stream->Data[stream->WPos], val, len);
    // move WPos
    stream->WPos = (stream->WPos + len) % stream->Size;

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


/**************** Read APIs **************/
Stream_LenType Stream_readBytes(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) < len) {
        return Stream_NoAvailable;
    }

    if (stream->RPos + len > stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->RPos;
        len -= tmpLen;
        memcpy(val, &stream->Data[stream->RPos], tmpLen);
        val += tmpLen;
        // move RPos
        stream->RPos = (stream->RPos + tmpLen) % stream->Size;
        stream->Overflow = 0;
    }

    memcpy(val, &stream->Data[stream->RPos], len);
    // move RPos
    stream->RPos = (stream->RPos + len) % stream->Size;

    return Stream_Ok;
}
Stream_LenType Stream_readBytesReverse(Stream* stream, uint8_t* val, Stream_LenType len) {
    if (Stream_available(stream) < len) {
        return Stream_NoAvailable;
    }

    if (stream->RPos + len > stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - stream->RPos;
        len -= tmpLen;
        memrcpy(val, &stream->Data[stream->RPos + len], tmpLen);
        val += tmpLen;
        // move RPos
        stream->RPos = (stream->RPos + tmpLen) % stream->Size;
    }

    memrcpy(val, &stream->Data[stream->RPos], len);
    // move RPos
    stream->RPos = (stream->RPos + len) % stream->Size;

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

// TODO: need to implement with more performance
void memrcpy(void* dest, const void* src, int len) {
    uint8_t* pDest = (uint8_t*) dest;
    const uint8_t* pSrc = (const uint8_t*) src + len - 1;

    while (len-- > 0) {
        *pDest++ = *pSrc--;
    }
}

