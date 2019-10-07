/* Author: Frederico Dargel
 * Purpose: Simple Console-Based 2048 Game.
 * Language: C
 */

// Libraries
#include <string.h>

#include "definitions.h"
#include "game.h"
#include "score.h"

// Function Index:
void printHelp ();
void unknownArgv (const char *);
void tooManyArgc ();

/* Function: main
 * --------------
 * Handles arguments.
 *
 * argc: argument counter
 * argv: argument vector
 *
 * returns: Progam exit status.
 */
int main (int argc, char const *argv[]) {

  printf(CLEAR);

  initScoreFile();

  // No argv, "-p" or "play" will play the game.
  if (argc == 1 ||
      (argc == 2 &&
        ((strcmp (argv[1], "-p") == 0) || (strcmp (argv[1], "play") == 0)))) {

      saveHigh(game());

  } else if (argc == 2) {

    // Acess score via "-s" or "score".
    if ((strcmp (argv[1], "-s") == 0) || (strcmp (argv[1], "score") == 0)) {
      printScore();

    // Help menu using "-h" or "help".
    } else if ((strcmp (argv[1], "-h") == 0) ||
      (strcmp (argv[1], "help") == 0)) {

      printHelp();

    // Unkown Argument:
    } else {
      unknownArgv(argv[1]);
      return -1;

    }

  // Too Many Arguments:
  } else if (argc > 2){
    tooManyArgc();
    return 1;

  }

   return 0;
}

/* Function: unknownArgv
 * --------------
 * Prints unkown argv message.
 *
 * argv: argument vector.
 */
void unknownArgv (const char * argv) {
  printf(BWHT "\t\t\t2048 - ???\n\n");
  printf(BRED "\t\tUnknown argument:\t \'%s\'\n\n", argv);
}

/* Function: tooManyArgc
 * --------------
 * Prints too many arguments message.
 */
void tooManyArgc () {
  printf(BWHT "\t\t\t2048 - ???\n\n");
  printf (BRED "\t\tUSAGE:");
  printf("\t./2048 [help]\n\t\t\t./2048 [-h]\n\n");
}

/* Function: printHelp
 * --------------
 * Prints Help menu.
 */
void printHelp () {

  printf(BWHT "\t\t\t2048 - HELP\n\n");
  printf(BCYN "\t\tPlays 2048:\t\t\'-p or play\'\n");
  printf(BGRN "\t\tShows high score:\t\'-s or score\'\n");
  printf(BYEL "\t\tPrints this message:\t\'-h or help\'\n\n\n" RESET);

}
