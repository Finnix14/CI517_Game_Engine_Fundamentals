#pragma once
#include <SDL.h>
#include <iostream>
#include "Sprite.h"

class Engine
{
public:
    Engine();
    ~Engine();

    void Run();

private:
    bool Initialize();
    void ProcessInput();
    void Update();
    void Render();
    void Cleanup();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool isRunning = false;

    Sprite* testSprite = nullptr;
};
