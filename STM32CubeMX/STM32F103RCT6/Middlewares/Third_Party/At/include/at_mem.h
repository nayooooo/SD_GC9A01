#ifndef __AT_MEM_H__
#define __AT_MEM_H__

#include <stdint.h>
#include "at_type.h"

void *at_malloc(size_t _Size);
void at_free(void* _Memory);
void *at_memset(void *_Dst, int _Val, size_t _Size);
int at_memcmp(const void *_Buf1, const void *_Buf2, size_t _Size);
void *at_memcpy(void * _Dst, const void * _Src, size_t _Size);

#endif  // !__AT_MEM_H__
