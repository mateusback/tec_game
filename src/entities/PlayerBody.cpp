#include "../../include/entities/PlayerBody.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/items/ItemTypes.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/managers/EventManager.h"
#include "../../include/core/Events.h"

#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

    void PlayerBody::handleInput(const Manager::PlayerInput& input)
    {
        Vector2f playerDirection = input.moveDirection;

        if (input.moveDirection.x != 0.f || input.moveDirection.y != 0.f) {
    
            if (state != EntityState::Attacking) {
                this->animationManager.setAnimation("walk");
                this->setState(EntityState::Moving);
            }
    
        } else {
            if (input.shootDirection.x != 0.f || input.shootDirection.y != 0.f) {
                this->attack(input.shootDirection);
            }
    
            if (state == EntityState::Moving) {
                this->animationManager.setAnimation("idle");
                this->setState(EntityState::Idle);
            }
        }

        if(input.putBomb) {
            this->tryPlaceBomb();
        }
        
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

        if (this->invencible) {
            this->invencibleTimer -= deltaTime;
            if (this->invencibleTimer <= 0.0f) {
                this->invencible = false;
            }
        }

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

    void PlayerBody::attack(Vector2f direction)
    {
        this->setState(EntityState::Attacking);
        this->animationManager.setAnimation("attack", [this]() {
            this->setState(EntityState::Idle);
        });
        this->weaponHandler.attack(direction);
    }

    void PlayerBody::onCollision(Body* other)
    {
        other->onCollision(this);
        Physics::CollisionManager::resolveCollision(this, other);
    }

    void PlayerBody::takeDamage(float damage) {
        if (this->invencible) return;
        this->health -= damage; //TODO - USAR DEFESA
        this->invencible = true;
        this->invencibleTimer = 1.0f;
        audio()->playSoundEffect("hit-player", 0);
    }

    void PlayerBody::setWeapon(std::shared_ptr<Weapon> weapon, Manager::EntityManager* entityManager) {
        weapon->setEntityManager(entityManager);
        weaponHandler.setWeapon(std::move(weapon));
        weaponHandler.getWeapon()->setOwner(this);
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

        this->animationManager.setFrameTime("attack", 0.05f);
    
        this->animationManager.setAnimation("idle");
    }

    void PlayerBody::tryPlaceBomb() {
        if (this->getBombs() > 0 && this->getBombCooldown() <= 0.0f) {
            this->consumeBomb();
    
            EventManager::Emit(Event::PlayerPlacedBomb{
                .position = this->getPosition()
            });
        }
    }

    void PlayerBody::consumeBomb() {
        if (this->getBombs() > 0) {
            this->setBombs(this->getBombs() - 1);
            this->setBombCooldown(5.0f);
        }
    }

}