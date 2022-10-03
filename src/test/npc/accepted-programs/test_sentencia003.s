	.globl main
main:
	pushq   %rbp
	movq    %rsp, %rbp
	movl    $1, %edx
	movl    $0, %eax
	andl    %edx, %eax
	movl    %eax, -4(%rbp)
	movl    $1, %edx
	movl    -4(%rbp), %eax
	orl     %edx, %eax
	movl    %eax, -8(%rbp)
	movl    -8(%rbp), %eax
	movl    %eax, %edi
	call    print
	movq    %rsp, %rbp
	popq    %rbp
	ret
