#ifndef MOVING_BODY_H
#define MOVING_BODY_H

#include "Body.h"

namespace Entities
{
	class MovingBody : public Body {
	protected:
		Vector2f speed;
		float acceleration;
	
	public:
		#pragma region Constructors
		MovingBody(Vector2f pos, Vector2f scl, bool collision = false, bool visible = true, float acceleration = GameConstants::Moving::DEFAULT_ACCELERATION)
			: MovingBody(Vector4f(pos.x, pos.y, scl.x, scl.y), collision, visible, acceleration) {}

		MovingBody(Vector4f collider, bool collision = false, bool visible = true, float acceleration = GameConstants::Moving::DEFAULT_ACCELERATION)
			: Body(collider, collision, visible),
			speed(0.0f, 0.0f),
			acceleration(acceleration) {}
		#pragma endregion

		void move(float deltaTime) {
			this->position += this->speed * deltaTime;
		}

		#pragma region Getters
		float getAcceleration() const {	return this->acceleration; }
		#pragma endregion

		#pragma region Setters
		void setSpeed(Vector2f speed) { this->speed = speed; }
		void setAcceleration(float acceleration) { this->acceleration = acceleration; }
		#pragma endregion
	};
}

#endif
