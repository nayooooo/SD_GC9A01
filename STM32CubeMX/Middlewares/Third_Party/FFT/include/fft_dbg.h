#ifndef __FFT_DBG_H__
#define __FFT_DBG_H__

#include <stdio.h>

#define fft_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)

#define FFT_DBG_OFF         (0)
#define FFT_DBG_ERROR       (1)
#define FFT_DBG_WARNING     (2)
#define FFT_DBG_INFO        (3)
#define FFT_DBG_LOGE        (4)
#define FFT_DBG_VERBOSE     (5)

#ifdef FFT_DBG
#   if FFT_DBG_LVL >= FFT_DBG_ERROR
#       define FFT_DBG_ERR(fmt, ...) printf("[FFT ERROR] ");printf(fmt, ##__VA_ARGS__);printf("\r\n")
#   endif
#   if FFT_DBG_LVL >= FFT_DBG_WARNING
#       define FFT_DBG_WAR(fmt, ...) printf("[FFT WARNING] ");printf(fmt, ##__VA_ARGS__);printf("\r\n")
#   endif
#   if FFT_DBG_LVL >= FFT_DBG_INFO
#       define FFT_DBG_INF(fmt, ...) printf("[FFT INFO] ");printf(fmt, ##__VA_ARGS__);printf("\r\n")
#   endif
#   if FFT_DBG_LVL >= FFT_DBG_LOGE
#       define FFT_DBG_LOG(fmt, ...) printf("[FFT LOGE] ");printf(fmt, ##__VA_ARGS__);printf("\r\n")
#   endif
#   if FFT_DBG_LVL >= FFT_DBG_VERBOSE
#       define FFT_DBG_VER(fmt, ...) printf("[FFT VERBOS] ");printf(fmt, ##__VA_ARGS__);printf("\r\n")
#   endif
#endif  // FFT_DBG

#ifndef FFT_DBG_ERR
#   define FFT_DBG_ERR(fmt, ...)
#endif  // !FFT_DBG_ERR
#ifndef FFT_DBG_WAR
#   define FFT_DBG_WAR(fmt, ...)
#endif  // !FFT_DBG_WAR
#ifndef FFT_DBG_INF
#   define FFT_DBG_INF(fmt, ...)
#endif  // !FFT_DBG_INF
#ifndef FFT_DBG_LOG
#   define FFT_DBG_LOG(fmt, ...)
#endif  // !FFT_DBG_LOG
#ifndef FFT_DBG_VER
#   define FFT_DBG_VER(fmt, ...)
#endif  // !FFT_DBG_VER

#endif  // !__FFT_DBG_H__
