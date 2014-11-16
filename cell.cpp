#include "cell.h"

Cell::Cell(){
  _type = Cell::Type::Empty;
  _state = Cell::State::NotVisible;
}

void
Cell::initialize(Cell::Type type, Cell::State state) {
  _type = type;
  _state = state;
};

bool
Cell::is_bomb() {
	return (_type == Cell::Type::Bomb);
}

Cell::Type
Cell::get_type() {
  return _type;
}

Cell::State
Cell::is_visible() {
  return _state;
}

void
Cell::chage_type_near_to_bomb() {
	_type = Type::Near;
	/*
	if (_type != Type::Bomb)
		_type = static_cast<Type> (_type + 1);
		*/
}
