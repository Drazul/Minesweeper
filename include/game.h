#ifndef GAME_H
#define GAME_H

#include "cell.h"


#include <vector>

enum Difficulty {VeryEasy=9, Easy=64, Medium=256, Hard=480};
class Minesweeper {
  std::vector<Cell> _board;
  Difficulty _level;
  int _number_of_bombs;

 public:
  Minesweeper();
  //Minesweeper(Difficulty level);

  void
  set_difficulty(Difficulty level);

  Difficulty
  get_difficulty();

  int 
  get_number_of_bombs();

  std::vector<Cell>
  get_board();
};
#endif
