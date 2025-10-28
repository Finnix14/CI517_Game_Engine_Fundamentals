#include "Engine.h"
#include "InputManager.h"
#include "PlayerCharacter.h"
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

    Uint64 lastTick = SDL_GetTicks();

    while (isEngineRunning)
    {
        Uint64 currentTick = SDL_GetTicks();
        float deltaTime = (currentTick - lastTick) / 1000.0f;
        lastTick = currentTick;

        ProcessInput();
        game.Update(deltaTime);

        SDL_RenderClear(renderer);
        game.Render(); 
        SDL_RenderPresent(renderer);
    }

    Cleanup();
}
//event polling / input handling
void Engine::ProcessInput()
{
    SDL_Event event;

    //collect all events for this frame
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isEngineRunning = false;
        }

        //send event to input manager
        inputManager.ProcessInput(event);
    }

	//update input manager state
    inputManager.Update();

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

    SDL_Quit();
    std::cout << "Engine shut down\n";
}
