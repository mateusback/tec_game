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

		#pragma region Getters
		float getAttackDamage() { return this->attack_damage; }
		float getAttackRange() { return this->attack_range; }
		float getAttackDuration() { return this->attack_duration; }
		float getLifeSteal() { return this->life_steal; }
		float getCriticalChance() { return this->critical_chance; }
		float getCriticalDamage() { return this->critical_damage; }
		#pragma endregion

		#pragma region Setters
		void setAttackDamage(float attack_damage) { this->attack_damage = attack_damage; }
		void setAttackRange(float attack_range) { this->attack_range = attack_range; }
		void setAttackDuration(float attack_duration) { this->attack_duration = attack_duration; }
		void setLifeSteal(float life_steal) { this->life_steal = life_steal; }
		void setCriticalChance(float critical_chance) { this->critical_chance = critical_chance; }
		void setCriticalDamage(float critical_damage) { this->critical_damage = critical_damage; }
		#pragma endregion
    };
}

#endif
