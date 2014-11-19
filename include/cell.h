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
  bool is_visible();
  bool is_bomb();
  void change_to_type_near();
  void put_flag();
  void remove_flag();
  void execute();
  bool is_flagged();
  int  get_number_near_bombs();

 private:
  Type _type;
  std::vector<Cell> _neightbors;
  State _state;
  Flag _flag;
  int _near_bombs;
};

#endif
