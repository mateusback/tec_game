#ifndef CHARACTER_BODY_H
#define CHARACTER_BODY_H

#include <string>
#include <list>
#include "MovingBody.h"
#include "AttackBody.h"

namespace Entities
{
	class CharacterBody : public MovingBody {
	protected:
		std::string name;
		float health;
		float max_health;
		float attack_damage;
		float attack_speed;
		float attack_range;
		float attack_duration;
		float defense;
		float fire_rate;
        float fire_timer;
		int level;

		CharacterBody(EBodyType type, float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true, float acceleration = 100.0f)
		: MovingBody(type, x, y, w, h, collision, visible, acceleration), health(100.0f), max_health(100.0f), attack_damage(10.0f), 
		attack_speed(200.0f), attack_range(1.0f), attack_duration(3.0f), defense(0.0f), fire_rate(0.3f), fire_timer(0.0f), level(1) {}

	public:
		#pragma region Constructors
		CharacterBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: MovingBody(EBodyType::Character, x, y, w, h, collision, visible, acceleration),
			health(100.0f), 
			max_health(100.0f), 
			attack_damage(10.0f), 
			attack_speed(200.0f),
			attack_range(1.0f), 
			attack_duration(3.0f), 
			defense(0.0f),
			fire_rate(0.3f), 
			fire_timer(0.0f), 
			level(1) {}

		CharacterBody(Vector pos, Vector scl, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: MovingBody(EBodyType::Character, pos.x, pos.y, scl.x, scl.y, collision, visible, acceleration),
			health(100.0f), 
			max_health(100.0f), 
			attack_damage(10.0f), 
			attack_speed(200.0f),
			attack_range(1.0f), 
			attack_duration(3.0f), 
			defense(0.0f),
			fire_rate(0.3f), 
			fire_timer(0.0f), 
			level(1) {}

		CharacterBody(Vector4 collider, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: MovingBody(EBodyType::Character, collider.x, collider.y, collider.z, collider.w, collision, visible, acceleration),
			health(100.0f), 
			max_health(100.0f), 
			attack_damage(10.0f), 
			attack_speed(200.0f),
			attack_range(1.0f), 
			attack_duration(3.0f), 
			defense(0.0f),
			fire_rate(0.3f), 
			fire_timer(0.0f), 
			level(1) {}
		#pragma endregion

		#pragma region Getters
		std::string getName() const { return this->name; }
		float getAttackSpeed() const { return this->attack_speed; }
		float getAttackDuration() const { return this->attack_duration; }
		float getFireRate() const { return this->fire_rate; }
		float getFireTimer() const { return this->fire_timer; }
		float getAttackDamage() const { return this->attack_damage; }
		float getAttackRange() const { return this->attack_range; }
		float getDefense() const { return this->defense; }
		float getHealth() const { return this->health; }
		float getMaxHealth() const { return this->max_health; }
		int getLevel() const { return this->level; }
		#pragma endregion

		#pragma region Setters
		void setName(const std::string& name) { this->name = name; }
		void setFireRate(float fire_rate) { this->fire_rate = fire_rate; }
		void setFireTimer(float fire_timer) { this->fire_timer = fire_timer; }
		void setAttackSpeed(float attack_speed) { this->attack_speed = attack_speed; }
		void setAttackDuration(float attack_duration) { this->attack_duration = attack_duration; }
		void setAttackDamage(float attack_damage) { this->attack_damage = attack_damage; }
		void setAttackRange(float attack_range) { this->attack_range = attack_range; }
		void setDefense(float defense) { this->defense = defense; }
		void setHealth(float health) { this->health = health; }
		void setMaxHealth(float max_health) { this->max_health = max_health; }
		void setLevel(int level) { this->level = level; }
		#pragma endregion
	};
}

#endif
