
/* 
partners: vaa3, agrimk2
This program helps implement the helper functions for creating a sparse matrix and manipulating it. The function
load_tuples takes in a string (name of the input file), and creates a sparse matrix based on this. It allocates memory for a
sp_tuples structure, and initializes its members by reading the file line by line. Each line, this function calls set_tuples
to modify the linked list - the head of the linked list is a pointer that is an element of the sp_tuples structure. gv_tuples takes in
a pointer to the sp_tuples structure and a row and a column, and returns the value in the linked list (if it exists). It uses
a single pointer to traverse the list - if it doesn't find the node with the right row and column, it returns 0. set_tuples takes
in a pointer to the sp_tuples structure, a row value, a column value, and a double value. First it checks if the values is 0 - 
if so, it calls another function to possibly delete a node. Then it checks if the row and column already exist in the list - if so 
it calls a function to modify a node in the list. Otherwise, it adds a new node to the list. save_tuples prints out the 
height, width, and non zero entries in a format similar to the input matrices based on the file name specified. Add tuples takes
in two pointer to structures. It makes a new structure and fills in the nodes of the first argument into the new structure. It then
does that again with the second argument, except if the row and columns of nodes overlap, their values are added and put into the new
structure. destroy_tuples deallocates the memory for both the sp_tuples structrues and all the nodes in the linked list. delete_node
traverses the list using a double pointer, and if it finds the approriate node, it deallcoates memory and reassigns the list connections.
check_exist checks if the row and column values exist in the nodes of the list -  if they do this specific node is modified.
addnew inserts a new node into the list by comparing the row and column of the node to the already existing nodes in the list.

partner: vaa3, agrimk2
*/

#include "sparsemat.h"
#include <stdio.h>
#include <stdlib.h>



void delete_node(sp_tuples * list, sp_tuples_node ** headpptr, int row, int col); //deletes specificed node
int check_exist(sp_tuples_node ** headpptr, int row, int col, double value); //modifies existing node (if it exists)
void addnew(sp_tuples * list, sp_tuples_node ** headpptr, sp_tuples_node * temp); //inserts node at certain location

sp_tuples * load_tuples(char* input_file) //make the sparse matrix
{
    
    sp_tuples * list = (sp_tuples *) malloc(sizeof(sp_tuples)); //allocate memory

    FILE *file = fopen(input_file, "r"); //create file pointer
    if(file == NULL) //if the file pointer can't be obtained
    {
        printf("Can't find file. \n"); //this is used for debugging purposes
        return NULL; //exit the function
    }

    
   
    
    int numbersread = 0; //this makes sure that 2 numbers are given at the beginning of the text file
    int width; //width of the board
    int height; //height of the board
    numbersread = fscanf(file, "%d %d", &height, &width); //obtain the width and the height from the first line
    if(numbersread != 2) //if the width and the height isn't proper in the text file
    return NULL; //exit function
    
    list->m = height; //modify structure
    list->n = width; //modify structure
    list->nz = 0; //initialize number of nonzero elements to 0
    list->tuples_head = NULL; //initialize pointer to 0
    
    //initialize row, column, and value variables
    int col = 0; 
    int row = 0; 
    double value = 0;
    

    
    

    while(fscanf(file, "%d %d %lf", &row, & col, &value) != EOF) //read line by line
    {
        set_tuples(list, row, col, value); //modify existing linked list using this function
       
    
    }
    
    
    fclose(file); //finish the file stream

    return list; //return pointer to the structure
}



double gv_tuples(sp_tuples * mat_t,int row,int col) //return value in list given by row and column
{
    sp_tuples_node * headptr = mat_t->tuples_head; //single pointer to traverse list
    while(headptr != NULL) //while list hasn't finished
    {
        if((headptr)->row == row && (headptr)->col == col) //if row and column are found
        {
            
            return (headptr)->value; //return the value

        }
        headptr = ((headptr)->next); //otherwise change the pointer to point to the next node



    }
    return 0; //element not found
}



void set_tuples(sp_tuples * mat_t, int row, int col, double value) //modify existing linked list
{
    
    if (value == 0) //if value is 0
    {
        delete_node(mat_t, &(mat_t->tuples_head),row,col); //want to delete node
        return; //end function
        
    }
    
    else  //otherwise
    {
    
        //1st helper: see if node with same row and column exist and change this
        if(check_exist(&(mat_t->tuples_head), row, col, value)) //change existing
        return; //if something was changed end this function


        //make new node
        sp_tuples_node temp; //temp of type sp_tuples_node
        //initialize members
        temp.value = value;
        temp.row = row;
        temp.col = col;
        //2nd helper: if node with same row and column does not exist, insert node at proper place
        addnew(mat_t, &(mat_t->tuples_head), &temp); //insert node at proper place



    }
    
}



void save_tuples(char * file_name, sp_tuples * mat_t) //print matrix
{
    FILE *file = fopen(file_name, "w"); //new file
    fprintf(file, "%d %d\n", mat_t->m, mat_t->n); //print out height and width
    sp_tuples_node ** headpptr = &(mat_t->tuples_head); //double pointer to traverse list
    while(*headpptr != NULL) //while list hasn't finished
    {
        fprintf(file, "%d %d %lf\n", (*headpptr)->row,(*headpptr)->col,(*headpptr)->value); //print row, col, value
        headpptr = &((*headpptr)->next); //indirectly point to next node
    }



	return; //finish method
}



sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
    //these next 4 lines obtain measurements of sp_tuples structures
    int height1 = matA->m;
    int width1 = matA->n;
    int height2 = matB->m;
    int width2 = matB->n;
    double stuff = 0; //stuff is used as temporary variable
    if(!(height1 == height2 && width1 == width2)) //lists are compared size wise
    return NULL; //not same size
    sp_tuples * retmat = (sp_tuples *) malloc(sizeof(sp_tuples)); //allocate memory for new structure
    //these 4 lines initialize members
    retmat->nz = 0; 
    retmat->m = height1;
    retmat->n = width1;
    retmat->tuples_head = NULL;

    sp_tuples_node ** headpptr = &(matA->tuples_head); //headpptr points to beginning of list A
    while(*headpptr != NULL) //while list hasn't finished
    {
        
        set_tuples(retmat,(*headpptr)->row, (*headpptr)->col,(*headpptr)->value); //add node to retmax list
        
        headpptr = &((*headpptr)->next); //indirectly point to next node
        
    }


    sp_tuples_node ** headpptr2 = &(matB->tuples_head); //now start at beginning of B
    while(*headpptr2 != NULL) //while list hasn't finished
    {
       if(gv_tuples(retmat,(*headpptr2)->row,(*headpptr2)->col)) //if this row and column exist in retmax
       {
            stuff = gv_tuples(retmat,(*headpptr2)->row,(*headpptr2)->col) + (*headpptr2)->value; //stuff = new value + old one
            set_tuples(retmat,(*headpptr2)->row, (*headpptr2)->col,stuff); //put stuff inside retmax's linked list


       }
       else
       {
           set_tuples(retmat,(*headpptr2)->row, (*headpptr2)->col,(*headpptr2)->value); //otherwise put the new value directly into retmax's linked list

       }
       headpptr2 = &((*headpptr2)->next); //indirectly point to the next node
        
        
        
    }


	return retmat; //return pointer
     
}



sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){ 
    //return retmat;
    return NULL;

}


	
void destroy_tuples(sp_tuples * mat_t){ //deallocate memory
    //deallocate memory for all the nodes
    //then free the structure itself
    sp_tuples_node * headptr = mat_t->tuples_head; //single pointer to beginning of list
    
    while(headptr != NULL) //while list hasn't finished
    {
        sp_tuples_node * temp = headptr; //save pointer
        headptr = (headptr)->next; //point to next node
        free(temp); //free previous node


    }
	free(mat_t); //free entire sp_tuples structure

    return;
}  




void delete_node(sp_tuples * list, sp_tuples_node ** headpptr, int row, int col){ //delete certain node
    
    int nz = list->nz; //number of nonzero elements
    if(nz == 0) //if no elements
    return; //end function


    while(*headpptr != NULL) //while not at the end of the list
    {
        if((*headpptr)->row == row && (*headpptr)->col == col) //if row and column is found
        {
            list->nz = nz - 1; //decrease nz
            sp_tuples_node * temp = *headpptr; //save pointer
            *headpptr = (*headpptr)->next; //reform connection
            free(temp); //deallocate memory
            return; //finish



        }
        headpptr = &((*headpptr)->next); //indirectly point to next node
    }

    


}




int check_exist(sp_tuples_node ** headpptr, int row, int col, double value) //see if element exists, if it does, change it
{

    while(*headpptr != NULL) //while not at the end of the list
    {
        if((*headpptr)->row == row && (*headpptr)->col == col) //if row and column is found
        {
            (*headpptr)->value = value; //modify node
            return 1; //return successful

        }
        headpptr = &((*headpptr)->next); //indirectly point to the next node



    }
    return 0; //no modifications were made




}




void addnew(sp_tuples * list, sp_tuples_node ** headpptr, sp_tuples_node * temp) //insert new node somewhere
{
    sp_tuples_node * insertion = (sp_tuples_node *) malloc(sizeof(sp_tuples_node)); //allocate memory for node to be inserted
    *insertion = *temp; //initialize members of insertion node
    
    int row = insertion->row; //save its row
    int col = insertion->col; //save its column
    

    //now want to insert insertion node in appropriate place

   
    while( *headpptr != NULL && ( (*headpptr)->row < row || ((*headpptr)->row == row && (*headpptr)->col <= col ) ) ) //loop gives you node to place insertion node before
    {

        headpptr = &((*headpptr)->next); //indirectly point to next node


    }

    insertion->next = *headpptr; //connect insertion node to appropriate one after it
    *headpptr = insertion; //modify node behind insertion node to connect to insertion

    list->nz = list->nz + 1; //change the value of nz



}