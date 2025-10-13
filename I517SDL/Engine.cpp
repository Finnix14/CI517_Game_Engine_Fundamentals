#include "Engine.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

Engine::Engine() {}
Engine::~Engine() { Cleanup(); }

bool Engine::Initialize()
{
    //initialise SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! " << SDL_GetError() << std::endl;
        return false;
    }

    //create a window for rendering output
    window = SDL_CreateWindow("CI517 Engine System",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        960, 540, SDL_WINDOW_SHOWN);

    //renderer handles all drawing to the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //load a sprite to visualize the system running
    testSprite = new Sprite(renderer, "assets/fin.png", 128, 128);

    isRunning = true;
    return true;
}

void Engine::Run()
{
    //main engine loop
    if (!Initialize()) return;

    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
        SDL_Delay(16); //fixed 60 FPS loop
    }
}

void Engine::ProcessInput()
{
    //collects window and player input events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            isRunning = false;
    }
}

void Engine::Update()
{
    //update sprite logic for movment and bouncing on window
    testSprite->Update(960, 540);
}

void Engine::Render()
{
    //rendering pipeline, like clear, draw and present
    SDL_RenderClear(renderer);
    testSprite->Render();
    SDL_RenderPresent(renderer);
}

void Engine::Cleanup()
{
    //responsible resource management
    //frees allocated memory and SDL resources
    delete testSprite;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
