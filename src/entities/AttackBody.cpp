#include "../../include/entities/AttackBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    void AttackBody::update(float deltaTime)
    {
        this->attack_duration -= deltaTime;
        if (this->attack_duration <= 0.f) {
            this->setActive(false);
            std::cout << "Retornando" << std::endl;
            return;
        }
        this->move(deltaTime);
        this->attack_duration -= deltaTime;
    }
}
