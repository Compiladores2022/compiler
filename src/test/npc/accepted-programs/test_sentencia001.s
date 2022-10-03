	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
	movl    $14, %edx
	movl    $2, %eax
	imull   %edx, %eax
	movl    %eax, -4(%rbp)
	movl    $1, %edx
	movl    -4(%rbp), %eax
	addl    %edx, %eax
	movl    %eax, -8(%rbp)
	movl    -8(%rbp), %eax
	movl    %eax, %edi
	call    print
	movq    %rsp, %rbp
	popq    %rbp
	ret
