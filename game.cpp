#include "game.h"

Minesweeper::Minesweeper() {}
//Minesweeper::Minesweeper(Difficulty level): _level(level) {}; 

void
Minesweeper::set_difficulty(Difficulty level) {
  _level = level;
  _board = std::vector<Cell>(_level);
}


std::vector<Cell>
Minesweeper::get_board() {
  return _board;
}

Difficulty
Minesweeper::get_difficulty() {
  return _level;
}
