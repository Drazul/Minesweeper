HEADERDIR=include tests
OBJECTFILES=game.o cell.o

CXX=g++ -std=c++11
CXXFLAGS = -ggdb $(foreach directory, $(HEADERDIR), -I$(directory))

all: minesweeper exec

exec:
	./minesweeper_test

minesweeper: $(OBJECTFILES)
		$(CXX)  $(CXXFLAGS) $(OBJECTFILES) tests/*.cpp -o minesweeper_test

clean:
	$(RM) *# *~ *.o minesweeper_test tests/*.o