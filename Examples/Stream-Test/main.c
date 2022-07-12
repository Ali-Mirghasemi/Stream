#include <stdio.h>

#include "StreamBuffer.h"
#include <string.h>

#define CYCLES_NUM      20

#define PRINTF          printf


uint32_t assertResult;
uint8_t cycles;
uint8_t index = 0;
// assert functions
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

#define assert(TYPE, ...)               if ((assertResult = Assert_ ##TYPE (__VA_ARGS__, __LINE__, cycles, index))) return assertResult;

typedef uint32_t (*Test_Fn)(void);

uint32_t Test_readWrite(void);
uint32_t Test_readStream(void);

static const Test_Fn TESTS[] = {
    Test_readWrite,
    Test_readStream,
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
    Stream stream;

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
}
/********************************************************/
uint32_t Test_readStream(void) {
    const uint8_t PAT[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77
    };

    uint8_t tempBuff[40];
    uint8_t inBuff[40];
    uint8_t outBuff[40];
    Stream in;
    Stream out;
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
}
/********************************************************/
#define ASSERT_NUM(TYPE, DTYPE)     uint32_t Assert_ ##TYPE (DTYPE num1, DTYPE num2, uint16_t line, uint8_t cycle, uint8_t index) {\
                                        if (num1 != num2) {\
                                        PRINTF("Expected: %ld, Found: %ld\n", (long int) num2, (long int) num1);\
                                            return line << 16 | index << 8 | cycles;\
                                        }\
                                        return 0;\
                                    }

ASSERT_NUM(Int8, int8_t);
ASSERT_NUM(Int16, int16_t);
ASSERT_NUM(Int32, int32_t);
ASSERT_NUM(UInt8, uint8_t);
ASSERT_NUM(UInt16, uint16_t);
ASSERT_NUM(UInt32, uint32_t);
ASSERT_NUM(Float, float);
#if STREAM_UINT64
    ASSERT_NUM(Int64, int64_t);
    ASSERT_NUM(UInt64, uint64_t);
#endif // STREAM_UINT64
#if STREAM_DOUBLE
    ASSERT_NUM(Double, double);
#endif // STREAM_DOUBLE

uint32_t Assert_Bytes(uint8_t* a, uint8_t* b, uint32_t len, uint16_t line, uint8_t cycle, uint8_t index) {
    if (memcmp(a, b, len) != 0) {
        return line << 16 | index << 8 | cycles;
    }
    return 0;
}
