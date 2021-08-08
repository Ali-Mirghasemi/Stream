#include "OutputStream.h"


void OStream_init(OStream* stream, OStream_TransmitFn transmitFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(&stream->Buffer, buff, size);
    stream->transmit = transmitFn;
    stream->Args = (void*) 0;
}
void OStream_deinit(OStream* stream) {
    memset(stream, 0, sizeof(OStream));
}


/* Output Bytes of OStream */
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

void  OStream_setArgs(OStream* stream, void* args) {
    stream->Args = args;
}
void* OStream_getArgs(OStream* stream) {
    return stream->Args;
}

