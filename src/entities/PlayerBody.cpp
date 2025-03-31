#include "../../include/entities/PlayerBody.h"
#include <SDL2/SDL.h>
#include <cmath>

namespace Entites
{
    const float PLAYER_SPEED = 100.0f;

    void PlayerBody::HandleInput(const Uint8* keystates)
    {
        Vector speed(0.0f, 0.0f);
        Vector shootDirection(0.0f, 0.0f);

        if (keystates[SDL_SCANCODE_W]) 
        {
            speed.y -= PLAYER_SPEED;
            shootDirection.y = -1;
        }
        if (keystates[SDL_SCANCODE_S]) 
        {
            speed.y += PLAYER_SPEED;
            shootDirection.y = 1;
        }
        if (keystates[SDL_SCANCODE_A]) 
        {
            speed.x -= PLAYER_SPEED;
            shootDirection.x = -1;
        }
        if (keystates[SDL_SCANCODE_D]) 
        {
            speed.x += PLAYER_SPEED;
            shootDirection.x = 1;
        }

        if (shootDirection.x != 0 || shootDirection.y != 0) 
        {
            float length = sqrtf(shootDirection.x * shootDirection.x + shootDirection.y * shootDirection.y);
            shootDirection.x /= length;
            shootDirection.y /= length;
    
            lastDirection = shootDirection;
        }

        if (speed.x != 0 && speed.y != 0)
        {
            float invRaiz = 1.0f / sqrtf(2.0f);
            speed.x *= invRaiz;
            speed.y *= invRaiz;
        }

        if (keystates[SDL_SCANCODE_F] && fire_timer <= 0.0f)
        {
            Attack(rect.x + rect.w / 2, rect.y + rect.h / 2, lastDirection.x, lastDirection.y);
            fire_timer = fire_rate;
        }

        this->setSpeed(speed);
    }

    void PlayerBody::Update(float deltaTime) 
    {
        this->move(deltaTime);
    
        if (fire_timer > 0.0f)
            fire_timer -= deltaTime;
    
        for (auto it = attacks.begin(); it != attacks.end(); )
        {
            it->Update(deltaTime);
    
            if (it->getAttackDuration() <= 0)
                it = attacks.erase(it);
            else
                ++it;
        }
    }

    void PlayerBody::Attack(float x, float y, float vx, float vy)
    {
        const float ATTACK_SPEED = this->getAttackSpeed();
        const float ATTACK_DURATION = this->getAttackDuration();

        attacks.emplace_back(x, y, 10, 10, true, true, 20.0f, 200.0f, ATTACK_DURATION);  
        attacks.back().setSpeed(Vector(vx * ATTACK_SPEED, vy * ATTACK_SPEED));  
    }

    void PlayerBody::Render(SDL_Renderer* renderer)
    {
        if (this->texture) 
        {
            SDL_RenderCopyF(renderer, this->texture, nullptr, &this->rect);
        } 
        else 
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect intRect = this->GetIntRect();
            SDL_RenderFillRect(renderer, &intRect);
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (auto& attack : attacks)
        {
            SDL_Rect attackRect = attack.GetIntRect();
            SDL_RenderFillRect(renderer, &attackRect);
        }
    }
}