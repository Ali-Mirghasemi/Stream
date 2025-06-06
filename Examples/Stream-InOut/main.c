#include <stdio.h>
#include <stdlib.h>

#include "InputStream.h"
#include "OutputStream.h"
#include "StreamBuffer.h"

uint8_t mediaBuff[200] = {0};
StreamBuffer mediaStream;

void printArray(uint8_t* buff, int len) {
    putchar('[');
    while (len-- > 0) {
        printf("%02X, ", *buff++);
    }
    putchar(']');
    putchar('\n');
}

Stream_Result MyStream_receive(StreamIn* stream, uint8_t* buff, Stream_LenType len) {
	Stream_LenType available = Stream_available(&mediaStream);

	if (available < len) {
		len = available;
	}

	if (available > 0) {
		Stream_readBytes(&mediaStream, buff, len);
		// call read completed
		IStream_handle(stream, len);
	}

	return Stream_Ok;
}

Stream_Result MyStream_transmit(StreamOut* stream, uint8_t* buff, Stream_LenType len) {
	Stream_writeBytes(&mediaStream, buff, len);
	// call transmit completed
	return OStream_handle(stream, len);
}

int main()
{
    uint8_t rxBuff[50] = {0};
    uint8_t txBuff[50] = {0};
    StreamIn inStream;
    StreamOut outStream;

    puts("--------------------------------------");
    printf("StreamBuffer Ver: %s\n", STREAM_VER_STR);
    printf("StreamOut Ver: %s\n", OSTREAM_VER_STR);
    printf("StreamIn  Ver: %s\n", ISTREAM_VER_STR);
    puts("--------------------------------------\n");

    Stream_init(&mediaStream, mediaBuff, sizeof(mediaBuff));
    IStream_init(&inStream, MyStream_receive, rxBuff, sizeof(rxBuff));
    OStream_init(&outStream, MyStream_transmit, txBuff, sizeof(txBuff));

    // write to out stream
	OStream_writeInt32(&outStream, 213);
	OStream_writeInt8(&outStream, -100);
	OStream_writeChar(&outStream, 'X');
	OStream_writeUInt16(&outStream, 60000);
	OStream_writeFloat(&outStream, 2.5468);
	// flush buffer and transmit bytes as possible
	printf("Space: %u\n", OStream_space(&outStream));
	printf("Flush to Media..\n");
	printArray(OStream_getDataPtr(&outStream), OStream_getBufferSize(&outStream));
	OStream_flush(&outStream);

	// start receive
	printf("Start receive from Media..\n");
	IStream_receive(&inStream);
	printf("Available: %u\n", IStream_available(&inStream));
	printArray(IStream_getDataPtr(&outStream), IStream_getBufferSize(&inStream));

	printf("V0: %d\n", IStream_readInt32(&inStream));
	printf("V1: %d\n", IStream_readInt8(&inStream));
	printf("V2: %c\n", IStream_readChar(&inStream));
	printf("V3: %u\n", IStream_readUInt16(&inStream));
	printf("V4: %g\n", IStream_readFloat(&inStream));
	printf("V5: %u\n", IStream_readInt32(&inStream));


}
