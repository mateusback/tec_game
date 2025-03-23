#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <SDL2/SDL.h>
#include <my-lib/math-vector.h>
#include "../../Entities/Interface/Body.h"


class CollisionManager {
public:
    static bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b);
    static bool CheckCollision(const Vector4 &a, const Vector4 &b);
    static bool CheckCollision(const Body &a, const Body &b);
};

#endif