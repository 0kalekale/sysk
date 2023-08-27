/*
	intel AVX v4df multiplication  
	-mavx while compiling

	written by 0kalekale, this is under AGPL3	
*/

	.file	"_mulv4df.s"
	.text
	.globl	_mulv4df
	.type	_mulv4df, @function

// v4df _mulv4df(v4df, v4df);
_mulv4df:
	pushq	%rbp
	movq	%rsp, %rbp
	
	andq	$-32, %rsp
	vmovapd	%ymm0, -32(%rsp)
	vmovapd	%ymm1, -64(%rsp)
	vmovapd	-32(%rsp), %ymm0
	vmulpd	-64(%rsp), %ymm0, %ymm0
	leave
	
	ret
