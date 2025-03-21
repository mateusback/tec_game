#ifndef BODY_H
#define BODY_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>

using Vector = Mylib::Math::Vector<float, 2>;
using Point = Vector;

class Body {
protected:
    //TODO - Possivel reaftoração para usar Vector
    SDL_FRect rect;
    bool has_collision;
    bool is_visible;

public:
    Body(float x = 0, float y = 0, float w = 0, float h = 0, bool collision = false, bool visible = true)
    : has_collision(collision), is_visible(visible) {
        this->rect.x = x;
        this->rect.y = y;
        this->rect.w = w;
        this->rect.h = h;
        this->has_collision = collision;
        this->is_visible = visible;
    }

    Body(SDL_FRect rect, bool collision = false, bool visible = true)
    : has_collision(collision), is_visible(visible) {
        this->rect = rect;
        this->has_collision = collision;
        this->is_visible = visible;
    }

    Body(SDL_FPoint position, SDL_FPoint size, bool collision = false, bool visible = true)
    : has_collision(collision), is_visible(visible) {
        this->rect.x = position.x;
        this->rect.y = position.y;
        this->rect.w = size.x;
        this->rect.h = size.y;
        this->has_collision = collision;
        this->is_visible = visible;
    }
};

#endif
