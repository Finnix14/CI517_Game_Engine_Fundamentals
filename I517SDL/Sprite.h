#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

//handles image loading and rendering
class Sprite
{
public:
    //creates sprite and loads the texture
    Sprite(SDL_Renderer* renderer, const std::string& filePath, int x, int y);
    ~Sprite();

    void Update(int windowWidth, int windowHeight);

    void Render(float angle = 0.0f);
    void setPosition(int x, int y); //move sprite on screen
    void SetSize(int width, int height); //scale sprite

private:
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;   
    SDL_Rect srcRect = { 0, 0, 0, 0 }; //which part of image to draw
    SDL_Rect destRect = { 0, 0, 0, 0 }; //where to draw it on screen
};
