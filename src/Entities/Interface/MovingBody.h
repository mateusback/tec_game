#ifndef MOVING_BODY_H
#define MOVING_BODY_H

#include "Body.h"

enum EDirection {UP, DOWN, LEFT, RIGHT};

class MovingBody : public Body {
protected:
	float velocity;

public:
	MovingBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float velocity = 100.0f)
		: Body(x, y, w, h, collision, visible), velocity(velocity) {}

	void setVelocity(float velocity) {
		this->velocity = velocity;
	}
	
	void move(EDirection direction, float delta) {
		if (direction = EDirection::UP) this->rect.y -= velocity * delta;
		if (direction = EDirection::DOWN) this->rect.y += velocity * delta;
		if (direction = EDirection::LEFT) this->rect.x -= velocity * delta;
		if (direction = EDirection::RIGHT) this->rect.x += velocity * delta;
	}
};

#endif
