#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <SDL2/SDL.h>
#include "../../include/entities/Body.h"

namespace Physics
{
    class CollisionManager
    {
    public:
        static bool checkCollision(const Vector4f& a, const Vector4f& b);
        static void resolveCollision(Entities::Body* a, const Entities::Body* b);
    };
}


#endif