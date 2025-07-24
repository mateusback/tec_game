#include "../../include/entities/BossBody.h"

#include <iostream>
#include <bits/ostream.tcc>

#include "../../include/managers/TextureManager.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/utils/GlobalAccess.h"

namespace Entities {

    BossBody::BossBody(Vector4f collider, const Enemies::BossData& data, Manager::EntityManager& entityManager)
        : CharacterBody(collider, true, true, 0.f),
          bossData(data),
          entityManager(entityManager)
    {
        this->setTexture(textures()->Get(data.getSpritePath()));
        this->setHealth(data.getHealth());
        this->setMaxHealth(data.getMaxHealth());
        this->setAttackDamage(data.getAttackDamage());
        this->setAttackRate(data.getAttackRate());
        this->setAttackRange(data.getAttackRange());
        this->setAggroRange(data.getAggroRange());
        this->hitboxOffset = Vector2f(0.1f, 0.1f);
        this->hitboxSize = Vector2f(0.8f, 0.8f);
        this->setState(EntityState::Idle);
        this->loadAnimations();
        this->setScale(virtualRenderer()->getTileSize() * 3, virtualRenderer()->getTileSize() * 3);
    }

    void BossBody::update(float deltaTime) {
        if (!this->isActive()) return;

        this->applyBossBehavior(deltaTime);

        if (this->is_animated) {
            this->animationManager.update(deltaTime);
        }
    }

    void BossBody::applyBossBehavior(float deltaTime) {
        if (!target || !target->isActive()) return;

        auto toPlayer = target->getCenterPoint() - this->getCenterPoint();
        float distance = toPlayer.length();

        if (distance > 0.5f) {
            Vector2f direction = toPlayer;
            this->speed = direction * bossData.getAcceleration();
            this->move(deltaTime);
        } else {
            this->speed = Vector2f(0, 0); 
        }

        switch (state) {
            case EntityState::Idle:
                this->state = EntityState::Attacking;
                this->stateTimer = 1.0f;
                break;

            case EntityState::Attacking:
                {
                    std::cout << "Boss summoned attacks!" << std::endl;
                    auto attacks = this->summonAttacks();
                    for (auto& atk : attacks) {
                        std::cout << "Summoning attack real: " << std::endl;
                        this->entityManager.add(std::move(atk));
                    }
                    this->state = EntityState::CoolingDown;
                    this->stateTimer = this->getAttackRate();
                }
                break;

            case EntityState::CoolingDown:
                this->stateTimer -= deltaTime;
                if (this->stateTimer <= 0.f) {
                    this->state = EntityState::Idle;
                }
                break;

            default:
                break;
        }

        this->setAnimationByState();
    }

    void BossBody::setAnimationByState() {
        switch (state) {
            case EntityState::Idle:
                this->animationManager.setAnimation("idle");
                break;
            case EntityState::Attacking:
                this->animationManager.setAnimation("attack");
                break;
            case EntityState::CoolingDown:
                this->animationManager.setAnimation("cooldown");
                break;
            default:
                this->animationManager.setAnimation("idle");
        }
    }

    std::vector<std::unique_ptr<AttackBody>> BossBody::summonAttacks() {
        std::vector<std::unique_ptr<AttackBody>> attacks;

        Vector2f center = this->getCenterPoint();
        Vector2f direction = this->target->getCenterPoint() - center;

        if (direction.x != 0.f || direction.y != 0.f) {
            direction.normalize();
        }

        float tileUnit = virtualRenderer()->getTileSize(); 

        for (int i = 1; i <= 3; ++i) {
            Vector2f offset = direction * static_cast<float>(i) * tileUnit;
            Vector2f attackPos = center + offset;

            float attackSize = tileUnit;

            auto attack = std::make_unique<AttackBody>(
                attackPos.x - attackSize / 2.f,
                attackPos.y - attackSize / 2.f,
                attackSize,
                attackSize,
                false,
                true,
                this->getAttackDamage(),
                this->getAttackRange(),
                2.0f,
                0.1f,
                2.0f,
                1.5f
            );

            attack->setSpeed({0, 0});
            attack->setTexture(textures()->Get("chain"));
            attack->setOrigin(this);

            std::cout << "Summoning attack at tile offset " << i << ": "
                    << attackPos.x << ", " << attackPos.y << std::endl;

            attacks.push_back(std::move(attack));
        }

        return attacks;
    }


    void BossBody::loadAnimations() {
        this->is_animated = true;

        SDL_Texture* texture = Manager::TextureManager::Get(this->bossData.getSpritePath());

        Manager::AnimationLoader::loadStaticAnimations(texture, {
            {"idle", 0, 0},
            {"attack", 0, 0},
            {"cooldown", 0, 0},
        }, this->animationManager, 96, 96);

        this->animationManager.setAnimation("idle");
    }

    void BossBody::onCollision(Body* other) {
        if (!other || !other->isActive()) return;

        auto* player = dynamic_cast<PlayerBody*>(other);
        if (player) {
            player->takeDamage(this->getAttackDamage());
        }
    }

}
