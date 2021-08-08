#include <stdio.h>
#include <stdlib.h>

#include "Stream.h"

const char* const ByteOrderStrs[2] = {
    "LittleEndian", "BigEndian"
};

void printArray(uint8_t* buff, int len) {
    putchar('[');
    while (len-- > 0) {
        printf("%02X, ", *buff++);
    }
    putchar(']');
    putchar('\n');
}

int main()
{
    uint8_t buff[23] = {0};

    Stream stream;

    Stream_init(&stream, buff, sizeof(buff));
    Stream_setByteOrder(&stream, ByteOrder_LittleEndian);

    printf("Sys ByteOrder: %s\n", ByteOrderStrs[Stream_getSystemByteOrder()]);

    Stream_writeFloat(&stream, 2.34f);
    Stream_writeUInt16(&stream, 25);
    Stream_writeInt8(&stream, -14);
    Stream_writeInt16(&stream, -30000);
    Stream_writeUInt32(&stream, 4000000000);
    Stream_writeChar(&stream, 'X');
    Stream_writeDouble(&stream, 8.124);
    Stream_writeUInt32(&stream, 1111); // buffer full!

    printf("Write Order: %s\n", ByteOrderStrs[Stream_getByteOrder(&stream)]);
    printf("Available: %u, Space: %u\n", Stream_available(&stream), Stream_space(&stream));
    printArray(Stream_getBuffer(&stream), Stream_getBufferSize(&stream));

    printf("Float:  %g\n", Stream_readFloat(&stream));
    printf("UInt16: %u\n", Stream_readUInt16(&stream));
    printf("Int8:   %d\n", Stream_readInt8(&stream));
    printf("Int16:  %d\n", Stream_readInt16(&stream));
    printf("UInt32: %u\n", Stream_readUInt32(&stream));
    printf("Char:   %c\n", Stream_readChar(&stream));
    printf("Double: %g\n", Stream_readDouble(&stream));
    printf("UInt32: %u\n", Stream_readUInt32(&stream)); // no bytes available

    printf("\nAvailable: %u, Space: %u\n", Stream_available(&stream), Stream_space(&stream));

    printf("\n---------------------------------------\n");
    // Second sequence Write & Read
    Stream_setByteOrder(&stream, ByteOrder_BigEndian);

    Stream_writeFloat(&stream, 5.54f);
    Stream_writeUInt16(&stream, 124);
    Stream_writeInt8(&stream, 127);
    Stream_writeInt16(&stream, 6000);
    Stream_writeUInt32(&stream, 1);
    Stream_writeChar(&stream, 'Z');
    Stream_writeDouble(&stream, 16.3);
    Stream_writeUInt32(&stream, 2222); // buffer full!

    printf("Write Order: %s\n", ByteOrderStrs[Stream_getByteOrder(&stream)]);
    printf("Available: %u, Space: %u\n", Stream_available(&stream), Stream_space(&stream));
    printArray(Stream_getBuffer(&stream), Stream_getBufferSize(&stream));

    printf("Float:  %g\n", Stream_readFloat(&stream));
    printf("UInt16: %u\n", Stream_readUInt16(&stream));
    printf("Int8:   %d\n", Stream_readInt8(&stream));
    printf("Int16:  %d\n", Stream_readInt16(&stream));
    printf("UInt32: %u\n", Stream_readUInt32(&stream));
    printf("Char:   %c\n", Stream_readChar(&stream));
    printf("Double: %g\n", Stream_readDouble(&stream));
    printf("UInt32: %u\n", Stream_readUInt32(&stream)); // no bytes available

    printf("\nAvailable: %u, Space: %u\n", Stream_available(&stream), Stream_space(&stream));

}
