#include "mines.hpp"

/*
g++ -std=c++17 main.cpp mines.cpp -Wall -lmingw32 -lSDL2main -mwindows -lm -lSDL2 -o main.exe
*/

/*
clang++-7 -pthread -std=c++17 -o main -lSDL2 -lSDL2main main.cpp mines.cpp && ./main
*/

int main(int argc, char* argv[]) 
{
	std::ofstream log("config/log.txt");
	minesweeper::Field f(&log);
	f.makeField(50, 50, 20);

	while(f.RUNNING)
	{
		f.renderUpdate();
		f.getInput();
		
	}

	SDL_DestroyWindow(f.win);
	SDL_DestroyRenderer(f.render);
	SDL_Quit();
	return 0;

}

