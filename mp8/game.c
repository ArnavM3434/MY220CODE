/* This program helps with the implementation of the 2048 game. It consists of many different functions to accomplish this goal. The
first function is make_game, which creates and initializes the game structure defined in the header file. It dynamically allocates memory
for both the structure and the structure member cells(pointer to an array). The members rows and cols are assigned based on inputs to this function.
Also, all elements in the member cells are intiliazed to -1; the function returns a pointer to this game structure. remake_game achieves
a similar purpose, except it also takes in a pointer to a pointer to a preexisting strucutre, which is modified according to similar
rules as in make_game. get_cell takes in a game pointer and row and column integers and returns a pointer to the specified cell.
It checks to make sure that the row and column are in bounds (otherwise NULL is returned), and then the memory location of the cell in the 
1D array cells is calculated and returned. move_w handles sliding and merging functionality. It takes in as input a pointer to the game structure.
The 1D array cells in the structure is turned into a 2D array. This array is traversed column by column, the row by row - in each column,
each non empty entry is slid up as far as possible until it meets a non empty entry (this is accomplished with a while loop and a target row).
Then, merging is handled. If an entry is the same as the one above, the entries are merged. Because merging can't happen on consecutive iterations,
a variable that takes on 1 or 0 is modified each iteration. The modified 2D array is converted back to a 1D array cells. move_s has an almost identical algorithm except that the target row
in each loop iteration starts at the bottom of each column, not at the top, and some of the conditional statements are reversed. move_a 
accomplished sliding and merging the tiles to the left, with a similar algorithm to move_w, except that now the 2D array is traversed row by row, and then column by 
column. move_d has almost identical functionality to move_a, except that the target row is now at the right of the 2D array and some of the
conditional statements are reversed. legal_move_check checks to see if there any legal moves left - it converts the 1D array cells
into a 2D array and checks to see if any cells are -1 or any cells have like neighbors. If so, a 1 is returned; otherwise a 0 is returned.
This is accomplished using a double for loop.
*/



#include "game.h"


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    
    mygame->rows = rows; //member rows gets rows
    mygame->cols = cols; //member cols gets cols
    mygame->score = 0;   //score starts off at 0

    for(int i = 0; i < rows * cols; i++) //rows * cols is number of elements in cells
    {
        mygame->cells[i] = -1; //each element in cells starts off as -1
    }



    


    return mygame; //returns pointer to game
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
     (*_cur_game_ptr)->rows = new_rows; //member rows gets new_rows
     (*_cur_game_ptr)->cols = new_cols; //member cols gets new_cols
     (*_cur_game_ptr)->score = 0;    //score is initialized to 0 as per instructions

     for(int i = 0; i < new_rows * new_cols; i++)
     {
         (*_cur_game_ptr)->cells[i] = -1; //each cell is initialized to -1
     }

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    if ( (row < 0 || row >= cur_game->rows) || (col < 0 || col >= cur_game->cols) ) //makes sure cell is in bounds
    return NULL;

     return &(cur_game->cells[row * cur_game->cols + col]); //memory location of cell in 1D array cells
    
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
    
    //convert to 2D array
    int numrows = cur_game -> rows; //saves number of rows
    int numcols = cur_game -> cols; //saves number of columns
    int board[numrows][numcols]; //creates new 2D array 
    int i; //row counter
    int j; //column counter
    

    for(i = 0; i < numrows; i++) //go row by row
    {
        for(j = 0; j < numcols; j++) //go column by column
        {
            board[i][j] = cur_game -> cells[i * numcols + j]; //properly assign each entry in 2D array value in cells
        }
    }

    //perform the slide
    int targetrow; //row entry should be slid up to
    int merged  = 0; //whether or not a merge took place
    int somethingchanged = 0; //whether any change took place

    for(j = 0; j < numcols; j++) //for each column
    {

        for(i = 0; i < numrows; i++) //for each row
        {
            targetrow = 0; //initalize target row to 0
            
            if (board[i][j] == -1) //if empty row continue
            continue;
            
            while(board[targetrow][j] != -1 && targetrow < numrows - 1) //while targetrow is in bounds and not empty
            targetrow++; //find max row entry can be slid up to

            if(targetrow < i) //if available space above entry
            {
                
                board[targetrow][j] = board[i][j]; //set free space to value of entry
                board[i][j] = -1; //replace original position with -1
                somethingchanged++; //make somethingchanged true
                
            }
            
            //check to see if cells should be merged
            if(targetrow < i) //if cell did end up moving
            {

                if(targetrow > 0 && board[targetrow][j] == board[targetrow - 1][j] && merged == 0) //targetrow is in bounds and cell has like neighbor
                {
                    board[targetrow - 1][j] = 2 * board[targetrow][j]; //merge
                    cur_game -> score = cur_game -> score + board[targetrow - 1][j]; //increment score
                    board[targetrow][j] = -1; //replace original entry with empty cell
                    merged = 1; //now merged is 1, can't merge on next iteration
                    somethingchanged++; //somethingchanged is true
                }
                else //now in next iteration merging is allowed
                {
                    merged = 0; //otherwise you can merge on next iteration

                }
                continue; //continue with the program
            }

            if(targetrow >= i) //cell hasn't moved
            {
                if(i > 0 && board[i][j] == board[i-1][j] && merged == 0) //if in bounds and cell has like neighbor and didn't merge on last iteration
                {
                    board[i - 1][j] = 2 * board[i][j]; //merge
                    cur_game -> score = cur_game -> score + board[i - 1][j];
                    board[i][j] = -1; //replace original entry with empty cell
                    merged = 1; //can't merge on next iteration
                    somethingchanged++; //something has changed

                }
                else
                {
                    merged = 0; //otherwise can merge on next iteration
                }
            }

            


        }
    }


    //transfer 2D array values back into 1D array cells

    for(i = 0; i < numrows; i++)
    {
        for(j = 0; j < numcols; j++)
        {
            cur_game -> cells[numcols * i + j] = board[i][j];
        }
    }

    if(somethingchanged)
    return 1;

    return 0;
};

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
    int numrows = cur_game -> rows; //save rows
    int numcols = cur_game -> cols; //save cols
    int board[numrows][numcols]; //make 2D array
    int i; //row counter
    int j; //col counter
    

    for(i = 0; i < numrows; i++) //turn 1D array into 2D array
    {
        for(j = 0; j < numcols; j++)
        {
            board[i][j] = cur_game -> cells[i * numcols + j];
        }
    }

    //perform the slide
    int targetrow; //row to be slid down to
    int merged  = 0; //whether or not merging happened
    int somethingchanged = 0; //whether or not something changed

    for(j = 0; j < numcols; j++) //column by column
    {

        for(i = numrows - 1; i >= 0; i--) //row by row
        {
            targetrow = numrows - 1; //targetrow is bottom
            
            if (board[i][j] == -1) //if empty move on
            continue;
            
            while(board[targetrow][j] != -1 && targetrow > 0) //find most downward targetrow
            targetrow--;

            if(targetrow > i) //if empty space is above entry
            {
                
                board[targetrow][j] = board[i][j]; //slide down
                board[i][j] = -1; //replace original with empty
                somethingchanged++; //something has changed
                
            }
            
            //check to see if cells should be merged
            if(targetrow > i) //cell has moved and want to check if it should be merged
            {

                if(targetrow < numrows - 1 && board[targetrow][j] == board[targetrow + 1][j] && merged == 0) //check for like neighbors
                {
                    board[targetrow + 1][j] = 2 * board[targetrow][j]; //merge
                    cur_game -> score = cur_game -> score + board[targetrow + 1][j]; //change score
                    board[targetrow][j] = -1; //make original cell empty
                    merged = 1; //now can't merge on next slide
                    somethingchanged++; //something has changed
                }
                else //now in next iteration merging is allowed
                {
                    merged = 0; //can merge on next turn

                }
            continue;
            }

            if(targetrow <= i) //cell hasn't moved
            {
                if(i < numrows - 1 && board[i][j] == board[i+1][j] && merged == 0) //check for like neighbor
                {
                    board[i + 1][j] = 2 * board[i][j]; //merge
                    cur_game -> score = cur_game -> score + board[i + 1][j]; //change score
                    board[i][j] = -1; //original cell is now empty
                    merged = 1; //can't merge on next slide
                    somethingchanged++; //something has changed

                }
                else
                {
                    merged = 0; //can merge on next iteration
                }
            }

            


        }
    }


    //transfer 2D array values back into 1D array cells

    for(i = 0; i < numrows; i++)
    {
        for(j = 0; j < numcols; j++)
        {
            cur_game -> cells[numcols * i + j] = board[i][j];
        }
    }

    if(somethingchanged)
    return 1;

    return 0;

    
};

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    int numrows = cur_game -> rows; //save rows
    int numcols = cur_game -> cols; //save cols
    int board[numrows][numcols]; //initialize 2D array
    int i; //row counter
    int j; //col counter
    

    for(i = 0; i < numrows; i++) //turn 1D array into 2D array
    {
        for(j = 0; j < numcols; j++)
        {
            board[i][j] = cur_game -> cells[i * numcols + j];
        }
    }

    //perform the slide
    int targetcol; //column entry should slide to
    int merged  = 0; //whether or not merge can happen
    int somethingchanged = 0; //whether or not something has changed

    for(i = 0; i < numrows; i++) //each row
    {

        for(j = 0; j < numcols; j++) //each column
        {
            targetcol = 0; //targetcol is all the way to the left
            
            if (board[i][j] == -1) //if empty
            continue;
            
            while(board[i][targetcol] != -1 && targetcol < numcols - 1) //find leftmost targetcol
            targetcol++;

            if(targetcol < j) //if empty space before cell
            {
                
                board[i][targetcol] = board[i][j]; //slide
                board[i][j] = -1; //replace with empty
                somethingchanged++; //something has changed
                
            }
            
            //check to see if cells should be merged
            if(targetcol < j) //cell has moved and want to check if it should be merged
            {

                if(targetcol > 0 && board[i][targetcol] == board[i][targetcol - 1] && merged == 0) //if like neighbors
                {
                    board[i][targetcol - 1] = 2 * board[i][targetcol]; //merge
                    cur_game -> score = cur_game -> score + board[i][targetcol - 1]; //change score
                    board[i][targetcol] = -1; //replace with empty
                    merged = 1; //can't merge on next slide
                    somethingchanged++; //something has changed
                }
                else //now in next iteration merging is allowed
                {
                    merged = 0; // can merge on next slide

                }
                continue;
            }

            if(targetcol >= j) //cell hasn't moved
            {
                if(j > 0 && board[i][j] == board[i][j - 1] && merged == 0) //check for like neighbors
                {
                    board[i][j - 1] = 2 * board[i][j]; //merge
                    cur_game -> score = cur_game -> score + board[i][j-1]; //change score
                    board[i][j] = -1; //replace with empty
                    merged = 1; //can't merge on next slide
                    somethingchanged++; //something has changed

                }
                else
                {
                    merged = 0; //can merge on next slide
                }
            }

            


        }
    }


    //transfer 2D array values back into 1D array cells

    for(i = 0; i < numrows; i++)
    {
        for(j = 0; j < numcols; j++)
        {
            cur_game -> cells[numcols * i + j] = board[i][j];
        }
    }

    if(somethingchanged)
    return 1;

    return 0;

    
};

int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
    int numrows = cur_game -> rows; //save rows
    int numcols = cur_game -> cols; //save cols
    int board[numrows][numcols]; //create 2D array
    int i; //row counter
    int j; //col counter
    

    for(i = 0; i < numrows; i++) //turn 1D array cells into 2D array
    {
        for(j = 0; j < numcols; j++)
        {
            board[i][j] = cur_game -> cells[i * numcols + j];
        }
    }

    //perform the slide
    int targetcol; //column for entry to be slid to 
    int merged  = 0; //whether or not merging can occur
    int somethingchanged = 0; //if something has changed

    for(i = 0; i < numrows; i++) //for each row
    {

        for(j = numcols - 1; j >= 0; j--) //for each column
        {
            targetcol = numcols - 1; //targetcol is rightmost column
            
            if (board[i][j] == -1) //if empty
            continue;
            
            while(board[i][targetcol] != -1 && targetcol > 0) //find targetcol
            targetcol--;

            if(targetcol > j) //if empty space to right of the entry
            {
                
                board[i][targetcol] = board[i][j]; //slide
                board[i][j] = -1; //replace original entry with -1
                somethingchanged++; //something has changed
                
            }
            
            //check to see if cells should be merged
            if(targetcol > j) //cell has moved and want to check if it should be merged
            {

                if(targetcol < numcols - 1 && board[i][targetcol] == board[i][targetcol + 1] && merged == 0) //check for like neighbors
                {
                    board[i][targetcol + 1] = 2 * board[i][targetcol]; //merge
                    cur_game -> score = cur_game -> score + board[i][targetcol + 1]; //change score
                    board[i][targetcol] = -1; //replace with empty
                    merged = 1; //merge can't happen on next slide
                    somethingchanged++; //something has changed
                }
                else //now in next iteration merging is allowed
                {
                    merged = 0; //can merge on next slide

                }
            continue;
            }

            if(targetcol <= j) //cell hasn't moved
            {
                if(j < numcols - 1 && board[i][j] == board[i][j + 1] && merged == 0) //check for like neighbor
                {
                    board[i][j + 1] = 2 * board[i][j]; //merge
                    cur_game -> score = cur_game -> score + board[i][j + 1]; //change the score
                    board[i][j] = -1; //replace with empty
                    merged = 1; //can't merge on next slide
                    somethingchanged++; //something has changed

                }
                else
                {
                    merged = 0; //can merge on next iteration
                }
            }

            


        }
    }


    //transfer 2D array values back into 1D array cells

    for(i = 0; i < numrows; i++)
    {
        for(j = 0; j < numcols; j++)
        {
            cur_game -> cells[numcols * i + j] = board[i][j];
        }
    }

    if(somethingchanged)
    return 1;

    return 0;

    
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
    int numrows = cur_game -> rows; //save rows
    int numcols = cur_game -> cols; //save columns
    int board[numrows][numcols]; //make 2D array
    int i; //row counter
    int j; //col counter
    

    for(i = 0; i < numrows; i++) //turn 1D array into 2D array
    {
        for(j = 0; j < numcols; j++)
        {
            board[i][j] = cur_game -> cells[i * numcols + j];
        }
    }

    for(i = 0; i < numrows; i++) //each row
    {
        for(j = 0; j < numcols; j++) //each column
        {
            if(board[i][j] == -1) //if empty cell there's legal move
            return 1;

            if(j > 0 && board[i][j-1] == board[i][j]) //like neighbors
            return 1;

            if(j < numcols - 1 && board[i][j+1] == board[i][j]) //like neighbors
            return 1;

            if(i > 0 && board[i-1][j] == board[i][j]) //like neighbors
            return 1;

            if(i < numrows - 1 && board[i + 1][j] == board[i][j]) //like neighbors
            return 1;

        }
    }

    return 0;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
