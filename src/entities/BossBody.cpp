#include "../../include/entities/BossBody.h"

#include <iostream>
#include <cstdlib>
#include <set>

#include "../../include/managers/TextureManager.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/entities/EffectBody.h"
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

        for (auto it = scheduledAttacks.begin(); it != scheduledAttacks.end(); ) {
            it->timer -= deltaTime;
            if (it->timer <= 0.f) {
                float tile = virtualRenderer()->getTileSize();
                Vector2f pos = it->position;

                auto attack = std::make_unique<AttackBody>(
                    pos.x - tile / 2.f,
                    pos.y - tile / 2.f,
                    tile,
                    tile,
                    false,
                    true,
                    this->getAttackDamage(),
                    this->getAttackRange(),
                    2.0f,
                    0.1f,
                    2.0f,
                    1.5f
                );
                audio()->playSoundEffect("chain", 0);

                attack->setSpeed({0, 0});
                attack->setTexture(textures()->Get("chain"));
                attack->setOrigin(this);

                entityManager.add(std::move(attack));
                it = scheduledAttacks.erase(it);
            } else {
                ++it;
            }
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
                std::cout << "Boss summoning portal attacks!" << std::endl;
                this->summonAttacksAroundPlayer();
                this->state = EntityState::CoolingDown;
                this->stateTimer = this->getAttackRate();
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

void BossBody::summonAttacksAroundPlayer() {
    Vector2f playerCenter = this->target->getCenterPoint();
    float tile = virtualRenderer()->getTileSize();

    for (int i = 0; i < 6; ++i) { // Mais ataques!
        float offsetX = ((rand() % 5) - 2) * tile;
        float offsetY = ((rand() % 5) - 2) * tile;
        Vector2f centerPos = playerCenter + Vector2f(offsetX, offsetY);

        // O portal aparece ANTES e fica embaixo do ataque
        auto effect = std::make_unique<EffectBody>(
            centerPos - Vector2f(tile / 2.f, tile / 2.f), // portal pelo topo esquerdo
            Vector2f(tile, tile),
            textures()->Get("portal_spawn"),
            1.0f
        );
        effect->setAnimationInfo({ "spin", 0, 8 });
        effect->loadAnimations();
        entityManager.add(std::move(effect));

        // Agendamos o ataque para surgir após o portal (0.5s)
        scheduledAttacks.push_back({ centerPos, 0.5f }); // centerPos, não offset
    }

    audio()->playSoundEffect("portal");
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
