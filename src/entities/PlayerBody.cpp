#include "../../include/entities/PlayerBody.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/items/ItemTypes.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/managers/AnimationLoader.h"
#include "../../include/managers/EventManager.h"
#include "../../include/core/Events.h"
#include "../../include/weapons/SwordWeapon.h"

#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

    void PlayerBody::handleInput(const Manager::PlayerInput& input)
    {

        if (this->state == EntityState::Dying || this->state == EntityState::Dead)
            return;

        Vector2f playerDirection = input.moveDirection;

        this->updateAnimationByInput(playerDirection);

        if (input.shootDirection.x != 0.f || input.shootDirection.y != 0.f) {
            this->attack(input.shootDirection);
        }

        if (input.putBomb) {
            this->tryPlaceBomb();
        }

        playerDirection *= this->getAcceleration();
        this->setSpeed(playerDirection);
    }

    void PlayerBody::update(float deltaTime)
    {
        this->animationManager.update(deltaTime);
        if(this->active == false) return;
        if (this->state == EntityState::Dead) return;
        if (this->state == EntityState::Dying) return;


        this->move(deltaTime);

        if (this->attackTimer > 0.0f)
        this->attackTimer -= deltaTime;

        if (this->invencible) {
            this->invencibleTimer -= deltaTime;
            if (this->invencibleTimer <= 0.0f) {
                this->invencible = false;
            }
        }
        if (this->health <= 0.0f && this->state != EntityState::Dying) {
            this->state = EntityState::Dying;
            this->setSpeed({0.f, 0.f});
            this->animationManager.setAnimation("death", [this]() {
                this->state = EntityState::Dead;
            });
            return;
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
            this->animationManager.setAnimation("idle");
        });
        this->weaponHandler.attack(direction);
    }

    void PlayerBody::onCollision(Body* other)
    {
        other->onCollision(this);
    }

    void PlayerBody::takeDamage(float damage) {
        if (this->invencible) return;
        this->health -= damage;
        this->invencible = true;
        this->invencibleTimer = 1.0f;
        audio()->playSoundEffect("hit-player", 0);
    }

    void PlayerBody::setWeapon(std::shared_ptr<Weapon> weapon, Manager::EntityManager* entityManager) {
        weapon->setEntityManager(entityManager);
        weaponHandler.setWeapon(std::move(weapon));
        weaponHandler.getWeapon()->setOwner(this);
        this->setAttackRate(2.5f);
        this->setAttackDamage(50.0f);
        this->setAttackDuration(0.5f);
    }

    void PlayerBody::pickUpItem(ItemBody* item){
        audio()->playSoundEffect("pickup-item", 0);

        if(item->getItem().getWeaponId() != "" && item->getItem().getWeaponId() == "sword") {
            auto newWeapon = std::make_shared<SwordWeapon>();
            this->setWeapon(newWeapon, this->weaponHandler.getWeapon()->getEntityManager());
            return;
        }
        //Todo - Colocar em um item manager, e verificar se dá pra fazer de uma forma melhor, tipo com um unordered_map
        for (const auto& effect : item->getItem().getEffects()) {
            switch (effect.target) {
				using enum Items::EEffectTarget;
                case AttackDamage:
                    this->setAttackDamage(this->getAttackDamage() + effect.value);
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
                case PlayerSize:
                    this->setScale(this->getScale() + Vector2f(effect.value, effect.value));
                    break;
                case PlayerSpeed:
                    this->setAcceleration(this->getAcceleration() + effect.value);
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
            {"walk",   0, 4},
            {"attack", 0, 4, false},
            {"death",  4, 5, false},
            {"walk-r", 1, 5, true},
        }, this->animationManager, 0.15f, 32, 34);

        Manager::AnimationLoader::loadNamedAnimations(texture, {
            {"transition-l", 2, 5, false},
            {"transition-r", 3, 5, false}
        }, this->animationManager, 0.05f, 32, 34);

        auto tex = textures()->Get("pf");
        Manager::AnimationLoader::loadNamedAnimations(tex, {
            {"idle",   0, 30},
        }, this->animationManager, 0.03f, 32, 64);

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

void PlayerBody::updateAnimationByInput(const Vector2f& moveDirection) {
    if (this->state == EntityState::Attacking)
        return;

    // evita sobrescrever transição
    std::string currAnim = this->animationManager.getCurrentAnimationName();
    if (currAnim == "transition-r" || currAnim == "transition-l")
        return;

    bool moved = false;

    if (moveDirection.x > 0.f) {
        this->currentDirection = EDirection::Right;
        moved = true;

        if (lastDirectionHorizontal != EDirection::Right) {
            lastDirectionHorizontal = EDirection::Right;
            this->animationManager.setAnimation("transition-r", [this]() {
                if (this->state != EntityState::Attacking) {
                    this->animationManager.setAnimation("walk-r");
                    this->setState(EntityState::Moving);
                }
            });
        } else if (currAnim != "walk-r") {
            this->animationManager.setAnimation("walk-r");
            this->setState(EntityState::Moving);
        }
    }
    else if (moveDirection.x < 0.f) {
        this->currentDirection = EDirection::Left;
        moved = true;

        if (lastDirectionHorizontal != EDirection::Left) {
            lastDirectionHorizontal = EDirection::Left;
            this->animationManager.setAnimation("transition-l", [this]() {
                if (this->state != EntityState::Attacking) {
                    this->animationManager.setAnimation("walk");
                    this->setState(EntityState::Moving);
                }
            });
        } else if (currAnim != "walk") {
            this->animationManager.setAnimation("walk");
            this->setState(EntityState::Moving);
        }
    }
    else if (moveDirection.y != 0.f) {
        moved = true;
        if (currAnim != "walk") {
            this->animationManager.setAnimation("walk");
        }
        this->setState(EntityState::Moving);
    }

    if (!moved && this->state == EntityState::Moving) {
        this->animationManager.setAnimation("idle");
        this->setState(EntityState::Idle);
    }
}


}