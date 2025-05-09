#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <my-lib/utils.h>
#include <string>
#include "entities/EntitiesTypes.h"
#include "../enemies/Enemy.h"

using namespace Enemies;

namespace Manager {
    class EnemyManager {
    private:
        Mylib::unordered_map_string_key<Enemy> enemyMap;
        static Entities::EEnemyBehavior stringToEnemyBehavior(const std::string& str);

    public:
        bool loadFromFile(const std::string& filePath);
        const Enemy* getEnemyById(int id) const;
        const Mylib::unordered_map_string_key<Enemy>& getAllEnemies() const { return enemyMap; }
    };
}

#endif
