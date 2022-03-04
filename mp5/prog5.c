/* This program defines all the functions that are used in main.c to implement the Mastermind / Codebreaker program. It consists of
* the first method, set_seed, whose purpose is to generate a seed from the user for pseudo random number generation purposes. This method
* takes in a string that the user entered, and uses the sscanf to obtain the number from the string and ensure its validity. If the string
* is valid the srand() is called for pseudo-random number generation later on. If the string is valid a 1 is returned - otherwise a 0 is returned.
* The method start_game is a void method that takes in memory addresses of the solution variables in main.c. This function uses rand()
* and modulo operators to come up with 4 random numbers (the secret) - these numbers are assigned to variables back in the main program
* and to static variables in this program. Finally, make_guess ensures valid user guesses using sscanf and a similar method described above.
* It returns a 1 or 0 and prints a message based on whether or not the input is valid. If the guess is valid, the static variable guess_number
* is incremented. This function also tells how many of the guesses are completely correct and how many are misplaced, through a series
* of if statements and variables that keep track of whether each guess and each answer have been paired.		
 *
 * prog5.c - source file adapted from UIUC ECE198KL Spring 2013 Program 4
 *           student code -- GOLD VERSION by Steven S. Lumetta
 */


/*
 * The functions that you must write are defined in the header file.
 * Blank function prototypes with explanatory headers are provided
 * in this file to help you get started.
 */



#include <stdio.h>
#include <stdlib.h>

#include "prog5.h"


/*
 * You will need to keep track of the solution code using file scope
 * variables as well as the guess number.
 */

static int guess_number; //how many guesses user has input
static int solution1; //first secret number
static int solution2; //second secret number
static int solution3; //third secret number
static int solution4; //fourth secret number


/*
 * set_seed -- This function sets the seed value for pseudorandom
 * number generation based on the number the user types.
 * The input entered by the user is already stored in the string seed_str by the code in main.c.
 * This function should use the function sscanf to find the integer seed value from the 
 * string seed_str, then initialize the random number generator by calling srand with the integer
 * seed value. To be valid, exactly one integer must be entered by the user, anything else is invalid. 
 * INPUTS: seed_str -- a string (array of characters) entered by the user, containing the random seed
 * OUTPUTS: none
 * RETURN VALUE: 0 if the given string is invalid (string contains anything
 *               other than a single integer), or 1 if string is valid (contains one integer)
 * SIDE EFFECTS: initializes pseudo-random number generation using the function srand. Prints "set_seed: invalid seed\n"
 *               if string is invalid. Prints nothing if it is valid.
 */
int
set_seed (const char seed_str[])
{
//    Example of how to use sscanf to read a single integer and check for anything other than the integer
//    "int seed" will contain the number typed by the user (if any) and the string "post" will contain anything after the integer
//    The user should enter only an integer, and nothing else, so we will check that only "seed" is read. 
//    We declare
//    int seed;
//    char post[2];
//    The sscanf statement below reads the integer into seed. 
//    sscanf (seed_str, "%d%1s", &seed, post)
//    If there is no integer, seed will not be set. If something else is read after the integer, it will go into "post".
//    The return value of sscanf indicates the number of items read succesfully.
//    When the user has typed in only an integer, only 1 item should be read sucessfully. 
//    Check that the return value is 1 to ensure the user enters only an integer. 
//    Feel free to uncomment these statements, modify them, or delete these comments as necessary. 
//    You may need to change the return statement below
   
    int seed = 0; //initialize seed
    char post[2]; //initialize string to detect garbage
    int check = sscanf(seed_str, "%d%1s",&seed, post); //modify seed based on integer input, assign sscanf value to check
    if(check != 1) //if nothing, or more than one thing was read
    {
        printf("set_seed: invalid seed\n"); //input is invalid, return 0
        return 0;
    }
    srand(seed); //start process of pseduo random number generation
    return 1; //return 1, valid input
}


/*
 * start_game -- This function is called by main.c after set_seed but before the user makes guesses.
 *               This function creates the four solution numbers using the approach
 *               described in the wiki specification (using rand())
 *               The four solution numbers should be stored in the static variables defined above. 
 *               The values at the pointers should also be set to the solution numbers.
 *               The guess_number should be initialized to 1 (to indicate the first guess)
 * INPUTS: none
 * OUTPUTS: *one -- the first solution value (between 1 and 8)
 *          *two -- the second solution value (between 1 and 8)
 *          *three -- the third solution value (between 1 and 8)
 *          *four -- the fourth solution value (between 1 and 8)
 * RETURN VALUE: none
 * SIDE EFFECTS: records the solution in the static solution variables for use by make_guess, set guess_number
 */
void
start_game (int* one, int* two, int* three, int* four)
{
    //your code here
    solution1 = rand(); //create a random number based on key
    solution1 = solution1 % 8 + 1; //Have the number be between 1 and 8
    solution2 = rand(); //create a random number based on key
    solution2 = solution2 % 8 + 1; //Have the number be between 1 and 8
    solution3 = rand(); //create a random number based on key
    solution3 = solution3 % 8 + 1; //Have the number be between 1 and 8
    solution4 = rand(); //create a random number based on key
    solution4 = solution4 % 8 + 1; //Have the number be between 1 and 8
    

    *one = solution1; //modify variables in main program so that the main program also has the solution
    *two = solution2;
    *three = solution3;
    *four = solution4;

    guess_number = 1; //After this point, it will be the user's first guess
    
}

/*
 * make_guess -- This function is called by main.c after the user types in a guess.
 *               The guess is stored in the string guess_str. 
 *               The function must calculate the number of perfect and misplaced matches
 *               for a guess, given the solution recorded earlier by start_game
 *               The guess must be valid (contain only 4 integers, within the range 1-8). If it is valid
 *               the number of correct and incorrect matches should be printed, using the following format
 *               "With guess %d, you got %d perfect matches and %d misplaced matches.\n"
 *               If valid, the guess_number should be incremented.
 *               If invalid, the error message "make_guess: invalid guess\n" should be printed and 0 returned.
 *               For an invalid guess, the guess_number is not incremented.
 * INPUTS: guess_str -- a string consisting of the guess typed by the user
 * OUTPUTS: the following are only valid if the function returns 1 (A valid guess)
 *          *one -- the first guess value (between 1 and 8)
 *          *two -- the second guess value (between 1 and 8)
 *          *three -- the third guess value (between 1 and 8)
 *          *four -- the fourth color value (between 1 and 8)
 * RETURN VALUE: 1 if the guess string is valid (the guess contains exactly four
 *               numbers between 1 and 8), or 0 if it is invalid
 * SIDE EFFECTS: prints (using printf) the number of matches found and increments guess_number(valid guess) 
 *               or an error message (invalid guess)
 *               (NOTE: the output format MUST MATCH EXACTLY, check the wiki writeup)
 */
int
make_guess (const char guess_str[], int* one, int* two, 
	    int* three, int* four)
{
//  One thing you will need to read four integers from the string guess_str, using a process
//  similar to set_seed
//  The statement, given char post[2]; and four integers w,x,y,z,
//  sscanf (guess_str, "%d%d%d%d%1s", &w, &x, &y, &z, post)
//  will read four integers from guess_str into the integers and read anything else present into post
//  The return value of sscanf indicates the number of items sucessfully read from the string.
//  You should check that exactly four integers were sucessfully read.
//  You should then check if the 4 integers are between 1-8. If so, it is a valid guess
//  Otherwise, it is invalid.  
//  Feel free to use this sscanf statement, delete these comments, and modify the return statement as needed

    int first; //variable to hold first guess
    int second; //second guess
    int third; //third guess
    int fourth; //fourth guess
    char post[2]; //string to detect garbage
    int check = sscanf(guess_str, "%d%d%d%d%1s", &first, &second, &third, &fourth, post); //obtain integers, assign sscanf value to check
    if(check != 4) //if less than 4 integers were assigned or some garbage appeared
    {
        printf("make_guess: invalid guess\n"); //invalid guess, return 0
        return 0;
    }


    if(first < 1 || first > 8) //if integers are not in the correct range, invalid guess, return 0
    {
        printf("make_guess: invalid guess\n");
        return 0;
    }
    if(second < 1 || second > 8) //if integers are not in the correct range, invalid guess, return 0
    {
        printf("make_guess: invalid guess\n");
        return 0;
    }
    if(third < 1 || third > 8) //if integers are not in the correct range, invalid guess, return 0
    {
        printf("make_guess: invalid guess\n");
        return 0;
    }
    if(fourth < 1 || fourth > 8) //if integers are not in the correct range, invalid guess, return 0
    {
        printf("make_guess: invalid guess\n");
        return 0;
    }

    
    *one = first; //want to modify variables in the main program so that it can also keep track of user guesses
    *two = second;
    *three = third;
    *four = fourth;


    
    int sol1p = 0; //keeps track of whether a solution number was paired or not - don't want it to be reused
    int sol2p = 0;
    int sol3p = 0;
    int sol4p = 0;

    int firstp = 0; //keeps track of whether a guess number was paired or not - don't want it to be reused
    int secondp = 0;
    int thirdp = 0;
    int fourthp = 0;

    int perfect = 0; //counter for number of perfect matches
    int misplaced = 0; //counter for number of misplaced matches



    //checking perfect matches first
    if (first == solution1 && sol1p == 0)
    {
        perfect = perfect + 1; //increment perfect
        sol1p = 1; //now solution is paired
        firstp = 1; //now guess is paired

     }

    if (second == solution2 && sol2p == 0)
    {
        perfect = perfect + 1; //increment perfect
        sol2p = 1; //now solution is paired
        secondp = 1; //now guess is paired
    }

    if (third == solution3 && sol3p == 0)
    {
        perfect = perfect + 1; //increment perfect
        sol3p = 1; //now solution is paired
        thirdp = 1; //now guess is paired

    }


    if (fourth == solution4 && sol4p == 0)
    {
        perfect = perfect + 1; //increment perfect
        sol4p = 1; //now solution is paired
        fourthp = 1; //now guess is paired

    }
    




    //checking guess 1 for mismatch

    
       
        if (first == solution2 && sol2p == 0 && firstp == 0) //first guess and solution number cannot be paired
        {
            misplaced = misplaced + 1; //increment misplaced counter
            sol2p = 1; //solution is paired
            firstp = 1; //guess is paired

        }
        if (first == solution3 && sol3p == 0 && firstp == 0)
        {
            misplaced = misplaced + 1; //increment misplaced counter
            sol3p = 1; //solution is paired
            firstp = 1; //guess is paired
            

        }
        if (first == solution4 && sol4p == 0 && firstp == 0)
        {
            misplaced = misplaced + 1; //increment misplaced counter
            sol4p = 1; //solution is paired
            firstp = 1; //guess is paired
        }
    
    

    //checking guess 2 for mismatch

    
     
        if (second == solution1 && sol1p == 0 && secondp == 0) //second guess and solution number can't be paired
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol1p = 1; //solution is paired
            secondp = 1; //guess is paired

        }
        if (second == solution3 && sol3p == 0 && secondp == 0)
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol3p = 1; //solution is paired
            secondp = 1; //guess is paired

        }
        if (second == solution4 && sol4p == 0 && secondp == 0)
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol4p = 1; //solution is paired
            secondp = 1; //guess is paired

        }
     
    

    //checking guess 3 for mismatch

    
        
        if (third == solution1 && sol1p == 0 && thirdp == 0) //third guess and solution can't be paired
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol1p = 1; //solution is paired
            thirdp = 1; //guess is paired

        }
        if (third == solution2 && sol2p == 0 && thirdp == 0)
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol2p = 1; //solution is paired
            thirdp = 1; //guess is paired

        }
        
        if (third == solution4 && sol4p == 0 && thirdp == 0)
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol4p = 1; //solution is paired
            thirdp = 1; //guess is paired
            

        }
    

    //checking guess 4 for mismatch

    
        
        if (fourth == solution1 && sol1p == 0 && fourthp == 0) //fourth guess and solution can't be paired
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol1p = 1; //solution is paired
            fourthp = 1; //guess is paired

        }
        if (fourth == solution2 && sol2p == 0 && fourthp == 0)
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol2p = 1; //solution is paired
            fourthp = 1; //guess is paired
            

        }
        if (fourth == solution3 && sol3p == 0 && fourthp == 0)
        {
            misplaced = misplaced + 1; //increment mismatch counter
            sol3p = 1; //solution is paired
            fourthp = 1; //guess is paired

        }
        
        //print how many mismatches and perfect matches there were
        printf("With guess %d, you got %d perfect matches and %d misplaced matches.\n",guess_number,perfect,misplaced);
        
    





   guess_number = guess_number + 1;  //increment guess_number
   return 1; //return 1 for valid input
}


