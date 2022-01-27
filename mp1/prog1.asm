; Arnav Mehta (arnavm7), worked alone, no group members

; Introduction (is also there right before my own code) - This program creates and prints 
; out a histogram to demonstrate the frequency of certain characters in a string that are provided at 
; memory locations starting at x0400. 2 problems were solved; one was how to print a binary value
; in hexadecimal, and the other was how to determine the frequency of characters in the string.
; The second problem was already solved - the first problem required this general approach: given a
; a binary value in R3, I extracted the 4 leading bits at a time from R3 and put them into R0. If the value 
; was greater than or equal to 10, letters needed to be used(which I had ascii codes for) - knowing the ascii
; codes for '0' and 'A' allowed me to offset the value in R0 appropriately to print the correct character.
; 2 for loops were used - one for each of the 4 bits, and one for each of the 4 groups of bits. 
; To print the histogram, I kept loading the frequencies that already existed in certain memory locations
; into R3. This required an additional loop. Also, before each hexadecimal was printed, I had to print
; a character and a space, which I calculated using similar offset techniques.
; REGISTER TABLE NEAR PRINT_HIST


; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code - I UPDATE THIS TABLE AT PRINT_HIST
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST

; Introduction - This program creates and prints 
; out a histogram to demonstrate the frequency of certain characters in a string that are provided at 
; memory locations starting at x0400. 2 problems were solved; one was how to print a binary value
; in hexadecimal, and the other was how to determine the frequency of characters in the string.
; The second problem was already solved - the first problem required this general approach: given a
; a binary value in R3, I extracted the 4 leading bits at a time from R3 and put them into R0. If the value 
; was greater than or equal to 10, letters needed to be used(which I had ascii codes for) - knowing the ascii
; codes for '0' and 'A' allowed me to offset the value in R0 appropriate to print the correct character.
; 2 for loops were used - one for each of 4 bits, and one for each of 4 groups. 
; To print the histogram, I kept loading the frequencies that already existed in certain memory locations
; into R3. This required an additional loop. Also, before each hexadecimal was printed, I had to print
; a character and a space, which I calculated using similar offset techniques.

; Register Table:
; R3 has 16 bits we want to turn to hexadecimal
; R1 will store 4 bits at a time - it also gets used as a temporary register when deciding which character to print
; R0 will store ascii value we want to output - it also gets used as a temporary register to decide which
; character to print
; R2 will store inner loop counter (to process 4 bits at a time)
; R5 stores outer loop counter (for 4 groups of 4 bits)
; R6 stores temp value which will be used to determine if value is greater than 9 and assign ASCII codes
; R4 stores memory location program is on, starting at beginning of histogram


; you will need to insert your code to print the histogram here

	LD R4, HIST_ADDR	; R4 will stores starting address of the histogram
	ADD R4, R4, #-1		; This is to make subsequent for loop easier

PRINT	
	ADD R4, R4, #1		; We subtracted 1 from it 6 lines earlier, modifying it now to store proper address
	NOT R0, R4			; Process of negating R4, storing in R0
	ADD R0, R0, #1		; R4 is negated, stored in R0
	LD R1, TEMP			; R1 stores x3F1A, starting address of histogram plus 26
	ADD R0, R0, R1		; We're subtracting current address from the x3F1A
	BRn DONE			; If result is negative, we're no longer on memory locations with histogram
	LDR R3, R4, #0		; Now R3 stores histogram number we need

	LD R0, HIST_ADDR	; This part of the code deals with printing out the character itself
	NOT R0, R0			; Beginning of process to negate R0, which now stores starting address of histogram
	ADD R0, R0, #1		; Now R0 stores negated value of starting address
	ADD R0, R0, R4		; R0 stores how many memory locations we are away from the starting histogram address
	BRz STARTING		; If R0 stores 0 we are on first line of histogram
	LD R1, LETTERCODE	; Fill R1 with ASCII code for 'A'
	ADD R1, R1, #-1		; This line fixes a slight counting error
	ADD R0, R0, R1		; Now R0 stores ASCII code for 'A' incremented by a certain amount
	BRnzp SUBSEQUENT	; Don't want to execute STARTING
STARTING
	LD R0, ATCODE		; Load R0 with '@' code
SUBSEQUENT
	OUT					; Print out character on histogram
	LD R0, SPACE		; Now R0 holds space character code
	OUT					; Print out a space

						; Subsequent code is to print the hexadecimal values




	AND R5, R5, #0		; Clear R5
	ADD R5, R5, #4		; Set outerloop counter to 4
OUTERLOOP 
	AND R6, R6, #0 		; Clear R6
	AND R1, R1, #0		; Clear R1
	AND R2, R2, #0		; Clear R2
	ADD R2, R2, #4		; Initialize loop counter with 4
	AND R0, R0, #0		; Reset R0 to 0

INNERLOOP 
	ADD R1, R1, R1		; Left shift R1
	ADD R3, R3, #0      ; To test if R3 is positive, negative, or zero
	BRzp IFNONNEGATIVE	; Depending on whether starts with 1 or 0, want to do something
	ADD R1, R1, #1		; Add 1 to R1
	BRnzp NOMATTERWHAT	; Don't want to execute subsequent instruction, skip IFNONNEGATIVE
IFNONNEGATIVE 
	ADD R1, R1, #0		; Add a zero if first bit of R1 is 0
NOMATTERWHAT  
	ADD R3, R3, R3		; Left shift R3
	ADD R2, R2, #-1		; Decrement counter
	BRp INNERLOOP		; Repeat process with remaining bits in group of 4
	NOT R6, R1			; Getting ready to subtract value in R1 from 10, and store result in R6
	ADD R6, R6, #1		; Now R6 stores negated value of R1
	ADD R6, R6, #10		; Now R6 holds 10 - R1
	BRnz GREATERTHAN	; Branches depending on if R1 < 10
	LD R0, NUMBERCODE	; Place ASCII code for 0 into R0
	ADD R0, R0, R1		; Add R1 to ASCII code in R0
	BRnzp ALWAYS		; Don't want to execute subsequent instruction
GREATERTHAN 
	LD R0, LETTERCODE	; Place ASCII code corresponding to 'A' in R0	
	NOT R6, R6			; Want to negate R6 so that it holds R1 - 10
	ADD R6, R6, #1		; Negate R6
	ADD R0, R0, R6		; R6 holds R1-10, so now R0 holds ASCII code for 'A' plus appropriate value
ALWAYS	
	OUT					; Print character to console
	ADD R5, R5, #-1		; Decrement outerloop counter
	BRp OUTERLOOP		; Now process next group of 4 bits
	AND R0, R0, #0		; Reset R0
	ADD R0, R0, #10		; R0 stores ASCII code for new line character
	OUT					; Print new line
	BRnzp PRINT












; do not forget to write a brief description of the approach/algorithm
; for your implementation, list registers used in this part of the code,
; and provide sufficient comments



DONE	HALT			; done


; the data needed by the program
LETTERCODE	.FILL	#65	; ASCII code for 'A'
NUMBERCODE	.FILL	#48	; ASCII code for '0'
ATCODE		.FILL	#64	; ASCII code for '@'
SPACE		.FILL	#32	; ASCII code for ' '
NUM_BINS	.FILL 	#27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
STR_START	.FILL x4000	; string starting address
TEMP		.FILL x3F1A

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
