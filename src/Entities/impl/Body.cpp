#include "../Interface/Body.h"

void Body::Update(float deltaTime) {
}

void Body::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRectF(renderer, &this->rect);
}