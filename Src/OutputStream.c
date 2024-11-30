#include "OutputStream.h"
#include <string.h>

#if OSTREAM

#if !STREAM_WRITE
    #error "For using OutputStream Library you must enable STREAM_WRITE in StreamBuffer.h"
#endif

#if !STREAM_PENDING_BYTES
    #error "For using OutputStream Library you must enable STREAM_PENDING_BYTES in StreamBuffer.h"
#endif

/**
 * @brief Initialize OutputStream
 *
 * @param stream OutputStream to initialize
 * @param transmitFn Function to transmit data
 * @param buff Buffer to hold data
 * @param size Size of buffer
 */
void OStream_init(OStream* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size) {
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
 * @brief call it in interrupt or TxCplt for Async Transmit
 *
 * @param stream
 * @param len
 * @return Stream_Result
 */
Stream_Result OStream_handle(OStream* stream, Stream_LenType len) {
    Stream_Result res;
	if (!stream->Buffer.InTransmit) {
		return Stream_NoTransmit;
	}

    if (stream->Buffer.PendingBytes < len) {
        len = stream->Buffer.PendingBytes;
    }

    stream->Buffer.InTransmit = 0;
    if ((res = Stream_moveReadPos(&stream->Buffer, len)) != Stream_Ok) {
        return res;
    }

#if OSTREAM_FLUSH_CALLBACK
    if (stream->flushCallback && OStream_pendingBytes(stream) == 0) {
        stream->flushCallback(stream);
    }
#endif

    return stream->Buffer.FlushMode != Stream_FlushMode_Single ? 
                                    OStream_flush(stream) :
                                    Stream_Ok;
}
/**
 * @brief start sending bytes and flush stream in Async Transmit
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result OStream_flush(OStream* stream) {
    if (!stream->Buffer.InTransmit) {
        Stream_LenType len = Stream_directAvailable(&stream->Buffer);
        stream->Buffer.PendingBytes = len;
        if (len > 0) {
            if (stream->transmit) {
                stream->Buffer.InTransmit = 1;
                return stream->transmit(stream, OStream_getDataPtr(stream), len);
            }
            else {
                return Stream_NoTransmitFn;
            }
        }
        else {
            return Stream_NoAvailable;
        }
    }
    else {
        return Stream_InTransmit;
    }
}
/**
 * @brief flush and wait for transmit all pending bytes
 * 
 * @param stream 
 * @return Stream_Result 
 */
Stream_Result OStream_flushBlocking(OStream* stream) {
    Stream_Result res;

    while (OStream_pendingBytes(stream) == 0) {
        if ((res = OStream_flush(stream)) != Stream_Ok) {
            break;
        }
    }

    return res;
}
/**
 * @brief blocking transmit 1 byte just call transmit function no need handle function
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result OStream_transmitByte(OStream* stream) {
    Stream_LenType len = Stream_directAvailable(&stream->Buffer);
    if (len > 0) {
        if (stream->transmit) {
            stream->transmit(stream, OStream_getDataPtr(stream), 1);
            return Stream_moveReadPos(&stream->Buffer, 1);
        }
        else {
            return Stream_NoTransmitFn;
        }
    }
    else {
        return Stream_NoAvailable;
    }
}
/**
 * @brief blocking transmit n byte just call transmit function no need handle function
 *
 * @param stream
 * @return Stream_Result
 */
Stream_Result OStream_transmitBytes(OStream* stream, Stream_LenType len) {
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
                    return res;
                }
                len -= dirLen;
            }
            else {
                return Stream_NoTransmitFn;
            }
        }
        else {
            return Stream_NoAvailable;
        }
    }
    return Stream_Ok;
}
#if OSTREAM_CHECK_TRANSMIT
/**
 * @brief set check transmit function for OStream
 *
 * @param stream
 * @param checkTransmit
 */
void OStream_setCheckTransmit(OStream* stream, OStream_CheckTransmitFn fn) {
    stream->checkTransmit = fn;
}
#endif // OSTREAM_CHECK_TRANSMIT
#if OSTREAM_FLUSH_CALLBACK
/**
 * @brief set flush callback function
 * 
 * @param stream 
 * @param fn 
 */
void OStream_setFlushCallback(OStream* stream, OStream_FlushCallbackFn fn) {
    stream->flushCallback = fn;
}
#endif
/**
 * @brief return available space for write in bytes
 *
 * @param stream
 * @return OStream_CheckTransmitFn
 */
Stream_LenType OStream_space(OStream* stream) {
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
    return Stream_space(&stream->Buffer);
}

#endif // OSTREAM
