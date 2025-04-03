#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>
#include "../../include/entities/Body.h"

namespace Physics
{
    class CollisionManager
    {
    public:
        static bool checkCollision(const SDL_FRect& a, const SDL_FRect& b);
    };
}


#endif