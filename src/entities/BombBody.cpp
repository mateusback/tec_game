#include "../../include/entities/BombBody.h"
#include "../../include/Utils/GlobalAccess.h"
#include "../../include/managers/EventManager.h"
#include "../../include/core/Events.h"

namespace Entities {
    void BombBody::update(float deltaTime) {
        this->timeToExplode -= deltaTime;
        if (this->timeToExplode <= 0.0f) {
            this->explode();
            this->setActive(false);
        }
    }

    void BombBody::explode() {
        audio()->playSoundEffect("bomb_explosion", 0);
        Vector2f explosionScale = { this->explosionRadius, this->explosionRadius };

        EventManager::Emit(Event::BombExploded{
            .position = this->getPosition(),
            .radius = this->explosionRadius,
            .damage = this->explosionDamage
        });
    }
}
