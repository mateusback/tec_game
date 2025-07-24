#ifndef BOSSBODY_H
#define BOSSBODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"
#include "AttackBody.h"
#include "../enemies/BossData.h"
#include "../managers/EntityManager.h"

namespace Entities {
    class BossBody : public CharacterBody {
    private:
        Enemies::BossData bossData;
        Manager::EntityManager& entityManager;
        PlayerBody* target = nullptr;
        float aggroRange = 0.f;
        float stateTimer = 0.f;
        EntityState state = EntityState::Idle;

    public:
        BossBody(Vector4f collider, const Enemies::BossData& data, Manager::EntityManager& entityManager);

        void applyBossBehavior(float deltaTime);
        void update(float deltaTime) override;
        void loadAnimations();
        void setAnimationByState();
        std::vector<std::unique_ptr<AttackBody>> summonAttacks();
        void onCollision(Body* other) override;

        inline void setTarget(PlayerBody* player) { this->target = player; }
        inline void setAggroRange(float value) { this->aggroRange = value; }
        inline float getAggroRange() const { return this->aggroRange; }
        inline Enemies::BossData& getBossData() { return this->bossData; }
    };
}
#endif
