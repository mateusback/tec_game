#ifndef MAGIC_HANDS_WEAPON_H
#define MAGIC_HANDS_WEAPON_H

#include "Weapon.h"

class MagicHandsWeapon : public Weapon {
public:
    void attack(const Vector2f& origin, const Vector2f& direction) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;

private:
};

#endif
