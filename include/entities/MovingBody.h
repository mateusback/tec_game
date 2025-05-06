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
		MovingBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: Body(x, y, w, h, collision, visible), speed(0.0f, 0.0f) {}
		
		MovingBody(Vector2f pos, Vector2f scl, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: Body(pos.x, pos.y, scl.x, scl.y, collision, visible), speed(0.0f, 0.0f) {}

		MovingBody(Vector4f collider, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: Body(collider.x, collider.y, collider.z, collider.w, collision, visible), speed(0.0f, 0.0f) {}
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
