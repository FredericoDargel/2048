// Libraries
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// Game's Grid
typedef struct grid {
  int size;
  int **board; // This plays out as int board[size][size];
} GRID;

// Function Index:
int controller (void);
int RNG (int, int);
GRID makeGrid (void);
GRID copyGrid (GRID);
GRID rodateBoard (GRID);
GRID slide (GRID);
int combine (GRID);
void newNumber (GRID);
bool isChanged (GRID, GRID);
bool isValue (GRID, int);
void printGame (GRID, int, int);
void printController (int);
void printBoard (GRID);


/* Function: game
 * --------------
 * Runs game logic.
 *
 * returns: score
 */
int game () {

  // Booting up some core variables.
  srand(time(NULL)); // random seed using functions from time.h
  int score = 0;
  int dir = -1;
  int lastDir = -1;
  bool isLost = false;
  bool isWon = false;

  GRID copy, buffer;
  GRID oGrid = makeGrid();

  for (int i = 0; i < 2; i++) {
    newNumber(oGrid);
  }
  // ... and we're ready!

  while (isLost == false) {

    while (dir == -1) {
      printGame(oGrid, score, lastDir);
      dir = controller();

    }

    copy    = copyGrid(oGrid);
    buffer  = copyGrid(oGrid);

    // "Rodates" the copy of the game's board.
    for (int i = 0; i < dir; i++) {
      buffer = copyGrid(rodateBoard(buffer));
    }

    buffer = slide(buffer); // slides numbers to the left.

    // combines the numbers and keeps track of score.
    if (isWon) {
      score += combine(buffer) * 2;

    } else {
      score += combine(buffer);

    }

    buffer = slide(buffer); // slides remaining numbers to the left.

    // Rodates to the original orientation.
    for (int i = 0; i < 4-dir; i++) {
      buffer = copyGrid(rodateBoard(buffer));
    }
    lastDir = dir;
    dir = -1; // reset flag;

    // Coppies the modified grid to game's main board.
    oGrid = copyGrid(buffer);

    // Check for 2048
    if (isValue(oGrid, 2048)) {
      isWon = true;
    }

    // If there's empty spaces and the board changed with the last movement:
    if (isValue(oGrid, 0) && isChanged(oGrid, copy)) {
      newNumber(oGrid);

    // Else, if there's not empty spaces:
    } else if (!(isValue(oGrid, 0))) {
      isLost = true;
      break;
    }
  }

  // Win/Loose messages.
  if (isWon) {
    printf(BBLU "\n\t\tYOU WON!\n");

  } else {
    printf(BRED "\n\n\t\t ...You lost!\n");

  }

  printf("\t\tFinal Score: %d\n\n" RESET, score);

  return score;
}

/* Function: controller
 * --------------
 * Converts input into a rotacional int.
 *
 * returns: number of times to rotate the board.
 */
int controller (void) {

  char rot = ' ';

  printf(BWHT "\n\t\t     ");
  printf("\033[4A"); // Move up X lines;
  printf(": ");
  scanf("%c", &rot);
  printf(RESET);

  if (rot == 'w'){          // up
    rot = 3;

  } else if (rot == 'a') {  // left
    rot = 0;

  } else if (rot == 's') {  // down
    rot = 1;

  } else if (rot == 'd') {  // right
    rot = 2;

  } else {
    return -1;              // unvalid char flag

  }

  return rot;
}

/* Function: RNG
 * --------------
 * Random Number Generator.
 *
 * min: minium value.
 * max: maximum value.
 *
 * returns: random value between min and max;
 */
int RNG (int min, int max) {

  int rng = 0;

  if (min < max) {
    rng = rand() % (max - min) + min;

  }

  return rng;
}

/* Function: newNumber
 * --------------
 * Adds a new number into the board.
 *
 * oGrid: game's original grid.
 */
void newNumber (GRID oGrid){

  int posSize = 2;
  int pos[posSize];

  do{

    for (int i = 0; i < posSize; i++){
      pos[i] = RNG(0, oGrid.size);

    }

  } while (oGrid.board[pos[0]][pos[1]] != 0); // checks if the position is 0;

  if (RNG(0, 100) > 50){
    oGrid.board[pos[0]][pos[1]] = 4;

  } else{
    oGrid.board[pos[0]][pos[1]] = 2;

  }

}

/* Function: makeGrid
 * --------------
 * Sets up a new Grid with standard values.
 *
 * returns: 2048 game's grid.
 */
GRID makeGrid (void) {

  GRID grid;
  grid.size = 4;

  grid.board = calloc(grid.size, sizeof(int*));
  for (int i = 0; i < grid.size; i++) {
    grid.board[i] = calloc(grid.size, sizeof(int));

  }

  return grid;
}

/* Function: copyGrid
 * --------------
 * Coppies a grid.
 *
 * returns: coppied grid.
 */
GRID copyGrid (GRID oGrid) {

  GRID copy = makeGrid();

  for (int i = 0; i < copy.size; i++) {
    for (int j = 0; j < copy.size; j++) {
      copy.board[i][j] = oGrid.board[i][j];

    }
  }

  return copy;
}

/* Function: rodateBoard
 * --------------
 * Makes a new grid that is 90º degrees in rotation from the original one.
 *
 * oGrid: game's original grid.
 *
 * returns: rotated version of oGrid.
 */
GRID rodateBoard (GRID oGrid) {

  GRID buffer = makeGrid();
  int x = 0;

  for (int i = 0; i < oGrid.size; i++) {
    for (int j = oGrid.size - 1; j >=0 ; j--) {
      buffer.board[i][x++] = oGrid.board[j][i];

    }
    x = 0;

  }

  return buffer;
}

/* Function: slide
 * --------------
 * slides numbers on the board to the left.
 *
 * oGrid: game's original grid.
 *
 * returns: slided version of the oGrid.
 */
GRID slide (GRID oGrid) {

  GRID buffer = makeGrid();
  int bCount = 0;

  for (int i = 0; i < oGrid.size; i++) {
    for (int j = 0; j < oGrid.size; j++) {
      if (oGrid.board[i][j] > 0) {
        buffer.board[i][bCount] = oGrid.board[i][j];
        oGrid.board[i][j] = 0;
        bCount++;

      }

    }
    bCount = 0;

  }

  return buffer;
}

/* Function: combine
 * --------------
 * Combines numbers and yield score.
 *
 * returns: the amount of points combinations did with this move.
 */
int combine (GRID oGrid) {
  
  int score = 0;
  
  for (int i = 0; i < oGrid.size; i++) {
    for (int j = 0; j < oGrid.size; j++) {
      if (oGrid.board[i][j] == oGrid.board[i][j+1] && oGrid.board[i][j] > 0) {
        oGrid.board[i][j] += oGrid.board[i][j];
        score += oGrid.board[i][j];
        oGrid.board[i][j+1] = 0;

      }
    }
  }

  return score;
}



/* Function: isChanged
 * --------------
 * Checks if GRID's board was changed.
 */
bool isChanged (GRID oGrid, GRID newGrid) {

  for (int i = 0; i < oGrid.size; i++) {
    for (int j = 0; j < oGrid.size; j++) {
      if (oGrid.board[i][j] != newGrid.board[i][j]) {
        return true;
      }
    }
  }
  return false;
  }

/* Function: isValue
 * --------------
 * Checks if there is a value in the GRID's board.
 */
bool isValue (GRID oGrid, int value) {

  for (int i = 0; i < oGrid.size; i++) {
    for (int j = 0; j < oGrid.size; j++) {
      if (oGrid.board[i][j] == value) {
        return true;
      }
    }
  }
  return false;
}

/* Function: printGame
 * --------------
 * Handles the game drawing.
 */
void printGame (GRID oGrid, int score, int dir) {

  printf(CLEAR);

  printf( WHT "\t   2048" RESET);
  printf( WHT " - SCORE: %d\n\n" RESET, score);
  printBoard(oGrid);
  printController(dir);
}

/* Function: printController
 * --------------
 * Prints the controller using last direction to change it's color.
 *
 * lasDir: uses last direction to paint controller.
 */
void printController (int lastDir) {

  printf(WHT);

  if (lastDir == 3) {
    printf(BRED);
  }
  printf( "\t\t     \u2191 (w)\n" WHT);

  if (lastDir == 0) {
    printf(BRED);
  }
  printf("\t     \u2190 (a)" WHT);

  if (lastDir == 2) {
    printf(BRED);
  }
  printf("\t     \u2192 (d)\n" WHT);

  if (lastDir == 1) {
    printf(BRED);
  }
  printf("\t\t     \u2193 (s)\n" WHT);

  printf("\n\t            [ENTER]");

  printf(RESET);
}

/* Function: printBoard
 * --------------
 * Prints the board from given GRID.
 *
 * oGrid: game's original grid.
 */
void printBoard (GRID oGrid) {


  for (int i = 0; i < oGrid.size; i++) {
    for (int j = 0; j < oGrid.size; j++) {

      switch (oGrid.board[i][j]) {

        default:
          printf(BRED "\t%d" RESET, oGrid.board[i][j]);
          break;

        case 0:
          printf(RESET "\t   %d" RESET, oGrid.board[i][j]);
          break;

        case 2:
          printf(WHT "\t   %d" RESET, oGrid.board[i][j]);
          break;

        case 4:
          printf(CYN "\t   %d" RESET, oGrid.board[i][j]);
          break;

        case 8:
          printf(BLU "\t   %d" RESET, oGrid.board[i][j]);
          break;

        case 16:
          printf(GRN "\t  %d" RESET, oGrid.board[i][j]);
          break;

        case 32:
          printf(YEL "\t  %d" RESET, oGrid.board[i][j]);
          break;

        case 64:
          printf(MAG "\t  %d" RESET, oGrid.board[i][j]);
          break;

        case 128:
          printf(RED "\t %d" RESET, oGrid.board[i][j]);
          break;

        case 256:
          printf(BWHT "\t %d" RESET, oGrid.board[i][j]);
          break;

        case 512:
          printf(BMAG "\t %d" RESET, oGrid.board[i][j]);
          break;

        case 1024:
          printf(BYEL "\t%d" RESET, oGrid.board[i][j]);
          break;

        case 2048:
          printf(BGRN "\t%d" RESET, oGrid.board[i][j]);
          break;
      }

    }
    printf("\n");

  }
  printf("\n");

}
