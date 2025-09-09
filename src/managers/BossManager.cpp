#include "../../include/managers/BossManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using namespace Enemies;

namespace Manager {

    bool BossManager::loadFromFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo de bosses: " << filePath << "\n";
            return false;
        }

        json data;
        file >> data;

        for (const auto& bossJson : data["bosses"]) {
            BossData boss;

            boss.setId(bossJson.at("id").get<int>());
            boss.setName(bossJson.at("name").get<std::string>());
            boss.setSpritePath(bossJson.at("spritePath").get<std::string>());
            boss.setAcceleration(bossJson.at("acceleration").get<float>());
            boss.setAggroRange(bossJson.at("aggroRange").get<float>());
            boss.setHealth(bossJson.at("health").get<float>());
            boss.setMaxHealth(bossJson.at("maxHealth").get<float>());
            boss.setAttackDamage(bossJson.at("attackDamage").get<float>());
            boss.setAttackRate(bossJson.at("attackRate").get<float>());
            boss.setAttackRange(bossJson.at("attackRange").get<float>());
            boss.setAttackSpeed(bossJson.at("attackSpeed").get<float>());
            boss.setDefense(bossJson.at("defense").get<float>());

            // Fases
            std::vector<BossPhase> phases;
            for (const auto& phaseJson : bossJson["phases"]) {
                phases.push_back({
                    phaseJson.at("threshold").get<float>(),
                    phaseJson.at("attackType").get<std::string>()
                });
            }
            boss.setPhases(phases);

            // Summon IDs
            std::vector<int> summons;
            for (const auto& summonId : bossJson["summonIds"]) {
                summons.push_back(summonId.get<int>());
            }
            boss.setSummonIds(summons);

            const auto& texturesJson = bossJson["specialTextures"];
            BossTextures textures;
            textures.tentacle = texturesJson.at("tentacle").get<std::string>();
            textures.chain = texturesJson.at("chain").get<std::string>();
            boss.setSpecialTextures(textures);

            this->bossMap[std::to_string(boss.getId())] = boss;
        }

        std::cout << "[BossManager] Bosses carregados com sucesso: " << bossMap.size() << "\n";
        return true;
    }

    const BossData* BossManager::getBossById(int id) const {
        auto it = this->bossMap.find(std::to_string(id));
        if (it != this->bossMap.end()) {
            return &it->second;
        }
        return nullptr;
    }

}
