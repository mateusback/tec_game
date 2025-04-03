#include "../../include/core/TextRenderer.h"
#include <iostream>

namespace Core {
    void TextRenderer::render(SDL_Renderer* renderer, TTF_Font* font, const std::string& text,
                            int x, int y, SDL_Color color) {
        if (!font) {
            std::cerr << "Fonte nula passada para TextRenderer!" << std::endl;
            return;
        }

        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
        if (!surface) {
            std::cerr << "Erro ao renderizar texto: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "Erro ao criar textura de texto: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }

        SDL_Rect dstRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}