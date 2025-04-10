#include "../../include/entities/PlayerBody.h"
#include "../../include/managers/TextureManager.h"
#include "../../include/items/ItemTypes.h"
#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

// TODO - fazer um inputmanager
    void PlayerBody::handleInput(const Uint8 *keystates)
    {
        Vector playerDirection(0.0f, 0.0f);
        Vector shootDirection(0.0f, 0.0f);

        //TODO - ARMA.SHOOT 
        if (keystates[SDL_SCANCODE_W])
            playerDirection.y -= this->getAcceleration();
        if (keystates[SDL_SCANCODE_S])
            playerDirection.y += this->getAcceleration();
        if (keystates[SDL_SCANCODE_A])
            playerDirection.x -= this->getAcceleration();
        if (keystates[SDL_SCANCODE_D])
            playerDirection.x += this->getAcceleration();

        if (keystates[SDL_SCANCODE_UP])
            shootDirection.y = -1;
        if (keystates[SDL_SCANCODE_DOWN])
            shootDirection.y = 1;
        if (keystates[SDL_SCANCODE_LEFT])
            shootDirection.x = -1;
        if (keystates[SDL_SCANCODE_RIGHT])
            shootDirection.x = 1;

        if (shootDirection.x != 0 || shootDirection.y != 0)
        {
            float length = sqrtf(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
            shootDirection.x /= length;
            shootDirection.y /= length;
            
            if(this->getFireTimer() <= 0.0f)
            {
                this->attack(this->getCenterPoint(), shootDirection);
                this->setFireTimer(this->getFireRate());
            }
        }

        if (playerDirection.x != 0 && playerDirection.y != 0)
        {
            float invRaiz = 1.0f / sqrtf(2.0f);
            playerDirection.x *= invRaiz;
            playerDirection.y *= invRaiz;
        }

        this->setSpeed(playerDirection);
    }

    void PlayerBody::update(float deltaTime)
    {
        this->move(deltaTime);

        if (fire_timer > 0.0f)
            fire_timer -= deltaTime;

        //TODO - REESTRUTURAR UMA LÓGICA BASEADA NO ATTACKSPEED E ATTACKRANGE
    }

    void PlayerBody::attack(Point characterCenter, Vector direction)
    {
        //TODO - IMPLEMENTAR O ATTACK BODY
    }

    void PlayerBody::onCollision(Body* other)
    {
        other->onCollision(this);
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