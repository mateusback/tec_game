#include "../../include/managers/EnemyManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Manager;

bool EnemyManager::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    json data;
    file >> data;

    for (const auto& entry : data) {
        int id = entry["id"];
        std::string name = entry["name"];
        std::string spritePath = entry["spritePath"];
        float acceleration = entry["acceleration"];
        float aggroRange = entry["aggroRange"];
        std::string behavior = entry["behavior"];

        Enemies::Enemy enemy(id, name, spritePath, acceleration, aggroRange, behavior);
        enemyMap[name] = enemy;
    }

    return true;
}

const Enemies::Enemy* EnemyManager::getEnemyById(int id) const {
    for (const auto& [_, enemy] : enemyMap) {
        if (enemy.getId() == id) return &enemy;
    }
    return nullptr;
}