#include "../../include/managers/EnemyManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

#include "entities/EntitiesTypes.h"

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
        Entities::EEnemyBehavior behavior = stringToEnemyBehavior(entry["behavior"]);
        float health = entry.value("health", 100.f);
        float maxHealth = entry.value("maxHealth", 100.f);
        float attackDamage = entry.value("attackDamage", 10.f);
        float attackRate = entry.value("attackRate", 1.f);
        float attackRange = entry.value("attackRange", 5.f);
        float attackSpeed = entry.value("attackSpeed", 1.f);
        float defense = entry.value("defense", 0.f);

        Enemies::Enemy enemy(id, name, spritePath, acceleration, aggroRange,
            health, maxHealth, attackDamage, attackRate, attackRange, 
            attackSpeed, defense, behavior);
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

Entities::EEnemyBehavior EnemyManager::stringToEnemyBehavior(const std::string& str) {
    if (str == "shell") return Entities::EEnemyBehavior::Shell;
    if (str == "jumper") return Entities::EEnemyBehavior::Jumper;
    throw std::invalid_argument("Invalid enemy behavior string");
}
