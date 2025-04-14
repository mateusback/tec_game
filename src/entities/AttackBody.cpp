#include "../../include/entities/AttackBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    void AttackBody::update(float deltaTime)
    {
        std::cout << "Attack Duration: " << this->attack_duration << std::endl;
        this->attack_duration -= deltaTime;
        if (this->attack_duration <= 0.f) {
            this->setActive(false);
            std::cout << "Retornando" << std::endl;
            return;
        }
        std::cout << "Movendo " << this->attack_duration << std::endl;
        this->move(deltaTime);
        this->attack_duration -= deltaTime;
    }
}
