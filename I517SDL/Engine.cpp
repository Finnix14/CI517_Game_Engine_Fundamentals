#include "Engine.h"
#include "InputManager.h"
#include "PlayerCharacter.h"
#include <SDL_ttf.h>

//add audio
#include <SDL_mixer.h>
InputManager inputManager;

//this engine structure mirros that of Davids SDL engine from CI517
//just adapted and modularised for readabilty in some classes

Engine::Engine() {}
Engine::~Engine() { Cleanup(); }

//initialise SDL systems, window and renderer
bool Engine::Initialise()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1)
    {
        std::cout << "Failed to init SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

	//initialize SDL_mixer
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    

    // Create the main window
    window = SDL_CreateWindow(
        GAME_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Engine initialized successfully.\n";
    isEngineRunning = true;


    return true;
}
//main loop
void Engine::Run()
{
    if (!Initialise()) return;

    Game game(renderer, &inputManager);

    Uint64 lastTick = SDL_GetTicks64();

    while (isEngineRunning)
    {
        Uint64 currentTick = SDL_GetTicks64();
		float deltaTime = (currentTick - lastTick) / 1000.0f; //convert ms to seconds
        lastTick = currentTick;

        ProcessInput();                 //SDL events only
        inputManager.Update();          //update current key states
        game.Update(deltaTime);


        SDL_RenderClear(renderer);
        game.Render();
        SDL_RenderPresent(renderer);

		inputManager.StorePreviousKeyStates();  //we update previous key states at end of frame
    }


    Cleanup();
}
//event polling / input handling
void Engine::ProcessInput()
{
	SDL_Event event; //event variable

	while (SDL_PollEvent(&event)) //if its type quit, set running to false
    {
        if (event.type == SDL_QUIT)
            isEngineRunning = false;

        inputManager.ProcessInput(event);
    }
}
//cleanup resources
void Engine::Cleanup()
{
    std::cout << "Cleaning up engine resources\n";

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

	//free audio resources
    Mix_FreeChunk(NULL);
	Mix_FreeMusic(NULL);
	Mix_CloseAudio();

    TTF_Quit();
    SDL_Quit();
    std::cout << "Engine shut down\n";
}
