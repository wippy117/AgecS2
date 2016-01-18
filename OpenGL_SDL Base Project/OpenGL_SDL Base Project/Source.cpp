#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Constants.h"
#include "Commons.h"
#include "GameScreenManager.h"

using namespace::std;

//----------------------------------------------------------------------------

bool InitSDL();
SDL_Surface* LoadSurface(string path);
void		 LoadMusic(string path);
void CloseSDL();

void Render();
bool Update();

//----------------------------------------------------------------------------

SDL_Window*   gWindow    = NULL;
SDL_GLContext gGLContext = NULL;
SDL_Surface*  gSurface   = NULL;
Mix_Music*	  gMusic	 = NULL;
Uint32		  gOldTime;

GameScreenManager* gameScreenManager;

//----------------------------------------------------------------------------

int main(int argc, char* args[])
{
	if(InitSDL())
	{
		gameScreenManager = new GameScreenManager(SCREEN_LEVEL1);
	
		bool quit = false;
		gOldTime = SDL_GetTicks();

		while(!quit)
		{
			Render();
			quit = Update();
		}	
	}
	CloseSDL();

	return 0;
}

//----------------------------------------------------------------------------

bool InitSDL()
{	
	//Setup SDL.
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	else
	{
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			cout << "Warning: Linear texture filtering not available";
		}

		gWindow = SDL_CreateWindow("Advanced Game Engine Creation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		gGLContext = SDL_GL_CreateContext(gWindow);
		if(gWindow != NULL)
		{

		}
		else
		{
			cout << "Window was not created. Error: " << SDL_GetError();
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------

SDL_Surface* LoadSurface(string path)
{
	SDL_Surface* pLoadedSurface = NULL;
	SDL_Surface* pOptimizedSurface = NULL;

	pLoadedSurface = IMG_Load(path.c_str());
	if(pLoadedSurface == NULL)
	{
		cout << "Failed to load image. Error: " << SDL_GetError();
		return NULL;
	}
	else
	{
		pOptimizedSurface = SDL_ConvertSurface( pLoadedSurface, gSurface->format, NULL);
		if(pOptimizedSurface == NULL)
		{
			cout << "Unable to optimize the surface. Error: " << SDL_GetError();
			return NULL;
		}
		SDL_FreeSurface(pLoadedSurface);
	}

	return pOptimizedSurface;
}

//----------------------------------------------------------------------------

void LoadMusic(string path)
{
	gMusic = Mix_LoadMUS(path.c_str());
	if(gMusic == NULL)
	{
		cout << "Failed to load background music! Error: " << Mix_GetError() << endl;
	}
}

//----------------------------------------------------------------------------

void CloseSDL()
{
	delete gameScreenManager;
	gameScreenManager = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_GL_DeleteContext(gGLContext);
	gGLContext = NULL;

	Mix_FreeMusic(gMusic);
	gMusic = NULL;

	IMG_Quit();
	SDL_Quit();
}

//----------------------------------------------------------------------------

void Render()
{
	gameScreenManager->Render();

	SDL_GL_SwapWindow(gWindow);
}

//----------------------------------------------------------------------------

bool Update()
{
	Uint32 newTime = SDL_GetTicks();
	SDL_Event e;
	SDL_PollEvent(&e);

	switch(e.type)
	{
		case SDL_QUIT:
			return true;
		break;

		default:
			gameScreenManager->Update((float)(newTime-gOldTime)/1000.0f, e);
		break;
	}
	gOldTime = newTime;

	return false;
}

//----------------------------------------------------------------------------