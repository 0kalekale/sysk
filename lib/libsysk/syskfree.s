// all it does is call munmap for now

	.file	"syskfree.s"
	.text
	.globl	syskfree
	.type	syskfree, @function
syskfree:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	-12(%rbp), %edx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	munmap@PLT
	leave
	ret
