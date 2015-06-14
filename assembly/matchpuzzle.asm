# This program solves a match puzzle.
#
# 14 October 2006

.data
Candidates:  .alloc	8			# allocate space for puzzle candidates

.text
p2p5:		addi	$1, $0, Candidates	# set memory base
		swi	528			# create and display puzzle

# $1**: RFlip, $2**: R, $3: loop control (X), $4: Copy of R, $5: elements of R

		addi	$1, $0, 0		# clear RFlip
		addi	$3, $0, 14		# set loop control variable
		add	$4, $0, $2		# make a copy of reference

# Loop calculates RFlip, the reference value flipped.

Loop:		andi	$5, $4, 3		# select first two bits of R
		sllv	$5, $5, $3		# shift bits left X times (multiply by 2^X)
		add	$1, $1, $5		# add component to RFlip (form running sum)
		beq	$3, $0, Traverse	# if X = 0, exit loop
		addi	$3, $3, -2		# decrement counter
		srl	$4, $4, 2		# move onto next two bits of R
		j	Loop			# continue Loop

# Traverse runs through the various candidates and compares them to R and RFlip. Note: $3 begins Traverse with value 0.
	
Traverse:	lw	$4, Candidates($3)	# load current candidate value
		addi	$6, $4, 0		# store current candidate value
		sll	$5, $4, 16		# shift candidate components left 16 times (over 8 components)
		add	$4, $4, $5		# add shifted components onto candidate value

InnerLoop:	srl	$4, $4, 2		# "Slide" components over one slot (2 bits).
		andi	$5, $4, 65535		# Note: 65535 = FFFF; obtain the first 8 components from the candidate
		beq	$5, $1, Done		# If first 8 components equals RFlip, match, end.
		beq	$5, $2, Done		# If first 8 components equals R, match, end.
		beq	$5, $6, ExitLoop	# If first 8 components equals original candidate, no match, move on.
		j	InnerLoop		# Continue loop

ExitLoop:	addi	$3, $3, 4		# move onto next candidate
		j	Traverse		# Continue loop

Done:		swi	529			# highlight match
		jr      $31			# return to operating system

