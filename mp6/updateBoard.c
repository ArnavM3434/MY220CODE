/*
* This program is used to implement the functions in the main program, which simulates Game of Life, which takes a 2 dimensional game
* board with 1's or 0's in each cell. The game then keeps updating the board based on a set of rules, which these functions help implement.
* The first function in this program is countLiveNeighbor, which takes in the address of the first element in the board array, the size of the rows
* , the size of the columns, and the row and column of the cell we want to check. This function iterates through the the surrounding
* rows and columns (to get all 8 adjacent neighbors) - if the rows and columns are in bounds and the cell under consideration is not the
* one we are trying to check, the number of live neighbors is updated if the cell under consideration is a 1. The number of live neighbors is returned.
* updateBoard updates the board based on a set of rules. First, the original board is copied. Then, each element in the original board is processed;
* if a cell is alive and does not have 2 or 3 live neighbors (checked using a call to countLiveNeighbor function), the cell dies. If a cell
* is dead and has 3 live neighbors, it is revived. These updates cell values are placed in the copied array. Then the copied array values are copied
* onto the original array. The final function, aliveStable, once again copies the original board. Then, it calls the updateBoard function to 
* update the copied version. Then the updated copied array is compared elementwise to the original array; if they differ, a 0 is returned. Otherwise,
* the board is stable and a 1 is returned.
* partners: vaa3@illinois.edu


 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
    int livecount = 0; //counter for number of live neighbors
    int currrow, currcol; //current column and row in iteration
    
    for(currrow = row - 1; currrow <= row + 1; currrow++) //currrow iterates to encompass surrounding rows
    {
        if(currrow < 0 || currrow >= boardRowSize) //ensures currrow is in bounds
        continue;

        for(currcol = col - 1; currcol <= col + 1; currcol++) //currcol iterates to encompass surrounding columns
        {
            if((currcol < 0 || currcol >= boardColSize) || (currrow ==  row && currcol == col)) //ensures currcol is in bounds and this is not the cell we are checking
            {
                continue;
            }
            if(board[boardColSize * currrow + currcol] == 1) //if the cell is alive
            livecount += 1; //update livecount
        }

    }

    return livecount;  //return livecount

}
/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
    int i, j, neighbors; //i and j are counters , neighbors is temporary variable
    int copy[boardRowSize * boardColSize]; //need to copy the array, don't want to modify original one
    for(i = 0; i < boardRowSize * boardColSize; i++) //iterate through each element in board
    {
        copy[i] = board[i]; //update copy so that it matches board

    }

    for(i = 0; i < boardRowSize; i++) //iterate through each row in board
    {
        for(j = 0; j < boardColSize; j++) //iterate through each column in board
        {
            
        neighbors = countLiveNeighbor(board, boardRowSize, boardColSize, i, j); //check the number of live neighbors
        int cell = board[boardColSize * i + j]; //cell stores the value at the current cell in the board
        if (cell == 1) //if the cell is alive
        {
            if(neighbors < 2 || neighbors > 3) //if less than 2 or more than 3 live neighbors
            cell = 0; //cell is dead
            
        }
        else 
        {
            if(neighbors == 3) //if cell is dead and three neighbors surround the cell
            cell = 1; //cell is alive
        }
        copy[boardColSize * i + j] = cell; //copied array is updated
        }
        

    }
    for(i = 0; i < boardColSize * boardRowSize; i++)
    {
        board[i] = copy[i]; //copy the copied array back into original board
    }


}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){

    int i; //i is loop counter
    int copy[boardRowSize * boardColSize]; //make a copied array of same size
    for(i = 0; i < boardRowSize * boardColSize; i++) //iterate through each element in board array
    {
        copy[i] = board[i]; //copy board values into copy array
    }
    updateBoard(copy, boardRowSize, boardColSize); //update the copied board

    for(i = 0; i < boardRowSize * boardColSize; i++) //iterate through all values in board
    {
        if (copy[i] != board[i]) //if the updated copied array is not equal to the original board, not stable
        return 0;
    }
    return 1; //board is stable, return 1


}

				
				
			

