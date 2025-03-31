#include "../../include/entities/Body.h"

namespace Entites
{
    void Body::Update(float deltaTime)  {}

    void Body::Render(SDL_Renderer* renderer) 
    {
        if (!is_visible) return;

        if (this->texture) 
        {
            SDL_RenderCopyF(renderer, this->texture, nullptr, &this->rect);
        } 
        else 
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRectF(renderer, &this->rect);
        }
    }
}
