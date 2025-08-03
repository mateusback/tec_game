#ifndef BOSSBODY_H
#define BOSSBODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"
#include "AttackBody.h"
#include "../enemies/BossData.h"
#include "../managers/EntityManager.h"

namespace Entities {

    struct PendingAttack {
        Vector2f position;
        float timer;
    };
    
    class BossBody : public CharacterBody {
    private:
        Enemies::BossData bossData;
        Manager::EntityManager& entityManager;
        PlayerBody* target = nullptr;
        float aggroRange = 0.f;
        float stateTimer = 0.f;
        EntityState state = EntityState::Idle;
        std::vector<PendingAttack> scheduledAttacks;

    public:
        BossBody(Vector4f collider, const Enemies::BossData& data, Manager::EntityManager& entityManager);

        void applyBossBehavior(float deltaTime);
        void update(float deltaTime) override;
        void loadAnimations();
        void setAnimationByState();
        void summonAttacksAroundPlayer();
        void onCollision(Body* other) override;

        inline void setTarget(PlayerBody* player) { this->target = player; }
        inline void setAggroRange(float value) { this->aggroRange = value; }
        inline float getAggroRange() const { return this->aggroRange; }
        inline Enemies::BossData& getBossData() { return this->bossData; }
    };
}
#endif
