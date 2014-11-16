#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>

class Cell {
 public:
  enum Type{Bomb, Near, Empty};
  enum State{Visible, NotVisible};


  Cell();
  void initialize(Type type, State state);
  std::vector<Cell> get_neightbors();
  Type get_type();
  State is_visible();
  bool is_bomb();

 private:
  Type _type;
  std::vector<Cell> _neightbors;
  State _state;
};

#endif
