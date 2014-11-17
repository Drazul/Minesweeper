HEADERDIR=include tests
OBJECTFILES=minesweeper.o cell.o

CXX=g++ -std=c++11
CXXFLAGS = -ggdb $(foreach directory, $(HEADERDIR), -I$(directory))

all: clean minesweeper exec

exec:
	./minesweeper_test

minesweeper: $(OBJECTFILES)
		$(CXX)  $(CXXFLAGS) $(OBJECTFILES) tests/*.cpp -o minesweeper_test

main: clean $(OBJECTFILES)
		$(CXX)  $(CXXFLAGS) $(OBJECTFILES) main.cpp -o main
		
gdb:
	gdb minesweeper_test
	
clean:
	$(RM) *# *~ *.o minesweeper_test tests/*.o