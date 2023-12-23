#ifndef __AT_H__
#define __AT_H__

#include "at_type.h"
#include "at_mem.h"
#include "at_stream_device.h"

#define AT_TERMINATOR_DEFAULT            ('\n')
#define AT_READSTRING_LEN_DEFAULT        (200)

struct At{
    At_Err_t (*cutString)(struct At* this, At_Param_t param, const char* atLable);
	At_State_t (*checkString)(struct At* this, At_Param_t param, const char* atLable);

    size_t (*getParamMaxNum)(struct At* this);
    At_State_t (*getStateTable)(struct At* this);

    At_Err_t (*setInputDevice)(struct At* this, Stream* input_dev);
    At_Err_t (*setOutputDevice)(struct At* this, Stream* output_dev);

    const char* (*errorToString)(At_Err_t error);

    At_Err_t (*handle)(struct At* this, const char* atLable);
    // 一次读取一个字符
    At_Err_t (*handleAuto)(struct At* this);

    // size_t (*printf)(struct At* this, const char* format, ...)  __attribute__ ((format (printf, 2, 3)));
    int (*printf)(struct At* this, const char* format, ...);
    int (*print)(struct At* this, const char* message);
    int (*println)(struct At* this, const char* message);
    At_Err_t (*printSet)(struct At* this, const char* name);
    At_Err_t (*sendInfor)(struct At* this, const char* infor);

    At_State_t _atTable;
    Stream* _input_dev;
    Stream* _output_dev;
    size_t _param_max_num;
    char _terminator;
    char* _readString;
    size_t _readString_len;
    size_t _readString_used;
};
typedef struct At At;

// dangerous
At_Err_t At_Create(
    At* this,
    const At_State_t atTable, Stream* input_dev, Stream* output_dev,
    size_t argc, ...
);

// safer
At_Err_t At_Create_s(
    At* this,
    const At_State_t atTable, Stream* input_dev, Stream* output_dev,
    size_t readString_len, size_t param_max_num, char terminator
);

At_Err_t At_Init(
    At* this,
    const At_State_t atTable, Stream* input_dev, Stream* output_dev,
    char* readString, size_t readString_len, size_t argc, ...
);

At_Err_t At_Delete(At* this);

#endif  // !__AT_H__
