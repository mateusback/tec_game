#ifndef BOSSDATA_H
#define BOSSDATA_H

#include <string>

namespace Enemies {
    class BossData {
        int id;
        std::string name;
        std::string spritePath;
        float acceleration;
        float aggroRange;
        float health;
        float maxHealth;
        float attackDamage;
        float attackRate;
        float attackRange;
        float attackSpeed;
        float defense;
    };
}

#endif
