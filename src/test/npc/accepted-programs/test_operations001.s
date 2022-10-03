	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
	movl    $3, -4(%rbp)
	movl    $7, -8(%rbp)
	movl    -8(%rbp), %edx
	movl    -4(%rbp), %eax
	subl    %edx, %eax
	movl    %eax, -16(%rbp)
	movl    -16(%rbp), %edx
	movl    %edx, -12(%rbp)
	movl    -12(%rbp), %eax
	movl    %eax, %edi
	call    print
	movq    %rsp, %rbp
	popq    %rbp
	ret
