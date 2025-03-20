#ifndef ENEMY_BODY_H
#define ENEMY_BODY_H

#include "CharacterBody.h"
#include "PlayerBody.h"

class EnemyBody : public CharacterBody {
    PlayerBody* target;
    float aggro_range;
};

#endif
