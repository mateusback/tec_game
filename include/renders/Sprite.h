#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>

namespace Renderer {
    class Sprite {
    private:
        SDL_Texture* texture = nullptr;
        SDL_Rect srcRect{};

    public:
        Sprite(SDL_Texture* texture, const SDL_Rect& srcRect)
            : texture(texture), srcRect(srcRect) {}

        SDL_Texture* getTexture() const { return texture; }
        const SDL_Rect& getSourceRect() const { return srcRect; }
    };
}

#endif
