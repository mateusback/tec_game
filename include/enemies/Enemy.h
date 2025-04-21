#ifndef ENEMY_H
#define ENEMY_H

#include <string>

namespace Enemies {

    class Enemy {
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
        std::string behavior;

    public:
        Enemy(int id = -1, std::string name = "", std::string sprite = "", float acceleration = 100.f,
            float range = 100.f, float health = 100.f, float maxHealth = 100.f, float attackDamage = 10.f, 
            float attackRate = 1.f, float attackRange = 5.f, float attackSpeed = 1.f,  float defense = 0.f, 
            std::string behavior = "")
            : id(id),
            name(name), 
            spritePath(sprite), 
            acceleration(acceleration), 
            aggroRange(range), 
            health(health), 
            maxHealth(maxHealth), 
            attackDamage(attackDamage), 
            attackRate(attackRate), 
            attackRange(attackRange),
            attackSpeed(attackSpeed),
            defense(defense),
            behavior(behavior) {}

        #pragma region Getters
        int getId() const { return id; }
        const std::string& getName() const { return name; }
        const std::string& getSpritePath() const { return spritePath; }
        float getAcceleration() const { return acceleration; }
        float getAggroRange() const { return aggroRange; }
        const std::string& getBehavior() const { return behavior; }
        float getHealth() const { return health; }
        float getMaxHealth() const { return maxHealth; }
        float getAttackDamage() const { return attackDamage; }
        float getAttackRate() const { return attackRate; }
        float getAttackRange() const { return attackRange; }
        float getAttackSpeed() const { return attackSpeed; }
        float getDefense() const { return defense; }
        #pragma endregion

        #pragma region Setters
        void setId(int id) { this->id = id; }
        void setName(const std::string& name) { this->name = name; }
        void setSpritePath(const std::string& sprite) { this->spritePath = sprite; }
        void setAcceleration(float acceleration) { this->acceleration = acceleration; }
        void setAggroRange(float range) { this->aggroRange = range; }
        void setBehavior(const std::string& behavior) { this->behavior = behavior; }
        void setHealth(float h) { health = h; }
        void setMaxHealth(float mh) { maxHealth = mh; }
        void setAttackDamage(float dmg) { attackDamage = dmg; }
        void setAttackRate(float rate) { attackRate = rate; }
        void setAttackRange(float range) { attackRange = range; }
        void setAttackSpeed(float speed) { attackSpeed = speed; }
        void setDefense(float def) { defense = def; }
        #pragma endregion
    };
}

#endif
