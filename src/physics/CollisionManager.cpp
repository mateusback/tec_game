#include "../../include/physics/CollisionManager.h"

namespace Physics {
	//resolver colisao junto
    bool CollisionManager::checkCollision(const Vector4& a, const Vector4& b) {
        if (std::abs(a.x - b.x) > 100 * 2) return false;
        bool colided = (
            a.x < b.x + b.z &&
            a.x + a.z > b.x &&
            a.y < b.y + b.w &&
            a.y + a.w > b.y
        );
        return colided;
    }

    void CollisionManager::resolveCollision(Vector4& a, const Vector4& b) {
        float axCenter = a.x + a.z / 2.0f;
        float ayCenter = a.y + a.w / 2.0f;
        float bxCenter = b.x + b.z / 2.0f;
        float byCenter = b.y + b.w / 2.0f;

        float dx = axCenter - bxCenter;
        float dy = ayCenter - byCenter;

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