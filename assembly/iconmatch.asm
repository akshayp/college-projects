#     I c o n   M a t c h
#
#
# This routine matches one of four candidate icons in a library
# of eight reference icons.
# The routine goes across a diagonal path to find the best match.
#
# 2 August 2008                               <Akshay Patel gth708n>

.data
Array:  .alloc	1728				# allocate icon data space
.text

IconMatch:	addi	$1, $0, Array		# point to array base
		swi	543			# generate puzzle icons
		addi 	$4, $1, 4608		# Point $4 to $ $1 so it points to reference icons
		addi	$2, $0, 0
		addi	$17, $0, 8
	
		# Code Begins

Loop:		lw	$5, 0($4)		# Loading element 1 from diagonal of reference icons
		lw 	$6, 52($4)		# Loading element 14 from diagonal of reference icons
		lw 	$7, 104($4)		# Loading element 27 from diagonal of reference icons
		lw 	$8, 156($4)		# Loading element 40 from diagonal of reference icons
		lw 	$9, 208($4)		# Loading element 53 from diagonal of reference icons
		lw 	$10, 260($4)		# Loading element 66 from diagonal of reference icons	
		lw 	$11, 312($4)		# Loading element 79 from diagonal of reference icons
		lw 	$12, 364($4)		# Loading element 92 from diagonal of reference icons
		lw 	$13, 416($4)		# Loading element 105 from diagonal of reference icons
		lw 	$14, 468($4)		# Loading element 118 from diagonal of reference icons	
		lw 	$15, 520($4)		# Loading element 131 from diagonal of reference icons
		lw 	$16, 572($4)		# Loading element 144 from diagonal of reference icons
		lw	$20, 528($4)		# Loading elements from other diagonal	
		lw	$22, 484($4)
		lw	$23, 440($4)
		lw	$24, 308($4)
		lw	$26, 88($4)

Compare:	lw	$18, 0($1)		# Load first element from Diagonal of candidate icon
		xor	$19, $18, $5		# Compare candidate and reference
		bne	$19, $0, Break		# If not equal break and move to next icon
		lw	$18, 88($1)		# The process is repeated for other elements of the diagonal
		xor	$19, $18, $26
		bne	$19, $0, Break
		lw	$18, 52($1)
		xor	$19, $18, $6
		bne	$19, $0, Break
		lw	$18, 104($1)
		xor	$19, $18, $7
		bne	$19, $0, Break
		lw	$18, 156($1)
		xor	$19, $18, $8
		bne	$19, $0, Break
		lw	$18, 208($1)
		xor	$19, $18, $9
		bne	$19, $0, Break
		lw	$18, 260($1)
		xor	$19, $18, $10
		bne	$19, $0, Break
		lw	$18, 312($1)
		xor	$19, $18, $11
		bne	$19, $0, Break
		lw	$18, 364($1)
		xor	$19, $18, $12
		bne	$19, $0, Break
		lw	$18, 416($1)
		xor	$19, $18, $13
		bne	$19, $0, Break
		lw	$18, 468($1)
		xor	$19, $18, $14
		bne	$19, $0, Break
		lw	$18, 520($1)
		xor	$19, $18, $15
		bne	$19, $0, Break
		lw	$18, 528($1)
		xor	$19, $18, $20
		bne	$19, $0, Break
		lw	$18, 484($1)
		xor	$19, $18, $22
		bne	$19, $0, Break
		lw	$18, 440($1)
		xor	$19, $18, $23
		bne	$19, $0, Break
		lw	$18, 308($1)
		xor	$19, $18, $24
		bne	$19, $0, Break
		lw	$18, 572($1)
		xor	$19, $18, $16
		bne	$19, $0, Break
		j 	Output			# If matched found, move to Output
	
Break:		addi	$1, $1, 576		# Move to next Icon
		addi	$2, $2, 1		# Initialize a counter to count icon number
		beq 	$2, $17, RefLoop	# If Icon counter gets to 8 move to next reference 
		j	Compare			# Jump to Compare to compare Icon Again


RefLoop:	addi 	$1, $1, -4608		# Reset the pointer on the Array
		addi	$4, $4, 576		# Move to next reference icon.	
		addi	$2, $0, 0		# Reset the Icon Counter
		j	Loop

		# Code Ends

Output:		addi    $2, $2, 0               # always guess the first icon
		swi	544			# submit answer and check
		jr	$31			# return to caller

