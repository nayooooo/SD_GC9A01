#ifndef __AT_STREAM_DEVICE_H__
#define __AT_STREAM_DEVICE_H__

#include "at_type.h"

typedef struct At_Stream_Device Stream;
typedef int (*Stream_Print)(Stream* this, const char* message);
typedef int (*Stream_Available)(Stream* this);
typedef int (*Stream_Read)(Stream* this);
struct At_Stream_Device{
    Stream_Print print;
    Stream_Available available;
    Stream_Read read;

    size_t _available_num;
};

At_Err_t Stream_Init(Stream* this, size_t argc, ...);
At_Err_t Stream_Init_s(Stream* this, Stream_Print print, Stream_Available available, Stream_Read read);

#endif  // !__AT_STREAM_DEVICE_H__
