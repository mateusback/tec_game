#include "../../include/weapons/SwordWeapon.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/EnemyBody.h"

void SwordWeapon::attack(const Vector2f& origin, const Vector2f& direction) {
    if (!entityManager || this->owner->getAttackTimer() > 0.f) return;

}

void SwordWeapon::update(float /*renderer*/) {
}

void SwordWeapon::render(SDL_Renderer* /*renderer*/) {
}
