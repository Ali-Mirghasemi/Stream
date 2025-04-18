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
#endif // STREAM_MEM_IO

/* private typedef */
typedef Stream_Result (*Stream_WriteBytesFn)(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_ReadBytesFn)(StreamBuffer* stream, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_GetBytesFn)(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
typedef Stream_Result (*Stream_SetBytesFn)(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len);
/* private variables */
#if STREAM_WRITE_REVERSE
static const Stream_WriteBytesFn writeBytes[2] = {
    Stream_writeBytes,
    Stream_writeBytesReverse,
};

    #define __writeBytes(STREAM, VAL, LEN)          writeBytes[STREAM->OrderFn]((STREAM), (VAL), (LEN))    
#else
    #define __writeBytes(STREAM, VAL, LEN)          Stream_writeBytes((STREAM), (VAL), (LEN))
#endif // STREAM_BYTE_ORDER

#if STREAM_READ_REVERSE
static const Stream_ReadBytesFn readBytes[2] = {
    Stream_readBytes,
    Stream_readBytesReverse,
};

    #define __readBytes(STREAM, VAL, LEN)           readBytes[STREAM->OrderFn]((STREAM), (VAL), (LEN))
#else
    #define __readBytes(STREAM, VAL, LEN)           Stream_readBytes((STREAM), (VAL), (LEN))
#endif

#if STREAM_GET_AT_BYTES_REVERSE
static const Stream_GetBytesFn getBytesAt[2] = {
    Stream_getBytesAt,
    Stream_getBytesReverseAt,
};

    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   getBytesAt[STREAM->OrderFn]((STREAM), (INDEX), (VAL), (LEN))
#else
    #define __getBytesAt(STREAM, INDEX, VAL, LEN)   Stream_getBytesAt((STREAM), (INDEX), (VAL), (LEN))
#endif // STREAM_GET_AT

#if STREAM_SET_AT_BYTES_REVERSE
static const Stream_SetBytesFn setBytesAt[2] = {
    Stream_setBytesAt,
    Stream_setBytesReverseAt,
};

    #define __setBytesAt(STREAM, INDEX, VAL, LEN)   setBytesAt[STREAM->OrderFn]((STREAM), (INDEX), (VAL), (LEN))
#else
    #define __setBytesAt(STREAM, INDEX, VAL, LEN)   Stream_setBytesAt((STREAM), (INDEX), (VAL), (LEN))
#endif // STREAM_SET_AT

#if STREAM_FIND_AT_VALUE
    #define __checkReverse(STREAM, VAL, LEN)        if (STREAM->OrderFn) {__memReverse((STREAM), &VAL, LEN);}
    #define __checkReverseOn(STREAM, VAL)           __checkReverse(STREAM, VAL, sizeof(VAL))
    #define __checkReverseOff(STREAM, VAL)
#else
    #define __checkReverse(STREAM, VAL, LEN)
    #define __checkReverseOn(STREAM, VAL)
    #define __checkReverseOff(STREAM, VAL)
#endif // STREAM_FIND_AT_VALUE

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
#if STREAM_ARGS
    stream->Args = (void*) 0;
#endif // STREAM_ARGS
#if STREAM_PENDING_BYTES
    stream->PendingBytes = 0;
#endif // STREAM_PENDING_BYTES
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
#if STREAM_WRITE_FLIP
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
#endif
#if STREAM_READ_FLIP
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
#endif
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
/* ------------------------------------ General Write APIs ---------------------------------- */
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
    Stream_LenType wpos = stream->WPos;

    if (wpos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - wpos;
        len -= tmpLen;
        __memCopy(stream, &stream->Data[wpos], val, tmpLen);
        val += tmpLen;
        // move WPos
        wpos = 0;
        stream->Overflow = 1;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopy(stream, &stream->Data[wpos], val, len);
        // move WPos
        wpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    stream->WPos = wpos;

    return Stream_Ok;
}
#if STREAM_WRITE_REVERSE
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
    Stream_LenType wpos = stream->WPos;

    if (wpos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - wpos;
        len -= tmpLen;
        __memCopyReverse(stream, &stream->Data[wpos], val + len, tmpLen);
        // move WPos
        wpos = 0;
        stream->Overflow = 1;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopyReverse(stream, &stream->Data[wpos], val, len);
        // move WPos
        wpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    stream->WPos = wpos;

    return Stream_Ok;
}
/**
 * @brief Write value to buffer based on byte order
 * 
 * @param stream 
 * @param val 
 * @param len 
 * @return Stream_Result 
 */
Stream_Result Stream_write(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
    return __writeBytes(stream, val, len);
}
#endif // STREAM_WRITE_REVERSE
#if STREAM_WRITE_STREAM
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
    Stream_LenType wpos = out->WPos;

    if (wpos + len >= out->Size) {
        Stream_LenType tmpLen;
        tmpLen = out->Size - wpos;
        len -= tmpLen;
        Stream_readBytes(in, &out->Data[wpos], tmpLen);
        // move WPos
        wpos = 0;
        out->Overflow = 1;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        Stream_readBytes(in, &out->Data[wpos], len);
        // move WPos
        wpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    out->WPos = wpos;

    return Stream_Ok;
}
#endif // STREAM_WRITE_STREAM 
#if STREAM_WRITE_PADDING
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
    Stream_LenType wpos = stream->WPos;

    if (wpos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - wpos;
        len -= tmpLen;
        __memSet(stream, &stream->Data[wpos], val, tmpLen);
        val += tmpLen;
        // move WPos
        wpos = 0;
        stream->Overflow = 1;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memSet(stream, &stream->Data[wpos], val, len);
        // move WPos
        wpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    stream->WPos = wpos;

    return Stream_Ok;
}
#endif // STREAM_WRITE_PADDING
/* ------------------------------------ General Read APIs ---------------------------------- */
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
    Stream_LenType rpos = stream->RPos;

    if (rpos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - rpos;
        len -= tmpLen;
        __memCopy(stream, val, &stream->Data[rpos], tmpLen);
        val += tmpLen;
        // move RPos
        rpos = 0;
        stream->Overflow = 0;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopy(stream, val, &stream->Data[rpos], len);
        // move RPos
        rpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    stream->RPos = rpos;

    return Stream_Ok;
}
#if STREAM_READ_REVERSE
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
    Stream_LenType rpos = stream->RPos;

    if (rpos + len >= stream->Size) {
        Stream_LenType tmpLen;

        tmpLen = stream->Size - rpos;
        len -= tmpLen;
        __memCopyReverse(stream, val + len, &stream->Data[rpos], tmpLen);
        // move RPos
        rpos = 0;
        stream->Overflow = 0;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopyReverse(stream, val, &stream->Data[rpos], len);
        // move RPos
        rpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    stream->RPos = rpos;

    return Stream_Ok;
}
/**
 * @brief read bytes for buffer based on byte order settngs
 *
 * @param stream
 * @return int16_t
 */
Stream_Result Stream_read(StreamBuffer* stream, uint8_t* val, Stream_LenType len) {
    return __readBytes(stream, val, len);
}
#endif // STREAM_READ_REVERSE
#if STREAM_READ_STREAM
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
    Stream_LenType rpos = in->RPos;

    if (rpos + len >= in->Size) {
        Stream_LenType tmpLen;

        tmpLen = in->Size - rpos;
        len -= tmpLen;
        Stream_writeBytes(out, &in->Data[rpos], tmpLen);
        // move RPos
        rpos = 0;
        in->Overflow = 0;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        Stream_writeBytes(out, &in->Data[rpos], len);
        // move RPos
        rpos += len;
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    in->RPos = rpos;

    return Stream_Ok;
}
#endif
#if STREAM_READ_VALUE
/**
 * @brief read value from buffer based on byte order
 * 
 * @param stream 
 * @return Stream_Value 
 */
Stream_Value  Stream_readValue(StreamBuffer* stream, Stream_LenType len) {
    Stream_Value val = {0};
    __readBytes(stream, (uint8_t*) &val, len);
    return val;
}
#endif // STREAM_READ_VALUE
/* ------------------------------------ General GetAt APIs ---------------------------------- */
#if STREAM_GET_AT
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
        index = 0;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopy(stream, val, &stream->Data[index], len);
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    
    return Stream_Ok;
}
#endif // STREAM_GET_AT
#if STREAM_GET_AT_BYTES_REVERSE
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
        __memCopyReverse(stream, val + len, &stream->Data[index], tmpLen);
        index = 0;
    }

#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopyReverse(stream, val, &stream->Data[index], len);
#if STREAM_CHECK_ZERO_LEN
    }
#endif

    return Stream_Ok;
}
Stream_Result Stream_getAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
    return __getBytesAt(stream, index, val, len);
}
#endif // STREAM_GET_AT_BYTES_REVERSE
#if STREAM_GET_AT_VALUE
Stream_Value  Stream_getValueAt(StreamBuffer* stream, Stream_LenType index, Stream_LenType len) {
    Stream_Value val = {0};
    __getBytesAt(stream, index, (uint8_t*) &val, len);
    return val;
}
#endif // STREAM_GET_AT_VALUE
/* ------------------------------------ GetAt Value Array APIs ---------------------------------- */
#if STREAM_GET_AT_ARRAY
Stream_Result Stream_getArrayAt(StreamBuffer* stream, Stream_LenType index, void* val, Stream_LenType itemLen, Stream_LenType len) {
    Stream_Result result;
#if STREAM_CHECK_ZERO_LEN
    if (len == 0 || itemLen == 0) {
      return Stream_ZeroLen;
    }
#endif
    if (Stream_available(stream) - index < len * itemLen) {
        return Stream_NoAvailable;
    }

    while (len-- > 0) {
        if ((result = Stream_getAt(stream, index, (uint8_t*) val, itemLen)) != Stream_Ok) {
            return result;
        }
        val = (uint8_t*) val + itemLen;
        index += itemLen;
    }
    return Stream_Ok;
}
#endif // STREAM_GET_AT_ARRAY
/* ------------------------------------ Write Value Array APIs ---------------------------------- */
#if STREAM_WRITE_ARRAY
Stream_Result Stream_writeArray(StreamBuffer* stream, void* val, Stream_LenType itemLen, Stream_LenType len) {
    Stream_Result result;
#if STREAM_CHECK_ZERO_LEN
    if (len == 0 || itemLen == 0) {
      return Stream_ZeroLen;
    }
#endif
    if (Stream_space(stream) < len * itemLen) {
        return Stream_NoSpace;
    }

    while (len-- > 0) {
        if ((result = Stream_write(stream, (uint8_t*) val, itemLen)) != Stream_Ok) {
            return result;
        }
        val = (uint8_t*) val + itemLen;
    }
    return Stream_Ok;
}
#endif
/* --------------------------------------- Set API------------------------------------------ */
#if STREAM_SET_AT
Stream_Result Stream_setBytesAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
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
        __memCopy(stream, &stream->Data[index], val, tmpLen);
        val += tmpLen;
        index = (index + tmpLen) % stream->Size;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopy(stream, &stream->Data[index], val, len);
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    
    return Stream_Ok;
}
#if STREAM_SET_AT_BYTES_REVERSE
Stream_Result   Stream_setBytesReverseAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
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
        __memCopyReverse(stream, &stream->Data[index], val + len, tmpLen);
        index = 0;
    }
#if STREAM_CHECK_ZERO_LEN
    if (len > 0) {
#endif
        __memCopyReverse(stream, &stream->Data[index], val, len);
#if STREAM_CHECK_ZERO_LEN
    }
#endif
    
    return Stream_Ok;
}
Stream_Result Stream_setAt(StreamBuffer* stream, Stream_LenType index, uint8_t* val, Stream_LenType len) {
    return __setBytesAt(stream, index, val, len);
}
#endif // STREAM_SET_AT_BYTES_REVERSE
#endif // STREAM_SET_AT
/* ------------------------------------ SetAt Value Array APIs ---------------------------------- */
#if STREAM_SET_AT_ARRAY
Stream_Result Stream_setArrayAt(StreamBuffer* stream, Stream_LenType index, void* val, Stream_LenType itemLen, Stream_LenType len) {
    Stream_Result result;
#if STREAM_CHECK_ZERO_LEN
    if (len == 0 || itemLen == 0) {
      return Stream_ZeroLen;
    }
#endif
    if (Stream_space(stream) - index < len * itemLen) {
        return Stream_NoSpace;
    }

    while (len-- > 0) {
        if ((result = Stream_setAt(stream, index, (uint8_t*) val, itemLen)) != Stream_Ok) {
            return result;
        }
        val = (uint8_t*) val + itemLen;
        index += itemLen;
    }
    return Stream_Ok;
}
#endif // STREAM_SET_AT_ARRAY
/* ------------------------------------ Read Value Array APIs ----------------------------------- */
Stream_Result Stream_readArray(StreamBuffer* stream, void* val, Stream_LenType itemLen, Stream_LenType len) {
    Stream_Result result;
#if STREAM_CHECK_ZERO_LEN
    if (len == 0 || itemLen == 0) {
      return Stream_ZeroLen;
    }
#endif
    if (Stream_available(stream) < len * itemLen) {
        return Stream_NoAvailable;
    }

    while (len-- > 0) {
        if ((result = Stream_read(stream, (uint8_t*) val, itemLen)) != Stream_Ok) {
            return result;
        }
        val = (uint8_t*) val + itemLen;
    }
    return Stream_Ok;
}
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
#if STREAM_WRITE_LOCK_CUSTOM
/**
 * @brief Lock the stream for writing with custom payload
 * 
 * Note: StreamBuffer must be in first position in the struct
 * 
 * @param stream 
 * @param lock 
 * @param len 
 * @param payload 
 * @param payloadLen 
 * @return Stream_Result 
 */
Stream_Result Stream_lockWriteCustom(void* stream, void* lock, Stream_LenType len, Stream_LenType datatypeLen) {
    Stream_Result res;
    if ((res = Stream_lockWrite((StreamBuffer*) stream, (StreamBuffer*) lock, len)) == Stream_Ok) {
        memcpy((uint8_t*) lock + sizeof(StreamBuffer), (uint8_t*) stream + sizeof(StreamBuffer), datatypeLen - sizeof(StreamBuffer));
    }
    return res;
}
#endif // STREAM_WRITE_LOCK_CUSTOM
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
#if STREAM_READ_LOCK_CUSTOM
/**
 * @brief Lock the stream for read operation
 * 
 * @param stream 
 * @param lock 
 * @param len 
 * @param datatypeLen 
 * @return Stream_Result 
 */
Stream_Result Stream_lockReadCustom(void* stream, void* lock, Stream_LenType len, Stream_LenType datatypeLen) {
    Stream_Result res;
    if ((res = Stream_lockRead((StreamBuffer*) stream, (StreamBuffer*) lock, len)) == Stream_Ok) {
        memcpy((uint8_t*) lock + sizeof(StreamBuffer), (uint8_t*) stream + sizeof(StreamBuffer), datatypeLen - sizeof(StreamBuffer));
    }
    return res;
}
#endif // STREAM_READ_LOCK_CUSTOM
#endif // STREAM_READ_LOCK

#if STREAM_ARGS
/**
 * @brief set buffer user arguments
 * 
 * @param stream 
 * @param args 
 */
void Stream_setArgs(StreamBuffer* stream, void* args) {
    stream->Args = args;
}
/**
 * @brief get buffer user arguments
 * 
 * @param stream 
 * @return void* 
 */
void* Stream_getArgs(StreamBuffer* stream) {
    return stream->Args;
}
#endif // STREAM_ARGS
#if STREAM_PENDING_BYTES
/**
 * @brief Set pending bytes
 * 
 * @param stream 
 * @param len 
 */
void Stream_setPendingBytes(StreamBuffer* stream, Stream_LenType len) {
    stream->PendingBytes = len;
}
/**
 * @brief Get pending bytes
 * 
 * @param stream 
 * @return Stream_LenType 
 */
Stream_LenType Stream_getPendingBytes(StreamBuffer* stream) {
    return stream->PendingBytes;
}
#endif

#if STREAM_FIND_AT
Stream_LenType Stream_findValueAt(StreamBuffer* stream, Stream_LenType offset, Stream_Value val, Stream_LenType len) {
    __checkReverse(stream, val, len);
    return Stream_findPatternAt(stream, offset, (uint8_t*) &val, len);
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
#if STREAM_READ_UNTIL_AT
Stream_LenType Stream_readBytesUntilAt(StreamBuffer* stream, Stream_LenType offset, uint8_t end, uint8_t* val, Stream_LenType len) {
    Stream_LenType tmpLen;
    // find end byte
    if ((tmpLen = Stream_findByteAt(stream, offset, end)) >= 0) {
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
#endif // STREAM_READ_UNTIL_AT
#endif // STREAM_FIND_AT
/* ------------------------------------ Transpose APIs ---------------------------------- */
#if STREAM_TRANSPOSE
/**
 * @brief Transpose a given bytes at index with available bytes in stream
 * 
 * @param stream 
 * @param offset 
 * @param len 
 * @param tmpBuf 
 * @param chunkLen 
 * @param transpose 
 * @param args
 * @return Stream_Result 
 */
Stream_Result Stream_transposeAt(StreamBuffer* stream, Stream_LenType offset, Stream_LenType len, uint8_t* tmpBuf, Stream_LenType chunkLen, Stream_TransposeFn transpose, void* args) {
    Stream_LenType dirLen;
    Stream_LenType tmpLen;
    Stream_LenType rpos;
    Stream_Result res;

#if STREAM_CHECK_ZERO_LEN
    if (len == 0) {
        return Stream_ZeroLen;
    }
#endif

    if (Stream_available(stream) - offset < len) {
        return Stream_NoAvailable;
    }

    if (chunkLen == 0) {
        chunkLen = len;
    }

    rpos = (stream->RPos + offset) % stream->Size;

    while (len > 0) {
        dirLen = Stream_directAvailableAt(stream, offset);
        // Check for Zero-Copy
        if (dirLen >= chunkLen || tmpBuf == NULL) {
            tmpLen = dirLen >= chunkLen ? chunkLen : dirLen;
            // Use zero copy method
            if ((res = transpose(args, &stream->Data[rpos], tmpLen)) != Stream_Ok) {
                return res;
            }
        }
        else {
            tmpLen = len >= chunkLen ? chunkLen : len;
            // Copy data to tmp buffer
            if ((res = Stream_getBytesAt(stream, offset, tmpBuf, tmpLen)) != Stream_Ok) {
                return res;
            }
            // Transpose data
            if ((res = transpose(args, tmpBuf, tmpLen)) != Stream_Ok) {
                return res;
            }
            // Write back to stream
            if ((res = Stream_setBytesAt(stream, offset, tmpBuf, tmpLen)) != Stream_Ok) {
                return res;
            }
        }

        len -= tmpLen;
        offset += tmpLen;
        rpos = (rpos + tmpLen) % stream->Size;
    }

    return Stream_Ok;
}
#endif
/* ------------------------------------ Compare APIs ---------------------------------- */
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
#if STREAM_MEM_IO_BUILT_IN
void memrcpy(void* dest, const void* src, int len) {
    uint8_t* pDest = (uint8_t*) dest;
    const uint8_t* pSrc = (const uint8_t*) src + len - 1;

    while (len-- > 0) {
        *pDest++ = *pSrc--;
    }
}
void memreverse(void* arr, int len) {
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
#endif // STREAM_MEM_IO_BUILT_IN
