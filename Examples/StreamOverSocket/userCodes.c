#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "UARTStream.h"
#include "Str.h"
#include "arduino.h"


uint8_t rxBuf1[64];
uint8_t txBuf1[64];
UARTStream uart1;


void setup()
{
    millis(); // reference it from now
    setbuf(stdout, NULL); // Disable buffering for stdout
    bool ret = UARTStream_init(
        &uart1, "127.0.0.1", 8888, "COM16",
        rxBuf1, sizeof(rxBuf1),
        txBuf1, sizeof(txBuf1));
    if (ret == false)
    {
        printf("ret == false");
        exit(0);
    }


    IStream_receive(&uart1.Input);
    
}
void loop()
{
    if(IStream_available(&uart1.Input)) {
        OStream_writeStream(&uart1.Output, &uart1.Input, IStream_available(&uart1.Input));
        OStream_flush(&uart1.Output);
    }

}

