#ifndef WEAPON_HANDLER_H
#define WEAPON_HANDLER_H

#include "../weapons/Weapon.h"
#include "../managers/EntityManager.h"

class WeaponHandler {
public:
    explicit WeaponHandler(Manager::EntityManager* entityManager);

    void setWeapon(std::shared_ptr<Weapon> weapon);
    void attack(const Vector2f& direction);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    std::shared_ptr<Weapon> getWeapon() { return this->currentWeapon; }

private:
    std::shared_ptr<Weapon> currentWeapon;
};

#endif
