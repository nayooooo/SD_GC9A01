#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <stdint.h>

typedef struct{
    double real;
    double imag;
} complex;
#define _COMPLEX_DEFINED

complex* complex_add(complex* result, complex a, complex b);
complex* complex_sub(complex* result, complex a, complex b);
complex* complex_mul(complex* result, complex a, complex b);
complex* complex_div(complex* result, complex a, complex b);

complex* complex_phaseInv(complex* data);

#endif  // !__COMPLEX_H__
