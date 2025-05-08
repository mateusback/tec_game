#include "../../include/entities/EnemyBody.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/SceneManager.h"
#include "managers/AnimationLoader.h"

namespace Entities {
    EnemyBody::EnemyBody(Vector4f collider, const Enemies::Enemy& data, Manager::EntityManager& entityManager)
    : CharacterBody(collider.x, collider.y, collider.z, collider.w, true, true, 0.f),
      enemyData(data),
      entityManager(entityManager) {
        this->setAggroRange(virtualRenderer()->normalizeValue(data.getAggroRange()));
        this->setAcceleration(virtualRenderer()->normalizeValue(data.getAcceleration()));
        this->setHealth(data.getHealth());
        this->setMaxHealth(data.getMaxHealth());
        this->setAttackDamage(data.getAttackDamage());
        this->setAttackRate(data.getAttackRate());
        this->setAttackRange(virtualRenderer()->normalizeValue(data.getAttackRange()));
        this->setAttackSpeed(data.getAttackSpeed());
        this->setDefense(data.getDefense());
        this->setTexture(textures()->Get(data.getSpritePath()));
        this->EnemyBody::loadAnimations();
    }

    void EnemyBody::applyEnemyBehavior(float deltaTime) {
        const EEnemyBehavior behavior = this->getEnemyData().getBehavior();
        auto* player = this->getTarget();
        Body::update(deltaTime);
        //if (!player) [[unlikely]] return;
    
        auto toPlayer = player->getPosition() - this->getPosition();
        float distance = toPlayer.length();
    
        if (behavior == EEnemyBehavior::Shell) {
            switch (this->state) {
                case EntityState::Idle:
                    this->animationManager.setAnimation("idle");
                    this->setCollision(true);
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Hidden;
                    }
                    break;
                case EntityState::Hidden:
                    this->animationManager.setAnimation("hidden");
                    this->setCollision(false);
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Attacking;
                    }
                    break;
    
                case EntityState::Attacking:
                    this->animationManager.setAnimation("attack");
                    toPlayer.normalize();
                    this->setCollision(true);
                    this->texture = textures()->Get(this->getEnemyData().getSpritePath());
                    this->state = EntityState::CoolingDown;
                    this->stateTimer = this->getAttackRate();
                {
                    auto attack = this->attack(this->getCenterPoint(), toPlayer);
                    if (attack) {
                        this->entityManager.add(std::move(attack));
                    }
                }
                    break;

                case EntityState::CoolingDown:
                    this->stateTimer -= deltaTime;
                    if(this->stateTimer <= this->getAttackRate() / 2) {
                        this->animationManager.setAnimation("hidden");
                        this->setCollision(false);
                    } else {
                        this->setCollision(true);
                        this->animationManager.setAnimation("idle");
                    }
                    if (stateTimer <= 0.0f) {
                        this->state = EntityState::Hidden;
                    }
                    break;
                default:
                    break;
            }
        } else if (behavior == EEnemyBehavior::Jumper) {
            switch (this->state) {
                case EntityState::Idle:
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Attacking;
                        this->stateTimer = 0.2f;
                    }
                    this->animationManager.setAnimation("idle");
                    break;

                case EntityState::Attacking:
                    this->stateTimer -= deltaTime;
                    if (this->stateTimer <= 0.f) {
                        toPlayer.normalize();
                        this->setSpeed(toPlayer * 150.0f);
                        this->state = EntityState::Jumping;
                        this->stateTimer = 0.6f;
                    }
                    this->animationManager.setAnimation("prep");
                    break;

                case EntityState::Jumping:
                    this->stateTimer -= deltaTime;
                    if (this->stateTimer <= 0.f) {
                        this->setSpeed({0, 0});
                        this->state = EntityState::CoolingDown;
                        this->stateTimer = this->getAttackRate();
                    }
                    this->animationManager.setAnimation("jump");
                    break;

                case EntityState::CoolingDown:
                    this->stateTimer -= deltaTime;
                    if (this->stateTimer <= 0.f) {
                        this->state = EntityState::Idle;
                    }
                    this->animationManager.setAnimation("idle");
                    break;

                default:
                    this->animationManager.setAnimation("idle");
                    break;
            }
        }
    }

    std::unique_ptr<AttackBody> EnemyBody::attack(Pointf origin, Vector2f direction) {
        float width = 16.f;
        float height = 16.f;
        float speed = 2;
    
        if (direction.x != 0 || direction.y != 0) {
            direction.normalize();
        }
    
        auto attack = std::make_unique<AttackBody>(
            origin.x - width / 2,
            origin.y - height / 2,
            width,
            height,
            true,
            true,
            this->getAttackDamage(),
            this->getAttackRange(),
            4.2f,
            0.f,
            0.1f,
            1.5f
        );
    
        attack->setScale(virtualRenderer()->getTileSizeDividedBy(3), virtualRenderer()->getTileSizeDividedBy(3));
        attack->setSpeed(direction * virtualRenderer()->normalizeValue(speed));
        attack->setOrigin(this);
        attack->setTexture(textures()->Get("attack")); 
    
        return attack;
    }

    void EnemyBody::loadAnimations() {
        if (this->enemyData.getBehavior() == EEnemyBehavior::Jumper) {
        SDL_Texture* texture = Manager::TextureManager::Get(this->enemyData.getSpritePath());
        this->is_animated = true;

        Manager::AnimationLoader::loadNamedAnimations(texture, {
            {"idle",   0, 0},
            {"prep",   0, 1},
            {"jump", 0, 2},
        }, this->animationManager);

        this->animationManager.setAnimation("idle");
        }
        if (this->enemyData.getBehavior() == EEnemyBehavior::Shell) {
            SDL_Texture* texture = Manager::TextureManager::Get("shell");
            this->is_animated = true;

            Manager::AnimationLoader::loadNamedAnimations(texture, {
                {"hidden",   0, 0},
                {"attack", 0, 1},
                {"idle",   0, 2},
            }, this->animationManager);

            this->animationManager.setAnimation("idle");
        }
    }

    void EnemyBody::onCollision(Body* other) {
        if (!other->isActive()) return;

        auto* player = dynamic_cast<PlayerBody*>(other);
        if (player) {
            float damage = this->getEnemyData().getAttackDamage();
            player->takeDamage(damage);
        }
    }


}
