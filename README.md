# Stream
`Stream` Library help you to serialize and deserialize binary data over any communication protocol, such as UART, Ethernet, ...

## Features
- Support Big-endian and Little-Endian
- Support serialize and deserialize all standard data types
- Memory safe for reading unaligned data
- `InputStream` and `OutputStream` can work over a HAL (Hardware Abstract Layer)
- User access to custom configuration base on hardware need
- Any bytes buffer can turn into `StreamBuffer` without zero copy
- Support Stream over Polling, Interrupt and DMA

## Dependencies
- `stdint.h`
- `string.h`

## Examples
- [Stream-Test](./Examples/Stream-Test/) test case for stream library
- [Stream-Example](./Examples/Stream-Example/) shows basic usage of `Stream` Library
- [Stream-InOut](./Examples/Stream-InOut/) shows basic usage of `Stream` Library for `InputStream` and `OutputStream`
- [Stream-Limit](./Examples/Stream-Limit/) shows basic usage of `Stream` Library for `InputStream` and `OutputStream` with limit
- [Stream-ReadLine](./Examples/Stream-ReadLine/) shows how to read single line or pattern over `InputStream`
- [STM32F4-Echo](./Examples/STM32F4-Echo/) shows how to port `Stream` Library for `STM32F4xx` microcontrollers and echo data over `UART`
- [STM32F4-ReadLine](./Examples/STM32F4-ReadLine/) shows how to port `Stream` Library for `STM32F4xx` microcontrollers and read single line or pattern over `UART`
