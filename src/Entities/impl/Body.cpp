#include "../Interface/Body.h"

void Body::Update(float deltaTime) {
}

void Body::Render(SDL_Renderer* renderer) {
    if (!is_visible) return;

    SDL_Rect dstRect = {
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.w),
        static_cast<int>(rect.h)
    };

    if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRectF(renderer, &rect);
    }
}
