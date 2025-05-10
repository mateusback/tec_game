#include "../../include/weapons/MagicHandsWeapon.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/AttackBody.h"

void MagicHandsWeapon::attack(const Vector2f& direction) {
    if (!entityManager || this->owner->getAttackTimer() > 0.f || direction == Vector2f{0, 0}) return;

    this->owner->setAttackTimer(this->owner->getAttackRate());

    audio()->playSoundEffect("shoot", 0);

    auto attack = std::make_unique<Entities::AttackBody>(
        this->owner->getCenterPoint(),
        virtualRenderer()->normalizeVector({DEFAULT_BULLET_SIZE, DEFAULT_BULLET_SIZE}),
        true,
        true, 
        this->owner->getAttackDamage(),
        this->owner->getAttackRange(),
        this->owner->getAttackDuration(),
        0.f,
        0.1f,
        2.0f
    );

    attack->setTexture(textures()->Get("attack"));
    attack->setOrigin(this->owner);
    attack->setSpeed(direction * virtualRenderer()->normalizeValue(this->owner->getAttackSpeed()));
    std::cout << "Attack created" << std::endl;
    entityManager->add(std::move(attack));
}

void MagicHandsWeapon::update(float /*deltaTime*/) {
}

void MagicHandsWeapon::render(SDL_Renderer* /*renderer*/) {
}
