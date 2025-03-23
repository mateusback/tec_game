#include "../Interface/PlayerBody.h"

void PlayerBody::HandleInput(const Uint8* keystates, float& moveX, float& moveY) {
    moveX = 0;
    moveY = 0;

    if (keystates[SDL_SCANCODE_W]) moveY -= 1;
    if (keystates[SDL_SCANCODE_S]) moveY += 1;
    if (keystates[SDL_SCANCODE_A]) moveX -= 1;
    if (keystates[SDL_SCANCODE_D]) moveX += 1;

    if (moveX != 0 && moveY != 0) {
        float inv = 1.0f / sqrtf(2.0f);
        moveX *= inv;
        moveY *= inv;
    }
}

void PlayerBody::Update(float deltaTime, float moveX, float moveY) {
    this->rect.x += moveX * this->velocity * deltaTime;
    this->rect.y += moveY * this->velocity * deltaTime;
}

