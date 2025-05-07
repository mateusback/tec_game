#include "../../include/entities/EnemyBody.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/SceneManager.h"

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
    }

    void EnemyBody::applyEnemyBehavior(float deltaTime) {
        const std::string& behavior = this->getEnemyData().getBehavior();
        auto* player = this->getTarget();
        if (!player) return;
    
        auto toPlayer = player->getPosition() - this->getPosition();
        float distance = toPlayer.length();
    
        if (behavior == "Shell") {
            switch (this->state) {
                case EntityState::Idle:
                    this->setCollision(true);
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Hidden;
                    }
                    break;
                case EntityState::Hidden:
                    this->texture = textures()->Get("shell_hidden");
                    this->setCollision(false);
                    if (distance < this->getAggroRange()) {
                        this->state = EntityState::Attacking;
                    }
                    break;
    
                case EntityState::Attacking:
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
                        this->texture = textures()->Get("shell_hidden");
                        this->setCollision(false);
                    } else {
                        this->setCollision(true);
                        this->texture = textures()->Get(this->getEnemyData().getSpritePath());
                    }
                    if (stateTimer <= 0.0f) {
                        this->state = EntityState::Hidden;
                    }
                    break;
                default:
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

}
