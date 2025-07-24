#ifndef BOSSDATA_H
#define BOSSDATA_H

#include <string>
#include <vector>

namespace Enemies {

    struct BossPhase {
        float threshold;
        std::string attackType;
    };

    struct BossTextures {
        std::string tentacle;
        std::string chain;
    };

    class BossData {
    private:
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

        std::vector<BossPhase> phases;
        std::vector<int> summonIds;
        BossTextures specialTextures;

    public:
        int getId() const { return id; }
        const std::string& getName() const { return name; }
        const std::string& getSpritePath() const { return spritePath; }
        float getAcceleration() const { return acceleration; }
        float getAggroRange() const { return aggroRange; }
        float getHealth() const { return health; }
        float getMaxHealth() const { return maxHealth; }
        float getAttackDamage() const { return attackDamage; }
        float getAttackRate() const { return attackRate; }
        float getAttackRange() const { return attackRange; }
        float getAttackSpeed() const { return attackSpeed; }
        float getDefense() const { return defense; }

        const std::vector<BossPhase>& getPhases() const { return phases; }
        const std::vector<int>& getSummonIds() const { return summonIds; }
        const BossTextures& getSpecialTextures() const { return specialTextures; }

        void setId(int v) { id = v; }
        void setName(const std::string& v) { name = v; }
        void setSpritePath(const std::string& v) { spritePath = v; }
        void setAcceleration(float v) { acceleration = v; }
        void setAggroRange(float v) { aggroRange = v; }
        void setHealth(float v) { health = v; }
        void setMaxHealth(float v) { maxHealth = v; }
        void setAttackDamage(float v) { attackDamage = v; }
        void setAttackRate(float v) { attackRate = v; }
        void setAttackRange(float v) { attackRange = v; }
        void setAttackSpeed(float v) { attackSpeed = v; }
        void setDefense(float v) { defense = v; }
        void setPhases(const std::vector<BossPhase>& v) { phases = v; }
        void setSummonIds(const std::vector<int>& v) { summonIds = v; }
        void setSpecialTextures(const BossTextures& v) { specialTextures = v; }

        std::unordered_map<std::string, std::string> getSpecialTexturesMap() const {
            std::unordered_map<std::string, std::string> textures;
            if (!specialTextures.tentacle.empty()) textures["tentacle"] = specialTextures.tentacle;
            if (!specialTextures.chain.empty()) textures["chain"] = specialTextures.chain;
            return textures;
        }

    };
}

#endif
