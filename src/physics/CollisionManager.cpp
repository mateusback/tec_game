#include "../../include/physics/CollisionManager.h"

namespace Physics {
	//resolver colisao junto
    bool CollisionManager::checkCollision(const Vector4& a, const Vector4& b) {
        return (
            a.x < b.x + b.z &&
            a.x + a.z > b.x &&
            a.y < b.y + b.w &&
            a.y + a.w > b.y
        );
    }

    void CollisionManager::resolveCollision(Vector4& a, const Vector4& b) {
        float aCenterX = a.x + a.z / 2.0f;
        float aCenterY = a.y + a.w / 2.0f;
        float bCenterX = b.x + b.z / 2.0f;
        float bCenterY = b.y + b.w / 2.0f;

        float dx = aCenterX - bCenterX;
        float dy = aCenterY - bCenterY;

        float combinedHalfWidth = (a.z + b.z) / 2.0f;
        float combinedHalfHeight = (a.w + b.w) / 2.0f;

        float overlapX = combinedHalfWidth - std::abs(dx);
        float overlapY = combinedHalfHeight - std::abs(dy);

        if (overlapX > 0 && overlapY > 0) {
            if (overlapX < overlapY) {
                if (dx > 0)
                    a.x += overlapX;
                else
                    a.x -= overlapX;
            } else {
                if (dy > 0)
                    a.y += overlapY;
                else
                    a.y -= overlapY;
            }
        }
    }
}