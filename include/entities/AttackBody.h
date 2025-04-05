#ifndef ATTACK_BODY_H
#define ATTACK_BODY_H

#include "MovingBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    class AttackBody : public MovingBody 
    {
    protected:
        float attack_damage;
        float attack_range;
        float attack_duration;
        float life_steal;
        float critical_chance;
        float critical_damage;

    public:
        AttackBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, 
                   float dmg = 0, float range = 0, float duration = 0, float lifesteal = 0, float crit_chance = 0, float crit_dmg = 0);

        void update(float deltaTime);
        void render(SDL_Renderer* renderer);
        bool isExpired() const;

        float getAttackDamage() const;
        float getAttackRange() const;
        float getAttackDuration() const;
        float getLifeSteal() const;
        float getCriticalChance() const;
        float getCriticalDamage() const;
    };
}

#endif
