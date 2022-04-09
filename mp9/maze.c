
/* 
This file implements the helper functions to solve a maze. The first function is createMaze, which takes in the name of a file
with maze information and returns a pointer to the maze structure that memory is allocated for. fopen is used to make a stream for
the file, and scanf is used to obtain the width and the height. Memory is allocated for the maze structure using malloc. Maze elements
width and height are updated using the values obtained from the text file. Then, memory is allocated for the 2D array cells,
using the values of width and height and the fact that a 2D array is stored as an array of pointers. fgets is used to read subsequent 
lines of the file, and the string is copied into each row of the 2D array rows. A pointer to maze is returned. destroyMaze frees the 
previously allocated memory. It first frees each row of the 2D array cells, and then the memory space for the array of pointers.
Finally, it frees the memory space for the entire maze structure. printMaze takes in the maze structure as an input, and prints the maze.
It uses a double for loop(the sizes of which correspond to the maze width and height) and prints out each character. At the end of
each line, a newline character is printed. solveMazeDFS is the recursive function used to find a solution to the maze. It takes in the
maze structure and the current row and column as arguments. If the maze has a solution it returns 1 - otherwise it returns 0.
 It first checks if the position is in bounds - if not, it returns 0. Then, it checks if the end is reached - if it has, a 1 is returned.
 If the position is neither a space or an 'S' a 0 is returned. Then, the current position(assuming the function is still running)
 gets a '*' (even the starting position, which gets reverted to 'S' later). The function is called recrusively on each of the current cell's
 neighbors, and if any of them return 1, then the function returns a 1. Otherwise, backtracking occurs where the current position 
 obtains a '~' for visited and a 0 is returned.
*/

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    maze_t * maze = (maze_t *) malloc(sizeof(maze_t)); //allocate memory for the maze structure
    FILE *file = fopen(fileName, "r"); //create file pointer
    if(file == NULL) //if the file pointer can't be obtained
    {
        printf("Can't find file. \n"); //this is used for debugging purposes
        return NULL; //exit the function
    }

    
    //read in width and height
    int i = 0; //this is used as a loop counter
    int numbersread = 0; //this makes sure that 2 numbers are given at the beginning of the text file
    int width; //width of the board
    int height; //height of the board
    numbersread = fscanf(file, "%d %d", &width, &height); //obtain the width and the height from the first line
    if(numbersread != 2) //if the width and the height isn't proper in the text file
    return NULL; //exit function
    maze -> width = width; //modify structure with width
    maze -> height = height; //modify structure with height

    
    //allocate memory for cells
    maze->cells = (char**) malloc(height * sizeof(char*)); //allocate memory for an array of pointers
    for(i = 0; i < height; i++) //for each row in the maze
    {
        maze->cells[i] = (char*) malloc (width * sizeof(char)); //for each pointer in the array allocate memory for the maze width
    }
    

    
    //initialize other parameters
    int row = 0; //used for assigning characters to the maze
    char buffer[width * 2]; //creates a string of big enough length
    fgets(buffer,width*2,file); //we don't want the first line of the textfile

    
    
    while(fgets(buffer,width*2,file) != NULL) //read in each line and place in buffer
    {
        
        if(row < height) //avoids segmentation faults
        {

            for(i = 0; i < width; i++) //avoids segmentation faults
            {
            
                maze->cells[row][i] = buffer[i]; //assign maze location to character in text file
            
                if(buffer[i] == 'S') //found starting position
                {
                    maze->startColumn = i; //set startColumn in structure
                    maze->startRow = row; //set startRow in structure
                }
                if(buffer[i] == 'E') //found ending position
                {
                    maze->endColumn = i; //set endColumn in structure
                    maze->endRow = row; //set endRow in structure
                }
            
            }
        

        }
       
        
        row += 1; //increment row
        
    }
    
    
    fclose(file); //finish the file stream

    return maze; //return the maze pointer


}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    
    // Your code here.
    int height = maze -> height; //obtain the maze height
    int i; //loop counter
    for(i = 0; i < height; i++)
    {
        free(maze->cells[i]); //free memory for each row
    }
    free(maze->cells); //free memory for the array of pointers
    free(maze); //free entire maze structure
    
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    int height = maze -> height; //obtain maze height
    int width = maze -> width; //obtain maze width
    int i, j; //loop counters
  

    for(i = 0; i < height; i++) //for each row
    {
        
        for( j = 0; j < width; j++) //for each column
        {
            printf("%c", maze->cells[i][j]); //print out the appropriate character
        }
        printf("\n"); //move to a new line
        
     
    }


    
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.
    int width = maze->width; //obtain maze width
    int height = maze->height; //obtain maze height
    int startR = maze->startRow; //obtain the starting row
    int startC = maze->startColumn; //obtain the starting column
    
    
    
    if(col < 0 || col >= width || row < 0 || row >= height) //if cell is not in bounds
    return 0; //return false
    if(maze->cells[row][col] == 'E') //if at the end goal
    {
        maze->cells[startR][startC] = 'S'; //revert starting position to 'S'
        return 1; //return true
    }
    if(maze->cells[row][col] != ' ' && maze->cells[row][col] != 'S') //if not an empty space nor the starting position
    return 0; //return false
    maze->cells[row][col] = '*'; //otherwise set the curent cell to be part of the solution
    
    if(solveMazeDFS(maze,col-1,row) == 1) //check for paths starting from the left neighbor
    return 1; //return true
    if(solveMazeDFS(maze,col+1,row) == 1) //check for paths starting from the right neighbor
    return 1; //return true
    if(solveMazeDFS(maze,col,row-1) == 1) //check for paths starting from the top neighbor
    return 1; //return true
    if(solveMazeDFS(maze,col,row+1) == 1) //check for paths starting from the bottom neighbor
    return 1; //return true
    
    maze->cells[row][col] = '~'; //otherwise mark the cell as visited
    return 0; //return false for no solution

    
    


}
