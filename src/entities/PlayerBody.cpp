#include "../../include/entities/PlayerBody.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/items/ItemTypes.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/AnimationLoader.h"

#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

    void PlayerBody::handleInput(const Manager::PlayerInput& input)
    {
        Vector2f playerDirection = input.moveDirection;
        Vector2f spriteDirection;

        if (input.moveDirection.x != 0.f || input.moveDirection.y != 0.f) {
            spriteDirection = input.moveDirection;
            if(state != EntityState::Attacking) {
                this->animationManager.setAnimation("walk");
            }
        } else if (input.shootDirection.x != 0.f || input.shootDirection.y != 0.f) {
            spriteDirection = input.shootDirection;
        } 

        if(this->state == EntityState::Idle) {
            this->setState(EntityState::Idle);
        }
        
        updateDirectionSprite(spriteDirection);
        playerDirection *= this->getAcceleration();
        this->setSpeed(playerDirection);
    }

    void PlayerBody::update(float deltaTime)
    {
        this->animationManager.update(deltaTime);
        if(this->active == false) return;
        this->move(deltaTime);

        if (this->attackTimer > 0.0f)
        this->attackTimer -= deltaTime;

        if (this->health <= 0.0f) {
            this->animationManager.setAnimation("death", [this]() {
                this->state = EntityState::Dead;
                this->setActive(false);
            });
        }

        if (this->bombCooldown > 0.0f) {
            this->bombCooldown -= deltaTime;
        }
    }

    std::unique_ptr<Entities::AttackBody> PlayerBody::attack(Pointf characterCenter, Vector2f direction)
    {
        if(this->attackTimer > 0.0f) return nullptr;
        float width = 16.f;
        float height = 16.f;
        this->setState(EntityState::Attacking);
        this->animationManager.setAnimation("attack", [this]() {
            this->animationManager.setAnimation("idle");
            this->setState(EntityState::Idle);
        });
        audio()->playSoundEffect("shoot", 0);
    
        if (direction.x != 0 || direction.y != 0) {
            float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction.x /= len;
            direction.y /= len;
        }
        this->attackTimer = this->attackRate;

        auto attack = std::make_unique<Entities::AttackBody>(
            characterCenter.x - width / 2,
            characterCenter.y - height / 2,
            width,
            height,
            true,
            true,
            10.f,
            200.f,
            4.2f,
            0.f,
            0.1f,
            1.5f 
        );

        attack->setScale(virtualRenderer()->getTileSizeDividedBy(3), virtualRenderer()->getTileSizeDividedBy(3));
        attack->setSpeed(direction * virtualRenderer()->normalizeValue(this->attackSpeed));
        attack->setOrigin(this);
        attack->setTexture(Manager::TextureManager::Get("attack"));
        return attack;
    }

    void PlayerBody::onCollision(Body* other)
    {
        other->onCollision(this);
        Physics::CollisionManager::resolveCollision(this, other);
    }

    //TODO - DÁ PRA COLOCAR NO ITEM MANAGER
    void PlayerBody::pickUpItem(ItemBody* item){
        audio()->playSoundEffect("pickup-item", 0);
        for (const auto& effect : item->getItem().getEffects()) {
            switch (effect.target) {
				using enum Items::EEffectTarget;
                case AttackDamage:
                    std::cout << "Attack Damage Antigo: " << this->getAttackDamage() << std::endl;
                    this->setAttackDamage(this->getAttackDamage() + effect.value);
                    std::cout << "Attack Damage Novo: " << this->getAttackDamage() << std::endl;
                    break;
                case AttackSpeed:
                    this->setAttackSpeed(this->getAttackSpeed() + effect.value);
                    break;
                case AttackRange:
                    this->setAttackRange(this->getAttackRange() + effect.value);
                    break;
                case AttackDuration:
                    this->setAttackDuration(this->getAttackDuration() + effect.value);
                    break;
                case FireRate:
                    this->setAttackRate(this->getAttackRate() + effect.value);
                    break;
                case Defense:
                    this->setDefense(this->getDefense() + effect.value);
                    break;
                case Health:
                    this->setHealth(this->getHealth() + effect.value);
                    break;
                case MaxHealth:
                    this->setMaxHealth(this->getMaxHealth() + effect.value);
                    break;
            }
        }
        Manager::TextureManager::Clear("player");
        this->setTexture(Manager::TextureManager::Get("player_with_item"));
    }

    void PlayerBody::loadAnimations() {
        SDL_Texture* texture = Manager::TextureManager::Get("player_sheet");
        this->is_animated = true;
    
        Manager::AnimationLoader::loadNamedAnimations(texture, {
            {"idle",   4, 5},
            {"walk",   3, 7},
            {"attack", 8, 7, false},
            {"death",  7, 7, false}
        }, this->animationManager);
    
        this->animationManager.setAnimation("idle");
    }

    //TODO - COLOCAR UMA CLASSE SPRITE QUE É UM VETOR DE TEXTURAS, E DEPOIS UM VETOR DE ANIMAÇÕES
    //TODO - CRIAR UMA CLASSE DE ANIMAÇÃO QUE TEM UM VETOR DE TEXTURAS E UM VETOR DE TEMPOS
    void PlayerBody::updateDirectionSprite(const Vector2f& direction) {
        // if (direction.y < 0) {
        //     this->setTexture(Manager::TextureManager::Get("player_b"));
        // } else if (direction.y > 0) {
        //     this->setTexture(Manager::TextureManager::Get("player_f"));
        // } else if (direction.x < 0) {
        //     this->setTexture(Manager::TextureManager::Get("player_l"));
        // } else if (direction.x > 0) {
        //     this->setTexture(Manager::TextureManager::Get("player_r"));
        // }
    }

}