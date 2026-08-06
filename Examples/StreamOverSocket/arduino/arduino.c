#include "arduino.h"
#include <windows.h>

void delay(uint32_t time)
{
    Sleep(time);
}
uint32_t millis()
{
    static uint32_t _startMillis;
    if(_startMillis == 0){
        _startMillis = GetTickCount();
        return 0;
    }
    return  GetTickCount() - _startMillis;
}

void replaceSpecialChar(char ch, uint8_t *buffer)
{
    switch (ch)
    {
    case '\r':
        snprintf(buffer, 6, "\\r");
        break;
    case '\n':
        snprintf(buffer, 6, "\\n");
        break;
    case '\t':
        snprintf(buffer, 6, "\\t");
        break;
    case '\0':
        snprintf(buffer, 6, "\\0");
        break;
    default:
        snprintf(buffer, 6, "%c", ch); // Return the original character if it's not a special character
    }
}
void print_arr_hex(uint8_t *buffer, int len)
{
    printf("array(%d)[ ", len);
    for (int i = 0; i < len; i++)
    {
        printf("%02X", buffer[i]);
        if (i != len - 1)
        {
            printf(" ");
        }
    }
    printf(" ]");
}
void print_arr_norm(uint8_t *buffer, int len)
{
    uint8_t temp[6];
    printf("array(%d)[ ", len);
    for (int i = 0; i < len; i++)
    {
        replaceSpecialChar(buffer[i], temp);
        printf("%s", temp);
        if (i != len - 1)
        {
        }
    }
    printf(" ]");
}
void print_hex(uint8_t *buffer, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%02X", buffer[i]);
        if (i != len - 1)
        {
            printf(" ");
        }
    }
}
void print_norm(uint8_t *buffer, int len)
{
    uint8_t temp[6];
    for (int i = 0; i < len; i++)
    {
        replaceSpecialChar(buffer[i], temp);
        printf("%s", temp);
        if (i != len - 1)
        {
        }
    }
}

void print_f(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

