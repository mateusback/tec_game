#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace Core
{

    class TextRenderer 
    {
    public:
        static void render(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
                int x, int y, SDL_Color color = {255, 255, 255}, bool center = false);
    };
        
}
#endif
