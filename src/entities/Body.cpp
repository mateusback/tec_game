#include "../../include/entities/Body.h"

namespace Entities
{
    void Body::update(float deltaTime)  {}

    void Body::render(SDL_Renderer* renderer) 
    {
        if (!this->is_visible || !this->active) return;
        Vector4 fullSize = this->getFullSize();

        if (this->texture) 
        {
            SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.w, fullSize.z };
            SDL_RenderCopyF(renderer, this->texture, nullptr, &rect);
        } 
        else 
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_FRect rect = { fullSize.x, fullSize.y, fullSize.w, fullSize.z };
            SDL_RenderFillRectF(renderer, &rect);
        }
    }
}
