#include <stdlib.h>
#include <string.h>
#include <kmk/_avx_asm.h>
#include <kmk/_simd.h>

void _simd_vecadd(v4df *a, v4df *b, v4df *out, unsigned int n) {
	for (int i = 0; i<n; i++) {
		*out = _addv4df(*a, *b);
		out++; a++; b++;
	}
}

void _simd_vecmul(v4df *a, v4df *b, v4df *out, unsigned int n) {
	for  (int i = 0; i<n; i++) {
		*out = _mulv4df(*a, *b);
		out++; a++; b++;
	}
}

void _simd_vecsqrt(v4df *a, v4df *out, unsigned int n) {
	for (int i = 0; i<n; i++) {
		*out = _sqrtv4df(*a);
		out++; a++;
	}
}
