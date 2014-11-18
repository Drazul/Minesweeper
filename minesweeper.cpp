#include "minesweeper.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <queue>

Minesweeper::Minesweeper() {}
//Minesweeper::Minesweeper(Difficulty level): _level(level) {}; 

void
Minesweeper::set_difficulty(Difficulty level) {
  _level = level;
  _board = std::vector<Cell>(_level * _level);
  _mines = std::vector<int>(_level);
  _number_of_bombs = _level;
}


std::vector<Cell>
Minesweeper::get_board() {
  return _board;
}

Difficulty
Minesweeper::get_difficulty() {
  return _level;
}

int 
Minesweeper::get_number_of_bombs() {
	return static_cast<int>(_number_of_bombs);
}

void
Minesweeper::initialize() {
  initialize_empty();
  initialize_bombs();
  initialize_near();
}

void
Minesweeper::initialize_empty() {
  for(Cell cell: _board){
    cell.initialize(Cell::Type::Empty, Cell::State::NotVisible);
  }
}

void
Minesweeper::initialize_bombs() {
  srand (time(NULL));
  int counter_bombs = 0;
  int x, y, index;

  while(counter_bombs < get_number_of_bombs()){
    x = rand() % _level;
    y = rand() % _level;

    index = (x * _level) + y;
    if(!_board[index].is_bomb()){
      _mines[counter_bombs] = index;

      _board[index].initialize(Cell::Type::Bomb, Cell::State::NotVisible);
      counter_bombs++;
    }
  }
}

void
Minesweeper::initialize_near() {
  int index, x, y;
  for(int mine: _mines){
    x = mine / _level;
    y = mine % _level;
    for (int i = x - 1; i < x + 1; i++){
      for (int j = y -1; j < y + 1; j++){
        if (i > 0 && i < _level && j > 0 && j < _level){
          index = (i * _level) + j; 
          if (!_board[index].is_bomb())
            _board[index].chage_type_near_to_bomb();
        }
      }
    }
  }
}

std::vector<Cell> 
Minesweeper::execute(int x, int y) {
  int index;
  std::queue<std::pair<int, int>> queue;
  std::pair<int, int> coord;

  queue.push(std::make_pair(x, y));

  while(!queue.empty()){
    coord = queue.back();
    queue.pop();

    index = (coord.first * _level) + coord.second;
    _board[index].execute();

    if(_board[index].get_type() == Cell::Type::Empty){
      for (int i = x - 1; i <= x + 1; i++){
        for(int j = y - 1; j <= y + 1; j++){
          index = (i * _level) + j;
          if(!_board[index].is_visible()){
            _board[index].execute();
            queue.push(std::make_pair(i, j));
          }
        }
      }
    }else{
      if(_board[index].is_bomb()){
        execute_all_bombs();
      }
    }
  }
  return _board;
}

void
Minesweeper::execute_all_bombs() {
  int index = _mines.size();
  for(int mine: _mines)
    _board[mine].execute();
  
}
