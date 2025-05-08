#ifndef ENEMY_BODY_H
#define ENEMY_BODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"
#include "../enemies/Enemy.h"
#include "../managers/EntityManager.h"

namespace Entities {

    class EnemyBody : public CharacterBody {
    private:
        Enemies::Enemy enemyData;
        PlayerBody* target = nullptr;
        float agrroRange = 0.f;
        Manager::EntityManager& entityManager;

    public:
        EnemyBody(Vector4f collider, const Enemies::Enemy& data, Manager::EntityManager& entityManager);
        EnemyBody(const EnemyBody& other)
        : CharacterBody(other),
        enemyData(other.enemyData),
        target(other.target),
        agrroRange(other.agrroRange),
        entityManager(other.entityManager) {
            
            this->setTexture(other.getTexture());
            this->setPosition(other.getPosition());
            this->EnemyBody::loadAnimations();
        }

        std::unique_ptr<AttackBody> attack(Pointf origin, Vector2f direction);

        void setTarget(PlayerBody* player) { this->target = player; }
        PlayerBody* getTarget() const { return this->target; }

        void update(float deltaTime) override {
            applyEnemyBehavior(deltaTime);
        }

        void applyEnemyBehavior(float deltaTime);
        void loadAnimations() override;
        void onCollision(Body* other) override;
        
        const Enemies::Enemy& getEnemyData() const { return enemyData; }
        void setEnemyData(const Enemies::Enemy& data) { this->enemyData = data; }
        float getAggroRange() const { return this->agrroRange; }
        void setAggroRange(float range) { this->agrroRange = range; }
    };
}

#endif
