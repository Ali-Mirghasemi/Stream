#include "buffer.h"


uint8_t *_buffer;
size_t _size;
volatile size_t _head,_tail;
void buffer_init(void * buffer,size_t len){
    _buffer = buffer;
    _size = len;
    _head = _tail = 0;

}

size_t buffer_available(){
    if(_head >= _tail){
        return _head - _tail;
    }
    else {
        return _size - _tail + _head ;
    }

}
bool buffer_add(uint8_t* buffer ,size_t len){
    if(len <= _size -  buffer_available()){
        for(size_t i=0;i<len;i++){
            _buffer[_head] = buffer[i];
            _head++;
            _head %= _size;
        }
        return 1;
    }
    else {
        return 0; //no space
    }

}
size_t buffer_read(uint8_t* buffer ,size_t len){
    size_t readed=0,available;
    if((available = buffer_available()))
    {
        if(available <= len){
            len = available;
        }
        for(size_t i=0;i<len;i++){
            buffer[i] = _buffer[_tail];
            _tail++;
            _tail %= _size;
            readed++;
        }
    }
    return readed;
}
