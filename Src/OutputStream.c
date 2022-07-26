#include "OutputStream.h"
#include <string.h>

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
    stream->transmit = transmitFn;
#if OSTREAM_ARGS
    stream->Args = (void*) 0;
#endif
#if OSTREAM_CHECK_TRANSMIT
    stream->checkTransmit = (OStream_CheckTransmitFn) 0;
#endif
    stream->OutgoingBytes = 0;
}
/**
 * @brief Deinitialize OutputStream
 *
 * @param stream OutputStream to deinitialize
 */
void OStream_deinit(OStream* stream) {
    memset(stream, 0, sizeof(OStream));
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

    if (stream->OutgoingBytes < len) {
        len = stream->OutgoingBytes;
    }

    stream->Buffer.InTransmit = 0;
    if ((res = Stream_moveReadPos(&stream->Buffer, len)) != Stream_Ok) {
        return res;
    }

    return OStream_flush(stream);
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
        stream->OutgoingBytes = len;
        if (len > 0) {
            if (stream->transmit) {
                stream->Buffer.InTransmit = 1;
                stream->transmit(stream, OStream_getDataPtr(stream), len);
                return Stream_Ok;
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
#if OSTREAM_ARGS
/**
 * @brief set args for OStream
 *
 * @param stream
 * @param args
 */
void  OStream_setArgs(OStream* stream, void* args) {
    stream->Args = args;
}
/**
 * @brief get args for OStream
 *
 * @param stream
 * @return void*
 */
void* OStream_getArgs(OStream* stream) {
    return stream->Args;
}
#endif // OSTREAM_ARGS
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
            if (stream->OutgoingBytes < len) {
                len = stream->OutgoingBytes;
            }
            Stream_moveReadPos(&stream->Buffer, len);
            stream->OutgoingBytes -= len;
            if (stream->Buffer.RPos == 0) {
                OStream_flush(stream);
            }
        }
    }
#endif // OSTREAM_CHECK_TRANSMIT
    return Stream_space(&stream->Buffer);
}
/**
 * @brief return number of bytes that are in transmit
 *
 * @param stream
 * @return Stream_LenType
 */
Stream_LenType OStream_outgoingBytes(OStream* stream) {
    return stream->OutgoingBytes;
}
#if OSTREAM_LOCK
/**
 * @brief lock output stream for fixed write
 *
 * @param stream
 * @param lock
 * @return Stream_Result
 */
Stream_Result OStream_lock(OStream* stream, OStream* lock, Stream_LenType len) {
    Stream_Result res;
    if ((res = Stream_lockWrite(&stream->Buffer, &lock->Buffer, len)) == Stream_Ok) {
        memcpy(&lock->transmit, &stream->transmit, sizeof(OStream) - sizeof(Stream));
    }
    return res;
}
#endif // OSTREAM_LOCK
