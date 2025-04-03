#include "../../include/entities/PlayerBody.h"
#include "../../include/core/TextureManager.h"
#include <SDL2/SDL.h>
#include <cmath>

namespace Entities
{

    void PlayerBody::handleInput(const Uint8 *keystates)
    {
        Vector playerDirection(0.0f, 0.0f);
        Vector shootDirection(0.0f, 0.0f);

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

        for (auto it = attacks.begin(); it != attacks.end();)
        {
            it->update(deltaTime);

            if (it->getAttackDuration() <= 0)
                it = attacks.erase(it);
            else
                ++it;
        }
    }

    void PlayerBody::attack(Point characterCenter, Vector direction)
    {
        this->attacks.emplace_back(characterCenter.x, characterCenter.y,
                                   10, 10, true, true, 20.0f, 200.0f, this->getAttackDuration());

        this->attacks.back().setSpeed(Vector(direction.x * this->getAttackSpeed(), direction.y * this->getAttackSpeed()));
    }

    //TODO REMOVER ISSO
    void PlayerBody::render(SDL_Renderer *renderer)
    {
        if (this->texture)
        {
            SDL_RenderCopyF(renderer, this->texture, nullptr, &this->rect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect intRect = this->getIntRect();
            SDL_RenderFillRect(renderer, &intRect);
        }

        // TODO - RENDER ATTACKS VAI ATTACK BODY TEM SEU RENDER
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (auto &attack : attacks)
        {
            SDL_Rect attackRect = attack.getIntRect();
            SDL_RenderFillRect(renderer, &attackRect);
        }
    }

    void PlayerBody::onCollision(Body* other)
    {
        other->onCollision(this);
    }

    void PlayerBody::pickUpItem(ItemBody* item){
        this->setAcceleration(this->getAcceleration() + 20.0f);
        Core::TextureManager::Clear("player");
        this->setTexture(Core::TextureManager::Get("player_with_item"));
    }
}