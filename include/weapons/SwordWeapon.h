#ifndef SWORD_WEAPON_H
#define SWORD_WEAPON_H

#include "Weapon.h"

class SwordWeapon : public Weapon {
public:
    void attack(const Vector2f& direction) override;
};

#endif
