#include "TextRenderer.h"
//Constructor, sets the current text to be rendered and gives it a font to find
TextRenderer::TextRenderer(SDL_Renderer* renderer, const std::string& fontPath, int fontSize)
{
    this->renderer = renderer;
    font = TTF_OpenFont(fontPath.c_str(), fontSize); //this finds the font path and converts it to a string

    if (!font)
        SDL_Log("Failed to load font: %s", TTF_GetError()); //throws an erro if no text is found
}
//Destructor
TextRenderer::~TextRenderer()
{
    if (font) //if a font exists then remove the font
        TTF_CloseFont(font);
}
//Render Text Method, takes in the message to be rendered, the x and y position and the color of the text
void TextRenderer::RenderText(const std::string& message, int x, int y, SDL_Color color)
{
	//create surface from font and message
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!surface)
    {
        SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
        return;
    }

	//create texture from surface and render it
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//define destination rectangle for rendering
    SDL_Rect dstRect = { x, y, surface->w, surface->h };

	//free surface after creating texture (cleanup memory)
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_DestroyTexture(texture);
}

