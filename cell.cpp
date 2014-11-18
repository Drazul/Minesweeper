#include "cell.h"

Cell::Cell(){
  _type = Cell::Type::Empty;
  _state = Cell::State::NotVisible;
  _flag = Flag::NotFlagged;
}

void
Cell::initialize(Cell::Type type, Cell::State state) {
  _type = type;
  _state = state;
  _flag = Flag::NotFlagged;
};

void 
Cell::put_flag() {
	_flag = Flag::Flagged;
}

void
Cell::remove_flag() {
	_flag = Flag::Flagged;
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
	/*
	if (_type != Type::Bomb)
		_type = static_cast<Type> (_type + 1);
		*/
}
