/*
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
    int livecount = 0;
    int currrow, currcol;
    
    for(currrow = row - 1; currrow <= row + 1; currrow++)
    {
        if(currrow < 0 || currrow >= boardRowSize)
        continue;

        for(currcol = col - 1; currcol <= col + 1; currcol++)
        {
            if((currcol < 0 || currcol >= boardColSize) || (currrow ==  row && currcol == col))
            {
                continue;
            }
            if(board[boardColSize * currrow + currcol] == 1)
            livecount += 1;



            

        }

    }

    return livecount;

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
    int i, j, neighbors;
    int copy[boardRowSize * boardColSize];
    for(i = 0; i < boardRowSize * boardColSize; i++)
    {
        copy[i] = board[i];

    }

    for(i = 0; i < boardRowSize; i++)
    {
        for(j = 0; j < boardColSize; j++)
        {
            
        neighbors = countLiveNeighbor(board, boardRowSize, boardColSize, i, j);
        int cell = board[boardColSize * i + j];
        if (cell == 1)
        {
            if(neighbors < 2 || neighbors > 3)
            cell = 0;
            
        }
        else 
        {
            if(neighbors == 3)
            cell = 1;
        }
        copy[boardColSize * i + j] = cell;
        }
        

    }
    for(i = 0; i < boardColSize * boardRowSize; i++)
    {
        board[i] = copy[i];
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

    int i;
    int copy[boardRowSize * boardColSize];
    for(i = 0; i < boardRowSize * boardColSize; i++)
    {
        copy[i] = board[i];
    }
    updateBoard(copy, boardRowSize, boardColSize);

    for(i = 0; i < boardRowSize * boardColSize; i++)
    {
        if (copy[i] != board[i])
        return 0;
    }
    return 1;


}

				
				
			

