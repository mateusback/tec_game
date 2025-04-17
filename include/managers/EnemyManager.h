#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "../enemies/Enemy.h"
#include <my-lib/utils.h>
#include <string>

using namespace Enemies;

namespace Manager {
    class EnemyManager {
    private:
        Mylib::unordered_map_string_key<Enemy> enemyMap;
    public:
        bool loadFromFile(const std::string& filePath);
        const Enemy* getEnemyById(int id) const;
        const Mylib::unordered_map_string_key<Enemy>& getAllEnemies() const { return enemyMap; }
    };
}

#endif
