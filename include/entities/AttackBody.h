#ifndef ATTACK_BODY_H
#define ATTACK_BODY_H

#include <string>
#include "MovingBody.h"

namespace Entites
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
        AttackBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float dmg = 0, float range = 0, float duration = 0, float lifesteal = 0, float crit_chance = 0, float crit_dmg = 0)
            : MovingBody(x, y, w, h, collision, visible), attack_damage(dmg), attack_range(range), attack_duration(duration), life_steal(lifesteal), critical_chance(crit_chance), critical_damage(crit_dmg) {}

        float getAttackDamage() const 
        {
            return this->attack_damage;
        }

        float getAttackRange() const 
        {
            return this->attack_range;
        }

        float getAttackDuration() const 
        {
            return this->attack_duration;
        }

        float getLifeSteal() const 
        {
            return this->life_steal;
        }

        float getCriticalChance() const 
        {
            return this->critical_chance;
        }

        float getCriticalDamage() const 
        {
            return this->critical_damage;
        }
    };
}
#endif
