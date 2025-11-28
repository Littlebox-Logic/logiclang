	.file	"sample.lc"
	.text
	.globl entry

entry:
	pushq	%rbp
	movq	%rsp, %rbp

	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)

	movl	$0, %eax
	popq	%rbp
	ret

	.size	entry, .-entry
	.ident	"<Logic Compiler>"