#include "Sprite.h"
#include <iostream>

//sprite constructor, loads texture from file
Sprite::Sprite(SDL_Renderer* renderer, const std::string& filePath, int x, int y)
    : renderer(renderer)
{
    SDL_Surface* tmpSurface = IMG_Load(filePath.c_str());
    if (!tmpSurface)
    {
        std::cout << "Image load failed: " << IMG_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    //get full texture dimensions
    int texWidth = 0, texHeight = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &texWidth, &texHeight);

    SDL_FreeSurface(tmpSurface);

    //full texture used
    srcRect = { 0, 0, texWidth, texHeight };

    //position + full size
    destRect.x = x;
    destRect.y = y;
    destRect.w = texWidth;
    destRect.h = texHeight;
}
//delets sprite files from memory  
Sprite::~Sprite()
{
    SDL_DestroyTexture(texture);
}
//updates sprite each frame (not used here)
void Sprite::Update(int, int) {}
//renders the sprite to the screen with optional rotation
void Sprite::Render(float angle)
{
    if (texture)
    {
        SDL_RenderCopyEx(
            renderer,
            texture,
            nullptr,
            &destRect,
            angle,      // rotation in degrees
            nullptr,    // rotation center (nullptr = center of rect)
            SDL_FLIP_NONE
        );
    }
}
//sets sprite position on screen
void Sprite::setPosition(int x, int y)
{
    destRect.x = x;
    destRect.y = y;
}
//sets the size of the sprite
void Sprite::SetSize(int width, int height)
{
    destRect.w = width;
    destRect.h = height;
}
