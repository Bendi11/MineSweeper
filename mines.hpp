#pragma once
#include <SDL2/SDL.h>
#include <time.h>
#include <random>
#include <iostream>
#include <vector>

/*----------TILE GRAPIHCS DEFINITIONS---------*/
#define EMPTY "tiles/empty.bmp"
#define HIDDEN "tiles/hidden.bmp"
#define ONE "tiles/one.bmp"
#define TWO "tiles/twoColor.bmp"
#define THREE "tiles/threeColor.bmp"
#define FOUR "tiles/fourColor.bmp"
#define FIVE "tiles/fiveColor.bmp"
#define SIX "tiles/sixColor.bmp"
#define FLAG "tiles/flag.bmp"
#define BOMB "tiles/bomb.bmp"

//Tile class for each tile in the map
class tile
{
	public:
	/*Booleans for the tile*/
	bool isMine;
	bool isFlagged;
	bool isHidden;
	bool isExplored = false;

	SDL_Texture* sprite;
	unsigned int neighborBombs;

	
};

//Minesweeper game class
class Mines
{
	private:
	//Constants defining the map size in tiles
	unsigned int MAP_SIZE_X = 50;
	unsigned int MAP_SIZE_Y = 50;

	unsigned int chance;

	//Ints defining the screen size in pixels
	unsigned int SCREEN_HEIGHT;
	unsigned int SCREEN_WIDTH;

	SDL_Event e; //User mouse event

	std::vector<SDL_Texture*> textures; //Vector of all textures that we load into memory

	public:

	//SDL objects
	SDL_Window* gameWin = NULL; //The window object 
	SDL_Renderer* render = NULL; //The renderer, which will render all sprites we copy to it

	bool running = true;

	bool lose = false;

	void findNeighbors(unsigned int x, unsigned int y); //Function to number the tiles based on neigbors

	bool reveal(unsigned int x, unsigned int y); //Function to reveal what the tile has on it
	
	SDL_Texture* loadBMP(std::string name); //Function to load a BMP to a texture

	std::vector<std::vector<tile> > map; //2D array of tiles that makes the map

	Mines(bool GPUaccel); //Class constructor for the Mines class, used for init SDL things mostly

	void renderUpdate(); //Function to display the map using the values changed during getInput

	void getInput(); //Function to get player clicks and see where they are, then adjust values accordingly

	void makeDebugMap(); //Function to make a test map for debugging

	void makeRandomMap(unsigned int numMines); //Function to make a pseudo-random layout of mines

	void revealAllHidden(unsigned int x, unsigned int y);

	void exploreAllHidden(); //Function to auto reveal empty tiles that have been discovered

};