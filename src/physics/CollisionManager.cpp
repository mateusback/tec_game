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

    void CollisionManager::resolveCollision(Entities::Body* a, const Entities::Body* b) {
        Vector4 aHitbox = a->getHitbox();
        Vector4 bHitbox = b->getHitbox();

        float aCenterX = aHitbox.x + aHitbox.z / 2.0f;
        float aCenterY = aHitbox.y + aHitbox.w / 2.0f;
        float bCenterX = bHitbox.x + bHitbox.z / 2.0f;
        float bCenterY = bHitbox.y + bHitbox.w / 2.0f;

        float dx = aCenterX - bCenterX;
        float dy = aCenterY - bCenterY;

        float combinedHalfWidth = (aHitbox.z + bHitbox.z) / 2.0f;
        float combinedHalfHeight = (aHitbox.w + bHitbox.w) / 2.0f;

        float overlapX = combinedHalfWidth - std::abs(dx);
        float overlapY = combinedHalfHeight - std::abs(dy);

        if (overlapX > 0 && overlapY > 0) {
            Vector pos = a->getPosition();

            if (overlapX < overlapY) {
                pos.x += (dx > 0) ? overlapX : -overlapX;
            } else {
                pos.y += (dy > 0) ? overlapY : -overlapY;
            }

            a->setPosition(pos);
        }
    }
}