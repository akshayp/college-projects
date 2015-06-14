#
# Version for PCSPIM software
# declare and reserve space for data and variables after .data directive
	.data
# string contants
title:	.asciiz	"Program computes the parity of the binary input"
prompt:	.asciiz	"Input an integer? "
o_title:	.asciiz	"The parity is "
# ascii string with carriage return and line feed (a newline)
newline:	.byte	10,13,00,00
# force address back to word boundary after putting bytes in memory
	.align 2
# integer variables
n:	.word	0
count:	.word	0
#
#instructions appear after .text directive
	.text
main:
# Print out program description
#	puts	title
	li	$v0,4
	la	$a0,title
	syscall
# Print out carriage return and line feed
#	putc	'\n'
	li	$v0,4
	la	$a0,newline
	syscall
# Print out carriage return and line feed
#	putc	'\n'
	li	$v0,4
	la	$a0,newline
	syscall
# Prompt user for input
#	puts	prompt
	li	$v0,4
	la	$a0,prompt
	syscall
# Read in an integer, N
#	geti	$a0
	li	$v0,5
	syscall
	add	$a0,$v0,$zero
#
# Program to compute Sum
#
# count = 0;
	xor 	$a1,$a1,$a1
# sum = 0;
	xor	$a2,$a2,$a2
# loop: sum = sum + count
loop:	add	$a2,$a2,$a1
# count = count + 1
	addi	$a1,1
# if count <= N then goto loop
	ble	$a1,$a0,loop
# save result (some registers will be destroyed by puts call)
	sw	$a2,sum
# Print out carriage return and line feed
#	putc	'\n'
	li	$v0,4
	la	$a0,newline
	syscall
# Print out output title string
#	puts	o_title
	li	$v0,4
	la	$a0,o_title
	syscall
# Load result
	lw	$a0,sum
# Print out result
#	puti $a0
	li	$v0,1
	syscall
# Print out carriage return and line feed
#	putc	'\n'
	li	$v0,4
	la	$a0,newline
	syscall
# End program and return to simulator
#	done
	li	$v0,10
	syscall


