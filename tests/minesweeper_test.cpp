#include <typeinfo>
#include <iostream>
#include <algorithm>

#include <bandit/bandit.h>

#include "minesweeper.h"
#include <stdlib.h>

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
                  Is().EqualTo(Difficulty::VeryEasy).Or()
                  .EqualTo(Difficulty::Easy).Or()
                  .EqualTo(Difficulty::Medium).Or()
                  .EqualTo(Difficulty::Hard));    
    });
    
    it("has a number of cells depending on difficulty level", [&](){
      Assert::That(minesweeper.get_board().size(),
                  Is().EqualTo(Difficulty::VeryEasy * Difficulty::VeryEasy).Or()
                  .EqualTo(Difficulty::Easy * Difficulty::Easy).Or()
                  .EqualTo(Difficulty::Medium * Difficulty::Medium).Or()
                  .EqualTo(Difficulty::Hard * Difficulty::Hard));
    });


    describe("has a board that contain cells", [&](){
      std::vector<Cell> board;

      before_each([&]() {
        minesweeper.initialize();
        board = minesweeper.get_board();
      });

      it("can check cell types", [&]() {
        std::for_each(board.begin(), board.end(), [](Cell& cell) {
          Assert::That(cell.get_type(),
                      Is().EqualTo(Cell::Type::Bomb).Or()
                      .EqualTo(Cell::Type::Near).Or()
                      .GreaterThan(Cell::Type::Near).Or()
                      .EqualTo(Cell::Type::Empty));
        });
      });

      it("can be different types", [&]() {
        int counterBombs=0, counterEmpty=0, counterNear=0;

        for(Cell cell : board){
          if (cell.get_type() == Cell::Type::Bomb) counterBombs++;
          if (cell.get_type() == Cell::Type::Empty) counterEmpty++;
          if (cell.get_type() == Cell::Type::Near) counterNear++;
        }

        Assert::That(0, 
                    Is().LessThan(counterBombs).And()
                    .LessThan(counterEmpty).And()
                    .LessThan(counterNear));

      });

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

      it("can chek if a cell have flag", [&]() {
        std::for_each(board.begin(), board.end(), [](Cell& cell) {
          Assert::That(cell.is_flagged(),
                      Is().EqualTo(true).Or()
                      .EqualTo(false));
        });
      });

      it("cannot execute a flagged cell", [&]() {
        bool before, after;
        Cell cell;
        before = cell.is_visible();
        cell.put_flag();
        cell.execute();
        after = cell.is_visible();

        Assert::That(before,
                    Is().EqualTo(after));
      });

      it("can execute all bombs", [&]() {
        minesweeper.execute_all_bombs();
        board = minesweeper.get_board();

        int counterBombs = 0;

        for(Cell cell : board){
          if (cell.get_type() == Cell::Type::Bomb) counterBombs++;
        }

        Assert::That(counterBombs, 
                      Is().EqualTo(minesweeper.get_number_of_bombs()));
      });

      it("execute a cell wicht state is Empty execute all neighbors cells", [&]() {
        int x, y, index;
        do{
          x = rand() % minesweeper.get_difficulty();
          y = rand() % minesweeper.get_difficulty();

          index = (x * minesweeper.get_difficulty()) + y;
        } while (board[index].get_type() != Cell::Type::Empty);
        
        minesweeper.execute(x, y);
        board = minesweeper.get_board();

        int counter=0;

        for(Cell cell : board)
          if (cell.is_visible()) counter++;

        Assert::That(counter,
                    Is().GreaterThan(1));
      });

      it("execute a cell wicht state is Near only execute this cell", [&]() {
        int x, y, index;
        do{
          x = rand() % minesweeper.get_difficulty();
          y = rand() % minesweeper.get_difficulty();

          index = (x * minesweeper.get_difficulty()) + y;
        } while (board[index].get_type() != Cell::Type::Near);
        
        board = minesweeper.execute(x, y); 

        int counter=0;

        for(Cell cell : board)
          if (cell.is_visible()) counter++;

        Assert::That(counter,
                    Is().EqualTo(1));
      });

      it("execute a cell wicht state is Bomb execute all bombs cells", [&]() {
        int x, y, index;
        do{
          x = rand() % minesweeper.get_difficulty();
          y = rand() % minesweeper.get_difficulty();

          index = (x * minesweeper.get_difficulty()) + y;
        } while (!board[index].is_bomb());
        
        board = minesweeper.execute(x, y);

        int counter=0;

        for(Cell cell : board)
          if (cell.is_visible()) counter++;

        Assert::That(counter,
                    Is().EqualTo(minesweeper.get_number_of_bombs()));
      });
    });
  });
});

int main(int argc, char *argv[]) {

  return bandit::run(argc, argv);
}
