#include "cell.h"

Cell::Cell(){
  _type = Cell::Type::Empty;
  _state = Cell::State::NotVisible;
}

void
Cell::initialize(Cell::Type type, Cell::State state) {
  _type = Cell::Type::Empty;
  _state = Cell::State::NotVisible;
};

std::vector<Cell> 
Cell::get_neightbors() {
  return _neightbors;
}

Cell::Type
Cell::get_type() {
  return _type;
}

Cell::State
Cell::is_visible() {
  return _state;
}
