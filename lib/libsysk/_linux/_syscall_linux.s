/*
linux system call wrapper for C (x86_64)
written by 0kalekale, under AGPL3 	
*/

	.file	"_syscall_linux.s"
	.text
	.globl	_syscall_linux
	.type	_syscall_linux, @function
_syscall_linux:
	movq %rdi, %rax	
	movq %rsi, %rdi		
	movq %rdx, %rsi
	movq %rcx, %rdx
	movq %r8, %r10
	movq %r9, %r8
	movq 8(%rsp),%r9	
	syscall			
	
	ret			
