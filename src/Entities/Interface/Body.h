#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>

class Body {
protected:
    SDL_FRect rect;
    bool has_collision;
    bool is_visible;

    public:
    	Body(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true)
        : has_collision(collision), is_visible(visible) {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
    }
};

#endif
