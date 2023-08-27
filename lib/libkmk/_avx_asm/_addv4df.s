/*
	intel AVX v4df addition 
	-mavx while compiling
	
	written by 0kalekale, this is under AGPL3 
*/

	.file	"_addv4df.s"
	.text
	.globl	_addv4df
	.type	_addv4df, @function

// v4df _addv4df(v4df, v4df);
_addv4df:
	pushq	%rbp
	movq	%rsp, %rbp
	
	andq	$-32, %rsp
	vmovapd	%ymm0, -32(%rsp)
	vmovapd	%ymm1, -64(%rsp)
	vmovapd	-32(%rsp), %ymm0
	vaddpd	-64(%rsp), %ymm0, %ymm0
	leave
	
	ret
