#pragma once
#include <SDL.h>
#include <iostream>
#include "Game.h"

//core engine class, handles main loop and SDL systems
class Engine
{
public:
    Engine();
    ~Engine();

    //starts main loop
    void Run();

private:
    //core systems
    bool Initialise();
    void ProcessInput();
    void Cleanup();

    //SDL core components
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    bool isEngineRunning = false;

    //input events
    SDL_Event event = {};
};
