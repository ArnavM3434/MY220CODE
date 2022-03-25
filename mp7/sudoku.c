#include "sudoku.h"

//This program takes a 9 x 9 sudoku board (represented by a 2D array) and fills in the rest of the numbers
//for a solution. It uses a backtracking algorithm that works as follows: the solve_sudoku method finds the first empty (0 filled)
//square in the board. It checks if the 1 can be legally filled into the board using
//some supplementary functions (and if it can a 1 is filled into the board). If a 1 can be filled in, recursion is applied to to see whether or not the next empty element 
//can be possibly filled with a 1...9, and so forth - if all the numbers can be legally filled in, the board is solved. Otherwise,
//backtracking occurs where an element that was previously inserted will be incremented, and all of those combinations
// are checked. The function returns a 1 if the board can be solved and a zero otherwise. To tell if a number can be placed in a cell
//, helper functions are used. The first is is_val_in_row, which takes in a constant integer, constant row value, and constant sudoku array.
// It then uses a for loop to check every entry in the row to see whether or not the constant integer already exists. The function 
// is_val_in_col is very similar; it takes in the same parameters as is_val_in_row, except for a constant column value instead of row value.
// It also uses a for loop to check every entry in the column to see if the value already exists in the column. The is_val_in_3x3_zone
// takes in a constant value, constant row and column, and constant sudoku board. It figures out whether the value already exists in the
// 3x3 zone the cell under consideration is in. The row and column are both divided by 3, which is used to figure out the 
// general sections (and thus box) the cell is in, and check it using 2 for loops. There is one more helper function called all_cells_assigned
// which uses a double for loop to see whether or not all cells are nonzero. This is used in the solve_sudoku method.
//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------
//


// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int col; //col is a variable used to process each element in the row
  for(col = 0; col < 9; col++) //process each column
  {
    if(sudoku[i][col] == val) //if value already exists
    return 1; //Return true
  }
  
  return 0; //values doesn't already exist in row
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  int row; //row is used to process each element in column
  for(row = 0; row < 9; row++) //process each row
  {
    if(sudoku[row][j] == val) //if value already exists
    return 1; //return true
  }
  
  return 0; //value doesn't already exist in column
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9);
  
  // BEG TODO

  //find box the cell is in
  //create new 3 by 3 array and use prior functions to tell what is in this box

  int colbox = j / 3; //determines which of 3 parts is the cell vertically in
  int rowbox = i / 3; //determines which of 3 parts is cell horizontally in

  int bigrow; //loop variable
  int bigcol; //loop variable
 
  
  
    for(bigrow = rowbox * 3; bigrow < rowbox * 3 + 3; bigrow++)
    {
      for(bigcol = colbox * 3; bigcol < colbox * 3 + 3; bigcol++)
      {
        if (sudoku[bigrow][bigcol] == val) //if value already exists in the 3x3 box
        return 1; //return true
        
      }
      
    }

  return 0; //return false
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  if (is_val_in_row(val, i, sudoku)) //if value is already in row
  return 0; //invalid
  if(is_val_in_col(val,j,sudoku)) //if value is already in column
  return 0; //invalid
  if(is_val_in_3x3_zone(val,i,j,sudoku)) //if value is already in 3x3 zone
  return 0; //invalid
  return 1; //character can be placed at this cell
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  if (all_cells_assigned(sudoku) == 1) //if no empty cells
  return 1; //board is solved

  int row; //tracks current cell
  int col; //tracks current cell
  int found = 0; //to see if empty cell has been found

  //find next available item
  for(int i = 0; i < 9; i++) //process each cell in the board
  {
    for (int j = 0; j < 9; j++) //process each cell in the board
    {
      if(sudoku[i][j] == 0) //if the cell is 0
      {
        row = i; //save the row position
        col = j; //save the column position
        found = 1;
        break; //exit the loop
      }
      if(found) //exits outer loop if empty cell is found. This actually isn't necessary - empty cells can be found in any order
      break;
      
    }
  }

  for(int num = 1; num <= 9; num++) //every square must be filled with number from 1 to 9. You are testing almost all combinations of numbers in certain cases.
  {
    if(is_val_valid(num, row, col, sudoku)) //see if number can be legally placed in this spot
    {
      sudoku[row][col] = num; //assign it to this spot
      if(solve_sudoku(sudoku)) //run function again for next empty spot
      return 1; //if base case (all cells are filled) is hit you are done
    }

    sudoku[row][col] = 0; //necessary for backtracking 


  }
  

  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}

//Check if all cells are assigned by nonzero numbers
int all_cells_assigned(const int sudoku[9][9])
{
  int row; //current row
  int col; //current column
  for(row = 0; row < 9; row++) //process each row
  {
    for(col = 0; col < 9; col++) //process each column
    {
      if(sudoku[row][col] == 0)  //if cell is empty
      return 0; //then not all cells are nonzero
    }
  }
  return 1;
}





