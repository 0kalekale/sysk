/*
	sysk: linear algebra library (sped up with avx)
*/

#include <stdio.h>
#include <stdlib.h>
#include <kmk/_simd.h>

struct matrix {
		double *elements; 
		unsigned int x; 
		unsigned int y;
};

void matrixadd(struct matrix *a, struct matrix *b, struct matrix *out) {
	_simd_vecadd((v4df*)a->elements, (v4df*)b->elements, (v4df*)out->elements, a->x/a->y);
} 
