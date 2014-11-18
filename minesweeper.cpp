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
  _game_state = GameState::Continue;
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
        if (i >= 0 && i < _level && j >= 0 && j < _level){
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
  _board[(x * _level) + y].execute();
  std::cout << "Execute " << x << " " << y << std::endl;

  while(!queue.empty()){
    coord = queue.back();
    queue.pop();

    index = (coord.first * _level) + coord.second;

    if(_board[index].get_type() == Cell::Type::Empty){
      for (int i = coord.first - 1; i <= coord.first + 1; i++){
        for(int j = coord.second - 1; j <= coord.second + 1; j++){
          std::cout << "punto " << i << " " << j << std::endl;
          if(i >= 0 && j >= 0 && i < _level && j < _level){
            index = (i * _level) + j;
            if(!_board[index].is_visible()){
               std::cout << "Execute " << i << " " << j << std::endl;
               std::cout << "Before execute " << _board[index].get_type() << std::endl;
              _board[index].execute();
              queue.push(std::make_pair(i, j));
            }
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

std::vector<char>
Minesweeper::get_visible_board() {
  std::vector<char> visible_board = std::vector<char>(_level * _level);
  visible_board.assign(_level*_level,'*');
/*
  for(int i = 0; i < _board.size(); i++){
    if(_board[i].is_visible()){
      if(_board[i].is_bomb()){
        visible_board[i] = 'B';
        _game_state = GameState::GameOver;
      }
      if(_board[i].get_type() == Cell::Type::Empty) visible_board[i] = ' ';
      if(_board[i].get_type() == Cell::Type::Near) visible_board[i] = '1';
    }else if(_board[i].is_flagged()) visible_board[i] = 'F';
  }
  */
  for (int i=0; i< _board.size(); i++){
    switch(_board[i].get_type()){
      case Cell::Type::Empty:
        visible_board[i] = ' ';
        break;
      case Cell::Type::Near:
        visible_board[i] = 'N';
        break;
      case Cell::Type::Bomb:
        visible_board[i] = 'B';
    }
  }

  return visible_board;
}

GameState
Minesweeper::get_game_state() {
  return _game_state;
}