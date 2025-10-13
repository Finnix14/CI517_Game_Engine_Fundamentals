#include "Sprite.h"
#include <iostream>


Sprite::Sprite(SDL_Renderer* renderer, const std::string& filePath, int w, int h)
    : renderer(renderer)
{

    // load a texture into memory from file
    SDL_Surface* tmpSurface = IMG_Load(filePath.c_str());
    if (!tmpSurface)
        std::cout << "Image load failed: " << IMG_GetError() << std::endl;


    // create image
    texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);


    //define sprite rectangle 
    rect = { 100, 100, w, h };
}

//destructor, destroys Texture when closed
Sprite::~Sprite()
{
    SDL_DestroyTexture(texture);
}

void Sprite::Update(int windowWidth, int windowHeight)
{
    rect.x += velX;
    rect.y += velY;

    //when reach screen edge, reverse velocity
    if (rect.x <= 0 || rect.x + rect.w >= windowWidth) velX = -velX;
    if (rect.y <= 0 || rect.y + rect.h >= windowHeight) velY = -velY;
}

void Sprite::Render()
{
    //draw texture to screen using, current position and size
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
