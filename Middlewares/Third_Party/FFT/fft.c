#include "fft.h"
#include <string.h>
#include <math.h>

#define FFT_DBG
#define FFT_DBG_LVL FFT_DBG_ERROR
#include "fft_dbg.h"

static fft_err_t fft_isNPowOf2(uint32_t n)
{
    if ((n & (n - 1)) != 0) {
        FFT_DBG_ERR("The number(%u) is not a 2^N number.", n);
        return FFT_ERROR;
    }
    return FFT_OK;
}

static fft_err_t fft_swap(complex* a, complex* b)
{
    complex t;
    memcpy(&t, a,  sizeof(complex));
    memcpy(a,  b,  sizeof(complex));
    memcpy(b,  &t, sizeof(complex));
    return FFT_OK;
}

// Reverse index
static fft_err_t fft_indexed(complex* data, uint32_t N)
{
    if (N == 0) {
        FFT_DBG_ERR("The data's number can't be 0.");
        return FFT_ERROR_POINTS;
    }
    if ((N == 1) || (N == 2)) {
        FFT_DBG_INF("The data's number is %u, do not need indexed.", N);
        return FFT_OK;
    }
    if (FFT_OK != fft_isNPowOf2(N)) {
        FFT_DBG_ERR("The number(%u) is not a 2^N number.", N);
        return FFT_ERROR_POINTS;
    }

    int i, j, k;
    for (i = 1, j = N>>1; i < N-1; i++) {
        if (i < j) fft_swap(&data[i], &data[j]);
        FFT_DBG_VER("[%d]<==>[%d]", i, j);
        k = N>>1;
        while(k <= j)
        {
            j -= k;
            k >>= 1;
        }
        j += k;
    }
    return FFT_OK;
}

// calculate WN
static fft_err_t fft_calcWNkn(complex* result, uint32_t N, uint32_t kn)
{
    result->real = cos(2 * PI * kn / N);
    result->imag = -sin(2 * PI * kn / N);
    return FFT_OK;
}

// butterfly operation
static fft_err_t fft_butterflyOperation(complex* data, uint32_t N)
{
    if (N < 2) {
        FFT_DBG_ERR("The number(%u) is less then 2.", N);
        return FFT_ERROR_POINTS;
    }
    if (FFT_OK != fft_isNPowOf2(N)) {
        FFT_DBG_ERR("The number(%u) is not a 2^N number.", N);
        return FFT_ERROR_POINTS;
    }

    uint32_t temp = N, M = 0;
    do { M++; temp >>= 1; } while (temp != 1);
    uint32_t pfac;
    complex WNp, t, addend;
    for (uint32_t L = 1; L <= M; L++) {  // L-layer butterfly operation
        pfac = 1U << (M - L);
        for (uint32_t J = 0; J < 1U << (L - 1); J++) {  // Calculate rotation factor
            fft_calcWNkn(&WNp, N, J * pfac);
            FFT_DBG_VER("L=%u, J=%u, WNp: ", L, J);
            fft_printData_ver(&WNp, 1);
            for (uint32_t i = J; i < N; i += 1ULL << L) {  // Perform butterfly operation
                memcpy(&addend, &data[i], sizeof(complex));
                complex_mul(&t, data[i + (1ULL << (L - 1))], WNp);
                complex_add(&data[i], addend, t);
                complex_sub(&data[i + (1ULL << (L - 1))], addend, t);
            }
        }
        if (L == 1) {
            FFT_DBG_VER("Calculation completed for the %ust level butterfly operation", L);
        }
        else if (L == 2) {
            FFT_DBG_VER("Calculation completed for the %und level butterfly operation", L);
        }
        else if (L == 3) {
            FFT_DBG_VER("Calculation completed for the %urd level butterfly operation", L);
        }
        else {
            FFT_DBG_VER("Calculation completed for the %uth level butterfly operation", L);
        }
        fft_printData_ver(data, N);
    }

    return FFT_OK;
}

// DIT-FFT
fft_err_t fft(complex* x, uint32_t x_N, complex* y, uint32_t N)
{
    if (x_N == 0) {
        FFT_DBG_ERR("The x data is empty!");
        return FFT_ERROR_INPUT_SIGNAL;
    }
    if (N == 0) {
        FFT_DBG_ERR("The y data is empty!");
        return FFT_ERROR_POINTS;
    }
    if (FFT_OK != fft_isNPowOf2(N)) {
        FFT_DBG_ERR("The number(%u) is not a 2^N number.", N);
        return FFT_ERROR_POINTS;
    }

    memset(y, 0, N * sizeof(complex));
    if (N >= x_N) {
        memcpy(y, x, x_N * sizeof(complex));
    } else {
        memcpy(y, x, N * sizeof(complex));
    }
    fft_indexed(y, N);
    FFT_DBG_VER("indexed sequence: ");
    fft_printData_ver(y, N);
    fft_butterflyOperation(y, N);

    return FFT_OK;
}

fft_err_t ifft(complex* x, uint32_t x_N, complex* y, uint32_t N)
{
    if (x_N == 0) {
        FFT_DBG_ERR("The x data is empty!");
        return FFT_ERROR_INPUT_SIGNAL;
    }
    if (N == 0) {
        FFT_DBG_ERR("The y data is empty!");
        return FFT_ERROR_POINTS;
    }
    if (FFT_OK != fft_isNPowOf2(N)) {
        FFT_DBG_ERR("The number(%u) is not a 2^N number.", N);
        return FFT_ERROR_POINTS;
    }

    // In order to not change x, it is necessary to restore it after fft.
    // If it involves multiple threads, please add a lock here.
    for (uint32_t i = 0; i < x_N; i++) {
        complex_phaseInv(&x[i]);
    }
    fft(x, x_N, y, N);
    for (uint32_t i = 0; i < x_N; i++) {
        complex_phaseInv(&x[i]);
    }

    complex temp = { N, 0 };
    for (uint32_t i = 0; i < N; i++) {
        complex_div(&y[i], y[i], temp);
        complex_phaseInv(&y[i]);
    }

    return FFT_OK;
}

fft_err_t fft_printData(complex* data, uint32_t N)
{
    fft_printf("complex sequence(N=%u): \r\n", N);
    for (uint64_t i = 0; i < N; i++) {
        fft_printf("[%u]\t%.4f%+.4fi\r\n", (uint32_t)i, data[i].real, data[i].imag);
    }
    return FFT_OK;
}

fft_err_t fft_printData_ver(complex* data, uint32_t N)
{
    if (FFT_DBG_LVL < FFT_DBG_VERBOSE) {
        return FFT_ERROR;
    }
    FFT_DBG_VER("complex sequence(N=%u): ", N);
    for (uint64_t i = 0; i < N; i++) {
        FFT_DBG_VER("[%u]\t%.4f%+.4fi", i, data[i].real, data[i].imag);
    }
    return FFT_OK;
}
