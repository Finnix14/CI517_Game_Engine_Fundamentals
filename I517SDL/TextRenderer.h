#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextRenderer {
public:
    TextRenderer(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    ~TextRenderer();

    void RenderText(const std::string& message, int x, int y, SDL_Color color);

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
};
