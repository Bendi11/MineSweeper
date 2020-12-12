#include "mines.hpp"

/*
TODO:
Make neighbor bomb detection work on outer rim of the grid
*/

//Function to load a BMP to an SDL_Texture
SDL_Texture* Mines::loadBMP(std::string name)
{
	SDL_Texture* loadedText = NULL;

	//Loading the BMP to a surface, then converting the surface to a texture
	loadedText = SDL_CreateTextureFromSurface(this->render, SDL_LoadBMP(name.c_str()));
	//Making sure the texture loaded correctly
	if(loadedText == NULL)
	{
		//std::cout<<"Error Loading BMP From: "<<name<<std::endl;
		exit(1);
	}
	//std::cout<<"Loaded Texture: "<<name<<" Good!\n";

	return loadedText;

	SDL_DestroyTexture(loadedText);
}

//Mines class constructor
Mines::Mines(bool GPUaccel)
{
	srand(rand()); //Randomly seeding the random # generator

	const SDL_MessageBoxButtonData buttons[] = {
	/*Flags, buttonid, text*/
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "10 / 100"},
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "15 / 100"},
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 3, "20 / 100"}
	};

	const SDL_MessageBoxColorScheme colors = {
		{
			/* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			{ 255,   255,   255 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */
			{   0, 0,   0 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			{ 60, 60,   60 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			{   230,   230, 230 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			{ 210,   210, 210 }
		}
	};

	const SDL_MessageBoxData msg = 
	{
		SDL_MESSAGEBOX_INFORMATION,
		this->gameWin,
		"Minesweeper",
		"Select a difficulty in mines per 100 squares",
		SDL_arraysize(buttons),
		buttons,
		&colors
	};

	int buttonID;
	SDL_ShowMessageBox(&msg, &buttonID);
	switch(buttonID)
	{
		case 1:
		this->chance = 10;
		break;

		case 2: 
		this->chance = 15;
		break;

		case 3:
		this->chance = 20;
		break;
	}

	const SDL_MessageBoxButtonData sizeB[] = {
	/*Flags, buttonid, text*/
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "20x20"},
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "50x50"},
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 3, "30x30"},
	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 4, "25x25"}
	};

	const SDL_MessageBoxData sizeMsg = 
	{
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		"Minesweeper",
		"Select size in tiles",
		SDL_arraysize(sizeB),
		sizeB,
		&colors
	};

	SDL_ShowMessageBox(&sizeMsg, &buttonID);
	switch(buttonID)
	{
		case 1:
		this->MAP_SIZE_X = 20;
		this->MAP_SIZE_Y = 20;
		break;

		case 2: 
		this->MAP_SIZE_X = 50;
		this->MAP_SIZE_Y = 50;
		break;

		case 3:
		this->MAP_SIZE_X = 30;
		this->MAP_SIZE_Y = 30;
		break;

		case 4:
		this->MAP_SIZE_X = 25;
		this->MAP_SIZE_Y = 25;
		break;
	}


	//Setting the screen size in pixels based on the number of tiles
	this->SCREEN_HEIGHT = this->MAP_SIZE_Y * 16;
	this->SCREEN_WIDTH = this->MAP_SIZE_X * 16;

	//Init SDL Video
	//SDL_Init returns 0 if it succeeded, and an int if it failed, so we only have to check if SDL_Init doesn't return 0
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		//std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
		exit(1);
	} 

	//std::cout<<"SDL_Init Good!\n";


	//Creating our window
	this->gameWin = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->SCREEN_WIDTH,this->SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Print an error message if our window failed to create
	if(this->gameWin == NULL)
	{
		//std::cout<<"CreateWindow Error: "<<SDL_GetError()<<std::endl;
		exit(1);
	}
	//std::cout<<"Window Created\n";

	//Creating our renderer based on the GPUaccel argument
	if(GPUaccel)
	{
		//Use GPU to render if the bool is true
		this->render = SDL_CreateRenderer(this->gameWin, -1, SDL_RENDERER_ACCELERATED);
		//std::cout<<"Renderer created with GPU acceleration!\n";
	}
	else 
	{
		//Use CPU for rendering if the bool is false
		this->render = SDL_CreateRenderer(this->gameWin, -1, SDL_RENDERER_SOFTWARE);
		//std::cout<<"Renderer created without GPU acceleration!\n";
	}

	//Resizing the textures vector
	this->textures.resize(10);

	/*Load one copy of each texture to memory*/
	this->textures[0] = this->loadBMP(EMPTY);
	this->textures[1] = this->loadBMP(HIDDEN);
	this->textures[2] = this->loadBMP(ONE);
	this->textures[3] = this->loadBMP(TWO);
	this->textures[4] = this->loadBMP(THREE);
	this->textures[5] = this->loadBMP(FOUR);
	this->textures[6] = this->loadBMP(FIVE);
	this->textures[7] = this->loadBMP(SIX);
	this->textures[8] = this->loadBMP(FLAG);
	this->textures[9] = this->loadBMP(BOMB);

	//Resizing the map matrix to the specified size
	this->map.resize(this->MAP_SIZE_Y);
	for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
	{
		this->map[y].resize(this->MAP_SIZE_X);
	}




}

//Function to render all tiles
void Mines::renderUpdate()
{
	//The position of a tile is determined by an SDL_Rect
	SDL_Rect pos;
	pos.w = 16;
	pos.h = 16;


	//Iterate through each tile in the map matrix
	for(unsigned x = 0; x < this->MAP_SIZE_X; ++x)
	{
		for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
		{
			pos.x = x * 16;
			pos.y = y * 16;
			if(this->map[x][y].isFlagged)
			{
				SDL_DestroyTexture(this->map[x][y].sprite);
				this->map[x][y].sprite = this->loadBMP(FLAG);
			}

			//Copy the tile's sprite to the renderer
			SDL_RenderCopy(this->render, this->map[x][y].sprite, NULL, &pos);
		}
	}

	SDL_RenderPresent(this->render);
}

//Function to make a test map for debugging
void Mines::makeDebugMap()
{
	this->map[5][5].isMine = true;
	for(unsigned x = 0; x < this->MAP_SIZE_X; ++x)
	{
		for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
		{
			if(x != 0 && y != 0)
			{
				this->findNeighbors(x, y);
			}
			this->map[x][y].sprite = this->loadBMP(HIDDEN);
		}
	}
	
}

bool Mines::reveal(unsigned int x, unsigned int y)
{
	SDL_DestroyTexture(this->map[x][y].sprite);
	this->map[x][y].isHidden = false;

	if(this->map[x][y].isMine)
	{
		this->map[x][y].sprite = this->loadBMP(BOMB);
		return true; //Return true if user just clicked a mine
	}
	else 
	{
		//Destroying the current texture to prevent a memory leak	
		switch(this->map[x][y].neighborBombs)
		{
			case 0:
				this->map[x][y].sprite = this->loadBMP(EMPTY);
				if((x != 0 && y != 0) && (x != this->MAP_SIZE_X - 1 && y != this->MAP_SIZE_Y - 1) )
				{
					//this->exploreAllHidden();
					for(int mX = -1; mX <= 1; ++mX)
					{
						for(int mY = -1; mY <= 1; ++mY)
						{
							if(this->map[x + mX][y + mY].neighborBombs == 0)
							{
								SDL_DestroyTexture(this->map[x + mX][y + mY].sprite);
								this->map[x + mX][y + mY].sprite = this->loadBMP(EMPTY);
							}
							else
							{
								SDL_DestroyTexture(this->map[x + mX][y + mY].sprite);
								switch(this->map[x + mX][y + mY].neighborBombs)
								{
									case 1:
										this->map[x + mX][y + mY].sprite = this->loadBMP(ONE);
									break;

									case 2:
										this->map[x + mX][y + mY].sprite = this->loadBMP(TWO);
									break;

									case 3:
										this->map[x + mX][y + mY].sprite = this->loadBMP(THREE);
									break;

									case 4:
										this->map[x + mX][y + mY].sprite = this->loadBMP(FOUR);
									break;

									case 5:
										this->map[x + mX][y + mY].sprite = this->loadBMP(FIVE);
									break;

									case 6:
										this->map[x + mX][y + mY].sprite = this->loadBMP(SIX);
									break;
								}
							}
							
						}
					}
				}
			break;

			case 1:
				this->map[x][y].sprite = this->loadBMP(ONE);
			break;

			case 2:
				this->map[x][y].sprite = this->loadBMP(TWO);
			break;

			case 3:
				this->map[x][y].sprite = this->loadBMP(THREE);
			break;

			case 4:
				this->map[x][y].sprite = this->loadBMP(FOUR);
			break;

			case 5:
				this->map[x][y].sprite = this->loadBMP(FIVE);
			break;

			case 6:
				this->map[x][y].sprite = this->loadBMP(SIX);
			break;
		}
		return false;
	}
}

//Function for getting all input events
void Mines::getInput()
{
	if(this->lose == true)
	{
		this->makeRandomMap(1);
		this->lose = false;
		SDL_Delay(500);
		return;
	}
	/*
	Explanation: SDL Input
	SDL handles all user input from keyboard and mouse, even joysticks, in an event queue 
	This function cycles through each event in the queue and checks if they are a mouse event

	*/

	unsigned int mX;
	unsigned int mY;

	SDL_WaitEvent(&this->e);
	if(e.type == SDL_QUIT)
	{
		this->running = false;
	}
	mX = this->e.button.x / 16;
	mY = this->e.button.y / 16;

	if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
	{

		if(this->e.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
		{	
			if(!this->map[mX][mY].isFlagged)
			{
				if(this->reveal(mX, mY))
				{
					this->lose = true;
				}
			}
		}
		if(this->e.button.button == SDL_BUTTON_RIGHT)
		{
			if(!this->map[mX][mY].isFlagged)
			{
				this->map[mX][mY].isFlagged = true;
				SDL_DestroyTexture(this->map[mX][mY].sprite);
				this->map[mX][mY].sprite = this->loadBMP(FLAG);
			}
			else 
			{
				this->map[mX][mY].isFlagged = false;
				SDL_DestroyTexture(this->map[mX][mY].sprite);
				this->map[mX][mY].sprite = this->loadBMP(HIDDEN);
			}
				
		}

	}


}

void Mines::findNeighbors(unsigned int x, unsigned int y)
{
	if( (x != this->MAP_SIZE_X-1 && y != this->MAP_SIZE_Y-1) && (x != 0 && y != 0) )
	{
		for(int nX = -1; nX < 2; ++nX)
		{
			for(int nY = -1; nY < 2; ++nY)
			{
				//Adding to the number of mines as neighbors
				this->map[x][y].neighborBombs += (unsigned int)this->map[x + nX][y + nY].isMine;
			}
		}
	}
	else if(x == 0 && (y != 0 && y != this->MAP_SIZE_Y - 1)) //If on the furthest left
	{
		for(int nX = 0; nX < 2; ++nX)
		{
			for(int nY = -1; nY < 2; ++nY)
			{
				//Adding to the number of mines as neighbors
				this->map[x][y].neighborBombs += (unsigned int)this->map[x + nX][y + nY].isMine;
			}
		}
	}
	else if(x == this->MAP_SIZE_X - 1 && (y != 0 && y != this->MAP_SIZE_Y - 1)) //If on the furthest right
	{
		for(int nX = -1; nX < 1; ++nX)
		{
			for(int nY = -1; nY < 2; ++nY)
			{
				//Adding to the number of mines as neighbors
				this->map[x][y].neighborBombs += (unsigned int)this->map[x + nX][y + nY].isMine;
			}
		}
	}
	else if((x != 0 && x != this->MAP_SIZE_X - 1) && y == this->MAP_SIZE_Y - 1) //If on the furthest down
	{
		for(int nX = -1; nX < 2; ++nX)
		{
			for(int nY = -1; nY < 1; ++nY)
			{
				//Adding to the number of mines as neighbors
				this->map[x][y].neighborBombs += (unsigned int)this->map[x + nX][y + nY].isMine;
			}
		}
	}
	else if((x != 0 && x != this->MAP_SIZE_X - 1) && y == 0) //If on the furthest up
	{
		for(int nX = -1; nX < 2; ++nX)
		{
			for(int nY = 0; nY < 2; ++nY)
			{
				//Adding to the number of mines as neighbors
				this->map[x][y].neighborBombs += (unsigned int)this->map[x + nX][y + nY].isMine;
			}
		}
	}

}

void Mines::makeRandomMap(unsigned int numMines)
{
	double r;
	srand(time(NULL));

	for(unsigned x = 0; x < this->MAP_SIZE_X; ++x)
	{
		for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
		{
			this->map[x][y].isMine = false;
			this->map[x][y].isFlagged = false;
			this->map[x][y].neighborBombs = 0;
			SDL_DestroyTexture(this->map[x][y].sprite);
			this->map[x][y].isHidden = true;
		}
	}
	for(unsigned x = 0; x < this->MAP_SIZE_X; ++x)
	{
		for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
		{
			r = rand() % 100;

			if(r > 100 - this->chance)
			{
				//if(x != 0 && y != 0)
				//{
					this->map[x][y].isMine = true;
				//}
			}
		}
	}

	for(unsigned x = 0; x < this->MAP_SIZE_X; ++x)
	{
		for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
		{
			//if(x != 0 && y != 0)
			//{
				this->findNeighbors(x, y);
			//}
			this->map[x][y].sprite = this->loadBMP(HIDDEN);
		}
	}
}

void Mines::exploreAllHidden()
{
	for(unsigned x = 0; x < this->MAP_SIZE_X; ++x)
	{
		for(unsigned y = 0; y < this->MAP_SIZE_Y; ++y)
		{
			if(this->map[x][y].neighborBombs == 0 && this->map[x][y].isMine == false && this->map[x][y].isHidden == false && !(this->map[x][y].isExplored))
			{
				if((x != 0 && y != 0) && (x != this->MAP_SIZE_X - 1 && y != this->MAP_SIZE_Y - 1) )
				{
					for(int mX = -1; mX <= 1; ++mX)
					{
						for(int mY = -1; mY <= 1; ++mY)
						{
							if(this->map[x + mX][y + mY].neighborBombs == 0)
							{
								SDL_DestroyTexture(this->map[x + mX][y + mY].sprite);
								this->map[x + mX][y + mY].sprite = this->loadBMP(EMPTY);
							}
							else
							{
								SDL_DestroyTexture(this->map[x + mX][y + mY].sprite);
								switch(this->map[x + mX][y + mY].neighborBombs)
								{
									case 1:
										this->map[x + mX][y + mY].sprite = this->loadBMP(ONE);
									break;

									case 2:
										this->map[x + mX][y + mY].sprite = this->loadBMP(TWO);
									break;

									case 3:
										this->map[x + mX][y + mY].sprite = this->loadBMP(THREE);
									break;

									case 4:
										this->map[x + mX][y + mY].sprite = this->loadBMP(FOUR);
									break;

									case 5:
										this->map[x + mX][y + mY].sprite = this->loadBMP(FIVE);
									break;

									case 6:
										this->map[x + mX][y + mY].sprite = this->loadBMP(SIX);
									break;
								}
							}
							this->map[x + mX][y + mY].isHidden = false;
							
						}
					}
				}
			}
		}
	}
}
