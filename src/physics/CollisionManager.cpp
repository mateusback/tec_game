#include "../../include/physics/CollisionManager.h"

namespace Physics {
    bool CollisionManager::checkCollision(const SDL_FRect& a, const SDL_FRect& b) {
        return (
            a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y
        );
    }
}