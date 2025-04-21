#include "../../include/entities/Body.h"

namespace Entities
{
    void Body::update(float deltaTime)  {}

    void Body::render(SDL_Renderer* renderer) 
    {
        if (!this->is_visible || !this->active) return;

        if (this->texture) 
        {
            SDL_FRect rect = { this->getCollider().x, this->getCollider().y, this->getCollider().w, this->getCollider().z };
            SDL_RenderCopyF(renderer, this->texture, nullptr, &rect);
        } 
        else 
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_FRect rect = { this->getCollider().x, this->getCollider().y, this->getCollider().w, this->getCollider().z };
            SDL_RenderFillRectF(renderer, &rect);
        }
    }
}
