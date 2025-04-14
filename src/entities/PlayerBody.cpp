#include "../../include/entities/PlayerBody.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/items/ItemTypes.h"
#include "../../include/physics/CollisionManager.h"
#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

    void PlayerBody::handleInput(const Manager::PlayerInput& input)
    {
        Vector playerDirection = input.moveDirection;
        Vector shootDirection = input.shootDirection;
    
        playerDirection *= this->getAcceleration();
        this->setSpeed(playerDirection);
    
        if ((shootDirection.x != 0 || shootDirection.y != 0) && this->getFireTimer() <= 0.0f)
        {
            this->attack(this->getCenterPoint(), shootDirection);
            this->setFireTimer(this->getFireRate());
        }
    }

    void PlayerBody::update(float deltaTime)
    {
        this->move(deltaTime);

        if (fire_timer > 0.0f)
            fire_timer -= deltaTime;

        //TODO - REESTRUTURAR UMA LÓGICA BASEADA NO ATTACKSPEED E ATTACKRANGE
    }

    std::unique_ptr<Entities::AttackBody> PlayerBody::attack(Point characterCenter, Vector direction)
    {
        float width = 16.f;
        float height = 16.f;
        float speed = 350.f;
    
        if (direction.x != 0 || direction.y != 0) {
            float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction.x /= len;
            direction.y /= len;
        }
        
        std::cout << "Attack Direction: " << direction.x << ", " << direction.y << std::endl;

        auto attack = std::make_unique<Entities::AttackBody>(
            characterCenter.x - width / 2,
            characterCenter.y - height / 2,
            width,
            height,
            true,
            true,
            10.f,
            200.f,
            1.2f,
            0.f,
            0.1f,
            1.5f 
        );
    
        attack->setSpeed(direction * speed);
        return attack;
    }

    void PlayerBody::onCollision(Body* other)
    {
        Vector4 collider = this->getCollider();
        other->onCollision(this);
        Physics::CollisionManager::resolveCollision(collider, other->getCollider());
        this->setPosition(Vector(collider.x, collider.y));
    }

    //TODO - DÁ PRA COLOCAR NO ITEM MANAGER
    void PlayerBody::pickUpItem(ItemBody* item){
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
                    this->setFireRate(this->getFireRate() + effect.value);
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
}