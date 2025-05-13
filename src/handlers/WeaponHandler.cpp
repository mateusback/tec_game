#include "../../include/handlers/WeaponHandler.h"

WeaponHandler::WeaponHandler(Manager::EntityManager* entityManager) {
}

void WeaponHandler::setWeapon(std::shared_ptr<Weapon> weapon) {
    this->currentWeapon = std::move(weapon);
}

void WeaponHandler::attack(const Vector2f& direction) {
    if (this->currentWeapon)
        this->currentWeapon->attack(direction);
}

void WeaponHandler::update(float deltaTime) {
    if (this->currentWeapon)
        this->currentWeapon->update(deltaTime);
}

void WeaponHandler::render(SDL_Renderer* renderer) {
    if (this->currentWeapon)
        this->currentWeapon->render(renderer);
}
