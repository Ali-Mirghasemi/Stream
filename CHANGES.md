# Changes Log


## 07/17/2022

#### StreamBuffer - v0.3.0 
- Add new Stream_Result
    - `Stream_InReceive` when user want to receive data from stream when last receive not completed
    - `Stream_InTransmit` when user want to transmit data to stream when last transmit not completed

#### IStream - v0.3.0 
- Add safety for receive when already in receive mode
- Fix configuration name, `ISTREAM_CHECK_RECEIVE`

#### OStream - v0.3.0
- Add safety for transmit when already in transmit mode

---
