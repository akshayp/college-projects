# This program finds the background in an image. Background pixels
# are set to black.
#
# 5 September 2006		      

.data
Array:  .alloc	2048				# allocate image data space

# $1 = Array Pointer, $2 = Img1 Data, $3 = Img2 Data
.text
P1p5:		addi	$1, $0, Array		# set memory base
		swi	526			# create and display image pair

		addi $5, $0, -4			# set Outer loop's control variable

Outer:		addi $5, $5, 4			# increment Outer's control variable
		slti $2, $5, 1024			# if control variable
		beq $2, $0, End			# is greater than or equal to 1024, exit outer loop
		addi $1, $5, Array			# otherwise, update pointer
		addi $6, $0, 0			# and clear inner loop's control variable and continue

Inner:		lbu $2, 0($1)			# load R, G, or B of image 1
		lbu $3, 1024($1)			# load R, G, or B of image 2
		sub $4, $3, $2			# obtain the difference

		slti $2, $4, 0			# if difference is negative
		beq $2, $0, Positive			# then take the absolute value
		addi $2, $0, -1			# by multiplying the value in $4
		mult $4, $2			# by -1
		mflo $4				# move abs value back into $4

Positive:		slti $2, $4, 51			# if abs(difference)
		beq $2, $0, Foreground		# is greater than or equal to 51 goto FG

		addi $2, $5, 1024			# otherwise,
		sw $0, Array($2)			# make the pixel black
		j Outer				# and move onto the next pixel

Foreground:	addi $6, $6, 1			# increment Inner's control variable
		slti $2, $6, 3			# if control variable
		beq $2, $0, Outer			# is greater than or equal to 3, exit inner loop
		addi $1, $1, 1			# otherwise, increment pointer
		j Inner				# and continue the loop

End:		addi	$1, $0, Array		# set memory base
		swi	527			# display foreground subarray
		jr      	$31			# return to operating system