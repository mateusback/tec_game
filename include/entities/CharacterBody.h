#ifndef CHARACTER_BODY_H
#define CHARACTER_BODY_H

#include <string>
#include <list>
#include "MovingBody.h"
#include "AttackBody.h"
#include "EntitiesTypes.h"

namespace Entities
{
	class CharacterBody : public MovingBody {
	protected:
		std::string name;
		float health;
		float maxHealth;
		float attackDamage;
		float attackSpeed;
		float attackRange;
		float attackDuration;
		float attackRate;
        float attackTimer;
		float defense;
		uint8_t level;
		float stateTimer = 0.0f;
		EntityState state = EntityState::Idle;

	public:
		#pragma region Constructors
		CharacterBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: MovingBody(x, y, w, h, collision, visible, acceleration),
			health(100.0f), 
			maxHealth(100.0f), 
			attackDamage(10.0f), 
			attackSpeed(200.0f),
			attackRange(1.0f), 
			attackDuration(3.0f), 
			attackRate(0.3f), 
			attackTimer(0.0f), 
			defense(0.0f),
			level(1) {}

		CharacterBody(Vector pos, Vector scl, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: MovingBody(pos.x, pos.y, scl.x, scl.y, collision, visible, acceleration),
			health(100.0f), 
			maxHealth(100.0f), 
			attackDamage(10.0f), 
			attackSpeed(200.0f),
			attackRange(1.0f), 
			attackDuration(3.0f), 
			attackRate(0.3f), 
			attackTimer(0.0f), 
			defense(0.0f),
			level(1) {}

		CharacterBody(Vector4 collider, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: MovingBody(collider.x, collider.y, collider.z, collider.w, collision, visible, acceleration),
			health(100.0f), 
			maxHealth(100.0f), 
			attackDamage(10.0f), 
			attackSpeed(200.0f),
			attackRange(1.0f), 
			attackDuration(3.0f), 
			attackRate(0.3f), 
			attackTimer(0.0f), 
			defense(0.0f),
			level(1) {}
		#pragma endregion

		void takeDamage(float dmg) {
			this->health -= dmg;
			if (this->health < 0) this->health = 0;
		}

		float getHealthPercent() const { return this->health / this->maxHealth; }
		bool isDead() const { return this->health <= 0; }

		#pragma region Getters
		std::string getName() const { return this->name; }
		float getAttackSpeed() const { return this->attackSpeed; }
		float getAttackDuration() const { return this->attackDuration; }
		float getAttackRate() const { return this->attackRate; }
		float getAttackTimer() const { return this->attackTimer; }
		float getAttackDamage() const { return this->attackDamage; }
		float getAttackRange() const { return this->attackRange; }
		float getDefense() const { return this->defense; }
		float getHealth() const { return this->health; }
		float getMaxHealth() const { return this->maxHealth; }
		int getLevel() const { return this->level; }
		float getStateTimer() const { return this->stateTimer; }
		EntityState getState() const { return this->state; }
		#pragma endregion

		#pragma region Setters
		void setName(const std::string& name) { this->name = name; }
		void setAttackRate(float attackRate) { this->attackRate = attackRate; }
		void setAttackTimer(float attackTimer) { this->attackTimer = attackTimer; }
		void setAttackSpeed(float attackSpeed) { this->attackSpeed = attackSpeed; }
		void setAttackDuration(float attackDuration) { this->attackDuration = attackDuration; }
		void setAttackDamage(float attackDamage) { this->attackDamage = attackDamage; }
		void setAttackRange(float attackRange) { this->attackRange = attackRange; }
		void setDefense(float defense) { this->defense = defense; }
		void setHealth(float health) { this->health = health; }
		void setMaxHealth(float maxHealth) { this->maxHealth = maxHealth; }
		void setLevel(int level) { this->level = level; }
		void setStateTimer(float stateTimer) { this->stateTimer = stateTimer; }
		void setState(EntityState state) { this->state = state; }
		#pragma endregion
	};
}

#endif
