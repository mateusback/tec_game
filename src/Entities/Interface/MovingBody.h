#ifndef MOVING_BODY_H
#define MOVING_BODY_H

#include "Body.h"

enum EDirection {UP, DOWN, LEFT, RIGHT};

class MovingBody : public Body {
protected:
	float velocity;

public:
	MovingBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float velocity = 0)
		: Body(x, y, w, h, collision, visible), velocity(velocity) {}

	void setVelocity(float velocity) {
		this->velocity = velocity;
	}
	
	void move(EDirection, float delta) {
		if (EDirection::UP) this->rect.y -= velocity * delta;
		if (EDirection::DOWN) this->rect.y += velocity * delta;
		if (EDirection::LEFT) this->rect.x -= velocity * delta;
		if (EDirection::RIGHT) this->rect.x += velocity * delta;
	}
};

#endif
