#include "InputStream.h"
#include <string.h>

#if ISTREAM

#if !STREAM_READ
    #error "For using InputStream Library you must enable STREAM_READ in StreamBuffer.h"
#endif

#if !STREAM_PENDING_BYTES
    #error "For using InputStream Library you must enable STREAM_PENDING_BYTES in StreamBuffer.h"
#endif

#if STREAM_MUTEX
#if STREAM_MUTEX_CHECK_RESULT
    #define __mutexVarInit()                        Stream_MutexResult mutexError
    #define __mutexInit(S)                          if ((mutexError = IStream_mutexInit((S)))) { return Stream_MutexError | mutexError; }
    #define __mutexLock(S)                          if ((mutexError = IStream_mutexLock((S)))) { return Stream_MutexError | mutexError; }
    #define __mutexUnlock(S)                        if ((mutexError = IStream_mutexUnlock((S)))) { return Stream_MutexError | mutexError; }
    #define __mutexDeInit(S)                        if ((mutexError = IStream_mutexDeInit((S)))) { return Stream_MutexError | mutexError; }
#else
    #define __mutexVarInit()
    #define __mutexInit(S)                          IStream_mutexInit((S))
    #define __mutexLock(S)                          IStream_mutexLock((S))
    #define __mutexUnlock(S)                        IStream_mutexUnlock((S))
    #define __mutexDeInit(S)                        IStream_mutexDeInit((S))
#endif
#else
    #define __mutexVarInit()
    #define __mutexInit(S)
    #define __mutexLock(S)
    #define __mutexUnlock(S)
    #define __mutexDeInit(S)
#endif

/**
 * @brief Initialize StreamIn
 *
 * @param stream StreamIn to initialize
 * @param receiveFn Function to receive data
 * @param buff Buffer to hold data
 * @param size Size of buffer
 */
void IStream_init(StreamIn* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(&stream->Buffer, buff, size);
    stream->receive = receiveFn;
#if ISTREAM_CHECK_RECEIVE
    stream->checkReceive = (IStream_CheckReceiveFn) 0;
#endif
#if ISTREAM_FULL_CALLBACK
    stream->onFull = (IStream_OnFullFn) 0;
#endif
}
/**
 * @brief De-Initialize input stream
 * 
 * @param stream 
 */
void IStream_deinit(StreamIn* stream) {
    Stream_deinit(&stream->Buffer);
    memset(stream, 0, sizeof(StreamIn));
}
/**
 * @brief call in interrupt or RxCplt callback for Async Receive
 *
 * @param stream
 * @param len
 * @return Stream_Result
 */
Stream_Result IStream_handle(StreamIn* stream, Stream_LenType len) {
    Stream_Result res;
    __mutexVarInit();
    __mutexLock(stream);

	if (!stream->Buffer.InReceive) {
        __mutexUnlock(stream);
		return Stream_NoReceive;
	}

    if (stream->Buffer.PendingBytes < len) {
        len = stream->Buffer.PendingBytes;
    }

    stream->Buffer.InReceive = 0;
    if ((res = Stream_moveWritePos(&stream->Buffer, len)) != Stream_Ok) {
        __mutexUnlock(stream);
        return res;
    }

#if ISTREAM_RECEIVE_CALLBACK
    if (stream->onReceive) {
        stream->onReceive(stream, len);
    }
#endif

#if ISTREAM_FULL_CALLBACK
    if (stream->onFull && Stream_isFull(&stream->Buffer)) {
        stream->onFull(stream);
    }
#endif

    __mutexUnlock(stream);
    return IStream_receive(stream);
}
/**
 * @brief call receive function for Async read
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result IStream_receive(StreamIn* stream) {
    __mutexVarInit();
    __mutexLock(stream);
    Stream_Result res;

    if (!stream->Buffer.InReceive) {
        Stream_LenType len = Stream_directSpace(&stream->Buffer);
        stream->Buffer.PendingBytes = len;
        if (len > 0) {
            if (stream->receive) {
                stream->Buffer.InReceive = 1;
                res = stream->receive(stream, IStream_getDataPtr(stream), len);
            }
            else {
                res = Stream_NoReceiveFn;
            }
        }
        else {
            res = Stream_NoSpace;
        }
    }
    else {
        res = Stream_InReceive;
    }

    __mutexUnlock(stream);
    return res;
}
/**
 * @brief return available bytes in buffer to read
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType IStream_available(StreamIn* stream) {
    __mutexVarInit();
    __mutexLock(stream);
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
    __mutexUnlock(stream);
    return Stream_available(&stream->Buffer);
}
#if ISTREAM_CHECK_RECEIVE
/**
 * @brief set check receive function
 *
 * @param stream
 * @param fn
 */
void IStream_setCheckReceive(StreamIn* stream, IStream_CheckReceiveFn fn) {
    __mutexVarInit();
    __mutexLock(stream);
    stream->checkReceive = fn;
    __mutexUnlock(stream);
}
#endif // ISTREAM_CHECK_RECEIVE
#if ISTREAM_RECEIVE_CALLBACK
void IStream_onReceive(StreamIn* stream, IStream_OnReceiveFn fn) {
    __mutexVarInit();
    __mutexLock(stream);
    stream->onReceive = fn;
    __mutexUnlock(stream);
}
#endif
#if ISTREAM_FULL_CALLBACK
void IStream_onFull(StreamIn* stream, IStream_OnFullFn fn) {
    __mutexVarInit();
    __mutexLock(stream);
    stream->onFull = fn;
    __mutexUnlock(stream);
}
#endif

#endif // ISTREAM
