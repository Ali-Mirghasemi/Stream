#include "InputStream.h"
#include <string.h>

void IStream_init(IStream* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(&stream->Buffer, buff, size);
    stream->receive = receiveFn;
    stream->Args = (void*) 0;
    stream->checkReceive = (IStream_CheckReceiveFn) 0;
}
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

    if (stream->IncomingBytes < len) {
        len = stream->IncomingBytes;
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
    Stream_LenType len = Stream_directSpace(&stream->Buffer);
    stream->IncomingBytes = len;
    if (len > 0) {
        if (stream->receive) {
            stream->Buffer.InReceive = 1;
            stream->receive(stream, IStream_getDataPtr(stream), len);
            return Stream_Ok;
        }
        else {
            return Stream_NoReceiveFn;
        }
    }
    else {
        return Stream_NoSpace;
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
 * @brief set args for IStream
 * 
 * @param stream 
 * @param args 
 */
void  IStream_setArgs(IStream* stream, void* args) {
    stream->Args = args;
}
/**
 * @brief get args form IStream
 * 
 * @param stream 
 * @return void* 
 */
void* IStream_getArgs(IStream* stream) {
    return stream->Args;
}

Stream_LenType IStream_available(IStream* stream) {
    // check
    if (stream->checkReceive) {
        Stream_LenType len = stream->checkReceive(stream);
        if (len > 0) {
            if (stream->IncomingBytes < len) {
                len = stream->IncomingBytes;
            }
            Stream_moveWritePos(&stream->Buffer, len);
            stream->IncomingBytes -= len;
            if (stream->Buffer.WPos == 0) {
                IStream_receive(stream);
            }
        }
    }
    // now get available bytes len
    return Stream_available(&stream->Buffer);
}

void IStream_setCheckReceive(IStream* stream, IStream_CheckReceiveFn fn) {
    stream->checkReceive = fn;
}
Stream_LenType IStream_incomingBytes(IStream* stream) {
    return stream->IncomingBytes;
}
