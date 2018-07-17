/*
 * Author: Frederico Dargel, aka: BikerViking
 * Purpose: Console Game - 2048
 * Language:  C
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

void game();
int setDir();
int ** makeGrid(int);
int ** copyGrid (int **, int);
bool checkChange(int **, int **, int);
void printGrid(int **, int);
bool checkValue(int **, int, int);
int RNG(int, int);
void newNumber(int **, int);
int ** rodateGrid(int **, int);
int ** slide(int **, int);
void combine(int **, int);

/*
  * Function:
  * --------------
  * Description
  *
  * Param
  * Vars
  *
  * Returns
 */
int main () {

  game();

  return 0;
}

/*
  * Function: game
  * --------------
  * Runs game logic,
  *   Player has a size x size board with 2 numbers ramdonly placed inside.
  *   Whenever player choose a (dir)ection, all numbers should slide to that
  *   direction if possible. Equal numbers are added in this process and a new
  *   number is placed randomly, as well, at the board.
  *   However, if nothing is changed on the board, no new numbers are added.
  *   The goal of the game is to make 2048.
  *
  *
  * size: int, size of the 2048 board.
  * dir: int, direction to be slid.
  * copy: int**, a copy of the actual board.
  * buffer: int**, another copy of the actual board.
  * grid: int**, the actual board.
  *
  *
 */
void game () {

  // Booting up...
  srand(time(NULL));

  int size = 4;

  int dir = 0;

  int ** copy, **buffer;

  int ** grid = makeGrid(size);

  for (int i = 0; i < 2; i++) {
    newNumber(grid, size);
  }
  // ,,,and we're ready!

  // While there's space on the grid and player has not yet reached 2048:
  while (checkValue(grid, size, 0) || (checkValue(grid, size, 2048))) {

    printGrid(grid, size);

    dir = setDir();

    copy = copyGrid(grid, size);
    buffer = copyGrid(grid, size);

    // Rodates the copy of the board based on dir
    for (int i = 0; i < dir; i++) {
      buffer = copyGrid(rodateGrid(buffer, size), size);
    }

    // Slides numbers to the left and combine them.
    buffer = slide(buffer, size);
    combine(buffer, size);

    // Rotates board again to original form
    for (int j = 0; j < 4-dir; j++) {
      buffer = copyGrid(rodateGrid(buffer, size), size);
    }

    // Copies back to grid
    grid = copyGrid(buffer, size);

    // Checks if there's free space and if board has changed
    if (checkValue(grid, size, 0) && checkChange(grid, copy, size)) {
      newNumber(grid, size);
    }
  }

  // Win/Loose messages.
  if (checkValue(grid, size, 2048)) {
    printf("YOU WON!\n");
  } else {
    printf("You lost!\n");
  }

  // Freedom!
  free(copy);
  free(buffer);
  free(grid);
}

/*
  * Function: setDir
  * --------------
  * Converts numpad arrows to a usable int.
  *
  * dir: int, set's direction of the slide.
  *
  * Returns
  *   dir
 */
int setDir() {

  int dir = 0;

  while (dir != 8 && dir != 2 && dir != 6 && dir != 4) {
    scanf("%d", &dir);
  }

  if (dir == 8){ // up
    dir = 3;
  } else if (dir == 2) { // down
    dir = 1;
  } else if (dir == 6) { // right
    dir = 2;
  } else if (dir == 4) { // left
    dir = 0;
  }

  return dir;
}

/*
* Function: RNG
* --------------
* Random Number Generator.
*
* Param
*   min: int, minium value.
*   max: int, maximum value.
*
* Returns
*   rng: int, random value between min and max;
*/
int RNG (int min, int max) {

  int rng = 0;

  if (min < max) {
    rng = rand() % (max - min) + min;

  }

  return rng;
}

/*
  * Function: makeGrid
  * --------------
  * makes a blank array
  *
  * Param
  *   size: int, size of the array.
  *
  * Returns
  *   grid, int**, pointer to a blank int matrix[size][size].
 */
int ** makeGrid (int size) {

  int ** grid = calloc(size, sizeof(int*));

  for (int i = 0; i < size; i++) {
    grid[i] = calloc(size, sizeof(int));

  }

  return grid;
}

/*
  * Function: printGrid
  * --------------
  * prints array
  *
  * Param
  *   grid: int**, array to be printed.
  *   size: int, size of the array.
 */
void printGrid(int **grid, int size) {

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("\t%d", grid[i][j]);

    }
    printf("\n");

  }
  printf("\n");
}

/*
  * Function: newNumber
  * --------------
  * generates a position to add a new number to the board
  *
  * Param
  *   grid: int**, array where new numbers will be added.
  *   size: int, size of the array.
 */
void newNumber(int **grid, int size) {

  int posSize = 2;
  int pos[posSize];

  do{

    for (int i = 0; i < posSize; i++){
      pos[i] = RNG(0, size);

    }

  } while (grid[pos[0]][pos[1]] != 0); // checks if the position is 0;

  if (RNG(0, 100) > 50){
    grid[pos[0]][pos[1]] = 4;

  } else{
    grid[pos[0]][pos[1]] = 2;

  }
}

/*
  * Function: slide
  * --------------
  * Slides numbers of the grid to the left.
  *
  * Param
  *   grid: int**, array to be slid.
  *   size: int, size of the array.
  *
  * Returns
  *   buffer: int**, new array with all numbers left slid.
 */
int ** slide(int **grid, int size){

  int ** buffer = makeGrid(size);
  int bCount = 0;

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (grid[i][j] > 0) {
        buffer[i][bCount] = grid[i][j];
        grid[i][j] = 0;
        bCount++;
      }
    }
    bCount = 0;
  }

  return buffer;
}

/*
  * Function: rodateGrid
  * --------------
  * Rodates array in 90º
  *
  * Param
  *   grid: int**, array to be rotated
  *   size: int, size of the array.
  *
  * Returns
  *   buffer: int**, rotated grid.
 */
int ** rodateGrid(int **grid, int size) {

  int ** buffer = makeGrid(size);
  int x = 0;

  for (int i = 0; i < size; i++) {
    for (int j = size-1; j >=0; j--) {
      buffer[i][x++] = grid[j][i];
    }
    x = 0;
  }

  return buffer;
}

/*
  * Function: combine
  * --------------
  * combines equal numbers of the grid,
  *
  * Param
  *   grid: int**, array to be combined.
  *   size: int, size of the array.
  *
 */
void combine(int **grid, int size) {

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if(grid[i][j] == grid[i][j+1] && grid[i][j] > 0) {
        grid[i][j] += grid[i][j];
        grid[i][j+1] = 0;
      }
    }
  }

}

/*
  * Function: copyGrid
  * --------------
  * Copies array
  *
  * Param
  *   grid: int**, array to be copied.
  *   size: int, size of the array.
  *
  * Returns
  *   copy: int**, a copy from grid.
 */
int ** copyGrid (int **grid, int size) {

  int ** copy = makeGrid(size);

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      copy[i][j] = grid[i][j];
    }
  }

  return copy;
}


/*
  * Function: checkValue
  * --------------
  * checks if there is a value on the grid.
  *
  * Param
  *   grid: int**, array to be checked.
  *   size: int, size of the array.
  *   value: int, checking value.
  *
  * Returns:
  *   true/false: if value is or isn't found on the array.
 */
bool checkValue(int ** grid, int size, int value) {

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (grid[i][j] == value) {
        return true;
      }
    }
  }

  return false;

}

/*
  * Function: checkChange
  * --------------
  * checks if an array is equal to another
  *
  * Param
  *   grid: int**, compare array 1.
  *   newGrid: int**, compare array 2.
  *   size: int, size of the array.
  *
  * Returns:
  *   true/false: if arrays are or not equal.
 */
bool checkChange(int **grid, int **newGrid, int size) {

  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {

      if (grid[i][j] != newGrid[i][j]) {
        return true;
      }

    }
  }

  return false;

}
