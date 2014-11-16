#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>

class Cell {
 public:
  enum Type{Bomb=-1, Empty=0, Near=1};
  enum State{Visible, NotVisible};

  Cell();
  void initialize(Type type, State state);
  Type get_type();
  State is_visible();
  bool is_bomb();
  void chage_type_near_to_bomb();

 private:
  Type _type;
  std::vector<Cell> _neightbors;
  State _state;
};

#endif
