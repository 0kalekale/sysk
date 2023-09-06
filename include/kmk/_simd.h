#include <kmk/_avx_asm.h>

void _simd_vecadd(v4df *a, v4df *b, v4df *out, unsigned int n);
void _simd_vecmul(v4df *a, v4df *b, v4df *out, unsigned int n);
void _simd_vecsqrt(v4df *a, v4df *out, unsigned int n);
