#include "minesweeper.h"
#include <iostream>
#include <vector>


void print(std::vector<char> v, int level) {
	std::cout << std::endl << "    0 1 2 3 4 5 6 7 8 9" << std::endl << std::endl;
	for(int i=0;i<level;i++){
		std::cout << i << "   ";
		for (int j=0;j<level;j++){
			std::cout<<v[(i*level)+j]<< " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


int main() {
	Minesweeper game;
	BoardState game_state;
	Difficulty difficulty = Difficulty::Easy;

	game.set_difficulty(difficulty);
	game.initialize();

	std::vector<char> board;
	int x, y, t;

	do{

		board = game.get_visible_board();
		print(board, difficulty);

		game_state = game.get_board_state();

		if(game_state == BoardState::GameOver){
			std::cout << "Game Over" << std::endl;
			break;
		}

		if(game_state == BoardState::Winner){
			std::cout << "YOU WON!" << std::endl;
			break;
		}

		std::cout << "Elige accion Flag (1) o Execute (0)" << std::endl;
		std::cin >> t;

		std::cout << "Introduce coordenada horizontal" << std::endl;
		std::cin >> x;
		std::cout << "Introduce coordenada vertical" << std::endl;
		std::cin >> y;

		//x = -1;
		if (x == -1 || y == -1) break;

		if (t == 1) game.put_flag(x, y);
		else				game.execute(x, y);

	}while (1);
	return 0;
}