#include "../../include/entities/AttackBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    void AttackBody::update(float deltaTime)
    {
        this->attackDuration -= deltaTime;
        if (this->attackDuration <= 0.f) {
            this->setActive(false);
            return;
        }
        this->move(deltaTime);
        this->attackDuration -= deltaTime;
    }
}
