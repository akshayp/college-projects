#     Median of 100 element integer array
# This routine finds the median of a 100 element array.
#	$1 = Array Pointer
#	$2 = Median Result , 50th Element
#	$3 = Counter 1
#	$4 = Counter 2
#	$5 = Current Element
#	$6 = Next Element
#	$7 = Compare Current and Next Elements
#	$8 = 51st Element
#  29 August 2009                       <Akshay Patel>
#										<gth708n>

.data
Array:  .alloc		100
.text

Sort:		addi	$1, $0, Array		# point to array base
			swi		542					# generate random numbers
Loop1:		addi 	$3, $1, 396			# Init Counter for loop
			addi	$4, $0,  0 			# 
Loop2:		lw 		$5, 0($3) 			# Load Current Element
			lw		$6, -4($3)			# Load Next Element
			slt		$7, $5, $6			# If Next < Current
			beq   	$7, $0, Skip		# Skip to next element
			sw 		$6, 0($3) 			# Swap Next and Current Elements
			sw		$5, -4($3)			# 
			addi	$4, $0, 1			# Increment counter for j
Skip:		addi	$3, $3, -4			# Increment counter for i
			bne		$3, $1, Loop2		# Loop for j
			bne		$4, $0, Loop1		# Loop for i
			lw		$2, 196($1)			# Load 50th Element from Ordered list
			lw		$8, 200($1)			# Load 51st Element
			add		$2, $2, $8			# Add Elements
			addi	$9, $0, 2			# Because srl didn't work for -ve
			div		$2, $9				# Divide by 2 to find mean
			mflo	$2
			jr		$31					# otherwise return to caller
