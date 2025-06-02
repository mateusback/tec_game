#include "../../include/entities/EnemyBody.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/SceneManager.h"
#include "managers/AnimationLoader.h"

namespace Entities {
    EnemyBody::EnemyBody(Vector4f collider, const Enemies::Enemy& data, Manager::EntityManager& entityManager)
    : CharacterBody(collider, true, true, 0.f),
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
        if (!player) [[unlikely]] return;
    
        auto toPlayer = player->getPosition() - this->getPosition();
        float distance = toPlayer.length();
        
        if (behavior == EEnemyBehavior::Shell) {
            switch (this->state) {
                case EntityState::Idle:
                    this->setCollision(true);
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Hidden;
                    }
                    break;
                case EntityState::Hidden:
                    this->setCollision(false);
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Attacking;
                    }
                    break;
    
                case EntityState::Attacking:
                    toPlayer.normalize();
                    this->setCollision(true);
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
                        this->setCollision(false);
                    } else {
                        this->setCollision(true);
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
                        this->stateTimer = 0.5f; 
                    }
                    break;
        
                case EntityState::Attacking:
                    this->stateTimer -= deltaTime;
                    this->setSpeed({0.f, 0.f});
                    if (this->stateTimer <= 0.f) {
                        if (distance > 0.f)
                            toPlayer.normalize();
                        this->setSpeed({toPlayer.x * virtualRenderer()->normalizeValue(6), toPlayer.y * virtualRenderer()->normalizeValue(6)});
                        this->state = EntityState::Jumping;
                        this->stateTimer = 0.4f;
                    }
                    break;
        
                case EntityState::Jumping:
                    this->stateTimer -= deltaTime;
                    if (this->stateTimer <= 0.f) {
                        this->setSpeed({0.f, 0.f});
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
        }
        this->setAnimationByState();
        if(this->speed.x == 0 && this->speed.y == 0) return;
        this->move(deltaTime);
    }

    void EnemyBody::setAnimationByState() {
        std::string desiredAnimation;

        //todo - refatorar tudo isso aqui, remover redundâncias e melhorar a lógica
        switch (this->state) {
            case EntityState::Idle:
                desiredAnimation = "idle";
                break;
            case EntityState::Hidden:
                desiredAnimation = "hidden";
                break;
            case EntityState::Attacking:
                if (this->getEnemyData().getBehavior() == EEnemyBehavior::Shell) {
                    desiredAnimation = "attack";
                } else {
                    desiredAnimation = "prep";
                }
                break;
            case EntityState::Jumping:
                desiredAnimation = "jump";
                break;
            case EntityState::CoolingDown:
                if (this->getEnemyData().getBehavior() == EEnemyBehavior::Shell) {
                    desiredAnimation = (this->stateTimer <= this->getAttackRate() / 2.f)
                                       ? "hidden" : "idle";
                } else {
                    desiredAnimation = "idle";
                }
                break;
            default:
                desiredAnimation = "idle";
                break;
        }
        this->animationManager.setAnimation(desiredAnimation);
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
        this->is_animated = true;
        if (this->enemyData.getBehavior() == EEnemyBehavior::Jumper) {
            SDL_Texture* texture = Manager::TextureManager::Get(this->enemyData.getSpritePath());

            Manager::AnimationLoader::loadStaticAnimations(texture, {
                {"idle",   0, 0},
                {"prep",   0, 1},
                {"jump", 0, 2},
            }, this->animationManager);

            this->animationManager.setAnimation("idle");
            return;
        }
        if (this->enemyData.getBehavior() == EEnemyBehavior::Shell) {
            SDL_Texture* texture = Manager::TextureManager::Get(this->enemyData.getSpritePath());

            Manager::AnimationLoader::loadStaticAnimations(texture, {
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
