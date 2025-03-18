#ifndef CHARACTER_BODY_H
#define CHARACTER_BODY_H

#include <string>

#include "MovingBody.h"

class CharacterBody : public MovingBody {
    std::string name;
    float health;
    float max_health;
    list<AttackBody> attacks;
    float attack_damage;
    float attack_speed;
    float attack_range;
    float attack_duration;
    float defense;
    int level; 
};

#endif
