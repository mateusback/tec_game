#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H

#include "CharacterBody.h"

class PlayerBody : public CharacterBody {
    list<Item> inventory;
    int money;
    int key;
    int bomb;
    float experience;
    float defense;
    float life_steal;
};

#endif
