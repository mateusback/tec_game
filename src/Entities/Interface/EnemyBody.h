#ifndef ENEMY_BODY_H
#define ENEMY_BODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"

class EnemyBody : public CharacterBody {
protected:
    PlayerBody* target;
    float aggro_range;

public:
    EnemyBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
        : CharacterBody(x, y, w, h, collision, visible), target(nullptr), aggro_range(100) {}

    void setTarget(PlayerBody* target) {
        this->target = target;
    }

    PlayerBody* getTarget() const {
        return target;
    }

    void setAggroRange(float range) {
        this->aggro_range = range;
    }

    float getAggroRange() const {
        return this->aggro_range;
    }
};

#endif
