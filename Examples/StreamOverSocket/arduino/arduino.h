#ifndef SERIALPORT_GENERAL_H
#define SERIALPORT_GENERAL_H
#include "stdint.h"
#include "stdio.h"
#include "stdarg.h"

#define STR(X) X, strlen(X)
#define STR_C(X) X, sizeof(X)-1

void delay(uint32_t time);
uint32_t millis();
void print_arr_hex(uint8_t *buffer, int len);
void print_arr_norm(uint8_t *buffer, int len);
void print_hex(uint8_t *buffer, int len);
void print_norm(uint8_t *buffer, int len);
void print_f(const char *format, ...);



#endif //SERIALPORT_GENERAL_H
