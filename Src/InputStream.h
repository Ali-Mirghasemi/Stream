#ifndef _INPUT_STREAM_H_
#define _INPUT_STREAM_H_

#include <stdint.h>


typedef struct {
    uint8_t*    Buffer;
    uint16_t    Length;
    uint16_t    WPos;
    uint16_t    RPos;
} Stream;





#endif /* _INPUT_STREAM_H_ */
