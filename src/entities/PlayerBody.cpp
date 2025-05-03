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
        Vector playerDirection = input.moveDirection;
        Vector spriteDirection;

        if (input.moveDirection.x != 0.f || input.moveDirection.y != 0.f) {
            spriteDirection = input.moveDirection;
            this->animationManager.setAnimation("walk");
        } else if (input.shootDirection.x != 0.f || input.shootDirection.y != 0.f) {
            spriteDirection = input.shootDirection;
            this->animationManager.setAnimation("walk");
        } else{
            this->animationManager.setAnimation("idle");
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

        if(this->health <= 0.0f) {
            this->setActive(false);
            std::cout << "Player morreu!" << std::endl;
            this->animationManager.setAnimation("death");
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
        this->animationManager.setAnimation("attack");
    
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

        using Manager::AnimationLoader;
    
        const int tileSize = 32;
        const float frameDuration = 0.15f;
    
        int sheetWidthPixels = 416; 
    
        auto loadAnim = [&](const std::string& name, int row, int frameCount, bool loop = true) {
            int startIndex = row * (sheetWidthPixels / tileSize); 
            auto frames = AnimationLoader::loadRange(texture, sheetWidthPixels, startIndex, frameCount, tileSize, tileSize);
            Renderer::Animation anim(frames, frameDuration, loop);
            this->animationManager.addAnimation(name, anim);
        };
        
            loadAnim("idle", 4, 5);
            loadAnim("walk", 3, 7);
            loadAnim("attack", 8, 7, false);
            loadAnim("death", 7, 7, false);
        
            this->animationManager.setAnimation("idle");
    }

    //TODO - COLOCAR UMA CLASSE SPRITE QUE É UM VETOR DE TEXTURAS, E DEPOIS UM VETOR DE ANIMAÇÕES
    //TODO - CRIAR UMA CLASSE DE ANIMAÇÃO QUE TEM UM VETOR DE TEXTURAS E UM VETOR DE TEMPOS
    void PlayerBody::updateDirectionSprite(const Vector& direction) {
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