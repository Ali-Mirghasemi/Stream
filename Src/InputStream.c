#include "InputStream.h"
#include <string.h>

/**
 * @brief Initialize InputStream
 *
 * @param stream InputStream to initialize
 * @param receiveFn Function to receive data
 * @param buff Buffer to hold data
 * @param size Size of buffer
 */
void IStream_init(IStream* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(&stream->Buffer, buff, size);
    stream->receive = receiveFn;
#if ISTREAM_CHECK_RECEIVE
    stream->checkReceive = (IStream_CheckReceiveFn) 0;
#endif
}
/**
 * @brief Deinitialize InputStream
 *
 * @param stream InputStream to deinitialize
 */
void IStream_deinit(IStream* stream) {
    memset(stream, 0, sizeof(IStream));
}

/**
 * @brief call in interrupt or RxCplt callback for Async Receive
 *
 * @param stream
 * @param len
 * @return Stream_Result
 */
Stream_Result IStream_handle(IStream* stream, Stream_LenType len) {
    Stream_Result res;

	if (!stream->Buffer.InReceive) {
		return Stream_NoReceive;
	}

    if (stream->Buffer.PendingBytes < len) {
        len = stream->Buffer.PendingBytes;
    }

    stream->Buffer.InReceive = 0;
    if ((res = Stream_moveWritePos(&stream->Buffer, len)) != Stream_Ok) {
        return res;
    }

    return IStream_receive(stream);
}
/**
 * @brief call receive function for Async read
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result IStream_receive(IStream* stream) {
    if (!stream->Buffer.InReceive) {
        Stream_LenType len = Stream_directSpace(&stream->Buffer);
        stream->Buffer.PendingBytes = len;
        if (len > 0) {
            if (stream->receive) {
                stream->Buffer.InReceive = 1;
                return stream->receive(stream, IStream_getDataPtr(stream), len);
            }
            else {
                return Stream_NoReceiveFn;
            }
        }
        else {
            return Stream_NoSpace;
        }
    }
    else {
        return Stream_InReceive;
    }
}
/**
 * @brief blocking receive for 1 byte
 *
 * @param stream
 * @param val
 * @return Stream_Result
 */
Stream_Result IStream_receiveByte(IStream* stream, uint8_t val) {
    *IStream_getDataPtr(stream) = val;
    return Stream_moveWritePos(&stream->Buffer, 1);

}
/**
 * @brief blocking receive for n bytes
 *
 * @param stream
 * @param val
 * @param len
 * @return Stream_Result
 */
Stream_Result IStream_receiveBytes(IStream* stream, uint8_t* val, Stream_LenType len) {
    return Stream_writeBytes(&stream->Buffer, val, len);
}
/**
 * @brief return available bytes in buffer to read
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType IStream_available(IStream* stream) {
#if ISTREAM_CHECK_RECEIVE
    if (stream->checkReceive) {
        Stream_LenType len = stream->checkReceive(stream);
        if (len > 0) {
            if (stream->Buffer.PendingBytes < len) {
                len = stream->Buffer.PendingBytes;
            }
            Stream_moveWritePos(&stream->Buffer, len);
            stream->Buffer.PendingBytes -= len;
            if (stream->Buffer.WPos == 0) {
                IStream_receive(stream);
            }
        }
    }
#endif // ISTREAM_CHECK_RECEIVE
    // now get available bytes len
    return Stream_available(&stream->Buffer);
}
#if ISTREAM_CHECK_RECEIVE
/**
 * @brief set check receive function
 *
 * @param stream
 * @param fn
 */
void IStream_setCheckReceive(IStream* stream, IStream_CheckReceiveFn fn) {
    stream->checkReceive = fn;
}
#endif // ISTREAM_CHECK_RECEIVE
#if ISTREAM_LOCK
/**
 * @brief lock input stream for fixed write
 *
 * @param stream
 * @param lock
 * @return Stream_Result
 */
Stream_Result IStream_lock(IStream* stream, IStream* lock, Stream_LenType len) {
    Stream_Result res;
    if ((res = Stream_lockRead(&stream->Buffer, &lock->Buffer, len)) == Stream_Ok) {
        memcpy(&lock->receive, &stream->receive, sizeof(IStream) - sizeof(StreamBuffer));
    }
    return res;
}
#endif // ISTREAM_LOCK
