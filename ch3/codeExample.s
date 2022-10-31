	.file	"codeExample.c"
	.text
	.globl	_multstore
	.def	_multstore;	.scl	2;	.type	32;	.endef
_multstore:
LFB0:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	36(%esp), %eax
	movl	%eax, 4(%esp)
	movl	32(%esp), %eax
	movl	%eax, (%esp)
	call	_mult2
	movl	40(%esp), %edx
	movl	%eax, (%edx)
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
LFE0:
	.ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
	.def	_mult2;	.scl	2;	.type	32;	.endef
