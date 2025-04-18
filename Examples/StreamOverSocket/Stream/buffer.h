#ifndef _OUTPUT_STREAM_H_
#define _OUTPUT_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "stdbool.h"

void buffer_init(void * buffer,size_t len);

size_t buffer_available();
bool buffer_add(uint8_t* buffer ,size_t len);
size_t buffer_read(uint8_t* buffer ,size_t len);

#ifdef __cplusplus
};
#endif
#endif