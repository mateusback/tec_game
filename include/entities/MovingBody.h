#ifndef MOVING_BODY_H
#define MOVING_BODY_H

#include "Body.h"
namespace Entities
{
	class MovingBody : public Body {
	protected:
		Vector speed;
		float acceleration;

	public:
		MovingBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float acceleration = 100.0f)
			: Body(x, y, w, h, collision, visible), speed(0.0f, 0.0f) {}

		void setSpeed(Vector speed) {
			this->speed = speed;
		}
		
		void move(float delta) {
			std::printf(">rect.x: %.2f, rect.y: %.2f\n", rect.x, rect.y);
			std::printf(">spidi: %.2f\n", acceleration);
			this->rect.x += this->speed.x * delta;
			this->rect.y += this->speed.y * delta;
		}

		void setAcceleration(float acceleration) {
			this->acceleration = acceleration;
		}

		float getAcceleration() const {
			return this->acceleration;
		}
	};
}

#endif
