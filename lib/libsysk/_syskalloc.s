/*
WIP fast memory alloc (basically a worse mmap)
could have written this in C easily, why the fuck not in  asm

just calls mmap for now, more stuff later
*/

	.file	"_syskalloc.s"
	.text
	.globl	_syskalloc
	.type	_syskalloc, @function
_syskalloc:
	pushq	%rbp
	movq	%rsp, %rbp
	
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	$0, %r9d
	movl	$0, %r8d
	movl	$34, %ecx
	movl	$3, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	mmap@PLT
	movq	%rax, -8(%rbp)
	cmpq	$-1, -8(%rbp)
	jne	.fail
	movq	$-1, %rax
	jmp	.succ
.fail:
	movq	-8(%rbp), %rax
.succ:
	leave
	ret
