#ifndef BOSS_MANAGER_H
#define BOSS_MANAGER_H

#include <my-lib/utils.h>
#include <string>
#include <vector>
#include "../enemies/BossData.h"

namespace Manager {
    class BossManager {
    private:
        Mylib::unordered_map_string_key<Enemies::BossData> bossMap;

    public:
        bool loadFromFile(const std::string& filePath);
        const Enemies::BossData* getBossById(int id) const;
        const Mylib::unordered_map_string_key<Enemies::BossData>& getAllBosses() const { return bossMap; }
    };
}

#endif
