	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
	movl    $32, -4(%rbp)
	movl    $1, %edx
	movl    $0, %eax
	orl     %edx, %eax
	movl    %eax, -8(%rbp)
	movl    -8(%rbp), %edx
	movl    %edx, -12(%rbp)
	movl    $2, -16(%rbp)
	movl    $1, %edx
	movl    $0, %eax
	orl     %edx, %eax
	movl    %eax, -20(%rbp)
	movl    -20(%rbp), %edx
	movl    %edx, -24(%rbp)
	movl    $0, -24(%rbp)
	movl    -24(%rbp), %eax
	movl    %eax, %edi
	call    print
	movq    %rsp, %rbp
	popq    %rbp
	ret
