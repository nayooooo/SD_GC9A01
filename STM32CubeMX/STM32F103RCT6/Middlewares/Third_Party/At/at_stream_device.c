#include "at_stream_device.h"

#include <stdarg.h>

static At_Err_t _Stream_Init(Stream* this, Stream_Print print, Stream_Available available, Stream_Read read)
{
    if (this == nullptr) return AT_ERROR;
    if (print == nullptr) return AT_ERROR;
    if (available == nullptr) return AT_ERROR;
    if (read == nullptr) return AT_ERROR;

    this->_available_num = 0;

    this->print = print;
    this->available = available;
    this->read = read;

    return AT_EOK;
}

At_Err_t Stream_Init(Stream* this, size_t argc, ...)
{
    if (this == nullptr) return AT_ERROR;
    if (argc != 3) return AT_ERROR;

    va_list args;
    va_start(args, argc);
    Stream_Print print = va_arg(args, Stream_Print);
    Stream_Available available = va_arg(args, Stream_Available);
    Stream_Read read = va_arg(args, Stream_Read);
    va_end(args);

    return _Stream_Init(this, print, available, read);
}

At_Err_t Stream_Init_s(Stream* this, Stream_Print print, Stream_Available available, Stream_Read read)
{
    return _Stream_Init(this, print, available, read);
}
