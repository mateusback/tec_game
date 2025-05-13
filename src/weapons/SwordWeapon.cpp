#include "../../include/weapons/SwordWeapon.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/EnemyBody.h"

void SwordWeapon::attack(const Vector2f& direction) {
    if (!entityManager || this->owner->getAttackTimer() > 0.f || direction == Vector2f{0, 0}) return;

    this->owner->setAttackTimer(this->owner->getAttackRate());

    audio()->playSoundEffect("shoot", 0);

    Vector2f dir = Mylib::Math::normalize(direction);

    Vector2f attackSize;
    if (std::abs(dir.x) > std::abs(dir.y)) {
        attackSize = {1.0f, 0.5f};
    } else {
        attackSize = {0.5f, 1.0f};
    }

    float offset = this->owner->getScale().x * 0.5f + virtualRenderer()->normalizeValue(0.2f);
    Vector2f origin = this->owner->getCenterPoint() + dir * offset;

    Vector2f pixelSize = virtualRenderer()->normalizeVector(attackSize);

    auto attack = std::make_unique<Entities::AttackBody>(
        origin - pixelSize * 0.5f,
        pixelSize,
        true,
        true,
        this->owner->getAttackDamage(),
        this->owner->getAttackRange(),
        this->owner->getAttackDuration(),
        0.f,
        0.1f,
        2.0f
    );

    attack->setTexture(textures()->Get("swing"));
    attack->setAnimated(true);
    attack->loadAnimations();
    attack->setOrigin(this->owner);
    entityManager->add(std::move(attack));
}

void SwordWeapon::update(float /*renderer*/) {
}

void SwordWeapon::render(SDL_Renderer* /*renderer*/) {
}
