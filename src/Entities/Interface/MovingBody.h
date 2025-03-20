#ifndef MOVING_BODY_H
#define MOVING_BODY_H

#include "Body.h"

class MovingBody : public Body {
protected:
	float speed;

public:
	MovingBody(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true, float spd = 0)
		: Body(x, y, w, h, collision, visible), speed(spd) {}
	
	void move(float dx, float dy) {
		this->rect.x += dx;
		this->rect.y += dy;
}
};

#endif
