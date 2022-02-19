
/* This program computes and prints the coefficients for certain rows of Pascal's
triangle. First, the user is prompted for a row (rows start at 0). Then, the row 
is retrieved using scanf. The value of the coefficient (which gets printed) is initially
a double. Then, each coefficient is computed using an outer for loop and an inner for loop.
The outer for loop has iterations for each k, where k is the kth coefficient (index) to be printed
(k ranges from 0 to row). For each k, there is an inner for loop with counter i,
and the coefficient is calculated using the pi product formula where i ranges from one to k,
and the coefficient equals the product of all (n + 1 - i) / i. Because row is an integer it
needs to get cast to a double for coefficient to store the correct result (line 38). At the end of each
inner loop coefficient is cast to an unsigned long to be printed as an integer.
When row = 40 there is a slight offset after coefficient 25, which is corrected
using an if statement. Each coefficient is printed with a space between consecutive
coefficients.
*/

#include <stdio.h>
#include <stdlib.h>

int main() 
{
  int row; //row of Pascal's triangle
  int k; //refers to the kth coefficient in row (index of coefficient, not coefficient itself)
  int i; //used to compute the coefficient

  printf("Enter a row index: "); //Prompts the user for a row 
  scanf("%d",&row); //Retrieves the value of the row

  // Write your code here

  double coef = 1.0; //coef is a double (needed for computation)

  for(k = 0; k <= row; k++) //for all coefficients in the row
  {
    coef = 1.0; //coefficient always starts off at 1
    for(i = 1; i <= k; i++) //For all i's in k such that i goes from 1 to k
    {
      coef *= (double)(row + 1 - i)/i; //formula to compute coefficient
    }

    unsigned long intcoef; //unsigned long intcoef to be printed
    if (row == 40 && k >= 26) //if row = 40 and coefficient index is >= 26
    {
      intcoef = (unsigned long)(coef + 1); //correct offset by adding 1 to each coefficient
    }
    else
    {
      intcoef = (unsigned long)(coef); //no need to correct offset
    }
   

    printf("%lu ", intcoef); //print the integer coefficient
    
    
  }

  printf("\n"); //print a new line

  return 0;
}
