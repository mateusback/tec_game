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
        static bool checkCollision(const Vector4& a, const Vector4& b);
        static void resolveCollision(Entities::Entity* a, Entities::Entity* b);
    };
}


#endif