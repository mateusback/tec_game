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
}