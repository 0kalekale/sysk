#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <kmk/_avx_asm.h>

/*
	build: gcc ../_avx_asm/_sqrtv4df.s -march=native main.c -I../../../../include 
	avx v4df sqrt testing
*/
 
int main() {
	v4df a = {2.0f, 3.0f, 4.0f, 5.0f};
	v4df out = _sqrtv4df(a);
	printf("%lf %lf %lf %lf", out[0], out[1], out[2], out[3]);
}
