#include "mines.hpp"

using namespace minesweeper;

//Function to read over descriptor line and return actual value of cfg file
std::string readCfgLine(std::ifstream& inFile)
{
	std::string line; //String holding contents of read line
	std::getline(inFile, line); //Read over the descriptor line

	std::getline(inFile, line); //Read the actual value
	return line; //Return the read value
}

//Function to load a config file and set values
void Field::loadCfg(std::string path)
{
	std::ifstream cfgFile(path); //Open the cfg file
	if(!cfgFile.is_open()) //Make sure the config file exists
	{
		*logger<<"Config file couldn't be opened from "<<path<<std::endl; //Log the error
		exit(EXIT_FAILURE); //Quit the program
	}
	*logger<<"Config file opened from "<<path<<std::endl; //Log that the file was opened

	std::string line; //String holding contents of read value from CFG file

	line = readCfgLine(cfgFile); //Read screen width from the cfg file
	SCREEN_WIDTH = stoi(line); //Convert line to an integer and store in SCREEN_WIDTH

	line = readCfgLine(cfgFile); //Read screen height from the cfg file
	SCREEN_HEIGHT = stoi(line); //Convert line to an integer and store in SCREEN_HEIGHT

	/*Start loading the tile textures using paths from cfg file*/

	line = readCfgLine(cfgFile); //Load the empty tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded

	line = readCfgLine(cfgFile); //Load the one tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the two tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the three tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the four tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the five tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) ); //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the six tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the seven tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded

	line = readCfgLine(cfgFile); //Load the eight tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded

	line = readCfgLine(cfgFile); //Load the bomb tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) );  //Push the loaded texture back
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded


	line = readCfgLine(cfgFile); //Load the hidden tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) ); //Attempt to load hidden tile from path 
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded

	line = readCfgLine(cfgFile); //Load the flag tile path
	
	texturesList.push_back(IMG_Load(line.c_str()) ); //Attempt to load flag tile from path 
	*logger<<"Loaded texture from "<<line<<std::endl; //Log that the texture was loaded

}

//Function to make a new minefield object
Field::Field(std::ofstream* _logger)
{
	logger = _logger; //Point to the given log gile object
	*logger<<"Log file opened!"<<std::endl; //Print some text to the file to show we opened the log file good
	
	//Init SDL2 
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		*logger<<"SDL_Init failed with error: "<<SDL_GetError()<<std::endl; //Print an error message
		exit(EXIT_FAILURE); //Quit the program
	}
	*logger<<"SDL2 Init good!"<<std::endl; //Print that the init went good

	//Now try to init SDL_TTF library
	if(TTF_Init() < 0)
	{
		*logger<<"TTF_Init failed with error: "<<TTF_GetError()<<std::endl; //Let the user know that TTF init failed
		exit(EXIT_FAILURE); //Quit the application
	}
	*logger<<"TTF_Init good!"<<std::endl; //Print that TTF init was good

	//Finally init SDL2_Image library
	if(!IMG_Init(IMG_INIT_PNG))
	{
		*logger<<"IMG_Init failed with error: "<<IMG_GetError()<<std::endl; //Print that IMG could not be init
		exit(EXIT_FAILURE); //Quit the program
	}
	*logger<<"IMG_Init good!"<<std::endl; //Print that IMG_Init was good

	loadCfg("config/config.conf"); //Load the CFG file from path

	//Attempt to create SDL2 window object
	win = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
	if(!win) //Check if the window doesn't exist
	{
		*logger<<"SDL_CreateWindow failed with error: "<<SDL_GetError()<<std::endl; //Print that SDL2 window couldn't be created
		exit(EXIT_FAILURE); //Quit the program
	}
	*logger<<"Window created with width "<<SCREEN_WIDTH<<" and height "<<SCREEN_HEIGHT<<std::endl; //Print that the window was created good

	render = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE); //Create a software renderer in SDL2
	if(!render) //Checking if the renderer creation failed
	{
		*logger<<"SDL_CreateRenderer failed with error: "<<SDL_GetError()<<std::endl; //Print an error message
		exit(EXIT_FAILURE); //Quit the program
	}
	*logger<<"SDL renderer created!"<<std::endl; //Print that we could make the window
	
	sans = TTF_OpenFont("config/FreeSans.ttf", 12); //Open the free sans font
	if(!sans) //Check if the font opened good
	{
		*logger<<"Couldn't open font: "<<TTF_GetError()<<std::endl; //Log the error
		exit(EXIT_FAILURE); //Quit the program
	}

	SDL_SetRenderDrawColor(render, 255, 255, 255, 255); //Set the clear color to white
	/*Set universal size rectangle*/
	size.x = 0;
	size.y = 0;
	size.w = TILE_WIDTH;
	size.h = TILE_HEIGHT;

}

//Function to make a minefield with specified height and width in tiles
void Field::makeField(unsigned int H, unsigned int W, float density)
{
	/*Reset values*/
	DENSITY = density;
	REVEALED_TILES = 0;
	NUM_MINES = 0;
	SDL_SetWindowSize(win, (SCREEN_WIDTH / W) * W, (SCREEN_HEIGHT / H) * H);
	SCREEN_HEIGHT = (SCREEN_HEIGHT / H) * H;
	SCREEN_WIDTH = (SCREEN_WIDTH / W) * W;
	mineField.clear();
	textureField.clear();
	posField.clear();

	SCALE = (SCREEN_HEIGHT / H) / TILE_HEIGHT; //Set the scale factor so tiles appear the right size
	srand(time(NULL)); //Randomly seed the random number generator based on the current time
	
	mineField.resize(W); //Resize matrices Y
	textureField.resize(W);
	posField.resize(W);

	for(unsigned n = 0; n < W; ++n)
	{
		mineField[n].resize(H);
		textureField[n].resize(H);
		posField[n].resize(H);
	}

	//Iterate through each tile
	for(unsigned x = 0; x < W; ++x)
	{
		for(unsigned y = 0; y < H; ++y)
		{
			//If random number generator decides the tile is a bomb, the tile is a bomb
			if(rand() % 100 < density)
			{
				mineField[x][y].isMine = true; 
				NUM_MINES++;
			}
		}
	}

	//Iterate through each tile
	for(unsigned x = 0; x < W; ++x)
	{
		for(unsigned y = 0; y < H; ++y)
		{
			findNeighbors(x, y); //Count up neighbors of the mine
			textureField[x][y] = SDL_CreateTextureFromSurface(render, texturesList[10]); //Set the tile texture to hidden
			/*Set texture position*/
			posField[x][y].x = x * (SCREEN_HEIGHT / H);
			posField[x][y].y = y * (SCREEN_WIDTH / W);
			/*Set texture size*/
			posField[x][y].w = (SCREEN_WIDTH / W);
			posField[x][y].h = (SCREEN_HEIGHT / H);

		}
	}
}

//Function to check if a tile is out of bounds or not
bool Field::isValid(int x, int y)
{
	//Check if the tile is out of bounds and return true if it is
	return ( (y > -1) && (y < mineField[0].size() ) && (x > -1) && (x < mineField.size()) );
		   
}

//Function to find the neighbor mines of a tile
unsigned int Field::findNeighbors(unsigned int _x, unsigned int _y)
{
	int x = _x;
	int y = _y;
	/*
	(x-1, y+1)	(x, y+1)	(x+1,y+1)

	(x-1, y)	(x, y)		(x+1,y)

	(x-1, y-1)	(x, y-1)	(x+1, y-1)
	*/
	/*Count adjacent mine of the tile*/
	if(isValid(x - 1, y + 1)) if(mineField[x - 1][y + 1].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x, y + 1)) if(mineField[x][y + 1].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x + 1, y + 1)) if(mineField[x + 1][y + 1].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x - 1, y)) if(mineField[x - 1][y].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x + 1, y)) if(mineField[x + 1][y].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x - 1, y - 1)) if(mineField[x - 1][y - 1].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x, y - 1)) if(mineField[x][y - 1].isMine) mineField[x][y].adjacentMines++;
	if(isValid(x + 1, y - 1)) if(mineField[x + 1][y - 1].isMine) mineField[x][y].adjacentMines++;
	return mineField[x][y].adjacentMines;
}

//Function to reveal a tile
void Field::reveal(unsigned int __x, unsigned int __y)
{
	int x = __x;
	int y = __y;
	if(mineField[x][y].isRevealed)
	{
		return;
	}
	SDL_DestroyTexture(textureField[x][y]); //Destroy the loaded texture

	if(mineField[x][y].isMine)
	{
		textureField[x][y] = SDL_CreateTextureFromSurface(render, texturesList[9]); //Set the texture to a mine
		events.MINE_REVEALED = true; //Set that we revealed a mine
		return; //Exit the function
	}
	//If the minefield tile is empty, reveal all of its neighbors
	if(!mineField[x][y].isRevealed && mineField[x][y].adjacentMines == 0)
	{
		textureField[x][y] = SDL_CreateTextureFromSurface(render, texturesList[EMPTY]);
		mineField[x][y].isRevealed = true;
		REVEALED_TILES++; //Increase how many tiles are revealed
		//Iterate through all neighbors and reveal them
		/*
		(x-1, y+1)	(x, y+1)	(x+1,y+1)

		(x-1, y)	(x, y)		(x+1,y)

		(x-1, y-1)	(x, y-1)	(x+1, y-1)
		*/
		if(isValid(x - 1, y + 1)) reveal(x - 1, y + 1);
		if(isValid(x, y + 1)) reveal(x, y + 1);
		if(isValid(x + 1, y + 1)) reveal(x + 1, y + 1);
		if(isValid(x - 1, y)) reveal(x - 1, y);
		if(isValid(x + 1, y)) reveal(x + 1, y);
		if(isValid(x - 1, y - 1)) reveal(x - 1, y - 1);
		if(isValid(x, y - 1)) reveal(x, y - 1);
		if(isValid(x + 1, y - 1)) reveal(x + 1, y - 1);
		
		return;
	}
	textureField[x][y] = SDL_CreateTextureFromSurface(render, texturesList[mineField[x][y].adjacentMines]); //Set the texture of the tile to the right value
	mineField[x][y].isRevealed = true;
	REVEALED_TILES++; //Increase how many tiles are revealed

}

//Function to reveal all non flagged tiles around a tile
void Field::revealNotFlagged(unsigned int _x, unsigned int _y)
{
	int x = _x;
	int y = _y;
	if(isValid(x - 1, y + 1)) if(!mineField[x - 1][y + 1].isFlagged) reveal(x - 1, y + 1);
	if(isValid(x, y + 1)) if(!mineField[x][y + 1].isFlagged) reveal(x, y + 1);
	if(isValid(x + 1, y + 1)) if(!mineField[x + 1][y + 1].isFlagged) reveal(x + 1, y + 1);
	if(isValid(x - 1, y)) if(!mineField[x - 1][y].isFlagged) reveal(x - 1, y);
	if(isValid(x + 1, y)) if(!mineField[x + 1][y].isFlagged) reveal(x + 1, y);
	if(isValid(x - 1, y - 1)) if(!mineField[x - 1][y - 1].isFlagged) reveal(x - 1, y - 1);
	if(isValid(x, y - 1)) if(!mineField[x][y - 1].isFlagged) reveal(x, y - 1);
	if(isValid(x + 1, y - 1)) if(!mineField[x + 1][y - 1].isFlagged) reveal(x + 1, y - 1);
}

//Function to get user input
void Field::getInput()
{
	while(SDL_PollEvent(&userE)) //Poll through all events in the queue
	{
		if(userE.type == SDL_QUIT)
		{
			RUNNING = false; //Quit the program
		}
		else if(userE.type == SDL_MOUSEBUTTONUP) //If the user clicked on a tile
		{
			if(userE.button.button == SDL_BUTTON_LEFT) //If the user wants to reveal a tile
			{
				unsigned int mX = (unsigned int)userE.motion.x / (SCREEN_WIDTH / mineField.size());
				unsigned int mY = (unsigned int)userE.motion.y / (SCREEN_HEIGHT / mineField[0].size());

				if(!mineField[mX][mY].isFlagged) reveal(mX, mY); //Reveal the tile if it isn't flagged as a bomb
			}
			else if(userE.button.button == SDL_BUTTON_RIGHT) //If the user wants to flag a tile
			{
				unsigned int mX = (unsigned int)userE.motion.x / (SCREEN_WIDTH / mineField.size());
				unsigned int mY = (unsigned int)userE.motion.y / (SCREEN_HEIGHT / mineField[0].size());

				if(!mineField[mX][mY].isFlagged) 
				{
					mineField[mX][mY].isFlagged = true; //Flag the tile
					textureField[mX][mY] = SDL_CreateTextureFromSurface(render, texturesList[11]);
				}
				else
				{
					mineField[mX][mY].isFlagged = false; //Unflag the tile
					if(mineField[mX][mY].isRevealed) textureField[mX][mY] = SDL_CreateTextureFromSurface(render, texturesList[mineField[mX][mY].adjacentMines]);
					else textureField[mX][mY] = SDL_CreateTextureFromSurface(render, texturesList[HIDDEN]);
				}
				
			}
			else if(userE.button.button == SDL_BUTTON_MIDDLE) //If the user is revealing all unflagged neighbors
			{
				unsigned int mX = (unsigned int)userE.motion.x / (SCREEN_WIDTH / mineField.size());
				unsigned int mY = (unsigned int)userE.motion.y / (SCREEN_HEIGHT / mineField[0].size());

				revealNotFlagged(mX, mY); //Reveal all non flagged tiles
			}
			
		}
	}
}

//Function to render the game
void Field::renderUpdate()
{
	SDL_Delay(13);
	SDL_RenderClear(render); //Clear the renderer of loaded textures

	for(unsigned x = 0; x < mineField.size(); ++x)
	{
		for(unsigned y = 0; y < mineField[0].size(); ++y)
		{
			SDL_RenderCopy(render, textureField[x][y], &size, &posField[x][y]);
		}
	}

	SDL_RenderPresent(render); //Present the field
}

//Function to get flags and see if any have been updated
void Field::flagUpdate()
{
	SDL_Surface * textSurface; //Surface for text
	SDL_Color txtCol = {0, 0, 0}; //Text color
	SDL_Rect pos;
	pos.y = SCREEN_HEIGHT / 2;

	if(events.MINE_REVEALED)
	{
		SDL_Delay(1000); 
		events.MINE_REVEALED = false;
		
		SDL_RenderClear(render); //Clear the renderer

		textSurface = TTF_RenderText_Shaded(sans, "You lost!", txtCol, {255, 255, 255});
		pos.x = (SCREEN_WIDTH / 2) - (textSurface->w / 2);
		pos.w = (textSurface->w);
		pos.h = TTF_FontHeight(sans); //Get font size

		SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, textSurface), NULL, &pos);
		SDL_RenderPresent(render);
		SDL_Delay(1000);
		makeField(mineField.size(), mineField[0].size(), DENSITY);
		SDL_FreeSurface(textSurface);
	}

	//Check if the user revealed all tiles
	if( (mineField.size() * mineField[0].size()) - REVEALED_TILES == NUM_MINES)
	{
		SDL_Delay(1000); 
		SDL_RenderClear(render); //Clear the renderer

		textSurface = TTF_RenderText_Shaded(sans, "You won!", txtCol, {255, 255, 255});
		pos.x = (SCREEN_WIDTH / 2) - (textSurface->w / 2);
		pos.w = (textSurface->w);
		pos.h = TTF_FontHeight(sans); //Get font size

		SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render, textSurface), NULL, &pos);
		SDL_RenderPresent(render);
		SDL_Delay(1000);
		makeField(mineField.size(), mineField[0].size(), DENSITY);
		SDL_FreeSurface(textSurface); 

	}
}

