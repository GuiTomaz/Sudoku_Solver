#include <stdio.h>
#include <stdlib.h>
#define dimension 9
//A way to simplify the boolean comparisons xD
typedef enum Bool{
  FALSE, // 0
  TRUE // 1
}Boolean;

Boolean isDigit(int c);
int charToInt(char s);
int** createGrid();
void freeMatrix(int** grid);
void printGrid(int** grid);
Boolean findEmptyLocation(int** grid, int* list);
Boolean checkLocationIsSafe(int** grid, int row, int col, int num);
Boolean usedInRow(int** grid, int row, int num);
Boolean usedInCol(int** grid, int col, int num);
Boolean usedInBox(int** grid, int row, int col, int num);
Boolean sudokuSolver(int** grid);

int main()
{   int countLine;
    FILE *arq; //The FILE will store the matrix which is the sudoku unsolved
    arq = fopen("sudokutests.txt","r");
  /*
    *Building the matrix in the memory
  */
    int** gridMatrix = createGrid();
    /*
      *The matrix given in input is stored in memory.
    */
    char line[18];
    for(int i = 0; i<dimension;i++){
      if(fgets(line,18,arq)!=NULL){
          countLine=0;
          for(int j = 0;j<dimension;j++){
            if(isDigit(line[countLine])){
                gridMatrix[i][j] = charToInt(line[countLine]);
            }
            countLine+=2;
        }
        fgets(line,17,arq);
      }

    }
    //If everything went well, and we have a sucess, print the sudoku solution.
    if(sudokuSolver(gridMatrix)){
      printGrid(gridMatrix);
    }else{
      printf("No solution exists");
    }

    //Deallocating the matrix
    freeMatrix(gridMatrix);
    fclose(arq);
    getchar();
    return 0;
}

Boolean isDigit(int c){
  if(c >= '0' && c<='9')
    return TRUE;
  return FALSE;
}

int charToInt(char s){
  //If the character is a number between 0 and 9, so, transform the char in a integer
  int num = 0;
  num = (s - '0');
  return num;
}


int** createGrid(){
  /*
    *Creating the sudoku grid 9x9
    *and returning the grid adress
  */
  int **pointerGrid = (int**)malloc(sizeof(int*)*dimension);
  for(int i =0;i<dimension;i++){
    pointerGrid[i] = (int*) malloc(sizeof(int)*dimension);
  }
  return pointerGrid;

}

void freeMatrix(int** grid){
  for(int i =0;i<dimension;i++){
    free(grid[i]);
  }
  free(grid);
}

void printGrid(int** grid){
  //Show the entire matrix
  for(int i =0;i<dimension;i++){
    for(int j =0;j<dimension;j++){
      printf("%d ", grid[i][j]);
    }
    printf("\n");
  }
}

/*
  *This function search for a location in the sudoku grid which wasn't filled with a number yet, if yes, he put the coordinates in the
  *list. After, this coordinates will be used for check if the location is safe for put any number 'N' or not.
*/
Boolean findEmptyLocation(int** grid, int* list){
  /*
    *row = i
    *col = j
  */
  for (int i =0;i<dimension;i++){
    for(int j =0;j<dimension;j++){
      if(grid[i][j] == 0){
        list[0] = i;
        list[1] = j;
        return TRUE;
      }
    }
  }
  return FALSE;

}

/*
  *This function check if is 'okay' for put the given number to the given matrix coordinates and returns a Boolean to say if the action
  *is legal or not.

*/
Boolean checkLocationIsSafe(int** grid, int row, int col, int num){
  //Will check if the given number is not already placed in current row, column, or 3x3 box.
  return (!usedInRow(grid, row, num) && !usedInCol(grid, col, num)) && (!usedInBox(grid,row - row%3, col - col%3, num));
}

//Return true if the given number is matched with other element in a given row
Boolean usedInRow(int** grid, int row, int num){
  for (int i = 0;i<dimension;i++){
    if (grid[row][i] == num){
      return TRUE;
    }
  }
  return FALSE;
}

//Return true if the given number is matched with other element in a given column
Boolean usedInCol(int** grid, int col, int num){
  for(int i = 0;i<dimension;i++){
    if (grid[i][col] == num){
      return TRUE;
    }
  }
  return FALSE;
}

//Return true if the given number is matched with other element in a given 3x3 box
Boolean usedInBox(int**grid, int row, int col, int num){
  for(int i =row;i<3;i++){
    for(int j =col;j<3;j++){
      if(grid[i][j] == num){
        return TRUE;
      }
    }
  }
  return FALSE;
}
/*
  *Takes an unsolved sudoku grid and attempts to solve it, assigning values to each void location at the grid,
  *and the same time respecting the game rules.(Non-duplicated numbers in the same row, column and box)

*/
Boolean sudokuSolver(int** grid){
  //'l' is a list which will get the coordinates of a void location at the grid
  int l[2] = {0, 0};

  //If don't exist a void location, so, the solution is over.
  if(!findEmptyLocation(grid, l)){
    return TRUE;
  }
  //Assign to row and col the coordinates of a void location that we received from the above function
  int row = l[0];
  int col = l[1];
  //Digits 1 to 9
  for(int num = 1;num<10;num++){
    //If seems okay...
    if(checkLocationIsSafe(grid, row, col, num)){
      //Attempts to assign the number
      grid[row][col] = num;

      //If true, sucess!We done
      if(sudokuSolver(grid)){
        return TRUE;
      }

      //if failure, unmake and try again
      grid[row][col] = 0;
    }

  }
  //This provokes the backtracking
  return FALSE;
}
