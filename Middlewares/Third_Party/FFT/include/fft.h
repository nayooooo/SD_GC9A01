#ifndef __FFT_H__
#define __FFT_H__

#include <fft_type.h>

fft_err_t fft(complex* x, uint32_t x_N, complex* y, uint32_t N);
fft_err_t ifft(complex* x, uint32_t x_N, complex* y, uint32_t N);
fft_err_t fft_printData(complex* data, uint32_t N);
fft_err_t fft_printData_ver(complex* data, uint32_t N);

#endif  // !__FFT_H__
