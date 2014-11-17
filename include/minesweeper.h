#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "cell.h"

#include <vector>

enum Difficulty {VeryEasy=3, Easy=8, Medium=16, Hard=32};
class Minesweeper {
  std::vector<Cell> _board;
  std::vector<std::pair<int, int>> _mines;

  Difficulty _level;
  int _number_of_bombs;

  void initialize_empty();
  void initialize_bombs();
  void initialize_near();

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

  void
  initialize();

  void 
  execute(int x, int y);

};
#endif
