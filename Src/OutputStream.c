#include "OutputStream.h"
#include <string.h>

#if OSTREAM

#if !STREAM_WRITE
    #error "For using OutputStream Library you must enable STREAM_WRITE in StreamBuffer.h"
#endif

#if !STREAM_PENDING_BYTES
    #error "For using OutputStream Library you must enable STREAM_PENDING_BYTES in StreamBuffer.h"
#endif

#if STREAM_MUTEX
#if STREAM_MUTEX_CHECK_RESULT
    #define __mutexVarInit()                        Stream_MutexResult mutexError
    #define __mutexInit(S)                          if ((mutexError = OStream_mutexInit((S)))) { return Stream_MutexError | mutexError; }
    #define __mutexLock(S)                          if ((mutexError = OStream_mutexLock((S)))) { return Stream_MutexError | mutexError; }
    #define __mutexUnlock(S)                        if ((mutexError = OStream_mutexUnlock((S)))) { return Stream_MutexError | mutexError; }
    #define __mutexDeInit(S)                        if ((mutexError = OStream_mutexDeInit((S)))) { return Stream_MutexError | mutexError; }
#else
    #define __mutexVarInit()
    #define __mutexInit(S)                          OStream_mutexInit((S))
    #define __mutexLock(S)                          OStream_mutexLock((S))
    #define __mutexUnlock(S)                        OStream_mutexUnlock((S))
    #define __mutexDeInit(S)                        OStream_mutexDeInit((S))
#endif
#else
    #define __mutexVarInit()
    #define __mutexInit(S)
    #define __mutexLock(S)
    #define __mutexUnlock(S)
    #define __mutexDeInit(S)
#endif

/**
 * @brief Initialize StreamOut
 *
 * @param stream StreamOut to initialize
 * @param transmitFn Function to transmit data
 * @param buff Buffer to hold data
 * @param size Size of buffer
 */
void OStream_init(StreamOut* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(&stream->Buffer, buff, size);
    stream->Buffer.FlushMode = OSTREAM_FLUSH_MODE;
    stream->transmit = transmitFn;
#if OSTREAM_CHECK_TRANSMIT
    stream->checkTransmit = (OStream_CheckTransmitFn) 0;
#endif
#if OSTREAM_FLUSH_CALLBACK
    stream->flushCallback = (OStream_FlushCallbackFn) 0;
#endif
}
/**
 * @brief De-Initialize output stream
 * 
 * @param stream 
 */
void OStream_deinit(StreamOut* stream) {
    Stream_deinit(&stream->Buffer);
    memset(stream, 0, sizeof(StreamOut));
}
/**
 * @brief call it in interrupt or TxCplt for Async Transmit
 *
 * @param stream
 * @param len
 * @return Stream_Result
 */
Stream_Result OStream_handle(StreamOut* stream, Stream_LenType len) {
    Stream_Result res;
    __mutexVarInit();
    __mutexLock(stream);

	if (!stream->Buffer.InTransmit) {
        __mutexUnlock(stream);
		return Stream_NoTransmit;
	}

    if (stream->Buffer.PendingBytes < len) {
        len = stream->Buffer.PendingBytes;
    }

    stream->Buffer.InTransmit = 0;
    if ((res = Stream_moveReadPos(&stream->Buffer, len)) != Stream_Ok) {
        __mutexUnlock(stream);
        return res;
    }

#if OSTREAM_FLUSH_CALLBACK
    if (stream->flushCallback && OStream_pendingBytes(stream) == 0) {
        stream->flushCallback(stream);
    }
#endif

    res = stream->Buffer.FlushMode != Stream_FlushMode_Single ? 
                                    OStream_flush(stream) :
                                    Stream_Ok;

    __mutexUnlock(stream);
    return res;
}
/**
 * @brief start sending bytes and flush stream in Async Transmit
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result OStream_flush(StreamOut* stream) {
    Stream_Result res;
    __mutexVarInit();
    __mutexLock(stream);
    if (!stream->Buffer.InTransmit) {
        Stream_LenType len = Stream_directAvailable(&stream->Buffer);
        stream->Buffer.PendingBytes = len;
        if (len > 0) {
            if (stream->transmit) {
                stream->Buffer.InTransmit = 1;
                res = stream->transmit(stream, OStream_getDataPtr(stream), len);
            }
            else {
                res = Stream_NoTransmitFn;
            }
        }
        else {
            res = Stream_NoAvailable;
        }
    }
    else {
        res = Stream_InTransmit;
    }

    __mutexUnlock(stream);
    return res;
}
/**
 * @brief flush and wait for transmit all pending bytes
 * 
 * @param stream 
 * @return Stream_Result 
 */
Stream_Result OStream_flushBlocking(StreamOut* stream) {
    Stream_Result res;
    __mutexVarInit();
    __mutexLock(stream);
    while (OStream_pendingBytes(stream) == 0) {
        if ((res = OStream_flush(stream)) != Stream_Ok) {
            break;
        }
    }

    __mutexUnlock(stream);
    return res;
}
/**
 * @brief blocking transmit 1 byte just call transmit function no need handle function
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result OStream_transmitByte(StreamOut* stream) {
    __mutexVarInit();
    __mutexLock(stream);
    
    Stream_LenType len = Stream_directAvailable(&stream->Buffer);
    Stream_Result res = Stream_Ok;

    if (len > 0) {
        if (stream->transmit) {
            stream->transmit(stream, OStream_getDataPtr(stream), 1);
            res = Stream_moveReadPos(&stream->Buffer, 1);
        }
        else {
            res = Stream_NoTransmitFn;
        }
    }
    else {
        res = Stream_NoAvailable;
    }

    __mutexUnlock(stream);
    return res;
}
/**
 * @brief blocking transmit n byte just call transmit function no need handle function
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result OStream_transmitBytes(StreamOut* stream, Stream_LenType len) {
    __mutexVarInit();
    __mutexLock(stream);

    Stream_LenType dirLen = OStream_pendingBytes(stream);
    Stream_Result res;
    if (dirLen < len) {
        len = dirLen;
    }
    while (len > 0) {
        dirLen = Stream_directAvailable(&stream->Buffer);
        if (dirLen > 0) {
            if (stream->transmit) {
                if (dirLen > len) {
                    dirLen = len;
                }
                stream->transmit(stream, OStream_getDataPtr(stream), dirLen);
                if ((res = Stream_moveReadPos(&stream->Buffer, 1)) != Stream_Ok) {
                    break;
                }
                len -= dirLen;
            }
            else {
                res = Stream_NoTransmitFn;
                break;
            }
        }
        else {
            res = Stream_NoAvailable;
            break;
        }
    }

    __mutexUnlock(stream);
    return res;
}
#if OSTREAM_CHECK_TRANSMIT
/**
 * @brief set check transmit function for StreamOut
 *
 * @param stream
 * @param checkTransmit
 */
void OStream_setCheckTransmit(StreamOut* stream, OStream_CheckTransmitFn fn) {
    __mutexVarInit();
    __mutexLock(stream);
    stream->checkTransmit = fn;
    __mutexUnlock(stream);
}
#endif // OSTREAM_CHECK_TRANSMIT
#if OSTREAM_FLUSH_CALLBACK
/**
 * @brief set flush callback function
 * 
 * @param stream 
 * @param fn 
 */
void OStream_setFlushCallback(StreamOut* stream, OStream_FlushCallbackFn fn) {
    __mutexVarInit();
    __mutexLock(stream);
    stream->flushCallback = fn;
    __mutexUnlock(stream);
}
#endif
/**
 * @brief return available space for write in bytes
 *
 * @param stream
 * @return OStream_CheckTransmitFn
 */
Stream_LenType OStream_space(StreamOut* stream) {
    __mutexVarInit();
    __mutexLock(stream);
#if OSTREAM_CHECK_TRANSMIT
    if (stream->checkTransmit) {
        Stream_LenType len = stream->checkTransmit(stream);
        if (len > 0) {
            if (stream->Buffer.PendingBytes < len) {
                len = stream->Buffer.PendingBytes;
            }
            Stream_moveReadPos(&stream->Buffer, len);
            stream->Buffer.PendingBytes -= len;
            if (stream->Buffer.RPos == 0) {
                OStream_flush(stream);
            }
        }
    }
#endif // OSTREAM_CHECK_TRANSMIT
    __mutexUnlock(stream);
    return Stream_space(&stream->Buffer);
}

#endif // OSTREAM
