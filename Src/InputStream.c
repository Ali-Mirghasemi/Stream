#include "InputStream.h"


void IStream_init(IStream* stream, IStream_ReceiveFn receiveFn, uint8_t* buff, Stream_LenType size) {
    Stream_init(stream->Buffer, buff, size);
    stream->receive = receiveFn;
    stream->Args = (void*) 0;
}
void IStream_deinit(IStream* stream) {
    memset(stream, 0, sizeof(IStream));
}


/* Input Bytes of IStream */
Stream_Result IStream_handle(IStream* stream, Stream_LenType len) {
    Stream_Result res;

    stream->Buffer.InReceive = 0;
    if (res = Stream_moveWritePos(stream->Buffer, len) != Stream_Ok) {
        return res;
    }

    return IStream_receive(stream);
}
Stream_Result IStream_receive(IStream* stream) {
    Stream_LenType len = Stream_directSpace(stream->Buffer);
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
uint8_t*      IStream_getDataPtr(IStream* stream) {
    return &stream->Buffer.Data[stream->Buffer.WPos];
}

void  IStream_setArgs(IStream* stream, void* args) {
    stream->Args = args;
}
void* IStream_getArgs(IStream* stream) {
    return stream->Args;
}
