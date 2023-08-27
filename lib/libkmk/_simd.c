#include <stdlib.h>
#include <string.h>
#include <kmk/_avx_asm.h>

// shit finally works, whew lad
void _simd_vecadd(v4df a[], v4df b[], v4df out[], unsigned int n) {
	for (int i = 0; i<n; i++) {
		out[i] = _addv4df(a[i], b[i]);
	}
}

void _simd_vecmul(v4df a[], v4df b[], v4df out[], unsigned int n) {
	for  (int i = 0; i<n; i++) {
		out[i] = _mulv4df(a[i], b[i]);
	}
}
