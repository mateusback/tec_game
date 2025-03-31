#ifndef CHARACTER_BODY_H
#define CHARACTER_BODY_H

#include <string>
#include <list>
#include "MovingBody.h"
#include "AttackBody.h"

namespace Entites
{
	class CharacterBody : public MovingBody {
	protected:
		std::string name;
		float health;
		float max_health;
		std::list<AttackBody> attacks;
		float attack_damage;
		float attack_speed;
		float attack_range;
		float attack_duration;
		float defense;
		float fire_rate;
        float fire_timer;
		int level;
		Vector lastDirection;

	public:
		CharacterBody(float x = 0, float y = 0, float w = 50, float h = 50, bool collision = false, bool visible = true)
			: MovingBody(x, y, w, h, collision, visible), health(100), max_health(100), attack_damage(10), attack_speed(200.0f), 
			attack_range(1.0f), attack_duration(3.0f), defense(0), fire_rate(0.3f), fire_timer(0.0f),  level(1), lastDirection(1.0f, 0.0f) {}

		float getAttackSpeed() const { return attack_speed; }
        float getAttackDuration() const { return attack_duration; }
	};
}

#endif
