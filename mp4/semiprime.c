/*There were a few errors in this program. When running the debugger for inputs
2 and 6, I noticed that in print_semiprimes, if i % j == 0, k would always = 0, which would be counted as prime. It became apparent
the line was supposed to be k = i / j, as we wanted the other factor of i.
I also noticed in the output that certain numbers were printed twice.
Thus, I added a break statement in print_semiprimes after the print statement.
The last modification I made to print_semiprimes was adding a ret = 1
line if a number was semiprime, as this function was supposed to return
whether or not any semiprimes existed in the interval.
Now for is_prime. I noticed with inputs 12 and 16 that 12 was printed. When running
the debugger, inside is_prime, I saw that 6 was treated as prime because 
inside the if statement, if 6 had a factor >= 2, a 1 was being returned. I changed
this line so that a zero would be returned instead, indicating that the number
is not prime. */

#include <stdlib.h>
#include <stdio.h>


/*
 * is_prime: determines whether the provided number is prime or not
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
    int i;
    if (number == 1) {return 0;}
    for (i = 2; i < number; i++) { //for each number smaller than it
        if (number % i == 0) { //check if the remainder is 0
            return 0;
        }
    }
    return 1;
}


/*
 * print_semiprimes: prints all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
    int i, j, k;
    int ret = 0;
    for (i = a; i <=b; i++) { //for each item in interval
        //check if semiprime
        for (j = 2; j <= i; j++) {
            if (i%j == 0) {
                if (is_prime(j)) {
                    k = i/j;
                    if (is_prime(k)) {
                        printf("%d ", i);
                        ret = 1;
                        break;
                    }
                }
            }
        }

    }
    printf("\n");
    return ret;

}
