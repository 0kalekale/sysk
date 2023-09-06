#include <stdio.h>
#include <stdlib.h>
#include <kmk/_avx_asm.h>
#include <kmk/_simd.h>

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
// gcc test/1.c _simd.c _avx_asm/_addv4df.s _avx_asm/_mulv4df.s _avx_asm/_sqrtv4df.s -I../../include -mavx
void main() {

	v4df a[] = {{0.1f, 0.2f, 0.3f, 0.4f}, {0.2f, 0.3f, 0.4f, 0.5f}};
	v4df b[] = {{0.1f, 0.2f, 0.3f, 0.4f}, {0.2f, 0.3f, 0.4f, 0.5f}};
	v4df *out = malloc(sizeof(v4df)*2);
	_simd_vecadd(&a, &b, out, 2);

	// checking the outputs :)
	printf("%lf %lf", out[0][1], out[1][1]);
}
