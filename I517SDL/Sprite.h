#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Sprite
{
public:
    Sprite(SDL_Renderer* renderer, const std::string& filePath, int w, int h);
    ~Sprite();

    void Update(int windowWidth, int windowHeight);
    void Render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    int velX = 3;
    int velY = 3;
};
