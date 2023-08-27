#include <stdio.h>
#include <stdlib.h>
#include <kmk/_avx_asm.h>

void _simd_vecadd(v4df a[], v4df b[], v4df out[], unsigned int n);

void main() {

	v4df a[] = {{0.1f, 0.2f, 0.3f, 0.4f}, {0.2f, 0.3f, 0.4f, 0.5f}};
	v4df b[] = {{0.1f, 0.2f, 0.3f, 0.4f}, {0.2f, 0.3f, 0.4f, 0.5f}};
	v4df out[2];
	_simd_vecadd(a, b, out, 2);

	// checking the outputs :)
	printf("%lf %lf", out[0][1], out[1][1]);
}
