#include "at_mem.h"
#include "malloc.h"

void *at_malloc(size_t _Size)
{
    return mymalloc(0, _Size);
}

void at_free(void* _Memory)
{
    myfree(0, _Memory);
}

void *at_memset(void *_Dst, int _Val, size_t _Size)
{
	mymemset(_Dst, (uint8_t)_Val, (uint32_t)_Size);
	return _Dst;
}

int at_memcmp(const void *_Buf1, const void *_Buf2, size_t _Size)
{
    return mymemcmp((void*)_Buf1, (void*)_Buf2, (uint32_t)_Size);
}

void *at_memcpy(void * _Dst, const void * _Src, size_t _Size)
{
    mymemcpy(_Dst, (void*)_Src, (uint32_t)_Size);
	return _Dst;
}
