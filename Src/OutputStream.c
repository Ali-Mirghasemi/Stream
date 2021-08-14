#include "OutputStream.h"


void OStream_init(OStream* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(&stream->Buffer, buff, size);
    stream->transmit = transmitFn;
    stream->Args = (void*) 0;
}
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
    Stream_LenType len = Stream_directAvailable(&stream->Buffer);
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
/**
 * @brief blocking transmite 1 byte just call transmit function no need handle function
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
 * @brief blocking transmite n byte just call transmit function no need handle function
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

