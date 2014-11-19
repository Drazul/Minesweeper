#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "cell.h"

#include <vector>

enum Difficulty {VeryEasy=3, Easy=8, Medium=16, Hard=32};
enum GameState {Continue=0, GameOver=-1};

class Minesweeper {
  std::vector<Cell> _board;
  std::vector<int> _mines;

  Difficulty _level;
  GameState _game_state;
  int _number_of_bombs;

  void initialize_empty();
  void initialize_bombs();
  void initialize_near();
  void initialize_near_index(int index, int pos);
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

  std::vector<Cell> 
  execute(int x, int y);

  void
  execute_all_bombs();

  std::vector<char>
  get_visible_board();

  GameState
  get_game_state();

};
#endif
