#ifndef ENEMY_BODY_H
#define ENEMY_BODY_H

#include <string>
#include "CharacterBody.h"

class EnemyBody : public CharacterBody {
    PlayerBody* target;
    float aggro_range;
};

#endif
