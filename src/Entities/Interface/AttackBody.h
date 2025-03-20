#ifndef ATTACK_BODY_H
#define ATTACK_BODY_H

#include <string>
#include "MovingBody.h"

class AttackBody : public MovingBody {
    float attack_damage;
    float attack_range;
    float attack_duration;
    float life_steal;
    float critical_chance;
    float critical_damage;
};

#endif
