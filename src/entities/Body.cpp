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
            SDL_FRect dst = {
                position.x + sprite->getOffset().x,
                position.y + sprite->getOffset().y,
                scale.x,
                scale.y
            };

            SDL_RenderCopyExF(
                renderer,
                sprite->getTexture(),
                &sprite->getSourceRect(),
                &dst,
                sprite->getAngle(),
                nullptr,
                sprite->getFlip()
            );
            return;
        }
        if (this->texture)
        {
            SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.z, fullSize.w };
            SDL_RenderCopyF(renderer, this->texture, nullptr, &rect);
            return;
        } 

        // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.z, fullSize.w };
        // SDL_RenderFillRectF(renderer, &rect);
    }
}
