#ifndef __FFT_TYPE_H__
#define __FFT_TYPE_H__

#ifndef PI
#   define PI       (3.14159265358979323846)
#endif  // !PI

#include <stdint.h>
#include "./complex.h"

typedef enum{
    FFT_OK = 0,                     // No errors
    FFT_ERROR,                      // Normal error
    FFT_ERROR_INPUT_SIGNAL,         // Input signal error
    FFT_ERROR_OUTPUT_SIGNAL,        // Output signal error
    FFT_ERROR_POINTS,               // Error number of transformation points
} fft_err_t;

#endif  // !__FFT_TYPE_H__
