;This program takes in keyboard input and echoes it to the screen. If the input is in valid postfix expression
;this program evaluates the result - otherwise it prints an error message. A stack is central to the program. 
;The user stack has the push and pop subroutines. Everytime the user inputs a value, the EVALUATE subroutine assess if the input is valid (0...9,
;+,-,*,/,^, ) and if it is, EVALUATE does one of two things. If the input is a digit, EVALUATE pushes it to
;stack (and checks for overflow). If the input is an operator, EVALUATE pops the top two values on the stack
;and performs the operation (given by subroutines) on them. Then the result is pushed onto the stack (this often)
;happens within the subroutines). If an invalid character is passed, a register (R6) has a value greater than 0,
;and the program stops executing EVALUATE. It then begins to print an error message. If an equal sign is inputted
;the program stores the element in the stack (two POPS are performed with one expected overflow to make sure
;there is only one value left in the stack). Then EVALUATE stops being executed. If the expression is valid PRINT_HEX
;is called to print the output (in R5) in hexadecimal. Otherwise, if not valid, the error message is printed.
;Then the program halts.
;
.ORIG x3000
	
;your code goes here
;R6 stores if expression is valid or not (0 for valid, 1 for invalid)	
;R1 is used for temp value to determine if equal sign occurs
AND R6, R6, #0		;Set R6 to 0
AND R0, R0, #0		;Set R0 to 0

 
PROCESS 
LD R1, SIGN			;R1 gets ASCII code corresponding to negative of equal sign 
ADD R1, R1, R0		;R1 gets R1 - R0
BRz PRINTING		;If the result is 0, equal sign has been input, and printing starts
ADD R6, R6, #0		;Test to see if expression is invalid
BRp PRINTING	  	;If R6 is greater than 0 invalid expression, print "Invalid Expression"
JSR EVALUATE		;If neither equal sign nor invalid expression, keep performing EVALUATE
BRnzp PROCESS		;Repeat process


PRINTING
ADD R6, R6, #0		;Set NZP bitd
BRp PRINTINVALID	;If R6 > 0, print error message by branching
JSR PRINT_HEX		;Otherwise, print the value in R5 using subroutine
BRnzp FINISHED		;Branch to HALT the program

PRINTINVALID		;For printing error message
LEA R0, MESSAGE		;Load address of starting character into R0
PUTS				;Print the message

FINISHED
HALT				;Halt the program
	

SIGN .FILL #-61		;Negated ASCII code for '='
MESSAGE .STRINGZ "Invalid Expression"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
;R1, R2, R3, R5, R6, R7 - intermediate registers
PRINT_HEX
ADD R3, R5, #0		;Want to transfer R5 to R3
ST R5, PRINTSAVE_R5	;Still want R5 to retain the output of expression
ST R7, PRINTSAVE_R7	;Want to return to caller
AND R5, R5, #0		;Clear R5
ADD R5, R5, #4		;Set outerloop counter to 4
OUTERLOOP 
AND R6, R6, #0		;Clear R6
AND R1, R1, #0		;Clear R1
AND R2, R2, #0		;Clear R2
ADD R2, R2, #4		;Initialize loop counter with 4
AND R0, R0, #0		;Reset R0 to 0

INNERLOOP 
ADD R1, R1, R1		;Left shift R1
ADD R3, R3, #0      ;To test if R3 is positive, negative, or zero
BRzp IFNONNEGATIVE	;Depending on whether starts with 1 or 0, want to do something
ADD R1, R1, #1		;Add 1 to R1
BRnzp NOMATTERWHAT	;Don't want to execute subsequent instruction, skip IFNONNEGATIVE
IFNONNEGATIVE 
ADD R1, R1, #0		;Add a zero if first bit of R1 is 0
NOMATTERWHAT  
ADD R3, R3, R3		;Left shift R3
ADD R2, R2, #-1		;Decrement counter
BRp INNERLOOP		;Repeat process with remaining bits in group of 4
NOT R6, R1			;Getting ready to subtract value in R1 from 10, and store result in R6
ADD R6, R6, #1		;Now R6 stores negated value of R1
ADD R6, R6, #10		;Now R6 holds 10 - R1
BRnz GREATERTHAN	;Branches depending on if R1 < 10
LD R0, NUMBERCODE	;Place ASCII code for 0 into R0
ADD R0, R0, R1		;Add R1 to ASCII code in R0
BRnzp ALWAYS		;Don't want to execute subsequent instruction
GREATERTHAN 
LD R0, LETTERCODE	;Place ASCII code corresponding to 'A' in R0	
NOT R6, R6			;Want to negate R6 so that it holds R1 - 10
ADD R6, R6, #1		;Negate R6
ADD R0, R0, R6		;R6 holds R1-10, so now R0 holds ASCII code for 'A' plus appropriate value
ALWAYS	
OUT					;Print character to console
ADD R5, R5, #-1		;Decrement outerloop counter
BRp OUTERLOOP		;Now process next group of 4 bits
LD R5, PRINTSAVE_R5	;Still want R5 to retain its value
LD R7, PRINTSAVE_R7 ;Want R7 to contain address of next instruction in caller
RET					;Return to caller

	LETTERCODE	 .FILL	#65 ;ASCII code for 'A'
	NUMBERCODE	 .FILL	#48 ;ASCII code for '0'
	PRINTSAVE_R5 .BLKW  #1
	PRINTSAVE_R7 .BLKW  #1






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - determines whether expression is valid or not
;R3 - operand register
;R4 - operand register
;R2 - temporary register used to determine which character was typed
EVALUATE
;your code goes here
ST R7, EVAL_SAVER7	;Want to store R7 to be able to return to caller
ST R3, EVAL_SAVER3	;Store R3 
ST R4, EVAL_SAVER4	;Store R4 

GETC				;Get character
ST R0, EVAL_SAVER0	;Want to save ASCII code entered
OUT					;Want to echo digit

LD R2, DIGITSTART	;R2 gets negative of ASCII code for '0'
ADD R2, R0, R2		;Compare it with inputted character
BRn ISPLUS		    ;If R2 is outside the range R0 is not a digit
LD R2, DIGITEND		;R2 gets negative of ASCII code for '9'
ADD R2, R0, R2		;Compare R2 with inputted character
BRp ISPLUS		    ;If R2 is outside the range it's not a digit
LD R2, DIGITSTART	;Want to turn R0 into decimal value
ADD R0, R2, R0		;This is to turn ASCII value into digit
JSR PUSH			;Push value to stack
ADD R5, R5, #0		;Test for overflow
BRp INVALID			;If there is overflow branch to invalid
BRnzp ALLDONE		;End subroutine


ISPLUS
LD R2, PLUSSIGN		;Check if R0 is a '+'
ADD R2, R0, R2		;Check if R0 is a '+'
BRnp ISMINUS		;If not a plus sign check if R0 holds a minus sign
JSR POP				;Pop value from the stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R4, R0, #0		;Put top value in R4
JSR POP				;Pop value from stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R3, R0, #0		;Put top value in R3
JSR PLUS			;Perform operation, already pushes it to stack
BRnzp ALLDONE		;Branch to end of subroutine


ISMINUS
LD R2, MINUSSIGN	;Check if R0 is a '-'
ADD R2, R0, R2		;Check if R0 is a '-'
BRnp ISMULTIPLY		;If not a minus sign check if R0 holds a multiply sign
JSR POP				;Pop value from the stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R4, R0, #0		;Put top value in R4
JSR POP				;Pop value from stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R3, R0, #0		;Put top value in R3
JSR MIN				;Perform operation, already pushes it to stack
BRnzp ALLDONE		;Branch to end of subroutine

ISMULTIPLY
LD R2, MULSIGN		;Check if R0 is a '*'
ADD R2, R0, R2		;Check if R0 is a '*'
BRnp ISDIVIDE		;If not a multiply sign check if R0 holds a divide sign
JSR POP				;Pop value from the stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R4, R0, #0		;Put top value in R4
JSR POP				;Pop value from stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R3, R0, #0		;Put top value in R3
JSR MUL				;Perform operation
JSR PUSH			;For MUL need to push it ourselves
BRnzp ALLDONE		;Branch to end of subroutine

ISDIVIDE
LD R2, DIVSIGN		;Check if R0 is a '/'
ADD R2, R0, R2		;Check if R0 is a '/'
BRnp ISEXP			;If not a divide sign check if R0 holds an exponent sign
JSR POP				;Pop value from the stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R4, R0, #0		;Put top value in R4
JSR POP				;Pop value from stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R3, R0, #0		;Put top value in R3
JSR DIV				;Perform operation, DIV already pushes R0 to stack
BRnzp ALLDONE		;Branch to end of subroutine

ISEXP
LD R2, EXPSIGN		;Check if R0 is a '^'
ADD R2, R0, R2		;Check if R0 is a '^'
BRnp ISEQUAL		;If not an exponent sign check if R0 holds an equal sign
JSR POP				;Pop value from the stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R4, R0, #0		;Put top value in R4
JSR POP				;Pop value from stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
ADD R3, R0, #0		;Put top value in R3
JSR EXP				;Perform operation, EXP already pushes R0 to stack
BRnzp ALLDONE		;Branch to end of subroutine

ISEQUAL
LD R2, EQUALSIGN	;Check if R0 is '='
ADD R2, R0, R2		;Check if R2 is a '='
BRnp ISSPACE		;If not an equal sign input check if input is space
JSR POP				;POP top value on the stack
ADD R5, R5, #0		;Test for underflow
BRp INVALID			;If there is branch to invalid
JSR POP				;Try and POP again - you shouldn't be able to
ADD R5, R5, #0		;Test for underflow - underflow should occur
BRz INVALID			;If there isn't underflow branch to invalid
ADD R5, R0, #0		;Place result in R5
BRnzp ALLDONE		;Branch to end of subroutine

ISSPACE			
LD R2, SPACE		;Check if R0 is ' '
ADD R2, R0, R2		;Check if R0 is ' ', the last valid possible character
BRz ALLDONE			;If R0 is ' ', branch to end of subroutine

INVALID	
ADD R6, R6, #1		;R0 has an invalid character, "record" this by incrementing value in R6



ALLDONE
LD R7, EVAL_SAVER7	;Want to load R7 to be able to return to caller
LD R3, EVAL_SAVER3	;Restore R3 
LD R4, EVAL_SAVER4	;Restore R4 
LD R0, EVAL_SAVER0	;Restore R0 

RET

EVAL_SAVER7	.BLKW #1	
EVAL_SAVER3 .BLKW #1
EVAL_SAVER4 .BLKW #1
EVAL_SAVER0 .BLKW #1
DIGITSTART  .FILL #-48	;Negative of ASCII code for '0'
DIGITEND    .FILL #-57	;Negative of ASCII code for '9'
PLUSSIGN	.FILL #-43	;Negative of ASCII code for '+'
MINUSSIGN   .FILL #-45	;Negative of ASCII code for '-'
MULSIGN     .FILL #-42	;Negative of ASCII code for '*'
DIVSIGN     .FILL #-47	;Negative of ASCII code for '/'
EXPSIGN     .FILL #-94	;Negative of ASCII code for '^'
EQUALSIGN	.FILL #-61	;Negative of ASCII code for '='
SPACE       .FILL #-32	;Negative of ASCII code for ' '





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
;your code goes here
ST R7, PLUS_SAVER7 	;Need to save R7 as we're calling another subroutine within this one
ADD R0, R3, R4		;Store R3 + R4 in R5
JSR PUSH			;Push value to stack
LD R7, PLUS_SAVER7 	;Reload original R7 to return to caller
RET					;Return to caller
PLUS_SAVER7 .BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
;your code goes here
ST R7, MINUS_SAVER7 ;Need to save R7 as we're calling another subroutine within this one
NOT R4, R4			;Begin to negate R4
ADD R4, R4, #1		;Negate R4
ADD R0, R3, R4		;Store R3 - R4 into R5
JSR PUSH			;Push value to stack
LD R7, MINUS_SAVER7 ;Reload original R7 to return to caller
RET					;Return to caller

MINUS_SAVER7 .BLKW #1



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Temporary register R5 to determine if product is positive or negative
;R2 is used as temporary register to hold R4's original value
;input R3, R4
;out R0
MUL	
;your code goes here
ST R7, MUL_SAVER7	;Save R7 as we call another subroutine
ST R2, MUL_SAVER2	;To restore R2 later
ST R3, MUL_SAVER3	;This value is important for EXP, so it needs to be saved here
ST R4, MUL_SAVER4	;This value is important for EXP, so it needs to be saved here
ST R5, MUL_SAVER5	;This value used to determine if result is positive or negative - has uses elsewhere in program

AND R5, R5, #0		;If R5 is 0 product should not be negated

ADD R3, R3, #0		;Check if R3 is 0
BRz ZERO			;If zero branch to ZERO
ADD R4, R4, #0		;Check if R4 is 0
BRz ZERO			;If zero branch to ZERO
BRnzp STARTPROCESS	;Begin multiplication

ADD R3, R3, #0		;Check if R3 is negative
BRp STARTPROCESS	;If R3 is positive multiplication works
NOT R3, R3			;Negate R3
ADD R3, R3, #1		;Negate R3
ADD R5, R5, #1		;This means the product will be negated at the end
BRnzp STARTPROCESS	;Branch to STARTPROCESS

ZERO
AND R0, R0, #0		;R0, the output, should store 0
BRnzp FINISHEDMUL	;Multiplication is finished



STARTPROCESS
ADD R2, R4, #0		;Want to save value of R4	
ADD R3, R3, #-1		;Want to use R3 as loop
MULTIPLY 
ADD R0, R4, #0		;Assign R0 to R4
ADD R3, R3, #0		;Set NZP bits
BRz FINISHEDMUL		;If R3 is 0 multiplication is done

ADD R4, R4, R2		;Add R4 to itself
ADD R3, R3, #-1		;Decrement R3 which acts as counter
BRnzp MULTIPLY		;Return to MULTIPLY

FINISHEDMUL
ADD R5, R5, #0		;Check if result should be negated
BRz NONEGATE		;If not skip next instructions
NOT R0, R0			;Negate R0
ADD R0, R0, #1		;Negate R0
NONEGATE
LD R7, MUL_SAVER7	;R7 is needed to return to caller
LD R2, MUL_SAVER2	;Restore R2
LD R3, MUL_SAVER3	;Restore R3
LD R4, MUL_SAVER4	;Restore R4
LD R5, MUL_SAVER5	;Restore R5
RET					;Return to caller

MUL_SAVER7	.BLKW #1
MUL_SAVER2  .BLKW #1
MUL_SAVER3	.BLKW #1
MUL_SAVER4	.BLKW #1
MUL_SAVER5  .BLKW #1




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV	  ; R3 / R4
;your code goes here
ST R7, DIV_SAVER7	;Need to store R7 for later
ST R2, DIV_SAVER2	;Store R2
ST R3, DIV_SAVER3	;Store R3
ST R4, DIV_SAVER4	;Store R4

AND R0, R0, #0		;Want R0 to start off as 0
NOT R2, R4			;Want to negate R4 and store it in R2
ADD R2, R2, #1		;R2 now stores -(R4)

DIVIDE 
ADD R3, R3, R2		;Repeated subtraction
ADD R0, R0, #1		;R0 contains quotient
ADD R3, R3, #0		;Check if R3 is less than 0
BRzp DIVIDE

ADD R0, R0, #-1		;Overcounted quotient, adjust value

JSR PUSH			;Push value to stack
LD R7, DIV_SAVER7	;Restore R7
LD R2, DIV_SAVER2	;Restore R2
LD R3, DIV_SAVER3	;Restore R3
LD R4, DIV_SAVER4	;Restore R4

RET					;Return to caller
	

DIV_SAVER7	.BLKW #1
DIV_SAVER2	.BLKW #1
DIV_SAVER3	.BLKW #1
DIV_SAVER4	.BLKW #1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;R2 is used as a counter
;out R0
EXP		;R3 ^ R4
;your code goes here
ST R7, EXP_SAVER7	;Save R7
ST R2, EXP_SAVER2	;Save R2
ST R3, EXP_SAVER3	;Save R3
ST R4, EXP_SAVER4	;Save R4


ADD R2, R4, #0		;R2 holds R4
ADD R4, R3, #0		;R4 holds R3
ADD R0, R3, #0		;R0 starts off holding R3

ADD R2, R2, #-1		;R2 is counter for how many times we multiply R3 with itself

EXPBEGIN 
ADD R2, R2, #0		;Want to set NZP bits
BRz FINISHEDTHIS	;If R2 is 0 multiplication is done
JSR MUL				;Execute MUL
ADD R4, R0, #0		;Put result to R4
ADD R2, R2, #-1		;Decrement counter
BRnzp EXPBEGIN		;Repeat process

FINISHEDTHIS
JSR PUSH			;Push value to stack
LD R7, EXP_SAVER7	;Restore R7
LD R2, EXP_SAVER2	;Restore R2
LD R3, EXP_SAVER3	;Restore R3
LD R4, EXP_SAVER4	;Restore R4
RET					;Return to caller


EXP_SAVER7	.BLKW #1
EXP_SAVER2	.BLKW #1
EXP_SAVER3	.BLKW #1
EXP_SAVER4  .BLKW #1


	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;Set R5 to 0
	LD R3, STACK_END	;Load R3 with STACK_END
	LD R4, STACK_TOP	;Load R4 with TOS
	ADD R3, R3, #-1		;Negate (R3 - 1)
	NOT R3, R3			;Negate (R3 - 1)
	ADD R3, R3, #1		;Negate (R3 - 1)
	ADD R3, R3, R4		;Subtract TOS and (STACK_END - 1)
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;Push value to top of stack
OVERFLOW
	ADD R5, R5, #1		;Set R5 to 1
DONE_PUSH
	LD R3, PUSH_SaveR3	;Restore R3
	LD R4, PUSH_SaveR4	;Restore R4
	RET					;Return to main program


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;Put STACK_START in R3
	LD R4, STACK_TOP	;Put STACK_TOP in R4
	NOT R3, R3			;Negate R3
	ADD R3, R3, #1		;Negate R3
	ADD R3, R3, R4		;Subtract TOS and STACK_START
	BRz UNDERFLOW		;If they are equal there is underflow
	ADD R4, R4, #1		;Update stack top
	LDR R0, R4, #0		;Put data at its address in R0
	ST R4, STACK_TOP	;Store new stack top
	BRnzp DONE_POP		;Finished popping
UNDERFLOW
	ADD R5, R5, #1		;Set R5 to 1
DONE_POP
	LD R3, POP_SaveR3	;R3 gets restored
	LD R4, POP_SaveR4	;R4 gets restored
	RET					;Return to caller program


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;End of stack
STACK_START	.FILL x4000	;Beginning of stack
STACK_TOP	.FILL x4000	;Stack pointer


.END
