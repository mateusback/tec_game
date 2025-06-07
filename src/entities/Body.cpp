#include "../../include/entities/Body.h"

namespace Entities
{
    void Body::update(float deltaTime)  {
        if (this->is_animated) {
            this->animator.update(deltaTime);
        }
    }

    void Body::render(SDL_Renderer* renderer) 
    {
        if (!this->is_visible || !this->active) return;
        Vector4f fullSize = this->getFullSize();

        auto sprites = this->animator.getCurrentSprites();
        for (const auto& s : sprites) {
            const Renderer::Sprite* sprite = s.sprite;
            if (!sprite || !sprite->getTexture()) continue;

            SDL_FRect dst = {
                position.x + s.offset.x + sprite->getOffset().x,
                position.y + s.offset.y + sprite->getOffset().y,
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
        }
        if (this->texture)
        {
            SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.z, fullSize.w };
            SDL_RenderCopyF(renderer, this->texture, nullptr, &rect);
        }
    }
}
