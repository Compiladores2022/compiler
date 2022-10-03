	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
	movl    $0, -4(%rbp)
	movl    $42, -8(%rbp)
	movl    $1, -12(%rbp)
	movl    -8(%rbp), %edx
	movl    -8(%rbp), %eax
	imull   %edx, %eax
	movl    %eax, -16(%rbp)
	movl    -16(%rbp), %edx
	movl    -4(%rbp), %eax
	addl    %edx, %eax
	movl    %eax, -20(%rbp)
	movl    -20(%rbp), %eax
	movl    %eax, %edi
	call    print
	movq    %rsp, %rbp
	popq    %rbp
	ret
