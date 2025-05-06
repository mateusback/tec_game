#include "../../include/entities/Body.h"

namespace Entities
{
    void Body::update(float deltaTime)  {
        if (this->is_animated) {
            this->animationManager.update(deltaTime);
        }
    }

    void Body::render(SDL_Renderer* renderer) 
    {
        if (!this->is_visible || !this->active) return;
        Vector4f fullSize = this->getFullSize();


        const Renderer::Sprite* sprite = this->animationManager.getCurrentSprite();
        if (sprite && sprite->getTexture()) {
            SDL_FRect rect = this->getRect();
            SDL_RenderCopyF(renderer, sprite->getTexture(), &sprite->getSourceRect(), &rect);
            return;
        }
        if (this->texture) 
        {
            SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.w, fullSize.z };
            SDL_RenderCopyF(renderer, this->texture, nullptr, &rect);
            return;
        } 

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.w, fullSize.z };
        SDL_RenderFillRectF(renderer, &rect);
    }
}
