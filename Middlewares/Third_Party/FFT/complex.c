#include "./include/complex.h"
#include <string.h>

complex* complex_add(complex* result, complex a, complex b)
{
    result->real = a.real + b.real;
    result->imag = a.imag + b.imag;
    return result;
}

complex* complex_sub(complex* result, complex a, complex b)
{
    result->real = a.real - b.real;
    result->imag = a.imag - b.imag;
    return result;
}

complex* complex_mul(complex* result, complex a, complex b)
{
    result->real = a.real * b.real - a.imag * b.imag;
    result->imag = a.real * b.imag + a.imag * b.real;
    return result;
}

complex* complex_div(complex* result, complex a, complex b)
{
    double den = b.real * b.real + b.imag * b.imag;
    result->real = (a.real * b.real + a.imag * b.imag) / den;
    result->imag = (a.imag * b.real - a.real * b.imag) / den;
    return result;
}

complex* complex_phaseInv(complex* data)
{
    data->imag = -data->imag;
    return data;
}
