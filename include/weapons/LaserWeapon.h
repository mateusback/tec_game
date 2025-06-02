#ifndef LASER_WEAPON_H
#define LASER_WEAPON_H

#include "Weapon.h"

class LaserWeapon : public Weapon {
public:
    void attack(const Vector2f& direction) override;
};

#endif
