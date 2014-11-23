#include "cell.h"

Cell::Cell(){
  _type = Cell::Type::Empty;
  _state = Cell::State::NotVisible;
  _flag = Flag::NotFlagged;
  _near_bombs = 48; /* This is the number 0 in ascii */
}

void
Cell::initialize(Cell::Type type, Cell::State state) {
  _type = type;
  _state = state;
  _flag = Flag::NotFlagged;
  _near_bombs = 0;
};

void 
Cell::put_flag() {
  if(_state == Cell::State::NotVisible)
    _flag = Flag::Flagged;

}

void
Cell::remove_flag() {
	_flag = Flag::NotFlagged;
}

bool
Cell::is_flagged() {
	return (_flag ==Flag::Flagged);
}

bool
Cell::is_bomb() {
	return (_type == Cell::Type::Bomb);
}

Cell::Type
Cell::get_type() {
  return _type;
}

bool
Cell::is_visible() {
  return (_state == Cell::State::Visible);
}

void
Cell::execute() {
	if(_flag != Flag::Flagged && _state == State::NotVisible)
		_state = State::Visible;
}

void
Cell::change_to_type_near() {
	_type = Type::Near;
	_near_bombs++;
}

int 
Cell::get_number_near_bombs() {
	return _near_bombs;
}