#include "../../include/weapons/LaserWeapon.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/LaserAttackBody.h"

void LaserWeapon::attack(const Vector2f& direction) {
    if (!entityManager || this->owner->getAttackTimer() > 0.f || direction == Vector2f{0, 0}) return;

    this->owner->setAttackTimer(this->owner->getAttackRate());

    audio()->playSoundEffect("shoot", 0);

    auto attack = std::make_unique<Entities::LaserAttackBody>(
        this->owner->getCenterPoint(),
        direction,
        this->owner->getAttackDamage(),
        this->owner->getAttackRate()
    );

    attack->setTexture(textures()->Get("attack"));
    attack->setOrigin(this->owner);
    entityManager->add(std::move(attack));
}