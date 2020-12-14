#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <random>
#include <fstream>
#include <vector>

#define EMPTY 0
#define HIDDEN 10
#define BOMB 9
#define FLAG 11

#define TILE_HEIGHT 64
#define TILE_WIDTH 64

namespace minesweeper //Namespace containing all minesweeper classes, etc.
{
	//Tile class that holds each tile on the map's data
	class tile_t
	{
	public:
		bool isMine; //If the tile is a mine
		bool isRevealed; //If the tile has been revealed yet
		bool isFlagged = false; //If the tile is flagged as a bomb
		uint8_t adjacentMines; //How many mines are adjacent to the tile

	};

	//Object holding event flags
	class eventMask_t
	{
	public:
		bool MINE_REVEALED = false; //If user clicked a mine
		bool HAS_WON = false; //If the user won
		bool MENU_ENTERED = false; //If user entered a menu

	};

	//Minefield class for holding all tiles and receiving all input
	class Field
	{
	public:
		/*SDL2 things*/
		SDL_Window* win = NULL; //SDL2 window object
		SDL_Renderer* render = NULL; //SDL2 renderer object for copying our textures
		SDL_Event userE; //User event queue used to get user input
		bool RUNNING = true;

		/*Screen dimensions*/
		unsigned int SCREEN_HEIGHT; 
		unsigned int SCREEN_WIDTH;
		float SCALE; //How much to scale texture tile textures for the given screen dimensions

		std::ofstream* logger; //Log file object pointer

		eventMask_t events; //Event mask

		std::vector<SDL_Surface *> texturesList; //List of all loaded textures used so we don't need to load a new texture from HDD every frame
		std::vector< std::vector< SDL_Texture *> > textureField; //A 2D matrix of all loaded textures
		std::vector< std::vector< SDL_Rect > > posField; //A 2D matrix of all texture positions
		SDL_Rect size; //Size rectangle
		unsigned int NUM_MINES; //How many mines there are in the field

		std::vector< std::vector<tile_t> > mineField; //2D matrix of all tiles

		/*Functions*/
		Field(std::ofstream* _logger); //Function to make a new minefield object
		void loadCfg(std::string path); //Function to load a config file and set values

		void makeField(unsigned int H, unsigned int W, float density); //Function to make a new minefield with random mine locations

		void reveal(unsigned int x, unsigned int y); //Function to reveal a tile
		unsigned int findNeighbors(unsigned int x, unsigned int y); //Function to get the number of neighboard a mine has
		bool isValid(int x, int y); //Function to check if a tile is valid, as in is on the map

		void renderUpdate(); //Function to render the board onscreen

		void getInput(); //Function to get user input and update the minefield accordingly
	};
}