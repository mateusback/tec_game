#include "../../include/physics/CollisionManager.h"

namespace Physics {
	//resolver colisao junto
    bool CollisionManager::checkCollision(const Vector4f& a, const Vector4f& b) {
        return (
            a.x < b.x + b.z &&
            a.x + a.z > b.x &&
            a.y < b.y + b.w &&
            a.y + a.w > b.y
        );
    }

    void CollisionManager::resolveCollision(Entities::Body* a, const Entities::Body* b) {
        Vector4i colisions = {0, 0, 0, 0};

        Vector2f aCenter = a->getCenterPoint();
        Vector2f bCenter = b->getCenterPoint();

        Vector4f aHitbox = a->getHitbox();
        Vector4f bHitbox = b->getHitbox();

        Vector2f distance = { aCenter.x - bCenter.x, aCenter.y - bCenter.y };
        Vector2f target_distance = {
            (aHitbox.z + bHitbox.z) / 2.0f,
            (aHitbox.w + bHitbox.w) / 2.0f
        };

        Vector2f collision_vector;
        bool colliding_x = std::abs(distance.x) < target_distance.x;
        bool colliding_y = std::abs(distance.y) < target_distance.y;

        collision_vector.x = std::copysign(target_distance.x - std::abs(distance.x), distance.x);
        collision_vector.y = std::copysign(target_distance.y - std::abs(distance.y), distance.y);

        if (colliding_x && colliding_y) {
            Vector2f pos = a->getPosition();

            if (std::abs(collision_vector.x) < std::abs(collision_vector.y)) {
                pos.x += collision_vector.x;
            } else {
                pos.y += collision_vector.y;
            }

            a->setPosition(pos);
        }
    }

}