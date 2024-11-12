#include <stdio.h>

#include "StreamBuffer.h"
#include <string.h>

#define CYCLES_NUM      20

#define PRINTF          printf
#define PRINT_DUMP      1


uint32_t assertResult;
uint8_t cycles;
uint8_t index = 0;
// assert functions
uint32_t Assert_Char(char num1, char num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_Int8(int8_t num1, int8_t num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_Int16(int16_t num1, int16_t num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_Int32(int32_t num1, int32_t num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_UInt8(uint8_t num1, uint8_t num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_UInt16(uint16_t num1, uint16_t num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_UInt32(uint32_t num1, uint32_t num2, uint16_t line, uint8_t cycle, uint8_t index);
uint32_t Assert_Float(float num1, float num2, uint16_t line, uint8_t cycle, uint8_t index);
#if STREAM_UINT64
    uint32_t Assert_Int64(int64_t num1, int64_t num2, uint16_t line, uint8_t cycle, uint8_t index);
    uint32_t Assert_UInt64(uint64_t num1, uint64_t num2, uint16_t line, uint8_t cycle, uint8_t index);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    uint32_t Assert_Double(double num1, double num2, uint16_t line, uint8_t cycle, uint8_t index);
#endif
uint32_t Assert_Bytes(uint8_t* a, uint8_t* b, uint32_t len, uint16_t line, uint8_t cycle, uint8_t index);

void Assert_dump(StreamBuffer* stream);

#if PRINT_DUMP == 0
    #define assert(TYPE, ...)               if ((assertResult = Assert_ ##TYPE (__VA_ARGS__, __LINE__, cycles, index))) return assertResult; Assert_dump(&stream);
#else
    #define assert(TYPE, ...)               if ((assertResult = Assert_ ##TYPE (__VA_ARGS__, __LINE__, cycles, index))) { Assert_dump(&stream); return assertResult; }
#endif

typedef uint32_t (*Test_Fn)(void);

uint32_t Test_readWrite(void);
#if STREAM_WRITE_STREAM && STREAM_READ_STREAM
    uint32_t Test_readStream(void);
#endif
#if STREAM_WRITE_FLIP && STREAM_READ_FLIP
    uint32_t Test_flip(void);
#endif
#if STREAM_WRITE_ARRAY && STREAM_READ_ARRAY
    uint32_t Test_readWriteArray(void);
#endif
#if STREAM_GET_AT_FUNCTIONS && STREAM_GET_FUNCTIONS
    uint32_t Test_get(void);
#endif
#if STREAM_FIND_AT_FUNCTIONS && STREAM_FIND_FUNCTIONS
    uint32_t Test_find(void);
#endif
#if STREAM_WRITE_LOCK && STREAM_READ_LOCK
    uint32_t Test_lock(void);
#endif

static const Test_Fn TESTS[] = {
    Test_readWrite,
#if STREAM_WRITE_STREAM && STREAM_READ_STREAM
    Test_readStream,
#endif
#if STREAM_WRITE_FLIP && STREAM_READ_FLIP
    Test_flip,
#endif
#if STREAM_WRITE_ARRAY && STREAM_READ_ARRAY
    Test_readWriteArray,
#endif
#if STREAM_GET_AT_FUNCTIONS && STREAM_GET_FUNCTIONS
    Test_get,
#endif
#if STREAM_FIND_AT_FUNCTIONS && STREAM_FIND_FUNCTIONS
    Test_find,
#endif
#if STREAM_WRITE_LOCK && STREAM_READ_LOCK
    Test_lock,
#endif
};
static const uint32_t TESTES_LEN = sizeof(TESTS) / sizeof(TESTS[0]);

int main()
{
    uint32_t result;
    uint32_t errorCount = 0;
    int testIndex;

    for (testIndex = 0; testIndex < TESTES_LEN; testIndex++) {
        result = TESTS[testIndex]();
        if (result) {
            errorCount++;
            PRINTF("Cycle: %3u, Index: %3u, Line: %3u\n\n", result & 0xFF, (result >> 8) & 0xFF, (result >> 16) & 0xFFFF);
        }
    }
    if (errorCount > 0) {
        PRINTF("Error Count: %u\n", errorCount);
    }
    else {
        PRINTF("Test Pass\n");
    }

}
/********************************************************/
uint32_t Test_readWrite(void) {
    #define test(TYPE, N)               PRINTF("Write/Read " #TYPE ", %ux\n", N);\
                                        for (cycles = 0; cycles < CYCLES_NUM; cycles++) {\
                                            for (index = 0; index < N; index++) {\
                                                Stream_write ##TYPE (&stream, cycles * N + index);\
                                            }\
                                            for (index = 0; index < N; index++) {\
                                                assert(TYPE, Stream_read ##TYPE (&stream), cycles * N + index);\
                                            }\
                                        }

    #define testBytes(TYPE, PAT, N)     PRINTF("Write/Read " #TYPE ", %ux\n", N);\
                                        for (cycles = 0; cycles < CYCLES_NUM; cycles++) {\
                                            for (index = 0; index < N; index++) {\
                                                Stream_write ##TYPE (&stream, (uint8_t*) PAT, sizeof(PAT));\
                                            }\
                                            for (index = 0; index < N; index++) {\
                                                Stream_read ##TYPE (&stream, tempBuff, sizeof(PAT));\
                                                assert(Bytes, tempBuff, (uint8_t*) PAT, sizeof(PAT));\
                                            }\
                                        }


    const uint8_t PAT1[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77
    };
    const uint8_t PAT2[] = {
        0x1A, 0x2A, 0x3A, 0x4A, 0x5A,
    };
    const uint8_t PAT3[] = {
        0x1B, 0x2B, 0x3B,
    };
    const uint8_t PAT4[] = {
        0x1C, 0x2C, 0x3C, 0x4C,
    };
    uint8_t tempBuff[20];
    uint8_t streamBuff[37];
    StreamBuffer stream;

    Stream_init(&stream, streamBuff, sizeof(streamBuff));

    testBytes(Bytes, PAT1, 1);
    testBytes(Bytes, PAT1, 3);
    testBytes(Bytes, PAT1, 5);
    testBytes(Bytes, PAT1, 7);

    testBytes(Bytes, PAT2, 1);
    testBytes(Bytes, PAT2, 3);
    testBytes(Bytes, PAT2, 5);
    testBytes(Bytes, PAT2, 7);

    testBytes(Bytes, PAT3, 1);
    testBytes(Bytes, PAT3, 3);
    testBytes(Bytes, PAT3, 5);
    testBytes(Bytes, PAT3, 7);

    testBytes(Bytes, PAT4, 1);
    testBytes(Bytes, PAT4, 3);
    testBytes(Bytes, PAT4, 5);
    testBytes(Bytes, PAT4, 7);

    testBytes(BytesReverse, PAT1, 1);
    testBytes(BytesReverse, PAT1, 3);
    testBytes(BytesReverse, PAT1, 5);
    testBytes(BytesReverse, PAT1, 7);

    testBytes(BytesReverse, PAT2, 1);
    testBytes(BytesReverse, PAT2, 3);
    testBytes(BytesReverse, PAT2, 5);
    testBytes(BytesReverse, PAT2, 7);

    testBytes(BytesReverse, PAT3, 1);
    testBytes(BytesReverse, PAT3, 3);
    testBytes(BytesReverse, PAT3, 5);
    testBytes(BytesReverse, PAT3, 7);

    testBytes(BytesReverse, PAT4, 1);
    testBytes(BytesReverse, PAT4, 3);
    testBytes(BytesReverse, PAT4, 5);
    testBytes(BytesReverse, PAT4, 7);

#if STREAM_BYTE_ORDER
    for (ByteOrder order = ByteOrder_LittleEndian; order <= ByteOrder_BigEndian; order++) {
    Stream_setByteOrder(&stream, order);
    PRINTF("--------- Read/Write Order: %u ---------\n", order);
#endif // STREAM_BYTE_ORDER

    test(Char, 1);
    test(Char, 3);
    test(Char, 5);
    test(Char, 7);

    test(Int8, 1);
    test(Int8, 3);
    test(Int8, 5);
    test(Int8, 7);

    test(UInt8, 1);
    test(UInt8, 3);
    test(UInt8, 5);
    test(UInt8, 7);

    test(Int16, 1);
    test(Int16, 3);
    test(Int16, 5);
    test(Int16, 7);

    test(UInt16, 1);
    test(UInt16, 3);
    test(UInt16, 5);
    test(UInt16, 7);

    test(Int32, 1);
    test(Int32, 3);
    test(Int32, 5);
    test(Int32, 7);

    test(UInt32, 1);
    test(UInt32, 3);
    test(UInt32, 5);
    test(UInt32, 7);

#if STREAM_UINT64
    test(Int64, 1);
    test(Int64, 3);
    test(Int64, 4);

    test(UInt64, 1);
    test(UInt64, 3);
    test(UInt64, 4);
#endif // STREAM_UINT64

    test(Float, 1);
    test(Float, 3);
    test(Float, 5);
    test(Float, 7);

#if STREAM_DOUBLE
    test(Double, 1);
    test(Double, 3);
    test(Double, 4);
#endif // STREAM_DOUBLE

#if STREAM_BYTE_ORDER
    }
#endif // STREAM_BYTE_ORDER

    return 0;
#undef test
#undef testBytes
}
/********************************************************/
#if STREAM_WRITE_STREAM && STREAM_READ_STREAM
uint32_t Test_readStream(void) {
// for compatibility with print dump
#define stream      out

    const uint8_t PAT[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77
    };

    uint8_t tempBuff[40];
    uint8_t inBuff[40];
    uint8_t outBuff[40];
    StreamBuffer in;
    StreamBuffer out;
    int cycles;

    Stream_init(&in, inBuff, sizeof(inBuff));
    Stream_init(&out, outBuff, sizeof(outBuff));

    for (cycles = 0; cycles < CYCLES_NUM; cycles++) {
        // write to out
        Stream_writeBytes(&out, (uint8_t*) PAT, sizeof(PAT));
        // transfer output to input
        Stream_readStream(&out, &in, Stream_available(&out));
        // read from input
        Stream_readBytes(&in, tempBuff, Stream_available(&in));
        // verify
        assert(Bytes, tempBuff, (uint8_t*) PAT, sizeof(PAT));
        // clear temp
        memset(tempBuff, 0, sizeof(tempBuff));
    }

    return 0;
#undef stream
}
#endif
/********************************************************/
#if STREAM_WRITE_LOCK && STREAM_READ_LOCK
uint32_t Test_readWriteArray(void) {
    #define test(TYPE, VAL_TY, L)       { \
                                            VAL_TY val[L] = {0}; \
                                            VAL_TY tmp[L] = {0}; \
                                            PRINTF("Write/Read Array " #TYPE ", %ux\n", L);\
                                            for (cycles = 0; cycles < CYCLES_NUM; cycles++) {\
                                                for (index = 0; index < L; index++) {\
                                                    val[index] =  cycles * L + index;\
                                                } \
                                                Stream_write ##TYPE ##Array (&stream, val, L); \
                                                Stream_read ##TYPE ##Array (&stream, tmp, L); \
                                                for (index = 0; index < L; index++) { \
                                                    assert(TYPE, val[index], tmp[index]); \
                                                } \
                                            } \
                                        }

    uint8_t streamBuff[37];
    StreamBuffer stream;

    Stream_init(&stream, streamBuff, sizeof(streamBuff));

#if STREAM_BYTE_ORDER
    for (ByteOrder order = ByteOrder_LittleEndian; order <= ByteOrder_BigEndian; order++) {
    Stream_setByteOrder(&stream, order);
    PRINTF("--------- Read/Write Array Order: %u ---------\n", order);
#endif // STREAM_BYTE_ORDER

    test(Char, char, 1);
    test(Char, char, 3);
    test(Char, char, 5);
    test(Char, char, 7);

    test(Int8, int8_t, 1);
    test(Int8, int8_t, 3);
    test(Int8, int8_t, 5);
    test(Int8, int8_t, 7);

    test(UInt8, uint8_t, 1);
    test(UInt8, uint8_t, 3);
    test(UInt8, uint8_t, 5);
    test(UInt8, uint8_t, 7);

    test(Int16, int16_t, 1);
    test(Int16, int16_t, 3);
    test(Int16, int16_t, 5);
    test(Int16, int16_t, 7);

    test(UInt16, uint16_t, 1);
    test(UInt16, uint16_t, 3);
    test(UInt16, uint16_t, 5);
    test(UInt16, uint16_t, 7);

    test(Int32, int32_t, 1);
    test(Int32, int32_t, 3);
    test(Int32, int32_t, 5);
    test(Int32, int32_t, 7);

    test(UInt32, uint32_t, 1);
    test(UInt32, uint32_t, 3);
    test(UInt32, uint32_t, 5);
    test(UInt32, uint32_t, 7);

#if STREAM_UINT64
    test(Int64, int64_t, 1);
    test(Int64, int64_t, 3);
    test(Int64, int64_t, 4);

    test(UInt64, uint64_t, 1);
    test(UInt64, uint64_t, 3);
    test(UInt64, uint64_t, 4);
#endif // STREAM_UINT64

    test(Float, float, 1);
    test(Float, float, 3);
    test(Float, float, 5);
    test(Float, float, 7);

#if STREAM_DOUBLE
    test(Double, double, 1);
    test(Double, double, 3);
    test(Double, double, 4);
#endif // STREAM_DOUBLE

#if STREAM_BYTE_ORDER
    }
#endif // STREAM_BYTE_ORDER

    return 0;
#undef test
}
#endif
/********************************************************/
#if STREAM_GET_AT_FUNCTIONS && STREAM_GET_FUNCTIONS
uint32_t Test_get(void) {
    #define test(TYPE, VAL_TY, N)       { \
                                            PRINTF("Get " #TYPE ", %ux\n", N);\
                                            for (cycles = 0; cycles < CYCLES_NUM; cycles++) {\
                                                for (index = 0; index < N; index++) {\
                                                    Stream_write ##TYPE (&stream, cycles * N + index); \
                                                } \
                                                for (index = 0; index < N; index++) { \
                                                    assert(TYPE, Stream_get ##TYPE ##At (&stream, index * sizeof(VAL_TY)), cycles * N + index); \
                                                } \
                                                Stream_resetIO(&stream); \
                                            } \
                                        }

    uint8_t streamBuff[37];
    StreamBuffer stream;

    Stream_init(&stream, streamBuff, sizeof(streamBuff));

#if STREAM_BYTE_ORDER
    for (ByteOrder order = ByteOrder_LittleEndian; order <= ByteOrder_BigEndian; order++) {
    Stream_setByteOrder(&stream, order);
    PRINTF("--------- Read/Write Array Order: %u ---------\n", order);
#endif // STREAM_BYTE_ORDER

    test(Char, char, 1);
    test(Char, char, 3);
    test(Char, char, 5);
    test(Char, char, 7);

    test(Int8, int8_t, 1);
    test(Int8, int8_t, 3);
    test(Int8, int8_t, 5);
    test(Int8, int8_t, 7);

    test(UInt8, uint8_t, 1);
    test(UInt8, uint8_t, 3);
    test(UInt8, uint8_t, 5);
    test(UInt8, uint8_t, 7);

    test(Int16, int16_t, 1);
    test(Int16, int16_t, 3);
    test(Int16, int16_t, 5);
    test(Int16, int16_t, 7);

    test(UInt16, uint16_t, 1);
    test(UInt16, uint16_t, 3);
    test(UInt16, uint16_t, 5);
    test(UInt16, uint16_t, 7);

    test(Int32, int32_t, 1);
    test(Int32, int32_t, 3);
    test(Int32, int32_t, 5);
    test(Int32, int32_t, 7);

    test(UInt32, uint32_t, 1);
    test(UInt32, uint32_t, 3);
    test(UInt32, uint32_t, 5);
    test(UInt32, uint32_t, 7);

#if STREAM_UINT64
    test(Int64, int64_t, 1);
    test(Int64, int64_t, 3);
    test(Int64, int64_t, 4);

    test(UInt64, uint64_t, 1);
    test(UInt64, uint64_t, 3);
    test(UInt64, uint64_t, 4);
#endif // STREAM_UINT64

    test(Float, float, 1);
    test(Float, float, 3);
    test(Float, float, 5);
    test(Float, float, 7);

#if STREAM_DOUBLE
    test(Double, double, 1);
    test(Double, double, 3);
    test(Double, double, 4);
#endif // STREAM_DOUBLE

#if STREAM_BYTE_ORDER
    }
#endif // STREAM_BYTE_ORDER

    return 0;
#undef test
}
#endif
/********************************************************/
#if STREAM_FIND_AT_FUNCTIONS && STREAM_FIND_FUNCTIONS
uint32_t Test_find(void) {
    #define test(TYPE, VAL_TY, N)       { \
                                            PRINTF("Find " #TYPE ", %ux\n", N);\
                                            for (cycles = 0; cycles < CYCLES_NUM; cycles++) {\
                                                for (index = 0; index < N; index++) {\
                                                    Stream_write ##TYPE (&stream, (VAL_TY) (cycles * N + index)); \
                                                } \
                                                for (index = 0; index < N; index++) { \
                                                    assert(TYPE, Stream_find ##TYPE (&stream, (VAL_TY) (cycles * N + index)), (VAL_TY) (index * sizeof(VAL_TY))); \
                                                } \
                                                Stream_resetIO(&stream); \
                                            } \
                                        }

    uint8_t streamBuff[37];
    StreamBuffer stream;

    Stream_init(&stream, streamBuff, sizeof(streamBuff));

#if STREAM_BYTE_ORDER
    for (ByteOrder order = ByteOrder_LittleEndian; order <= ByteOrder_BigEndian; order++) {
    Stream_setByteOrder(&stream, order);
    PRINTF("--------- Find Order: %u ---------\n", order);
#endif // STREAM_BYTE_ORDER

    test(Char, char, 1);
    test(Char, char, 3);
    test(Char, char, 5);
    test(Char, char, 7);

    test(Int8, int8_t, 1);
    test(Int8, int8_t, 3);
    test(Int8, int8_t, 5);
    test(Int8, int8_t, 7);

    test(UInt8, uint8_t, 1);
    test(UInt8, uint8_t, 3);
    test(UInt8, uint8_t, 5);
    test(UInt8, uint8_t, 7);

    test(Int16, int16_t, 1);
    test(Int16, int16_t, 3);
    test(Int16, int16_t, 5);
    test(Int16, int16_t, 7);

    test(UInt16, uint16_t, 1);
    test(UInt16, uint16_t, 3);
    test(UInt16, uint16_t, 5);
    test(UInt16, uint16_t, 7);

    test(Int32, int32_t, 1);
    test(Int32, int32_t, 3);
    test(Int32, int32_t, 5);
    test(Int32, int32_t, 7);

    test(UInt32, uint32_t, 1);
    test(UInt32, uint32_t, 3);
    test(UInt32, uint32_t, 5);
    test(UInt32, uint32_t, 7);

#if STREAM_UINT64
    test(Int64, int64_t, 1);
    test(Int64, int64_t, 3);
    test(Int64, int64_t, 4);

    test(UInt64, uint64_t, 1);
    test(UInt64, uint64_t, 3);
    test(UInt64, uint64_t, 4);
#endif // STREAM_UINT64

    test(Float, float, 1);
    test(Float, float, 3);
    test(Float, float, 5);
    test(Float, float, 7);

#if STREAM_DOUBLE
    test(Double, double, 1);
    test(Double, double, 3);
    test(Double, double, 4);
#endif // STREAM_DOUBLE

#if STREAM_BYTE_ORDER
    }
#endif // STREAM_BYTE_ORDER

    return 0;
#undef test
}
#endif
/********************************************************/
#if STREAM_WRITE_FLIP && STREAM_READ_FLIP
uint32_t Test_flip(void) {
    #define testFlipWrite(N, W, R, O)           Stream_init(&stream, streamBuff, sizeof(streamBuff));\
                                                stream.WPos = (W);\
                                                stream.RPos = (R);\
                                                stream.Overflow = (O);\
                                                Stream_flipWrite(&stream, (N));\
                                                assert(UInt32, Stream_space(&stream), (N));

    #define testFlipRead(N, W, R, O)            Stream_init(&stream, streamBuff, sizeof(streamBuff));\
                                                stream.WPos = (W);\
                                                stream.RPos = (R);\
                                                stream.Overflow = (O);\
                                                Stream_flipRead(&stream, (N));\
                                                assert(UInt32, Stream_available(&stream), (N));

    uint8_t streamBuff[36];
    StreamBuffer stream;

    testFlipWrite(0, 0, 0, 0);
    testFlipWrite(4, 0, 0, 0);
    testFlipWrite(4, 4, 0, 0);
    testFlipWrite(36, 0, 0, 0);
    testFlipWrite(36, 4, 4, 0);
    testFlipWrite(36, 32, 32, 0);
    testFlipWrite(0, 8, 0, 0);
    testFlipWrite(4, 8, 0, 0);
    testFlipWrite(4, 12, 4, 0);
    testFlipWrite(0, 32, 32, 0);
    testFlipWrite(4, 32, 32, 0);
    testFlipWrite(0, 4, 8, 1);
    testFlipWrite(4, 4, 8, 1);
    testFlipWrite(4, 12, 8, 0);
    testFlipWrite(0, 28, 32, 1);
    testFlipWrite(4, 28, 32, 1);
    testFlipWrite(4, 20, 32, 1);



    return 0;
#undef testFlipWrite
}
#endif
/********************************************************/
#if STREAM_WRITE_LOCK && STREAM_READ_LOCK
uint32_t Test_lock(void) {
    #define testLock(PAT, N)                    PRINTF("Lock R/W " #PAT " , %ux\n", N);\
                                                for (cycles = 0; cycles < CYCLES_NUM; cycles++) {\
                                                    for (index = 0; index < N; index++) {\
                                                        assert(UInt32, Stream_space(&stream), stream.Size - index * sizeof(PAT));\
                                                        Stream_lockWrite(&stream, &lock, sizeof(PAT));\
                                                        assert(UInt32, Stream_space(&lock), sizeof(PAT));\
                                                        Stream_unlockWrite(&stream, &lock);\
                                                        assert(UInt32, Stream_space(&stream), stream.Size - index * sizeof(PAT));\
                                                        Stream_lockWrite(&stream, &lock, sizeof(PAT));\
                                                        assert(UInt32, Stream_space(&lock), sizeof(PAT));\
                                                        Stream_writeBytes(&lock, (uint8_t*) PAT, sizeof(PAT));\
                                                        assert(UInt32, Stream_space(&lock), 0);\
                                                        Stream_unlockWrite(&stream, &lock);\
                                                    }\
                                                    for (index = 0; index < N; index++) {\
                                                        assert(UInt32, Stream_available(&stream), sizeof(PAT) * (N - index));\
                                                        Stream_lockRead(&stream, &lock, sizeof(PAT));\
                                                        assert(UInt32, Stream_available(&lock), sizeof(PAT));\
                                                        Stream_unlockRead(&stream, &lock);\
                                                        assert(UInt32, Stream_available(&stream), sizeof(PAT) * (N - index));\
                                                        Stream_lockRead(&stream, &lock, sizeof(PAT));\
                                                        assert(UInt32, Stream_available(&lock), sizeof(PAT));\
                                                        Stream_readBytes(&lock, tempBuff, sizeof(PAT));\
                                                        assert(UInt32, Stream_available(&lock), 0);\
                                                        assert(Bytes, tempBuff, (uint8_t*) PAT, sizeof(PAT));\
                                                        Stream_unlockRead(&stream, &lock);\
                                                    }\
                                                }

    const uint8_t PAT1[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    const uint8_t PAT2[] = {0x1A, 0x1B, 0x1C};
    const uint8_t PAT3[] = {0x0A, 0x0B, 0x0C, 0x0D};
    const uint8_t PAT4[] = {0x2A, 0x2B,};
    const uint8_t PAT5[36] = {0};

    uint8_t tempBuff[12];
    uint8_t streamBuff[36];
    StreamBuffer stream;
    StreamBuffer lock;

    Stream_init(&stream, streamBuff, sizeof(streamBuff));

    testLock(PAT1, 1);
    testLock(PAT1, 3);
    testLock(PAT1, 5);

    testLock(PAT2, 1);
    testLock(PAT2, 3);
    testLock(PAT2, 5);
    testLock(PAT2, 7);

    testLock(PAT3, 1);
    testLock(PAT3, 3);
    testLock(PAT3, 5);
    testLock(PAT3, 7);
    testLock(PAT3, 9);

    testLock(PAT4, 1);
    testLock(PAT4, 3);
    testLock(PAT4, 5);
    testLock(PAT4, 7);
    testLock(PAT4, 9);
    testLock(PAT4, 18);

    testLock(PAT5, 1);

    return 0;
#undef testLock
}
#endif
/********************************************************/
#define ASSERT_NUM(TYPE, DTYPE)     uint32_t Assert_ ##TYPE (DTYPE num1, DTYPE num2, uint16_t line, uint8_t cycle, uint8_t index) {\
                                        if (num1 != num2) {\
                                            PRINTF("Expected: %ld, Found: %ld\n", (long int) num2, (long int) num1);\
                                                return line << 16 | index << 8 | cycle;\
                                        }\
                                        return 0;\
                                    }

ASSERT_NUM(Char, char)
ASSERT_NUM(Int8, int8_t)
ASSERT_NUM(Int16, int16_t)
ASSERT_NUM(Int32, int32_t)
ASSERT_NUM(UInt8, uint8_t)
ASSERT_NUM(UInt16, uint16_t)
ASSERT_NUM(UInt32, uint32_t)
ASSERT_NUM(Float, float)
#if STREAM_UINT64
    ASSERT_NUM(Int64, int64_t)
    ASSERT_NUM(UInt64, uint64_t)
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    ASSERT_NUM(Double, double)
#endif // STREAM_DOUBLE

uint32_t Assert_Bytes(uint8_t* a, uint8_t* b, uint32_t len, uint16_t line, uint8_t cycle, uint8_t index) {
    if (memcmp(a, b, len) != 0) {
        return line << 16 | index << 8 | cycle;
    }
    return 0;
}

void Assert_dump(StreamBuffer* stream) {
    uint8_t* ptr = stream->Data;
    Stream_LenType len = stream->Size;
    PRINTF("----------------------- Stream Dump -----------------------\n");
    PRINTF("RPos: %u, WPos: %u, Size: %u, Available: %u, Space: %u, Overflow: %u\n", stream->RPos, stream->WPos, stream->Size, Stream_available(stream), Stream_space(stream), stream->Overflow);
    PRINTF("[");
    while (len-- > 0) {
        PRINTF("0x%02X, ", *ptr++);
    }
    PRINTF("]\n");
    PRINTF("-----------------------------------------------------------\n");
}
