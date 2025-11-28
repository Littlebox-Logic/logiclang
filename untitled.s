	.file	"sample.lc"
	.text
	.globl mainx

mainx:
	pushq	%rbp
	movq	%rsp, %rbp

	movl	%edi, -4(%rbp)
	movl	%rsi, -16(%rbp)
	movl	%rdx, -24(%rbp)

	movl	$0, %eax
	popq	%rbp
	ret

	.size	mainx, .-mainx
.ident	"<Logic Compiler>"