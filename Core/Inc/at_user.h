#ifndef __AT_USER_H__
#define __AT_USER_H__

#include "at.h"

#define AT_USER_OK "OK"
#define AT_USER_ERROR_ARGC "error by argc"
#define AT_USER_ERROR_ARGV "error by argv"
#define AT_USER_ERROR_MALLOC "error by malloc"
#define AT_USER_ERROR_TIMEOUT "error by timeout"
#define AT_USER_ERROR_NOTFIND "error by not found"
#define AT_USER_ERROR_FUNCTION "error by function"

extern At at;

At_Err_t at_user_init(void);

#endif  // !__AT_USER_H__
