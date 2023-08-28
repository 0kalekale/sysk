	.file	"_entry.s"
	.text
	.globl	main
	.type	main, @function
main:
	/*
	int argc and char *argv stuff
	*/
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	
	/*
	calling _sysk_stack() to start the master stack
	*/
	movl	$0, %eax
	call	_sysk_stack@PLT
	testl	%eax, %eax
	je	.init			# master stack initialized
	movl	$-1, %eax
	jmp	.exit			# master stack initialization failed 
.init:
	movq	-16(%rbp), %rdx
	movl	-4(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	master@PLT		# sysk master function, starting point for all programs 
	movl	$0, %eax
	call	_sysk_clean@PLT		# clean up allocated memory 
.exit:					
	leave
	ret				# exits with whatever _sysk_clean or a failed _sysk_stack returned 

