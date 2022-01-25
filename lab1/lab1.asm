.ORIG x3000
; write your code here

; R3 has 16 bits we want to turn to hexadecimal
; R1 will store 4 bits at a time
; R0 will store value we want to output
; R2 will store inner loop counter
; R5 stores outer loop counter
; R6 stores temp value which will be used to determine if value is greater than 9 and assign ASCII codes

	
	AND R5, R5, #0		;Clear R5
	ADD R5, R5, #4		;Set outerloop counter to 4
OUTERLOOP AND R6, R6, #0		;Clear R6
	AND R1, R1, #0		;Clear R1
	AND R2, R2, #0		;Clear R2
	ADD R2, R2, #4		;Initialize loop counter with 4
	AND R0, R0, #0		;Reset R0 to 0

INNERLOOP ADD R1, R1, R1		;Left shift R1
	ADD R3, R3, #0          ;To test if R3 is positive, negative, or zero
	BRzp IFNONNEGATIVE	;Depending on whether starts with 1 or 0, want to do something
	ADD R1, R1, #1		;Add 1 to R1
	BRnzp NOMATTERWHAT	;Don't want to execute subsequent instruction, skip IFNONNEGATIVE
IFNONNEGATIVE ADD R1, R1, #0		;Add a zero if first bit of R1 is 0
NOMATTERWHAT  ADD R3, R3, R3		;Left shift R3
	ADD R2, R2, #-1		;Decrement counter
	BRp INNERLOOP		;Repeat process with remaining bits in group of 4
	NOT R6, R1		;Getting ready to subtract value in R1 from 10, and store result in R6
	ADD R6, R6, #1		;Now R6 stores negated value of R1
	ADD R6, R6, #10		;Now R6 holds 10 - R1
	BRnz GREATERTHAN		;Branches depending on if R1 < 10
	LD R0, NUMBERCODE		;Place ASCII code for 0 into R0
	ADD R0, R0, R1		;Add R1 to ASCII code in R0
	BRnzp ALWAYS		;Don't want to execute subsequent instruction
GREATERTHAN LD R0, LETTERCODE		;Place ASCII code corresponding to 'A' in R0	
	NOT R6, R6		;Want to negate R6 so that it holds R1 - 10
	ADD R6, R6, #1		;Negate R6
	ADD R0, R0, R6		;R6 holds R1-10, so now R0 holds ASCII code for 'A' plus appropriate value
ALWAYS	OUT			;Print character to console
	ADD R5, R5, #-1		;Decrement outerloop counter
	BRp OUTERLOOP		;Now process next group of 4 bits
	HALT			;Stop running
	

LETTERCODE	.FILL	#65
NUMBERCODE	.FILL	#48


.END
