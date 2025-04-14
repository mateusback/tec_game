#ifndef MOVING_BODY_H
#define MOVING_BODY_H

#include "Body.h"
namespace Entities
{
	class MovingBody : public Body {
	protected:
		Vector speed;
		float acceleration;

		MovingBody(EBodyType type, float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: Body(type, x, y, w, h, collision, visible), speed(0.0f, 0.0f) {}
	
			public:
		MovingBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: Body(EBodyType::Moving, x, y, w, h, collision, visible), speed(0.0f, 0.0f) {}
		
		void move(float delta) {
			this->position += this->speed * delta;
		}

		#pragma region Getters
		float getAcceleration() const {	return this->acceleration; }
		#pragma endregion

		#pragma region Setters
		void setSpeed(Vector speed) { this->speed = speed; }
		void setAcceleration(float acceleration) { this->acceleration = acceleration; }
		#pragma endregion
	};
}

#endif
