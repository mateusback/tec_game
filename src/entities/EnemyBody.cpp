#include "../../include/entities/EnemyBody.h"

namespace Entities {

    void EnemyBody::applyEnemyBehavior(float deltaTime) {
        const std::string& behavior = this->getEnemyData().getBehavior();
        auto* player = this->getTarget();
        if (!player) return;

        auto toPlayer = player->getPosition() - this->getPosition();

        if (behavior == "Bat") {
            if (toPlayer.length() < this->getEnemyData().getAggroRange()) {
                toPlayer.normalize();
                this->applyForce(toPlayer);
            }
        } else if (behavior == "Shell") {
            if (toPlayer.length() < this->getEnemyData().getAggroRange() / 2) {
                // comportamento da concha
            }
        }
    }

}
