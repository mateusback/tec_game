#ifndef ATTACK_BODY_H
#define ATTACK_BODY_H

#include "MovingBody.h"
#include <SDL2/SDL.h>

namespace Entities
{
    class AttackBody : public MovingBody 
    {
    protected:
		Entity* origin = nullptr;
        float attackDamage;
        float attackRange;
        float attackDuration;
        float lifeSteal;
        float criticalChance;
        float criticalDamage;

    public:
		#pragma region Constructors
		AttackBody(Vector position, Vector scale, bool collision = false, bool visible = true, 
			float dmg = 0, float range = 0, float duration = 0, float lifesteal = 0, float crit_chance = 0, float crit_dmg = 0)
			: MovingBody(position.x, position.y, scale.x, scale.y, collision, visible, 0.0f),
			attackDamage(dmg),
			attackRange(range),
			attackDuration(duration),
			lifeSteal(lifesteal),
			criticalChance(crit_chance),
			criticalDamage(crit_dmg) {}

		AttackBody(float x, float y, float w, float h, bool collision = false, bool visible = true, 
			float dmg = 0, float range = 0, float duration = 0, float lifesteal = 0, float crit_chance = 0, float crit_dmg = 0)
			: MovingBody(x, y, w, h, collision, visible, 0.0f),
			attackDamage(dmg),
			attackRange(range),
			attackDuration(duration),
			lifeSteal(lifesteal),
			criticalChance(crit_chance),
			criticalDamage(crit_dmg) {}

		AttackBody(Vector4 collider, bool collision = false, bool visible = true, 
			float dmg = 0, float range = 0, float duration = 0, float lifesteal = 0, float crit_chance = 0, float crit_dmg = 0)
			: MovingBody(collider.x, collider.y, collider.w, collider.z, collision, visible, 0.0f),
			attackDamage(dmg),
			attackRange(range),
			attackDuration(duration),
			lifeSteal(lifesteal),
			criticalChance(crit_chance),
			criticalDamage(crit_dmg) {}
		#pragma endregion

        void update(float deltaTime);

		#pragma region Getters
		float getAttackDamage() { return this->attackDamage; }
		float getAttackRange() { return this->attackRange; }
		float getAttackDuration() { return this->attackDuration; }
		float getLifeSteal() { return this->lifeSteal; }
		float getCriticalChance() { return this->criticalChance; }
		float getCriticalDamage() { return this->criticalDamage; }
		Entity* getOrigin() { return this->origin; }
		#pragma endregion

		#pragma region Setters
		void setAttackDamage(float attackDamage) { this->attackDamage = attackDamage; }
		void setAttackRange(float attackRange) { this->attackRange = attackRange; }
		void setAttackDuration(float attackDuration) { this->attackDuration = attackDuration; }
		void setLifeSteal(float lifeSteal) { this->lifeSteal = lifeSteal; }
		void setCriticalChance(float criticalChance) { this->criticalChance = criticalChance; }
		void setCriticalDamage(float criticalDamage) { this->criticalDamage = criticalDamage; }
		void setOrigin(Entity* origin) { this->origin = origin; }
		#pragma endregion
    };
}

#endif
