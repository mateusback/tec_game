#include "../Interface/PlayerBody.h"

namespace Entites
{
    const float PLAYER_SPEED = 100;
    void PlayerBody::HandleInput(const Uint8* keystates) 
    {
        Vector speed;

        if (keystates[SDL_SCANCODE_W]) speed.y -= PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_S]) speed.y += PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_A]) speed.x -= PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_D]) speed.x += PLAYER_SPEED;

        if (speed.x != 0 && speed.y != 0) {
            float invRaiz = 1.0f / sqrtf(2.0f);
            speed.x *= invRaiz;
            speed.y *= invRaiz;
        }
        std::printf("moveX: %.2f, moveY: %.2f\n", speed.x, speed.y);
        this->setSpeed(speed);
    }
}