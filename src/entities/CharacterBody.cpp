#include "../../include/entities/PlayerBody.h"

namespace Entities
{
    void CharacterBody::takeDamage(float dmg) {
        this->health -= dmg;
        if (this->health < 0) this->health = 0;
    }
}