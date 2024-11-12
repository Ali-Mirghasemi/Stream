#include "StreamBuffer.h"
#include <string.h>

/* Memory IO Macros */
#if   STREAM_MEM_IO == STREAM_MEM_IO_DEFAULT
    #define __memCopy(S, DEST, SRC, LEN)             STREAM_MEM_COPY((DEST), (SRC), (LEN))
    #define __memCopyReverse(S, DEST, SRC, LEN)      STREAM_MEM_COPY_REVERSE((DEST), (SRC), (LEN))
    #define __memSet(S, SRC, VAL, LEN)               STREAM_MEM_SET((SRC), (VAL), (LEN))
    #define __memReverse(S, SRC, LEN)                STREAM_MEM_REVERSE((SRC), (LEN))
#elif STREAM_MEM_IO == STREAM_MEM_IO_CUSTOM
    #define __memCopy(S, DEST, SRC, LEN)             (S)->Mem.copy((DEST), (SRC), (LEN))
    #define __memCopyReverse(S, DEST, SRC, LEN)      (S)->Mem.copyReverse((DEST), (SRC), (LEN))
    #define __memSet(S, SRC, VAL, LEN)               (S)->Mem.set((SRC), (VAL), (LEN))
    #define __memReverse(S, SRC, LEN)                (S)->Mem.reverse((SRC), (LEN))
#elif STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
    #define __memCopy(S, DEST, SRC, LEN)             (S)->Mem->copy((DEST), (SRC), (LEN))
    #define __memCopyReverse(S, DEST, SRC, LEN)      (S)->Mem->copyReverse((DEST), (SRC), (LEN))
    #define __memSet(S, SRC, VAL, LEN)               (S)->Mem->set((SRC), (VAL), (LEN))
    #define __memReverse(S, SRC, LEN)                (S)->Mem->reverse((SRC), (LEN))
#else
    #error "STREAM_MEM_IO is invalid!"
#endif

#if STREAM_BYTE_ORDER
/* private typedef */
typedef Stream_Result (*Stream_WriteBytesFn)(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_ReadBytesFn)(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_GetBytesFn)(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
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

    #define __writeBytes(STREAM, VAL, LEN)          writeBytes[STREAM->OrderFn]((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)           readBytes[STREAM->OrderFn]((STREAM), (VAL), (LEN))
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   getBytesAt[STREAM->OrderFn]((STREAM), (INDEX), (VAL), (LEN))

    #define __checkReverse(STREAM, VAL)             if (STREAM->OrderFn) {__memReverse((STREAM), &VAL, sizeof(VAL));}

#else
    #define __writeBytes(STREAM, VAL, LEN)          Stream_writeBytes((STREAM), (VAL), (LEN))
    #define __readBytes(STREAM, VAL, LEN)           Stream_readBytes((STREAM), (VAL), (LEN))
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   Stream_getBytesAt((STREAM), (INDEX), (VAL), (LEN))

    #define __checkReverse(STREAM, VAL)
#endif // STREAM_BYTE_ORDER

/* private function */
static void memrcpy(void* dest, const void* src, int len);
static void memreverse(void* arr, int len);

/* StreamBuffer MemIO default driver*/
#if STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
static const Stream_MemIO STREAM_DEFAULT_DRIVER = {
    (Stream_MemCopyFn) STREAM_MEM_COPY,
    (Stream_MemCopyReverseFn) STREAM_MEM_COPY_REVERSE,
    (Stream_MemSetFn) STREAM_MEM_SET,
    (Stream_MemReverseFn) STREAM_MEM_REVERSE,
};
#endif

/**
 * @brief initialize stream
 *
 * @param stream address of stream struct
 * @param buffer address of byte buffer
 * @param size size of buffer
 */
void Stream_init(StreamBuffer* stream, uint8_t* buffer, Stream_LenType size) {
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
#if   STREAM_MEM_IO == STREAM_MEM_IO_CUSTOM
    Stream_setMemIO(
        stream,
        (Stream_MemCopyFn) STREAM_MEM_COPY,
        (Stream_MemCopyReverseFn) STREAM_MEM_COPY_REVERSE,
        (Stream_MemSetFn) STREAM_MEM_SET,
        (Stream_MemReverseFn) STREAM_MEM_REVERSE
    );
#elif STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
    Stream_setMemIO(stream, &STREAM_DEFAULT_DRIVER);
#endif
}
/**
 * @brief initialize stream with a buffer that already have data in it
 *
 * @param stream
 * @param buffer
 * @param size
 */
void Stream_fromBuff(StreamBuffer* stream, uint8_t* buffer, Stream_LenType size, Stream_LenType len) {
    Stream_init(stream, buffer, size);
    stream->WPos = len;
}
/**
 * @brief reset stream struct into default values
 *
 * @param stream address of stream
 */
void Stream_deinit(StreamBuffer* stream) {
    __memSet(stream, stream, 0, sizeof(StreamBuffer));
}
#if STREAM_MEM_IO == STREAM_MEM_IO_CUSTOM
/**
 * @brief Set custom memory io functions
 * if input is null use default functions
 *
 * @param stream
 * @param copy
 * @param copyReverse
 * @param set
 * @param reverse
 */
void Stream_setMemIO(
    StreamBuffer*                 stream,
    Stream_MemCopyFn          copy,
    Stream_MemCopyReverseFn   copyReverse,
    Stream_MemSetFn           set,
    Stream_MemReverseFn       reverse
) {
    stream->Mem.copy = copy ? copy : (Stream_MemCopyFn) STREAM_MEM_COPY;
    stream->Mem.copyReverse = copyReverse ? copyReverse : (Stream_MemCopyReverseFn) STREAM_MEM_COPY_REVERSE;
    stream->Mem.set = set ? set : (Stream_MemSetFn) STREAM_MEM_SET;
    stream->Mem.reverse = reverse ? reverse : (Stream_MemReverseFn) STREAM_MEM_REVERSE;
}
#elif STREAM_MEM_IO == STREAM_MEM_IO_DRIVER
/**
 * @brief Set memory io driver
 * if input is null use default driver. all function must exists
 *
 * @param stream
 * @param mem
 */
void Stream_setMemIO(StreamBuffer* stream, const Stream_MemIO* mem) {
    stream->Mem = mem ? mem : &STREAM_DEFAULT_DRIVER;
}
#endif

/**
 * @brief return available bytes to read
 *
 * @param stream
 * @return Stream_LenType available bytes
 */
Stream_LenType Stream_availableReal(StreamBuffer* stream) {
    return stream->Size * stream->Overflow + stream->WPos - stream->RPos;
}
/**
 * @brief return buffer space for write bytes
 *
 * @param stream
 * @return Stream_LenType space for write
 */
Stream_LenType Stream_spaceReal(StreamBuffer* stream) {
    return stream->Size * !stream->Overflow + stream->RPos - stream->WPos;
}
/**
 * @brief check stream is empty
 * no bytes available to read
 * @param stream
 * @return uint8_t 0 -> Not Empty, 1-> it's empty
 */
uint8_t Stream_isEmpty(StreamBuffer* stream) {
    return stream->RPos == stream->WPos && !stream->Overflow;
}
/**
 * @brief check stream it's full
 * no bytes can write
 *
 * @param stream
 * @return uint8_t 0 -> Not Full, 1-> it's Full
 */
uint8_t Stream_isFull(StreamBuffer* stream) {
    return stream->RPos == stream->WPos && stream->Overflow;
}
/**
 * @brief reset stream
 *
 * @param stream
 */
void Stream_reset(StreamBuffer* stream) {
    stream->RPos = 0;
    stream->WPos = 0;
    stream->Overflow = 0;
}
/**
 * @brief reset stream and ignore receive and transmit operations
 *
 * @param stream
 */
void Stream_resetIO(StreamBuffer* stream) {
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
void Stream_clear(StreamBuffer* stream) {
    Stream_reset(stream);
    __memSet(stream, stream->Data, 0, stream->Size);
}
/**
 * @brief
 *
 * @param stream
 * @param mode
 */
void Stream_setFlushMode(StreamBuffer* stream, Stream_FlushMode mode) {
    stream->FlushMode = mode;
}
/**
 * @brief return in receive flag
 *
 * @param stream
 * @return uint8_t
 */
uint8_t Stream_inReceive(StreamBuffer* stream) {
    return stream->InReceive;
}
/**
 * @brief return in transmit flag
 *
 * @param stream
 * @return uint8_t
 */
uint8_t Stream_inTransmit(StreamBuffer* stream) {
    return stream->InTransmit;
}
/**
 * @brief return Write Pos
 *
 * @param stream
 * @return Stream_LenType it's between 0 ~ Size
 */
Stream_LenType Stream_getWritePos(StreamBuffer* stream) {
    return stream->WPos;
}
/**
 * @brief return Read Pos
 *
 * @param stream
 * @return Stream_LenType it's between 0 ~ Size
 */
Stream_LenType Stream_getReadPos(StreamBuffer* stream) {
    return stream->RPos;
}
/**
 * @brief return number of bytes that it's in row in the ram
 * use for transmit function in OStream
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_directAvailable(StreamBuffer* stream) {
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
Stream_LenType Stream_directSpace(StreamBuffer* stream) {
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
Stream_LenType Stream_directAvailableAt(StreamBuffer* stream, Stream_LenType index) {
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
Stream_LenType Stream_directSpaceAt(StreamBuffer* stream, Stream_LenType index) {
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
uint8_t* Stream_getWritePtr(StreamBuffer* stream) {
    return &stream->Data[stream->WPos];
}
/**
 * @brief get ptr to start of RPos in ram
 *
 * @param stream
 * @return uint8_t*
 */
uint8_t* Stream_getReadPtr(StreamBuffer* stream) {
    return &stream->Data[stream->RPos];
}
uint8_t* Stream_getWritePtrAt(StreamBuffer* stream, Stream_LenType index) {
    index += stream->WPos;

    if (index >= stream->Size) {
        index %= stream->Size;
    }

    return &stream->Data[index];
}
uint8_t* Stream_getReadPtrAt(StreamBuffer* stream, Stream_LenType index) {
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
void Stream_setBuffer(StreamBuffer* stream, uint8_t* data, Stream_LenType size) {
    Stream_init(stream, data, size);
}
/**
 * @brief return byte buffer
 *
 * @param stream
 * @return uint8_t*
 */
uint8_t* Stream_getBuffer(StreamBuffer* stream) {
    return stream->Data;
}
/**
 * @brief return size of stream buffer
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_getBufferSize(StreamBuffer* stream) {
    return stream->Size;
}
/**
 * @brief can use for ignore bytes, and move WPos index
 *
 * @param stream
 * @param steps
 * @return Stream_Result
 */
Stream_Result Stream_moveWritePos(StreamBuffer* stream, Stream_LenType steps) {
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
Stream_Result Stream_moveReadPos(StreamBuffer* stream, Stream_LenType steps) {
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
void Stream_flipWrite(StreamBuffer* stream, Stream_LenType len) {
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
void Stream_flipRead(StreamBuffer* stream, Stream_LenType len) {
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
void       Stream_setByteOrder(StreamBuffer* stream, ByteOrder order) {
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
ByteOrder  Stream_getByteOrder(StreamBuffer* stream) {
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
void       Stream_setWriteLimit(StreamBuffer* stream, Stream_LenType len) {
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
uint8_t    Stream_isWriteLimited(StreamBuffer* stream) {
    return stream->WriteLimit >= 0;
}
/**
 * @brief return space available for write bytes respect to write limit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_spaceLimit(StreamBuffer* stream) {
    return stream->WriteLimit >= 0 ? stream->WriteLimit : Stream_spaceReal(stream);
}
Stream_LenType Stream_getWriteLimit(StreamBuffer* stream) {
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
void Stream_setReadLimit(StreamBuffer* stream, Stream_LenType len) {
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
uint8_t Stream_isReadLimited(StreamBuffer* stream) {
    return stream->ReadLimit >= 0;
}
/**
 * @brief retruna available bytes for read respect to read limit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_availableLimit(StreamBuffer* stream) {
    return stream->ReadLimit >= 0 ? stream->ReadLimit : Stream_availableReal(stream);
}
/**
 * @brief return read limit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType Stream_getReadLimit(StreamBuffer* stream) {
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
void Stream_getCursor(StreamBuffer* stream, Stream_Cursor* cursor) {
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
Stream_LenType Stream_getReadLen(StreamBuffer* stream, Stream_Cursor* cursor) {
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
Stream_LenType Stream_getWriteLen(StreamBuffer* stream, Stream_Cursor* cursor) {
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
Stream_Result Stream_writeBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        __memCopy(stream, &stream->Data[stream->WPos], val, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = 0;
        stream->Overflow = 1;
    }
    if (len > 0) {
        __memCopy(stream, &stream->Data[stream->WPos], val, len);
        // move WPos
        stream->WPos += len;
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
Stream_Result Stream_writeBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        __memCopyReverse(stream, &stream->Data[stream->WPos], val + len, tmpLen);
        // move WPos
        stream->WPos = 0;
        stream->Overflow = 1;
    }
    if (len > 0) {
        __memCopyReverse(stream, &stream->Data[stream->WPos], val, len);
        // move WPos
        stream->WPos += len;
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
Stream_Result Stream_writeStream(StreamBuffer* out, StreamBuffer* in, Stream_LenType len) {
    // check available space for write
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        out->WPos = 0;
        out->Overflow = 1;
    }
    if (len > 0) {
        Stream_readBytes(in, &out->Data[out->WPos], len);
        // move WPos
        out->WPos += len;
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
Stream_Result Stream_writePadding(StreamBuffer* stream, uint8_t val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        __memSet(stream, &stream->Data[stream->WPos], val, tmpLen);
        val += tmpLen;
        // move WPos
        stream->WPos = 0;
        stream->Overflow = 1;
    }
    if (len > 0) {
        __memSet(stream, &stream->Data[stream->WPos], val, len);
        // move WPos
        stream->WPos += len;
    }

    return Stream_Ok;
}
Stream_Result Stream_writeChar(StreamBuffer* stream, char val) {
    return Stream_writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeStr(StreamBuffer* stream, const char* val) {
    return Stream_writeBytes(stream, (uint8_t*) val, strlen(val));
}
Stream_Result Stream_writeUInt8(StreamBuffer* stream, uint8_t val) {
    return Stream_writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt8(StreamBuffer* stream, int8_t val) {
    return Stream_writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeUInt16(StreamBuffer* stream, uint16_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt16(StreamBuffer* stream, int16_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeUInt32(StreamBuffer* stream, uint32_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt32(StreamBuffer* stream, int32_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeFloat(StreamBuffer* stream, float val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
#if STREAM_UINT64
Stream_Result Stream_writeUInt64(StreamBuffer* stream, uint64_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
Stream_Result Stream_writeInt64(StreamBuffer* stream, int64_t val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
Stream_Result Stream_writeDouble(StreamBuffer* stream, double val) {
    return __writeBytes(stream, (uint8_t*) &val, sizeof(val));
}
#endif // STREAM_DOUBLE

/**
 * @brief read one byte from stream if available return value between 0~255, else return -1
 *
 * @param stream
 * @return int16_t
 */
int16_t  Stream_read(StreamBuffer* stream) {
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
Stream_Result Stream_readBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        __memCopy(stream, val, &stream->Data[stream->RPos], tmpLen);
        val += tmpLen;
        // move RPos
        stream->RPos = 0;
        stream->Overflow = 0;
    }
    if (len > 0) {
        __memCopy(stream, val, &stream->Data[stream->RPos], len);
        // move RPos
        stream->RPos += len;
    }

    return Stream_Ok;
}
Stream_Result Stream_readBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        __memCopyReverse(stream, val + len, &stream->Data[stream->RPos], tmpLen);
        // move RPos
        stream->RPos = 0;
        stream->Overflow = 0;
    }
    if (len > 0) {
        __memCopyReverse(stream, val, &stream->Data[stream->RPos], len);
        // move RPos
        stream->RPos += len;
    }

    return Stream_Ok;
}
Stream_Result Stream_readStream(StreamBuffer* in, StreamBuffer* out, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
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
        in->RPos = 0;
        in->Overflow = 0;
    }
    if (len > 0) {
        Stream_writeBytes(out, &in->Data[in->RPos], len);
        // move RPos
        in->RPos += len;
    }

    return Stream_Ok;
}
char     Stream_readChar(StreamBuffer* stream) {
    char val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
uint8_t  Stream_readUInt8(StreamBuffer* stream) {
    uint8_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int8_t   Stream_readInt8(StreamBuffer* stream) {
    int8_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
uint16_t Stream_readUInt16(StreamBuffer* stream) {
    uint16_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int16_t  Stream_readInt16(StreamBuffer* stream) {
    int16_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
uint32_t Stream_readUInt32(StreamBuffer* stream) {
    uint32_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int32_t Stream_readInt32(StreamBuffer* stream) {
    int32_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
float Stream_readFloat(StreamBuffer* stream) {
    float val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
#if STREAM_UINT64
uint64_t Stream_readUInt64(StreamBuffer* stream) {
    uint64_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
int64_t Stream_readInt64(StreamBuffer* stream) {
    int64_t val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_readDouble(StreamBuffer* stream) {
    double val = STREAM_READ_DEFAULT_VALUE;
    __readBytes(stream, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_DOUBLE

#if STREAM_GET_AT_FUNCTIONS && STREAM_GET_FUNCTIONS

Stream_Result Stream_getBytes(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
    return Stream_getBytesAt(stream, stream->RPos, val, len);
}
Stream_Result Stream_getBytesReverse(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
    return Stream_getBytesAt(stream, stream->RPos, val, len);
}
char     Stream_getChar(StreamBuffer* stream) {
    return Stream_getCharAt(stream, stream->RPos);
}
uint8_t  Stream_getUInt8(StreamBuffer* stream) {
    return Stream_getUInt8At(stream, stream->RPos);
}
int8_t   Stream_getInt8(StreamBuffer* stream) {
    return Stream_getInt8At(stream, stream->RPos);
}
uint16_t Stream_getUInt16(StreamBuffer* stream) {
    return Stream_getUInt16At(stream, stream->RPos);
}
int16_t  Stream_getInt16(StreamBuffer* stream) {
    return Stream_getInt16At(stream, stream->RPos);
}
uint32_t Stream_getUInt32(StreamBuffer* stream) {
    return Stream_getUInt32At(stream, stream->RPos);
}
int32_t  Stream_getInt32(StreamBuffer* stream) {
    return Stream_getInt32At(stream, stream->RPos);
}
float    Stream_getFloat(StreamBuffer* stream) {
    return Stream_getFloatAt(stream, stream->RPos);
}
#if STREAM_UINT64
uint64_t Stream_getUInt64(StreamBuffer* stream) {
    return Stream_getUInt64At(stream, stream->RPos);
}
int64_t  Stream_getInt64(StreamBuffer* stream) {
    return Stream_getInt64At(stream, stream->RPos);
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_getDouble(StreamBuffer* stream) {
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
Stream_Result Stream_getCharArray(StreamBuffer* stream, char* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt8Array(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt8Array(StreamBuffer* stream, int8_t* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt16Array(StreamBuffer* stream, uint16_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt16Array(StreamBuffer* stream, int16_t* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt32Array(StreamBuffer* stream, uint32_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt32Array(StreamBuffer* stream, int32_t* val, Stream_LenType len) {
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
Stream_Result Stream_getFloatArray(StreamBuffer* stream, float* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt64Array(StreamBuffer* stream, uint64_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt64Array(StreamBuffer* stream, int64_t* val, Stream_LenType len) {
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
Stream_Result Stream_getDoubleArray(StreamBuffer* stream, double* val, Stream_LenType len) {
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

Stream_Result Stream_getBytesAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
    if (Stream_available(stream) - index < len) {
        return Stream_NoAvailable;
    }

    index = (stream->RPos + index) % stream->Size;

    if (index + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - index;
        len -= tmpLen;
        __memCopy(stream, val, &stream->Data[index], tmpLen);
        val += tmpLen;
        index = (index + tmpLen) % stream->Size;
    }

    __memCopy(stream, val, &stream->Data[index], len);

    return Stream_Ok;
}
Stream_Result Stream_getBytesReverseAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
      return Stream_ZeroLen;
    }
#endif
    if (Stream_available(stream) - index < len) {
        return Stream_NoAvailable;
    }

    index = (stream->RPos + index) % stream->Size;

    if (index + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - index;
        len -= tmpLen;
        __memCopyReverse(stream, val, &stream->Data[index + len], tmpLen);
        val += tmpLen;
        index = (index + tmpLen) % stream->Size;
    }

    __memCopyReverse(stream, val, &stream->Data[index], len);

    return Stream_Ok;
}
char Stream_getCharAt(StreamBuffer* stream, Stream_LenType index) {
    char val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
uint8_t Stream_getUInt8At(StreamBuffer* stream, Stream_LenType index) {
    uint8_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int8_t Stream_getInt8At(StreamBuffer* stream, Stream_LenType index) {
    int8_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
uint16_t Stream_getUInt16At(StreamBuffer* stream, Stream_LenType index) {
    uint16_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int16_t Stream_getInt16At(StreamBuffer* stream, Stream_LenType index) {
    int16_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
uint32_t Stream_getUInt32At(StreamBuffer* stream, Stream_LenType index) {
    uint32_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int32_t Stream_getInt32At(StreamBuffer* stream, Stream_LenType index) {
    int32_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
float Stream_getFloatAt(StreamBuffer* stream, Stream_LenType index) {
    float val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
#if STREAM_UINT64
uint64_t Stream_getUInt64At(StreamBuffer* stream, Stream_LenType index) {
    uint64_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
int64_t  Stream_getInt64At(StreamBuffer* stream, Stream_LenType index) {
    int64_t val = STREAM_READ_DEFAULT_VALUE;
    __getBytesAt(stream, index, (uint8_t*) &val, sizeof(val));
    return val;
}
#endif // STREAM_UINT64
#if STREAM_DOUBLE
double   Stream_getDoubleAt(StreamBuffer* stream, Stream_LenType index) {
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
Stream_Result Stream_getCharArrayAt(StreamBuffer* stream, Stream_LenType index, char* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt8ArrayAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt8ArrayAt(StreamBuffer* stream, Stream_LenType index, int8_t* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt16ArrayAt(StreamBuffer* stream, Stream_LenType index, uint16_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt16ArrayAt(StreamBuffer* stream, Stream_LenType index, int16_t* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt32ArrayAt(StreamBuffer* stream, Stream_LenType index, uint32_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt32ArrayAt(StreamBuffer* stream, Stream_LenType index, int32_t* val, Stream_LenType len) {
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
Stream_Result Stream_getFloatArrayAt(StreamBuffer* stream, Stream_LenType index, float* val, Stream_LenType len) {
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
Stream_Result Stream_getUInt64ArrayAt(StreamBuffer* stream, Stream_LenType index, uint64_t* val, Stream_LenType len) {
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
Stream_Result Stream_getInt64ArrayAt(StreamBuffer* stream, Stream_LenType index, int64_t* val, Stream_LenType len) {
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
Stream_Result Stream_getDoubleArrayAt(StreamBuffer* stream, Stream_LenType index, double* val, Stream_LenType len) {
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
 * @brief write array of characters
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result Stream_writeCharArray(StreamBuffer* stream, char* val, Stream_LenType len) {
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
Stream_Result Stream_writeUInt8Array(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeInt8Array(StreamBuffer* stream, int8_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeUInt16Array(StreamBuffer* stream, uint16_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeInt16Array(StreamBuffer* stream, int16_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeUInt32Array(StreamBuffer* stream, uint32_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeInt32Array(StreamBuffer* stream, int32_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeFloatArray(StreamBuffer* stream, float* val, Stream_LenType len) {
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
Stream_Result Stream_writeUInt64Array(StreamBuffer* stream, uint64_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeInt64Array(StreamBuffer* stream, int64_t* val, Stream_LenType len) {
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
Stream_Result Stream_writeDoubleArray(StreamBuffer* stream, double* val, Stream_LenType len) {
    Stream_Result result = Stream_Ok;
    while (len-- > 0) {
        if ((result = Stream_writeDouble(stream, *val++)) != Stream_Ok) {
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
Stream_Result Stream_readCharArray(StreamBuffer* stream, char* val, Stream_LenType len) {
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
Stream_Result Stream_readUInt8Array(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
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
Stream_Result Stream_readInt8Array(StreamBuffer* stream, int8_t* val, Stream_LenType len) {
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
Stream_Result Stream_readUInt16Array(StreamBuffer* stream, uint16_t* val, Stream_LenType len) {
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
Stream_Result Stream_readInt16Array(StreamBuffer* stream, int16_t* val, Stream_LenType len) {
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
Stream_Result Stream_readUInt32Array(StreamBuffer* stream, uint32_t* val, Stream_LenType len) {
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
Stream_Result Stream_readInt32Array(StreamBuffer* stream, int32_t* val, Stream_LenType len) {
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
Stream_Result Stream_readFloatArray(StreamBuffer* stream, float* val, Stream_LenType len) {
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
Stream_Result Stream_readUInt64Array(StreamBuffer* stream, uint64_t* val, Stream_LenType len) {
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
Stream_Result Stream_readInt64Array(StreamBuffer* stream, int64_t* val, Stream_LenType len) {
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
Stream_Result Stream_readDoubleArray(StreamBuffer* stream, double* val, Stream_LenType len) {
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
Stream_Result Stream_lockWrite(StreamBuffer* stream, StreamBuffer* lock, Stream_LenType len) {
    Stream_LenType space = Stream_space(stream);
    if (space >= len && !stream->WriteLocked) {
        __memCopy(stream, lock, stream, sizeof(StreamBuffer));
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
void Stream_unlockWrite(StreamBuffer* stream, StreamBuffer* lock) {
    if (stream->WriteLocked) {
        Stream_moveWritePos(stream, Stream_lockWriteLen(stream, lock));
        stream->WriteLocked = 0;
    }
}
/**
 * @brief unlock stream for write with ignore changes
 *
 * @param stream
 */
void Stream_unlockWriteIgnore(StreamBuffer* stream) {
    if (stream->WriteLocked) {
        stream->WriteLocked = 0;
    }
}
/**
 * @brief return number of byte write in lock
 *
 * @param stream
 * @param lock
 */
Stream_LenType Stream_lockWriteLen(StreamBuffer* stream, StreamBuffer* lock) {
    if (stream->WPos != lock->WPos) {
        // some data wrote
        if (stream->WPos < lock->WPos) {
            return  lock->WPos - stream->WPos;
        }
        else {
            return (stream->Size - stream->WPos) + lock->WPos;
        }
    }
    else if (stream->RPos == lock->RPos &&
        stream->Overflow == 0 &&
        lock->Overflow) {

        return stream->Size;
    }
    else {
        return 0;
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
Stream_Result Stream_lockRead(StreamBuffer* stream, StreamBuffer* lock, Stream_LenType len) {
    Stream_LenType available = Stream_available(stream);
    if (available >= len && !stream->ReadLocked) {
        __memCopy(stream, lock, stream, sizeof(StreamBuffer));
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
void Stream_unlockRead(StreamBuffer* stream, StreamBuffer* lock) {
    if (stream->ReadLocked) {
        Stream_moveReadPos(stream, Stream_lockReadLen(stream, lock));
        stream->ReadLocked = 0;
    }
}
/**
 * @brief return number of bytes that read
 *
 * @param stream
 * @param lock
 * @return Stream_LenType
 */
Stream_LenType Stream_lockReadLen(StreamBuffer* stream, StreamBuffer* lock) {
    if (stream->RPos != lock->RPos) {
        // some data read
        if (stream->RPos < lock->RPos) {
            return lock->RPos - stream->RPos;
        }
        else {
            return (stream->Size - stream->RPos) + lock->RPos;
        }
    }
    else if (stream->WPos == lock->WPos &&
        stream->Overflow != 0 &&
        !lock->Overflow) {

        return stream->Size;
    }
    else {
        return 0;
    }
}
/**
 * @brief unlock stream for read with ignore changes
 *
 * @param stream
 */
void Stream_unlockReadIgnore(StreamBuffer* stream) {
    if (stream->ReadLocked) {
        stream->ReadLocked = 0;
    }
}
#endif // STREAM_READ_LOCK

#if STREAM_FIND_FUNCTIONS
Stream_LenType Stream_findByte(StreamBuffer* stream, uint8_t val) {
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
Stream_LenType Stream_findByteAt(StreamBuffer* stream, Stream_LenType offset, uint8_t val) {
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
Stream_LenType Stream_findPattern(StreamBuffer* stream, const uint8_t* pat, Stream_LenType patLen) {
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
Stream_LenType Stream_findPatternAt(StreamBuffer* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen)  {
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
Stream_LenType Stream_readBytesUntil(StreamBuffer* stream, uint8_t end, uint8_t* val, Stream_LenType len) {
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
Stream_LenType Stream_readBytesUntilPattern(StreamBuffer* stream, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len) {
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
Stream_LenType Stream_readBytesUntilPatternAt(StreamBuffer* stream, Stream_LenType offset, const uint8_t* pat, Stream_LenType patLen, uint8_t* val, Stream_LenType len){
    Stream_LenType tmpLen;
    // find end byte
    if ((tmpLen = Stream_findPatternAt(stream, offset, pat, patLen)) >= 0) {
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
 * @brief find a uint8_t value in stream
 *
 * @param stream
 * @param val
 * @return Stream_LenType
 */
Stream_LenType Stream_findChar(StreamBuffer* stream, char val) {
    return Stream_findByte(stream, val);
}
Stream_LenType Stream_findUInt8(StreamBuffer* stream, uint8_t val) {
    return Stream_findByte(stream, val);
}
Stream_LenType Stream_findInt8(StreamBuffer* stream, int8_t val) {
    return Stream_findByte(stream, (uint8_t) val);
}
Stream_LenType Stream_findUInt16(StreamBuffer* stream, uint16_t val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt16(StreamBuffer* stream, int16_t val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findUInt32(StreamBuffer* stream, uint32_t val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt32(StreamBuffer* stream, int32_t val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
#if STREAM_UINT64
Stream_LenType Stream_findUInt64(StreamBuffer* stream, uint64_t val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt64(StreamBuffer* stream, int64_t val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
#endif
Stream_LenType Stream_findFloat(StreamBuffer* stream, float val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
#if STREAM_DOUBLE
Stream_LenType Stream_findDouble(StreamBuffer* stream, double val) {
    __checkReverse(stream, val);
    return Stream_findPattern(stream, (uint8_t*) &val, sizeof(val));
}
#endif

#if STREAM_FIND_AT_FUNCTIONS
Stream_LenType Stream_findCharAt(StreamBuffer* stream, Stream_LenType offset, char val) {
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findUInt8At(StreamBuffer* stream, Stream_LenType offset, uint8_t val) {
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt8At(StreamBuffer* stream, Stream_LenType offset, int8_t val) {
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findUInt16At(StreamBuffer* stream, Stream_LenType offset, uint16_t val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt16At(StreamBuffer* stream, Stream_LenType offset, int16_t val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findUInt32At(StreamBuffer* stream, Stream_LenType offset, uint32_t val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt32At(StreamBuffer* stream, Stream_LenType offset, int32_t val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
#if STREAM_UINT64
Stream_LenType Stream_findUInt64At(StreamBuffer* stream, Stream_LenType offset, uint64_t val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
Stream_LenType Stream_findInt64At(StreamBuffer* stream, Stream_LenType offset, int64_t val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
#endif
Stream_LenType Stream_findFloatAt(StreamBuffer* stream, Stream_LenType offset, float val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
#if STREAM_DOUBLE
Stream_LenType Stream_findDoubleAt(StreamBuffer* stream, Stream_LenType offset, double val) {
    __checkReverse(stream, val);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, sizeof(val));
}
#endif
#endif // STREAM_FIND_AT_FUNCTIONS

#endif // STREAM_FIND_FUNCTIONS

/**
 * @brief compare a given bytes at index with available bytes in stream
 *
 * @param stream
 * @param val
 * @param len
 * @return int8_t
 */
int8_t Stream_compare(StreamBuffer* stream, const uint8_t* val, Stream_LenType len) {
    return Stream_compareAt(stream, 0, val, len);
}
/**
 * @brief compare a given bytes at index with available bytes in stream
 *
 * @param stream
 * @param index
 * @param val
 * @param len
 * @return int8_t
 */
int8_t Stream_compareAt(StreamBuffer* stream, Stream_LenType index, const uint8_t* val, Stream_LenType len) {
    int8_t result;
    Stream_LenType tmpLen;

    if (len == 0) {
      return 0;
    }

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
// TODO: need to implement with more performance
static void memrcpy(void* dest, const void* src, int len) {
    uint8_t* pDest = (uint8_t*) dest;
    const uint8_t* pSrc = (const uint8_t*) src + len - 1;

    while (len-- > 0) {
        *pDest++ = *pSrc--;
    }
}
static void memreverse(void* arr, int len) {
    uint8_t* pSrc = (uint8_t*) arr;
    uint8_t* pDest = (uint8_t*) arr + len - 1;
    uint8_t temp;

    len >>= 1;
    while (len-- > 0) {
        temp = *pSrc;
        *pSrc++ = *pDest;
        *pDest-- = temp;
    }
}
