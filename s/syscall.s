	;; this program is not fully functioning yet.
	;; the `sys_write` is not working, but `sys_exit` works.
	.data
_info:
	.string "hello, world!\n"

	.text
	.globl _start
_start:
	;; syscall id is stored in rax
	mov	$1, %rax
	;; arguments are stored in: rdi, rsi, rdx, r10, r8, r9
	;; sys_write
	mov	$1, %rdi
	mov	_info, %rsi
	mov	$14, %rdx
	syscall

	;; sys_exit
	mov	$60, %rax
	mov	$2, %rdi
	syscall


