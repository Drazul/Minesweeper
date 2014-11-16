#ifndef CELL_H
#define CELL_H
#include <iostream>
#include <vector>

class Cell {
 public:
  enum Type {Bomb=-1, Empty=0, Near=1};
  enum State {Visible, NotVisible};
  enum Flag {Flagged, NotFlagged};

  Cell();
  void initialize(Type type, State state);
  Type get_type();
  State is_visible();
  bool is_bomb();
  void chage_type_near_to_bomb();
  void put_flag();
  void remove_flag();
  void execute();
  bool is_flagged();

 private:
  Type _type;
  std::vector<Cell> _neightbors;
  State _state;
  Flag _flag;
};

#endif
