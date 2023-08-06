# StreamBuffer C Library

StreamBuffer is a lightweight C library that provides functionalities to handle input and output streams over a stream buffer. It allows you to efficiently read and write various data types to and from the buffer, making it easier to handle data in a streaming fashion.

## Features

- Support for both little-endian and big-endian byte order.
- Write and read various data types including float, integer, character, and double.
- Efficiently manage incoming and outgoing bytes in the stream buffer.
- User-configurable options to enable or disable certain features.

## Dependencies

StreamBuffer has no external dependencies. It is a self-contained library written in standard C, making it easy to integrate into your projects without worrying about additional dependencies.

## Getting Started

To use the StreamBuffer library in your C project, follow these steps:

1. Copy the `StreamBuffer.h` and `StreamBuffer.c` files into your project directory.
2. Include the `StreamBuffer.h` header file in your source files where you want to use the library functions.
3. Initialize a `Stream` object and a buffer to hold the data.

```c
#include "StreamBuffer.h"
#include <stdio.h>

int main() {
    uint8_t buff[128];
    Stream stream;

    Stream_init(&stream, buff, sizeof(buff));
    // Now you can use the Stream functions to write and read data.
    return 0;
}
```

## Writing Data

You can use the Stream functions to write data of various data types to the stream buffer:

```c
Stream_writeFloat(&stream, 3.14f);
Stream_writeUInt16(&stream, 42);
Stream_writeChar(&stream, 'A');
// ... and more
```

## Reading Data

Similarly, you can use the Stream functions to read data from the stream buffer:

```c
float floatValue = Stream_readFloat(&stream);
uint16_t uintValue = Stream_readUInt16(&stream);
char charValue = Stream_readChar(&stream);
// ... and more
```

## Byte Order

The StreamBuffer library allows you to set the byte order (endianess) of the data. By default, it uses the system byte order. However, you can change it to little-endian or big-endian as needed:

```c
Stream_setByteOrder(&stream, ByteOrder_LittleEndian);
// or
Stream_setByteOrder(&stream, ByteOrder_BigEndian);
```

## Configuration

The library comes with some configurable options that you can modify by defining specific macros before including `StreamBuffer.h` in your code:

- `STREAM_WRITE_LIMIT`: Enable or disable the write limit feature.
- `STREAM_CURSOR`: Enable or disable the cursor feature.
- `STREAM_UINT64`: Enable or disable support for 64-bit unsigned integers.
- `STREAM_DOUBLE`: Enable or disable support for double-precision floating-point numbers.

## IStream Example

```c
#include "StreamBuffer.h"
#include <stdio.h>

void myReceiveCallback(IStream* stream, uint8_t* data, Stream_LenType len) {
    // Process the received data
}

int main() {
    uint8_t rxBuffer[128];
    IStream iStream;

    IStream_init(&iStream, myReceiveCallback, rxBuffer, sizeof(rxBuffer));
    // Now you can receive data using the IStream functions.
    return 0;
}
```

## OStream Example

```c
#include "StreamBuffer.h"
#include <stdio.h>

void myTransmitFunction(OStream* stream, uint8_t* data, Stream_LenType len) {
    // Transmit the data
}

int main() {
    uint8_t txBuffer[128];
    OStream oStream;

    OStream_init(&oStream, myTransmitFunction, txBuffer, sizeof(txBuffer));
    // Now you can write data to be transmitted using the OStream functions.
    return 0;
}
```

## Examples
- [Stream-Test](./Examples/Stream-Test/) test case for stream library
- [Stream-Example](./Examples/Stream-Example/) shows basic usage of `Stream` Library
- [Stream-InOut](./Examples/Stream-InOut/) shows basic usage of `Stream` Library for `InputStream` and `OutputStream`
- [Stream-Limit](./Examples/Stream-Limit/) shows basic usage of `Stream` Library for `InputStream` and `OutputStream` with limit
- [Stream-ReadLine](./Examples/Stream-ReadLine/) shows how to read single line or pattern over `InputStream`
- [STM32F4-Echo](./Examples/STM32F4-Echo/) shows how to port `Stream` Library for `STM32F4xx` microcontrollers and echo data over `UART`
- [STM32F4-ReadLine](./Examples/STM32F4-ReadLine/) shows how to port `Stream` Library for `STM32F4xx` microcontrollers and read single line or pattern over `UART`
- [STM32F429-DISCO](./Examples/STM32F429-DISCO-Stream/) shows how to port `Stream` Library for `STM32F429xx` microcontrollers and echo data over `UART`


## Contributing

We welcome contributions to the StreamBuffer library. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

The StreamBuffer C library is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgments

We would like to thank all the contributors to this project for their valuable input and support.
