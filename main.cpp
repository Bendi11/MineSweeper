#include "mines.hpp"

/*
g++ -std=c++17 main.cpp mines.cpp -Wall -lmingw32 -lSDL2main -mwindows -lm -lSDL2 -o main.exe
*/

/*
clang++-7 -pthread -std=c++17 -o main -lSDL2 -lSDL2main main.cpp mines.cpp && ./main
*/

int main(int argc, char* argv[]) 
{
	Mines mine(0);
	mine.makeRandomMap(1);
	while(mine.running)
	{
		mine.renderUpdate();
		mine.getInput();
		
	}

	SDL_DestroyWindow(mine.gameWin);
	SDL_DestroyRenderer(mine.render);
	SDL_Quit();
	return 0;

}

