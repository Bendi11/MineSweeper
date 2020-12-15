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
	unsigned int boardSize;
	float density;
	
	/*Letting the user pick a size and difficulty*/
	SDL_MessageBoxButtonData buttons[] = 
	{
		{0, 0, "15x15"},
		{0, 1, "25x25"},
		{0, 2, "35x35"},
		{0, 2, "50x50"}

	};
	SDL_MessageBoxData sizeData = 
	{
		SDL_MESSAGEBOX_COLOR_TEXT,
		NULL,
		"Minesweeper",
		"Select a mine field size",
		SDL_arraysize(buttons),
		buttons,
		NULL
	};
	
	int chosenButton = 0;
	SDL_ShowMessageBox(&sizeData, &chosenButton); //Show the size selection box
	/*Set the size based on what button was pressed*/
	if(chosenButton == 0) boardSize = 15;
	else if(chosenButton == 1) boardSize = 25;
	else if(chosenButton == 2) boardSize = 35;
	else if(chosenButton == 3) boardSize = 50;

	/*Change the button data*/
	buttons[0] = {0, 0, "Easy"};
	buttons[1] = {0, 1, "Medium"};
	buttons[2] = {0, 2, "Hard"};
	buttons[3] = {0, 3, "Insane"};

	SDL_MessageBoxData difficultyData = 
	{
		SDL_MESSAGEBOX_COLOR_TEXT,
		NULL,
		"Minesweeper",
		"Select a difficulty",
		SDL_arraysize(buttons),
		buttons,
		NULL
	};
	SDL_ShowMessageBox(&difficultyData, &chosenButton); //Show difficulty select
	if(chosenButton == 0) density = 5;
	else if(chosenButton == 1) density = 10;
	else if(chosenButton == 2) density = 15;
	else if(chosenButton == 3) density = 25;

	f.makeField(boardSize, boardSize, density);

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

