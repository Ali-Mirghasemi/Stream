#include <stdio.h>
#include <stdlib.h>

#include "StreamBuffer.h"

#if !STREAM_WRITE_LIMIT || !STREAM_READ_LIMIT
    #error "Enable 'STREAM_WRITE_LIMIT' and 'STREAM_READ_LIMIT'"
#endif

void printBuff(uint8_t* buff, int len);

uint8_t buff[50] = {0};
Stream stream;

int main() {

    Stream_init(&stream, buff, sizeof(buff));

    // set limit
    Stream_setWriteLimit(&stream, 12);

    Stream_writeUInt32(&stream, 0xAAAA);
    Stream_writeUInt32(&stream, 0xBBBB);
    Stream_writeUInt32(&stream, 0xCCCC);
    Stream_writeUInt32(&stream, 0xDDDD); // can't write because of limit
    Stream_writeUInt32(&stream, 0xEEEE); // can't write because of limit

    printBuff(buff, sizeof(buff));

    Stream_setReadLimit(&stream, 8);

    printf("%X\n", Stream_readUInt32(&stream));
    printf("%X\n", Stream_readUInt32(&stream));
    printf("%X\n", Stream_readUInt32(&stream)); // can't read because of limit
    printf("%X\n", Stream_readUInt32(&stream)); // can't read because of limit
    printf("%X\n", Stream_readUInt32(&stream)); // can't read because of limit

}

void printBuff(uint8_t* buff, int len) {
    putchar('[');
    while (len-- > 0) {
        printf("0x%02X, ", *buff++);
    }
    puts("]");
}
