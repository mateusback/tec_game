#ifndef ENEMY_BODY_H
#define ENEMY_BODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"
#include "../enemies/Enemy.h"

namespace Entities {
    class EnemyBody : public CharacterBody {
    private:
        Enemies::Enemy enemyData;
        PlayerBody* target = nullptr;

    public:
        EnemyBody(Vector4 collider, const Enemies::Enemy& data, float acceleration)
            : CharacterBody(collider.x, collider.y, collider.z, collider.w, true, true, acceleration),
              enemyData(data) {}

        void setTarget(PlayerBody* player) { this->target = player; }
        PlayerBody* getTarget() const { return this->target; }

        void update(float deltaTime) override {
            applyEnemyBehavior(deltaTime);
            CharacterBody::update(deltaTime);
        }

        void applyEnemyBehavior(float deltaTime);
        
        const Enemies::Enemy& getEnemyData() const { return enemyData; }
    };
}

#endif
