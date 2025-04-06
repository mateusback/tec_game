#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
};

#endif
