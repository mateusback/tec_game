#ifndef CHARACTER_BODY_H
#define CHARACTER_BODY_H

#include <string>
#include <list>
#include "../core/GameConstants.h"
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
		bool invencible = false;
		float invencibleTimer = 0.0f;

	public:
		#pragma region Constructors
		CharacterBody(Vector2f pos, Vector2f scl, bool collision = GameConstants::Body::DEFAULT_COLLISION, bool visible = GameConstants::Body::DEFAULT_VISIBLE, float acceleration = GameConstants::Character::BASE_ACCELERATION)
			: CharacterBody(Vector4f(pos.x, pos.y, scl.x, scl.y), collision, visible, acceleration) {}

		CharacterBody(Vector4f collider, bool collision = GameConstants::Body::DEFAULT_COLLISION, bool visible = GameConstants::Body::DEFAULT_VISIBLE, float acceleration = GameConstants::Character::BASE_ACCELERATION)
			: MovingBody(collider, collision, visible, acceleration),
			health(GameConstants::Character::BASE_HEALTH),
			maxHealth(GameConstants::Character::BASE_HEALTH),
			attackDamage(GameConstants::Character::BASE_ATTACK_DAMAGE),
			attackSpeed(GameConstants::Character::BASE_ATTACK_SPEED),
			attackRange(GameConstants::Character::BASE_ATTACK_RANGE),
			attackDuration(GameConstants::Character::BASE_ATTACK_DURATION),
			attackRate(GameConstants::Character::BASE_ATTACK_RATE),
			attackTimer(0.0f),
			defense(GameConstants::Character::BASE_DEFENSE),
			level(GameConstants::Character::BASE_LEVEL) {}
		#pragma endregion

		virtual void takeDamage(float dmg);
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
		bool isInvencible() const { return this->invencible; }
		float getInvencibleTimer() const { return this->invencibleTimer; }
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
		void setInvencible(bool invencible) { this->invencible = invencible; }
		void setInvencibleTimer(float invencibleTimer) { this->invencibleTimer = invencibleTimer; }
		#pragma endregion
	};
}

#endif
