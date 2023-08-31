/*
	intel AVX v4df sqrt  
	-mavx while compiling

	written by 0kalekale, this is under AGPL3	
*/

	.file	"_sqrtv4df.s"
	.text
	.globl	_sqrtv4df
	.type	_sqrtv4df, @function

// v4df _sqrtv4df(v4df);
_sqrtv4df:
	pushq	%rbp
	movq	%rsp, %rbp
	
	andq	$-32, %rsp
	vmovapd	%ymm0, -32(%rsp)
	vsqrtpd	-32(%rsp), %ymm0
	leave
	
	ret
