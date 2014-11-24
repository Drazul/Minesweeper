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
  _game_state = BoardState::Continue;

  initialize_empty();
  initialize_bombs();
  initialize_near();
}

void
Minesweeper::initialize_empty() {
  for(Cell& cell: _board){
    cell.initialize(Cell::Type::Empty, Cell::State::NotVisible);
  }
  /*
  for(int i = 0; i < (int) _board.size(); i++){
    _board[i].initialize(Cell::Type::Empty, Cell::State::NotVisible);
  }*/
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
Minesweeper::initialize_near_index(int index, int pos) {
  if( index >= 0 && index < (int) _board.size() && !_board[index].is_bomb() 
                                  && abs((index % _level) - pos) <= 1)
    _board[index].change_to_type_near();
}

void
Minesweeper::initialize_near() {
  for(int mine: _mines){
    initialize_near_index(mine - 1,          mine % _level);
    initialize_near_index(mine - 1 - _level, mine % _level);
    initialize_near_index(mine - 1 + _level, mine % _level);
    initialize_near_index(mine + 1,          mine % _level);
    initialize_near_index(mine + 1 - _level, mine % _level);
    initialize_near_index(mine + 1 + _level, mine % _level);
    initialize_near_index(mine -     _level, mine % _level);
    initialize_near_index(mine +     _level, mine % _level);
  }
}

std::vector<char> 
Minesweeper::execute(int x, int y) {
  int index;
  index = (x * _level) + y;
  
  if(_board[index].is_flagged()) {
    return get_visible_board();
  }
  std::queue<int> queue;

  _board[index].execute();

  queue.push(index);

  while(!queue.empty()){
    index = queue.front();
    queue.pop();

    switch(_board[index].get_type()){

      case Cell::Type::Empty:
        x = index / _level;
        y = index % _level;
        for (int i = x - 1; i <= x + 1; i++){
          for(int j = y - 1; j <= y + 1; j++){
            if(i >= 0 && j >= 0 && i < _level && j < _level){
              index = (i * _level) + j;
              if(!_board[index].is_visible() && !_board[index].is_flagged()){
                _board[index].execute();
                queue.push(index);
              }
            }
          }
        }
        break;

      case Cell::Type::Bomb:
        execute_all_bombs();
        _game_state = BoardState::GameOver;
        break;

      case Cell::Type::Near:
        _board[index].execute();
    }
  }
  return get_visible_board(); 
}

void
Minesweeper::execute_all_bombs() {
  for(int mine: _mines)
    _board[mine].execute();
  
}

std::vector<char>
Minesweeper::get_visible_board() {
  std::vector<char> visible_board = std::vector<char>(_level * _level);
  visible_board.assign(_level*_level,'*');
  int num_visibles = 0;

  for (int i=0; i< (int) _board.size(); i++){
    if(_board[i].is_visible()){
      num_visibles++;
      switch(_board[i].get_type()){
        case Cell::Type::Empty:
          visible_board[i] = ' ';
          break;
        case Cell::Type::Near:
          char aux;
          aux = (char) _board[i].get_number_near_bombs();
          visible_board[i] =  aux;
          break;
        case Cell::Type::Bomb:
          visible_board[i] = 'B';
          break;
      }
    }else {
      if(_board[i].is_flagged())
        visible_board[i] = 'F';
      else
        visible_board[i] = '*';
    }
  }

  if(num_visibles == ((int) _board.size() - _number_of_bombs) && _game_state == BoardState::Continue)
    _game_state = BoardState::Winner;

  return visible_board;
}

BoardState
Minesweeper::get_board_state() {
  return _game_state;
}

std::vector<char> 
Minesweeper::put_flag(int x, int y) {
  int index = (x * _level) + y;
  if(_board[index].is_flagged())
    _board[index].remove_flag();
  else
    _board[index].put_flag();

  return get_visible_board();
}