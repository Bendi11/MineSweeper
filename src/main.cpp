#include "mines.hpp"

/*
g++ -std=c++17 src/main.cpp src/mines.cpp -Isrc/include -lmingw32 -lSDL2main -mwindows -lm -lSDL2 -lSDL2_image -lSDL2_ttf -o main.exe icon.o -O3
*/

/*
clang++-7 -pthread -std=c++17 -o main -lSDL2 -lSDL2main main.cpp mines.cpp && ./main
*/

int main(int argc, char* argv[]) 
{
	std::ofstream log("config/log.txt");
	minesweeper::Field f(&log);
	f.makeField(15, 15, 10);

	while(f.RUNNING)
	{
		f.getInput();
		f.renderUpdate();
		f.flagUpdate();
		
	}

	SDL_DestroyWindow(f.win);
	SDL_DestroyRenderer(f.render);
	SDL_Quit();
	return 0;

}

