// =======================================================
// CI517
// David Dorrington UoBGames, 2025
// =======================================================
// Libraries to include
#include <SDL.h>
#include <iostream>
// =======================================================
int main(int argc, char* args[])
{
	printf("SDL test");
	SDL_Window* gameWindow = NULL;

	//Create window
	gameWindow = SDL_CreateWindow("CI517 - SDL Test", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 960, 540, SDL_WINDOW_SHOWN);

	//If a window is not created 
	if (gameWindow == NULL) printf(" Window Error %s\n", SDL_GetError());

	//after 5 seconds, quit the window
	SDL_Delay(5000);
	SDL_Quit;
	return 0;
}//---