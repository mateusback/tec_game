#include "../../include/entities/PlayerBody.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/items/ItemTypes.h"
#include "../../include/physics/CollisionManager.h"
#include "../../include/utils/GlobalAccess.h"

#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

    void PlayerBody::handleInput(const Manager::PlayerInput& input)
    {
        Vector playerDirection = input.moveDirection;
        Vector spriteDirection;

        if (input.moveDirection.x != 0.f || input.moveDirection.y != 0.f) {
            spriteDirection = input.moveDirection;
        } else if (input.shootDirection.x != 0.f || input.shootDirection.y != 0.f) {
            spriteDirection = input.shootDirection;
        }
        
        updateDirectionSprite(spriteDirection);
        playerDirection *= this->getAcceleration();
        this->setSpeed(playerDirection);
    }

    void PlayerBody::update(float deltaTime)
    {
        if(this->active == false) return;
        this->move(deltaTime);

        if (this->attackTimer > 0.0f)
        this->attackTimer -= deltaTime;

        if(this->health <= 0.0f) {
            this->setActive(false);
            std::cout << "Player morreu!" << std::endl;
        }

        if (this->bombCooldown > 0.0f) {
            this->bombCooldown -= deltaTime;
        }
    }

    std::unique_ptr<Entities::AttackBody> PlayerBody::attack(Point characterCenter, Vector direction)
    {
        //TODO - DEPOIS AJUSTAR A FORMA DE INSTANCIAR O TAMANHO
        if(this->attackTimer > 0.0f) return nullptr;
        float width = 16.f;
        float height = 16.f;
    
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

    void PlayerBody::updateDirectionSprite(const Vector& direction) {
        if (direction.y < 0) {
            this->setTexture(Manager::TextureManager::Get("player_b"));
        } else if (direction.y > 0) {
            this->setTexture(Manager::TextureManager::Get("player_f"));
        } else if (direction.x < 0) {
            this->setTexture(Manager::TextureManager::Get("player_l"));
        } else if (direction.x > 0) {
            this->setTexture(Manager::TextureManager::Get("player_r"));
        }
    }

}