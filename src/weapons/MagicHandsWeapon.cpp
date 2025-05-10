#include "../../include/weapons/MagicHandsWeapon.h"
#include "../../include/utils/GlobalAccess.h"
#include "../../include/entities/AttackBody.h"

void MagicHandsWeapon::attack(const Vector2f& origin, const Vector2f& direction) {
    std::cout << "MagicHandsWeapon::attack" << std::endl;
    if (!this->owner) {
        std::cerr << "Owner is null" << std::endl;
        return;
    }
    if (!entityManager || this->owner->getAttackTimer() > 0.f || direction == Vector2f{0, 0}) return;

    this->owner->setAttackTimer(this->owner->getAttackRate());

    audio()->playSoundEffect("shoot", 0);

    // Tamanho e velocidade
    float width = 16.f;
    float height = 16.f;
    float damage = 10.f;
    float speed = 200.f;
    float lifetime = 1.5f;

    auto attack = std::make_unique<Entities::AttackBody>(
        origin.x - width / 2,
        origin.y - height / 2,
        width,
        height,
        true,
        true, 
        damage,
        speed,
        4.2f,
        0.f,
        0.1f,
        lifetime
    );

    attack->setScale(virtualRenderer()->normalizeVector({0.3f, 0.3f}));
    attack->setSpeed(virtualRenderer()->normalizeVector({4, 4}));
    attack->setTexture(textures()->Get("attack"));
    attack->setOrigin(this->owner);

    std::cout << "Attack created" << std::endl;
    entityManager->add(std::move(attack));
}

void MagicHandsWeapon::update(float /*deltaTime*/) {
}

void MagicHandsWeapon::render(SDL_Renderer* /*renderer*/) {
}
