#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include "../utils/Types.h"

namespace Renderer {
    class Sprite {
    private:
        SDL_Texture* texture = nullptr;
        SDL_Rect srcRect{};
        Vector2f offset = {0.f, 0.f};
        double angle = 0.0;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

    public:
        Sprite(SDL_Texture* texture, const SDL_Rect& srcRect)
            : texture(texture), srcRect(srcRect) {}

        SDL_Texture* getTexture() const { return texture; }
        const SDL_Rect& getSourceRect() const { return srcRect; }
        Vector2f getOffset() const { return offset; }
        SDL_RendererFlip getFlip() const { return flip; }
        double getAngle() const { return angle; }

        void setOffset(Vector2f o) { offset = o; }
        void setAngle(double a) { angle = a; }
        void setFlip(SDL_RendererFlip f) { flip = f; }
    };
}

#endif
