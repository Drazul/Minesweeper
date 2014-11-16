#include <typeinfo>
#include <iostream>
#include <algorithm>

#include <bandit/bandit.h>

#include "game.h"


using namespace bandit;

go_bandit([] () {

  describe("minesweeper game", [] () {
    static Minesweeper minesweeper;

    before_each( [&](){
      minesweeper.set_difficulty(Difficulty::VeryEasy);
    });

    it("has a difficulty level", [&]() {
      Assert::That(minesweeper.get_difficulty(),
                  Is().EqualTo(Difficulty::VeryEasy).Or()
                  .EqualTo(Difficulty::Easy).Or()
                  .EqualTo(Difficulty::Medium).Or()
                  .EqualTo(Difficulty::Hard));
    });

    it("has a number of bombs depending on difficulty level", [&]() {
      Assert::That(minesweeper.get_number_of_bombs(),
                  Is().EqualTo((minesweeper.get_difficulty()/10)+1));
    });
    
    it("has a number of cells depending on difficulty level", [&](){
      Assert::That(minesweeper.get_board().size(),
                  Is().EqualTo(3*3).Or()
                  .EqualTo(8*8).Or()
                  .EqualTo(16*16).Or()
                  .EqualTo(30*16));
    });


    describe("has a board that contain cells", [&](){
      std::vector<Cell> board;

      before_each([&]() {
        board = minesweeper.get_board();

        for(Cell cell: board){
          cell.initialize(Cell::Type::Empty, Cell::State::Visible);
        }

      });

      it("can check cell types", [&]() {
        std::for_each(board.begin(), board.end(), [](Cell& cell) {
          Assert::That(cell.get_type(),
                      Is().EqualTo(Cell::Type::Bomb).Or()
                      .EqualTo(Cell::Type::Near).Or()
                      .EqualTo(Cell::Type::Empty));
        });
      });
/*
      it("know their neightbors", [&]() {
        std::for_each(board.begin(), board.end(), [](Cell& cell) {
          Assert::That(cell.get_neightbors().size(),
                      Is().Not().EqualTo(0));
        });
      });
*/
      it("can check the visibility of cells", [&]() {
        std::for_each(board.begin(), board.end(), [](Cell& cell) {
          Assert::That(cell.is_visible(),
                      Is().EqualTo(Cell::State::Visible).Or()
                      .EqualTo(Cell::State::NotVisible));
        });
      });

      it("can check the number of cells that have bomb type", [&]() {
        int counter=0;
        for(Cell cell : board)
          if (cell.get_type() == Cell::Type::Bomb) counter++;

        Assert::That(counter, 
                      Is().EqualTo(minesweeper.get_number_of_bombs()));
      });
    });
  });
});

int main(int argc, char *argv[]) {

  return bandit::run(argc, argv);
}
